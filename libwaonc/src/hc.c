/*
 * half-complex format routines
 *
 * Copyright (C) 2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: hc.c,v 1.7 2007/10/22 04:26:38 kichiki Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * \file          hc.c
 *
 *    This module provides half-format complex routines.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-23
 * \version       $Revision$
 * \license       GNU GPL
 */

#include <math.h>
#include <stdlib.h>                    /* malloc()                            */
#include <stdio.h>                     /* fprintf()                           */

#include "hc.h"                        /* declares the function of module     */
#include "memory-check.h"              /* CHECK_MALLOC() macro                */

/**
 *    Returns the angle (arg) of the complex number (freq(k),freq(len-k));
 *    where (real,imag) = (cos(angle), sin(angle)).
 *
 * \param len
 *    Provides the length of the frequency array.
 *
 * \param freq[len]
 *    Provides the frequency array.
 *
 * \param conjugate
 *    Set to 0 (wfalse) for the normal case of computation.  Set to 1
 *    (wtrue) for the conjugate of the complex (freq(k), freq(len-k))
 *    that is, for (freq(k), -freq(len-k)).
 *
 * \param [out] amp[len/2+1]
 *    Provides the output buffer for the amplitude.
 *
 * \param [out] phs[len/2+1]
 *    Provides the output buffer for the phase.
 */

void
HC_to_polar
(
   long len,
   const double * freq,
   wbool_t conjugate,
   double * amp,
   double * phs
)
{
   int i;
   double rl, im;
   phs[0] = 0.0;
   amp[0] = sqrt(freq[0] * freq[0]);
   for (i = 1; i < (len+1)/2; i ++)
   {
      rl = freq[i];
      im = freq[len - i];
      amp[i] = sqrt(rl*rl + im*im);
      if (amp[i] > 0.0)
      {
         if (conjugate)
            phs[i] = atan2(-im, rl);
         else
            phs[i] = atan2(+im, rl);
      }
      else
         phs[i] = 0.0;
   }
   if (len % 2 == 0)
   {
      phs[len/2] = 0.0;
      amp[len/2] = sqrt(freq[len/2] * freq[len/2]);
   }
}

/**
 *    Returns the angle (arg) of the complex number (freq(k),freq(len-k));
 *    where (real,imag) = (cos(angle), sin(angle)).
 *
 * \param len
 *    Provides the length of the frequency buffer.
 *
 * \param freq[len]
 *    Provides the frequency buffer.
 *
 * \param conjugate
 *    Set to 0 (wfalse) for the normal case of computation.  Set to 1
 *    (wtrue) for the conjugate of the complex (freq(k), freq(len-k))
 *    that is, for (freq(k), -freq(len-k)).
 *
 * \param scale
 *    Provides the scale factor for amp2[].
 *
 * \param [out] amp[len/2+1]
 *    Provides the output buffer for the amplitude.
 *
 * \param [out] phs[len/2+1]
 *    Provides the output buffer for the phase.
 */

void
HC_to_polar2
(
   long len,
   const double * freq,
   wbool_t conjugate,
   double scale,
   double * amp2,
   double * phs
)
{
   int i;
   double rl, im;
   phs[0] = 0.0;
   amp2[0] = freq[0] * freq[0] / scale;
   for (i = 1; i < (len+1)/2; i ++)
   {
      rl = freq[i];
      im = freq[len - i];
      amp2[i] = (rl*rl + im*im) / scale;
      if (amp2[i] > 0.0)
      {
         if (conjugate)
            phs[i] = atan2(-im, rl);
         else
            phs[i] = atan2(+im, rl);
      }
      else
         phs[i] = 0.0;
   }
   if (len % 2 == 0)
   {
      phs[len/2] = 0.0;
      amp2[len/2] = freq[len/2] * freq[len/2] / scale;
   }
}

/**
 *    Returns the  power (amp2) of the complex number (freq(k), freq(len-k)),
 *    where (real,imag) = (cos(angle), sin(angle)).
 *
 * \param len
 *    Provides the length of the frequency buffer.
 *
 * \param freq[len]
 *    Provides the frequency buffer.
 *
 * \param scale
 *    Provides the scale factor for the amp2[] output buffer.
 *
 * \param [out] amp2[len/2+1]
 *    Provides the destination buffer for the "(real^2 + imag^2) / scale"
 *    operation.
 */

void
HC_to_amp2
(
   long len,
   const double * freq,
   double scale,
   double * amp2
)
{
   int i;
   amp2[0] = freq[0] * freq[0] / scale;
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      double rl = freq[i];
      double im = freq[len - i];
      amp2[i] = (rl * rl + im * im)  / scale;
   }
   if (len % 2 == 0)
      amp2[len/2] = freq[len/2] * freq[len/2] / scale;
}

/**
 *    Converts from polar coordinates to HC values.
 *
 * \param len
 *    Provides the base length of the amplitude and phase buffers, which
 *    are actually len/2+1 in length.
 *
 * \param amp[len/2+1]
 *    Provides the buffer for the amplitude values.
 *
 * \param phs[len/2+1]
 *    Provides the buffer for the phase values.
 *
 * \param conjugate
 *    Set to 0 (wfalse) for the normal case of computation.  Set to 1
 *    (wtrue) for the conjugate of the complex (freq(k), freq(len-k))
 *    that is, for (freq(k), -freq(len-k)).
 *
 * \param [out] freq[len]
 *    Provides the output buffer for the frequency values.
 *
 * \note
 *    If \a len is even:
 *
\verbatim
    (freq(0)        ... freq(N/2)) = (r(0)     ... r(N/2)) and
    (endfreq(N/2+1) ... freq(N)  ) = (i(N/2-1) ... i(1))
\endverbatim
 *
 *    where Fourier coefficient Y(k) = r(k) + i i(k).  Note that Y(k) =
 *    Y*(N-k) and Y(0) and Y(N/2) are real.
 *
 *    If \a len is odd:
 *
\verbatim
    (freq(0)        ... freq(N/2)) = (r(0)     ... r(N/2)) and
    (freq(N/2+1)    ... freq(N)  ) = (i(N/2-1) ... i(1))
\endverbatim
 *
 *    Note in this case that Y(0) is real but Y(N/2) is not.  In either
 *    case, number of elements for the coefficients are N/2+1.
 */

void
polar_to_HC
(
   long len,
   const double * amp,
   const double * phs,
   int conjugate,
   double * freq
)
{
   int i;
   double rl, im;

   /* calc phase and amplitude (power) */

   freq[0] = amp[0];
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      rl = amp[i] * cos(phs[i]);
      if (conjugate)
         im = -amp[i] * sin(phs[i]);
      else
         im = +amp[i] * sin(phs[i]);

      freq[i] = rl;
      freq[len - i] = im;
   }
   if (len % 2 == 0)
      freq[len/2] = amp[len/2];
}

/**
 *    Converts polar to HC with the scaling in frequency domain.
 *
 *    Bin k in the input is placed in scale*k in the output except for k=0
 *    and k=len/2 for even \a len.
 *
 * \param len
 *    Provides the length of the frequency array; but applies here to the
 *    amplitude and phase arrays, which are of length len/2+1..
 *
 * \param amp[len/2+1]
 *    Provides the array of amplitude values.
 *
 * \param phs[len/2+1]
 *    Provides the array of phase values.
 *
 * \param conjugate
 *    Set to 0 (wfalse) for the normal case of computation.  Set to 1
 *    (wtrue) for the conjugate of the complex (freq(k), freq(len-k))
 *    that is, for (freq(k), -freq(len-k)).
 *
 * \param scale
 *    Provides the integer scale factor.
 *
 * \param [out] freq[len*scale]
 *    Proivdes the output buffer for the frequencies.
 */

void
polar_to_HC_scale
(
   long len,
   const double * amp,
   const double * phs,
   int conjugate,
   int scale,
   double * freq
)
{
   int i;
   for (i = 0; i < len * scale; i ++)
      freq[i] = 0.0;

   /* calc phase and amplitude (power) */

   freq[0] = amp[0];
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      double im;
      double rl = amp[i] * cos(phs[i]);
      if (conjugate)
         im = -amp[i] * sin(phs[i]);
      else
         im = +amp[i] * sin(phs[i]);

      freq[i*scale] = rl;
      freq[len*scale - i*scale] = im;
   }
   if (len % 2 == 0)
      freq[len] = amp[len/2];
}

/**
 *    Calculates the "z" values.
 *
 *    Z = X * Y.  That is:
 *
\verbatim
 * (rz + i iz) = (rx + i ix) * (ry + i iy)
 *             = (rx * ry - ix * iy) + i (rx * iy + ix * ry)
\endverbatim
 *
 *    In other words, calculates the phase and amplitude (power).
 *
 * \param len
 *    Provides the length of the arrays.
 *
 * \param x
 *    Provides the x array.
 *
 * \param y
 *    Provides the y array.
 *
 * \param [out] z
 *    Provides the output z array.
 */

void
HC_mul
(
   long len,
   const double * x,
   const double * y,
   double * z
)
{
   int i;


   z[0] = x[0] * y[0];
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      double rx = x[i];
      double ix = x[len - i];
      double ry = y[i];
      double iy = y[len - i];
      z[i]       = rx * ry - ix * iy;
      z[len - i] = rx * iy + ix * ry;
   }
   if (len % 2 == 0)
      z[len/2] = x[len/2] * y[len/2];
}

/**
 *    Calculates the "z" values.
 *
 *    Z = X / Y.  That is:
 *
\verbatim
 * (rz + i iz) = (rx + i ix) / (ry + i iy)
 *             = (rx + i ix) * (ry - i iy) / (ry*ry + iy*iy)
 *             = (rx*ry + ix*iy + i (ix*ry - rx*iy)) / (ry*ry + iy*iy)
\endverbatim
 *
 * \param len
 *    Provides the length of the arrays.
 *
 * \param x
 *    Provides the x array.
 *
 * \param y
 *    Provides the y array.
 *
 * \param [out] z
 *    Provides the output z array.
 */

void
HC_div
(
   long len,
   const double * x,
   const double * y,
   double * z
)
{
   int i;
   z[0] = x[0] / y[0];
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      double rx = x[i];
      double ix = x[len - i];
      double ry = y[i];
      double iy = y[len - i];
      double den = ry * ry + iy * iy;
      z[i]     = (rx*ry + ix*iy) / den;
      z[len-i] = (ix*ry - rx*iy) / den;
   }
   if (len % 2 == 0)
      z[len/2] = x[len/2] / y[len/2];
}

/**
 *    Calculates the "z" values.
 *
 * \param len
 *    Provides the length of the arrays.
 *
 * \param x
 *    Provides the x array.
 *
 * \param [out] z
 *    Provides the output z array.
 */

void
HC_abs
(
   long len,
   const double * x,
   double * z
)
{
   int i;
   z[0] = fabs (x[0]);
   for (i = 1; i < (len + 1) / 2; i ++)
   {
      double rx = x[i];
      double ix = x[len - i];
      z[i]     = sqrt (rx * rx + ix * ix);
      z[len-i] = 0.0;
   }
   if (len % 2 == 0)
      z[len/2] = fabs(x[len/2]);
}

/**
 *    Calculates the "z" values for the "Puckette" lock..
 *
 * \note:
 *    y cannot be z!
 *
 * \param len
 *    Provides the length of the arrays.
 *
 * \param y
 *    Provides the y array.
 *
 * \param [out] z
 *    Provides the output z array.
 */

void
HC_puckette_lock (long len, const double * y, double * z)
{
   int k;
   z[0] = y[0];
   for (k = 1; k < (len + 1) / 2; k ++)
   {
      z[k]       = y[k];
      z[len - k] = y[len - k];
      if (k > 1)
      {
         z[k]       += y[k - 1];
         z[len - k] += y[len - (k - 1)];
      }
      if (k < (len + 1) / 2 - 1)
      {
         z[k]       += y[k + 1];
         z[len - k] += y[len - (k + 1)];
      }
   }
   if (len % 2 == 0)
      z[len/2] = y[len/2];
}

/**
 *    Provides the following calculations to implement a complex-phase
 *    vocoder:
 *
\verbatim
      Y[u_i] = X[t_i] (Y[u_{i-1}]/X[s_i]) / |Y[u_{i-1}]/X[s_i]|
\endverbatim
 *
 * \reference
 *    M.Puckette (1995).
 *
 * \param len
 *    Provides the length of the buffers.
 *
 * \param fs[]
 *    Provides X[s_i], analysis-FFT at starting time of i step
 *
 * \param ft[]
 *    Provides X[t_i], analysis-FFT at terminal time of i step.
 *    Note: t_i - s_i = u_i - u_{i-1} = hop_out
 *
 * \param f_out_old[]
 *    Provides Y[u_{i-1}], synthesis-FFT at (i-1) step
 *
 * \param [out] f_out[]
 *    Provides Y[u_i], synthesis-FFT at i step.
 *    ~ou can use the same point f_out_old[] for this.
 */

void
HC_complex_phase_vocoder
(
   int len,
   const double * fs,
   const double * ft,
   const double * f_out_old,
   double * f_out
)
{
   static double * tmp1 = nullptr;
   static double * tmp2 = nullptr;
   static int n0 = 0;
   if (tmp1 == nullptr)
   {
      tmp1 = (double *) malloc(sizeof(double) * len);
      tmp2 = (double *) malloc(sizeof(double) * len);
      CHECK_MALLOC(tmp1, "HC_complex_phase_vocoder");
      CHECK_MALLOC(tmp2, "HC_complex_phase_vocoder");
      n0 = len;
   }
   else if (n0 < len)
   {
      tmp1 = (double *) realloc(tmp1, sizeof (double) * len);
      tmp2 = (double *) realloc(tmp2, sizeof (double) * len);
      CHECK_MALLOC(tmp1, "HC_complex_phase_vocoder");
      CHECK_MALLOC(tmp2, "HC_complex_phase_vocoder");
      n0 = len;
   }
   HC_div(len, f_out_old, fs, tmp1);   /* tmp1 = Y[u_{i-1}]/X[s(i)]     */
   HC_abs(len, tmp1, tmp2);            /* tmp2 = |Y[u_{i-1}]/X[s(i)]|   */

   /*
    * tmp1 = (Y[u_{i-1}]/X[s_i]) / |Y[u_{i-1}]/X[s_i]|
    */

   HC_div(len, tmp1, tmp2, tmp1);

   /*
    * f_out = X[t_i] (Y[u_{i-1}]/X[s_i]) / |Y[u_{i-1}]/X[s_i]|
    */

   HC_mul(len, ft, tmp1, f_out);
}

/*
 * hc.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
