/*
 * FFT subroutine for WaoN with FFTW library
 * Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: fft.c,v 1.9 2007/11/04 23:44:44 kichiki Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**
 * \file          fft.c
 *
 *    This module provides functions for setting up FFT functions and
 *    implementing them.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2014-01-01
 * \version       $Revision$
 * \license       GNU GPL
 *
 * \references
 *    -# "Numerical Recipes in C" 2nd Ed.
 *       by W.H.Press, S.A.Teukolsky, W.T.Vetterling, B.P.Flannery
 *       (1992) Cambridge University Press.
 *       ISBN 0-521-43108-5.
 *       Sec.13.4 - Data Windowing
 *    -# "Digital Filters and Signal Processing" 2nd Ed.
 *       by L. B. Jackson. (1989) Kluwer Academic Publishers.
 *       ISBN 1-89838-276-9.
 *       Sec.7.3 - Windows in Spectrum Analysis. (Hamming window).
 */

#include <math.h>
#include <stdlib.h>                    /* realloc()                           */
#include <strings.h>                   /* GNU: strncasecmp()                  */
#include <stdio.h>                     /* fprintf()                           */

#include "fft.h"                       /* windowing() etc.                    */
#include "macros.h"                    /* wbool_t, errprint() macros          */
#include "memory-check.h"              /* CHECK_MALLOC() macro                */
#include "hc.h"                        /* HC_to_amp2()                        */

/*
 * @gcc
 *    The original implementation seems to be very slightly faster,
 *    surprisingly.
 */

#define USE_ORIGINAL_WINDOWING_IMPLEMENTATION

#ifndef USE_ORIGINAL_WINDOWING_IMPLEMENTATION

/**
 *    Provides a function pointer for a windowing function.
 *
 *    This typedef provides an internal implementation detail used to
 *    speed up the windowing() function.
 */

typedef double (* window_func_t) (int, int);

#endif

/**
 *    Converts a string to the corresponding windowing-flag value.
 *
 *    The allowable values are the last part of the enumeration values in
 *    the filter_window_t type.
 *
 *    Only the first three characters of the filter-name are required.
 *    The match is case-insensitive.  That's all the fanciness we need.
 *
 * \param value
 *    Provides the string to convert to an integer filter-window value.
 *
 * \return
 *    Returns the proper value if the string is a match, otherwise
 *    FILTER_WINDOW_MAX is returned, which indicates an error in the value
 *    string.
 */

filter_window_t
fft_window_value (const char * value)
{
   filter_window_t result = FILTER_WINDOW_MAX;
   if (strncasecmp(value, "none", 3) == 0)
      result = FILTER_WINDOW_NONE;
   else if (strncasecmp(value, "rectangular", 3) == 0)
      result = FILTER_WINDOW_NONE;
   else if (strncasecmp(value, "parzen", 3) == 0)
      result = FILTER_WINDOW_PARZEN;
   else if (strncasecmp(value, "welch", 3) == 0)
      result = FILTER_WINDOW_WELCH;
   else if (strncasecmp(value, "hanning", 3) == 0)
      result = FILTER_WINDOW_HANNING;
   else if (strncasecmp(value, "hamming", 3) == 0)
      result = FILTER_WINDOW_HAMMING;
   else if (strncasecmp(value, "blackman", 3) == 0)
      result = FILTER_WINDOW_BLACKMAN;
   else if (strncasecmp(value, "steeper", 3) == 0)
      result = FILTER_WINDOW_STEEPER;

   /*
    * NOT YET SUPPORTED:
    *
   else if (strncasecmp(value, "nuttall", 3) == 0)
      result = FILTER_WINDOW_NUTTALL;
   else if (strncasecmp(value, "triangle", 3) == 0)
      result = FILTER_WINDOW_TRIANGLE;
    *
    */

   return result;
}

/**
 *    Implements the Parzen window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index one past the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
parzen (int i, int nn)
{
   return
   (
      1.0 - fabs(((double)i - 0.5 * (double)(nn - 1)) / (0.5 * (double)(nn + 1)))
   );
}

/**
 *    Implements the Welch window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index of the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
welch (int i, int nn)
{
   return
   (
      1.0 -
      (
         ((double)i - 0.5 * (double)(nn - 1)) / (0.5 * (double)(nn + 1))
      ) *
      (
         ((double)i - 0.5 * (double)(nn - 1)) / (0.5 * (double)(nn + 1))
      )
   );
}

/**
 *    Implements the Hanning window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index of the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
hanning (int i, int nn)
{
   return ( 0.5 * (1.0 - cos (2.0 * M_PI * (double)i / (double)(nn - 1))) );
}

/**
 *    Implements the Hamming window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index of the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
hamming (int i, int nn)
{
   return 0.54 - 0.46 * cos(2.0 * M_PI * (double)i / (double)(nn - 1));
}

/**
 *    Implements the Blackman window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index of the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
blackman (int i, int nn)
{
   return
   (
      0.42
         - 0.5  * cos(2.0 * M_PI * (double) i / (double)(nn - 1))
         + 0.08 * cos(4.0 * M_PI * (double) i / (double)(nn - 1))
   );
}

/**
 *    Implements the "steeper" window function.
 *
 * \param i
 *    Provides the index of the current data point in the window buffer.
 *
 * \param nn
 *    Provides the index of the last data point in the window buffer.
 *
 * \return
 *    Returns the scale-value for this window function at the specified
 *    point.
 */

double
steeper (int i, int nn)
{
   return
   (
       0.375
         - 0.5   * cos(2.0 * M_PI * (double) i / (double)(nn - 1))
         + 0.125 * cos(4.0 * M_PI * (double) i / (double)(nn - 1))
   );
}

/**
 *    Applies a window function to the data array.
 *
 * \param n
 *    Provides the size of the data buffer.
 *
 * \param data
 *    Provides a pointer to the data buffer.  The pointer is NOT checked.
 *
 * \param flag_window
 *    Indicates which windowing type will be applied to the data.  The
 *    type is one of the supported values in the filter_window_t
 *    enumeration.
 *
 * \param scale
 *    Provides a scaling factor for the data.
 *
 * \param out
 *    Provides the output buffer, which must not be null, and which must
 *    be larger enough (\a n) to hold the result of the Windowing.
 *    Not sure if it can be the same value as \a data.
 *
 * \return
 *    Returns wtrue (1) if the function succeeded, wfalse (0) otherwise.
 */

#ifdef USE_ORIGINAL_WINDOWING_IMPLEMENTATION

/*
 * Our attempted optimization does not seem to make a difference.
 */

wbool_t
windowing
(
   int n,
   const double * data,
   filter_window_t flag_window,
   double scale,
   double * out
)
{
   wbool_t result = wtrue;
   int i;
   for (i = 0; i < n; i++)
   {
      switch (flag_window)
      {
      case FILTER_WINDOW_NONE:               /* square (no window) */

         out[i] = data[i] / scale;
         break;

      case FILTER_WINDOW_PARZEN:

         out[i] = data[i] * parzen(i, n) / scale;
         break;

      case FILTER_WINDOW_WELCH:

         out[i] = data[i] * welch(i, n) / scale;
         break;

      case FILTER_WINDOW_HANNING:

         out[i] = data[i] * hanning(i, n) / scale;
         break;

      case FILTER_WINDOW_HAMMING:

         out[i] = data[i] * hamming(i, n) / scale;
         break;

      case FILTER_WINDOW_BLACKMAN:

         out[i] = data[i] * blackman(i, n) / scale;
         break;

      case FILTER_WINDOW_STEEPER: /* steeper 30-dB/octave rolloff window  */

         out[i] = data[i] * steeper(i, n) / scale;
         break;

      default:

         errprint("invalid windowing type");
         result = wfalse;
         goto window_end;
      }
   }

window_end:

   return result;
}

#else    /* ! USE_ORIGINAL_WINDOWING_IMPLEMENTATION */

wbool_t
windowing
(
   int n,
   const double * data,
   filter_window_t flag_window,
   double scale,
   double * out
)
{
   wbool_t result = wtrue;
   int i;
   window_func_t windfunc;
   double scale_reciprocal = 1.0 / scale;
   switch (flag_window)
   {
   case FILTER_WINDOW_NONE:            /* square (no window)   */

      windfunc = nullptr;              /* scaling only         */
      break;

   case FILTER_WINDOW_PARZEN:

      windfunc = parzen;
      break;

   case FILTER_WINDOW_WELCH:

      windfunc = welch;
      break;

   case FILTER_WINDOW_HANNING:

      windfunc = hanning;
      break;

   case FILTER_WINDOW_HAMMING:

      windfunc = hamming;
      break;

   case FILTER_WINDOW_BLACKMAN:

      windfunc = blackman;
      break;

   case FILTER_WINDOW_STEEPER: /* steeper 30-dB/octave rolloff window  */

      windfunc = steeper;
      break;

   default:

      errprint("invalid flag_window");
      result = wfalse;
      goto window_end;
   }
   if (not_nullptr(windfunc))
   {
      for (i = 0; i < n; i++)
         out[i] = data[i] * windfunc(i, n) * scale_reciprocal;
   }
   else
   {
      for (i = 0; i < n; i++)
         out[i] = data[i] * scale_reciprocal;
   }

window_end:

   return result;
}

#endif   /* USE_ORIGINAL_WINDOWING_IMPLEMENTATION */

/**
 *    Prints the window name to the given file.
 *
 * \param out
 *    The output file for the window name.
 *
 * \param flag_window
 *    Indicates the window type.
 */

void
fprint_window_name (FILE * out, filter_window_t flag_window)
{
   if (is_nullptr(out))
      out = stdout;

   switch (flag_window)
   {
   case FILTER_WINDOW_NONE:               /* square (no window) */

      fprintf(out, "No window (rectangular)\n");
      break;

   case FILTER_WINDOW_PARZEN:

      fprintf(out, "Parzen window\n");
      break;

   case FILTER_WINDOW_WELCH:

      fprintf(out, "Welch window\n");
      break;

   case FILTER_WINDOW_HANNING:

      fprintf(out, "Hanning window\n");
      break;

   case FILTER_WINDOW_HAMMING:

      fprintf(out, "Hamming window\n");
      break;

   case FILTER_WINDOW_BLACKMAN:

      fprintf(out, "Blackman window\n");
      break;

   case FILTER_WINDOW_STEEPER:

      fprintf(out, "Steeper 30-dB/octave rolloff window\n");
      break;

   default:
      fprintf(out, "Invalid window\n");
      break;
   }
}

/**
 *    Applies an FFT with the window and returns the amplitude and phase.
 *
 *    This function is mainly a wrapper for the phase-vocoder process. The
 *    steps:
 *
 *       -# Apply the windowing() function to the \a in buffer with a
 *          scale of 1.0.
 *       -# Call fftw_execute(plan).
 *       -# Call HC_to_polar() in order to.... TBD.
 *       -# Apply the scale to the amplitude portion of the output.
 *
 * \note
 *    I (CA) don't understand why the scale factor is applied after the
 *    FFT, rather than in the call to windowing().  A precision issue?
 *
 * \param len
 *    Provides the FFT length
 *
 * \param data[len]
 *    Provides the data to analyze
 *
 * \param flag_window
 *    Provides the window type
 *
 * \param plan
 *    Provides the "plan" for the FFTW3 algorithm.
 *
 * \param in[len]
 *    Provides the input for the FFTW3 algorithm.
 *
 * \param out[len]
 *    Provides the output for the FFTW3 algorithm.
 *
 * \param scale
 *    Provides the amplitude scale factor.
 *
 * \param amp[len/2+1]
 *    Provides the output amplitude multiplied by the factor "scale" above.
 *
 * \param phs[len/2+1]
 *    Provides the output phase.
 */

void
apply_FFT
(
   int len,
   const double * data,
   filter_window_t flag_window,
   fftw_plan plan,
   double * in,
   double * out,
   double scale,
   double * amp,
   double * phs
)
{
   if (windowing(len, data, flag_window, 1.0, in))
   {
      fftw_execute(plan);                    /* FFT: in[] -> out[]            */
      HC_to_polar(len, out, 0, amp, phs);    /* freq[] -> (amp, phs)          */
      if (scale != 1.0)
      {
         int i;
         for (i = 0; i < len/2+1; i++)
            amp[i] /= scale;
      }
   }
}

/**
 *    Prepares the window for the FFT.
 *
 *    For each value of i = 0 to n-1, the window amplitude is obtained
 *    from the selected windowing function.  Call this value "a(i)".  Then
 *    the resulting density factor is:
 *
\verbatim
 *                      2
 *       den = n SUM   a (i)
 *               i=1,n
\endverbatim
 *
 * \param n
 *    Provides the number of samples for the FFT.
 *
 * \param flag_window
 *    Provides the indicator for the desired kind of filtering window.
 *
 * \return
 *    Returns the density factor, den.
 */

double
init_den (int n, filter_window_t flag_window)
{
   double den = 0.0;
   int i;
   for (i = 0; i < n; i++)
   {
      double winresult;
      switch (flag_window)
      {
      case FILTER_WINDOW_NONE:               /* square (no window) */

         winresult = 1.0;
         break;

      case FILTER_WINDOW_PARZEN:

         winresult = parzen(i, n);
         break;

      case FILTER_WINDOW_WELCH:

         winresult = welch(i, n);
         break;

      case FILTER_WINDOW_HANNING:

         winresult = hanning(i, n);
         break;

      case FILTER_WINDOW_HAMMING:

         winresult = hamming(i, n);
         break;

      case FILTER_WINDOW_BLACKMAN:

         winresult = blackman(i, n);
         break;

      case FILTER_WINDOW_STEEPER: /* steeper 30-dB/octave rolloff window  */

         winresult = steeper(i, n);
         break;

      default:
         errprint("invalid flag_window");
      }
      den += winresult * winresult;
   }
   den *= (double) n;
   return den;
}

/**
 *    Calculates the power spectrum of real data x[n].
 *
 *    The steps are:
 *
 *       -# Apply the windowing() function with a scale of 2^32-1.
 *       -# Call fftw_execute(plan).
 *       -# Call HC_to_amp2().
 *
 * \param n
 *    Provides the number of data points in x.
 *
 * \param x[]
 *    Provides the data.
 *
 * \param den
 *    Provides the weight of window function; calculated by init_den().
 *
 * \param flag_window
 *    Provides the type of filter window to be applied.
 *
 * \param plan
 *    Provides the "plan" of the algorithm.  (Would like to figure out
 *    what a "plan" actually is!)
 *
 * \param [out] y[]
 *    Provides the buffer for output.  This buffer must be allocated
 *    before calling this function.  It provides the amplitude part of
 *    the Fourier transform of x[].
 *
 * \param [out] p[]
 *    Provides the phase output fo the Fourier transform.
 *
 * \param [out] p[(n+1)/2]
 *    Provides the stored-only n/2 data.
 */

void
power_spectrum_fftw
(
   int n,
   double * x,
   double * y,
   double * p,
   double den,
   filter_window_t flag_window,
#ifdef FFTW2
   rfftw_plan plan
#else
   fftw_plan plan
#endif
)
{
   static double maxamp = 2147483647.0;      /* 2^32-1            */
   windowing(n, x, flag_window, maxamp, x);  /* window            */

#ifdef FFTW2
   rfftw_one(plan, x, y);                    /* FFTW library      */
#else
   fftw_execute(plan);                       /* x[] -> y[]        */
#endif

   HC_to_amp2(n, y, den, p);
}

/**
 *    Subtracts the average from the power spectrum.
 *
 *    This function is intended to remove non-tonal signals (such as
 *    drums, percussions.)
 *
 * \param n
 *    Provides the FFT size.
 *
 * \param p[(n+1)/2]
 *    Provides the power spectrum.
 *
 * \param m
 *    Provides the number of bins to average out.
 *
 * \param factor
 *    Provides the factor * average is subtracted from the power:
 *       -  factor = 0.0 means no subtraction
 *       -  factor = 1.0 means full subtraction of the average
 *       -  factor = 2.0 means over subtraction
 *
 * \param [out] p[(n+1)/2]
 *    Provides the subtracted power spectrum as a side-effect.
 */

void
power_subtract_ave (int n, double * p, int m, double factor)
{
   static double * ave = nullptr;
   static int n_ave = 0;
   int nlen = n / 2 + 1;
   int i;
   int k;
   if (is_nullptr(ave))
   {
      ave = (double *) malloc(sizeof(double) * nlen);
      CHECK_MALLOC(ave, "power_subtract_ave");
      n_ave = nlen;
   }
   else if (n_ave < nlen)
   {
      ave = (double *) realloc(ave, sizeof(double) * nlen);
      CHECK_MALLOC(ave, "power_subtract_ave");
      n_ave = nlen;
   }
   for (i = 0; i < nlen; i++)         /* full span */
   {
      int nave = 0;
      ave[i] = 0.0;
      for (k = -m; k <= m; k++)
      {
         if ((i + k) < 0 || (i + k) >= nlen)
            continue;

         ave[i] += p[i + k];
         nave++;
      }
      if (nave > 1)
         ave[i] /= (double) nave;
   }
   for (i = 0; i < nlen; i++)         /* full span */
   {
      p[i] = sqrt(p[i]) - factor * sqrt(ave[i]);
      if (p[i] > 0.0)
         p[i] *= p[i];
      else
         p[i] = 0.0;
   }
   free(ave);
}

/**
 *    Provides an octave remover function.
 *
 * \param n
 *    Provides the FFT size.
 *
 * \param p[(n+1)/2]
 *    Provides the input power spectrum buffer.
 *
 * \param factor
 *    Provides the factor (factor * average is subtracted from the power):
 *       -  factor = 0.0 means no subtraction
 *       -  factor = 1.0 means full subtraction of the average
 *       -  factor = 2.0 means over subtraction
 *
 * \param [out] p[(n+1)/2]
 *    Provides the subtracted power spectrum.
 */

void
power_subtract_octave (int n, double * p, double factor)
{
   static double * oct = NULL;
   static int n_oct = 0;
   int nlen = (n + 1) / 2;
   int i;
   int i2;
   if (oct == NULL)
   {
      oct = (double *) malloc(sizeof(double) * (n / 2 + 1));
      CHECK_MALLOC(oct, "power_subtract_octave");
      n_oct = n / 2 + 1;
   }
   else if (n_oct < (n / 2 + 1))
   {
      oct = (double *) realloc(oct, sizeof(double) * (n / 2 + 1));
      CHECK_MALLOC(oct, "power_subtract_octave");
      n_oct = n / 2 + 1;
   }
   oct[0] = p[0];
   for (i = 1; i < nlen / 2 + 1; i++)
   {
      i2 = i * 2;
      if (i2 >= n / 2 + 1)
         break;

      oct[i2] = factor * p[i];
      if (i2 - 1 > 0)
         oct[i2 - 1] = 0.5 * factor * p[i];

      if (i2 + 1 < nlen)
         oct[i2 + 1] = 0.5 * factor * p[i];
   }
   for (i = 0; i < nlen; i++)          /* full span            */
   {
      p[i] = sqrt(p[i]) - factor * sqrt(oct[i]);
      if (p[i] > 0.0)
         p[i] *= p[i];
      else
         p[i] = 0.0;
   }
   free(oct);
}

/*
 * fft.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
