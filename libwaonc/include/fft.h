#ifndef WAONC_FFT_H_
#define WAONC_FFT_H_

/*
 * Header file for fft.c
 *
 * FFT subroutine for WaoN with FFTW library
 *
 * Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: fft.h,v 1.7 2007/02/28 08:34:23 kichiki Exp $
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
 * \file          fft.h
 *
 *    This module provides functions for setting up FFT functions and
 *    implementing them.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-22
 * \version       $Revision$
 * \license       GNU GPL
 *
 * http://www.fftw.org/:
 *
 *    Our benchmarks, performed on on a variety of platforms, show that
 *    FFTW's performance is typically superior to that of other publicly
 *    available FFT software, and is even competitive with vendor-tuned
 *    codes. In contrast to vendor-tuned codes, however, FFTW's
 *    performance is portable: the same program will perform well on most
 *    architectures without modification. Hence the name, "FFTW," which
 *    stands for the somewhat whimsical title of "Fastest Fourier
 *    Transform in the West."
 */

/*
 * Other modules besides fft.c need the information in this header file.
 */

#ifdef FFTW2
#include <rfftw.h>                     /* FFTW library                        */
#else
#include <fftw3.h>
#endif

#include "macros.h"                    /* wbool_t, errprint() macros          */

/**
 *    Provides manifest constants for the type of filter window.
 *
 *    See http://en.wikipedia.org/wiki/Window_function.
 *
 * @todo
 *    Some of the values are not yet supported.
 *
 * @todo
 *    We should add a triangular window (B-spline k = 2) as well.
 *
 * @var FILTER_WINDOW_NONE
 *    Provides a rectangular window, also known as the boxcar or Dirichlet
 *    window.  Introduces a wide spread in the spectrum.
 *    One of the B-spline windows (k = 1).
 *
 * @var FILTER_WINDOW_PARZEN
 *    The Parzen window, also known as the <i>de la Vallé Poussin
 *    window</i>.
 *    One of the B-spline windows, of the fourth order (k = 4).
 *
 * @var FILTER_WINDOW_WELCH
 *    Provides the Welch window, which forms a single parabolic section.
 *
 * @var FILTER_WINDOW_HANNING
 *    Provides the Hanning window, a raised cosine window.
 *    This filter window is the default filter window for waon.
 *
 * @var FILTER_WINDOW_HAMMING
 *    Provides the Hamming windows, a specific type of cosine window.
 *    The window is optimized to minimize the maximum (nearest) side lobe,
 *    giving it a height of about one-fifth that of the Hann window.
 *
 * @var FILTER_WINDOW_BLACKMAN
 *    Similar to the Hanning/Hamming windows, but provides an even more
 *    narrow frequency representation.
 *
 * @var FILTER_WINDOW_STEEPER
 *    Provides a steeper 6 30-dB/octave rolloff window.
 *
 * @var FILTER_WINDOW_MAX
 *    Indicates one greater than the maximum value of this parameter.
 *    Values greater than or equal to this value are currently not
 *    supported.
 *
 * @var FILTER_WINDOW_NUTTALL
 *    Provides an even more narrow frequency range than the Nuttall
 *    window.  NOT YET SUPPORTED.
 *
 * @var FILTER_WINDOW_TRIANGLE
 *    Provides a triangular window.  NOT YET SUPPORTED.
 */

typedef enum
{
   FILTER_WINDOW_NONE,
   FILTER_WINDOW_PARZEN,
   FILTER_WINDOW_WELCH,
   FILTER_WINDOW_HANNING,
   FILTER_WINDOW_HAMMING,
   FILTER_WINDOW_BLACKMAN,
   FILTER_WINDOW_STEEPER,
   FILTER_WINDOW_MAX,
   FILTER_WINDOW_NUTTALL,
   FILTER_WINDOW_TRIANGLE

} filter_window_t;

/*
 * Global function declarations
 */

extern filter_window_t fft_window_value (const char * value);
extern double parzen (int i, int nn);
extern double welch (int i, int nn);
extern double hanning (int i, int nn);
extern double hamming (int i, int nn);
extern double blackman (int i, int nn);
extern double steeper (int i, int nn);
extern wbool_t windowing
(
   int n,
   const double * data,
   filter_window_t flag_window,
   double scale,
   double * out
);
extern void fprint_window_name (FILE * out, filter_window_t flag_window);
extern void apply_FFT
(
   int len,
   const double * data,
   filter_window_t flag_window,
   fftw_plan plan,
   double *in,
   double *out,
   double scale,
   double * amp,
   double * phs
);
extern double init_den (int n, filter_window_t flag_window);
extern void power_spectrum_fftw
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
);
extern void power_subtract_ave (int n, double * p, int m, double factor);
extern void power_subtract_octave (int n, double * p, double factor);

#endif         /* WAONC_FFT_H_ */

/*
 * fft.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
