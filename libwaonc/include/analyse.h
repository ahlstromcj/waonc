#ifndef WAONC_ANALYSE_H_
#define WAONC_ANALYSE_H_

/*
 * Header file for analyse.c
 *
 * Routines to analyse power spectrum and output notes
 *
 * Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: analyse.h,v 1.5 2007/11/04 23:46:03 kichiki Exp $
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
 * \file          analyse.h
 *
 *    This module provides functions for analysing the power spectrum and
 *    outputing notes.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-22
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    Note that this version of the module eliminates global variables and
 *    provides additional parameters for scratchpad data.
 */

#include "macros.h"                    /* wbool_t and errprint() macros       */
#include "fft.h"                       /* power_spectrum_fftw()            */

/**
 *    Provides a way to collect some global variables for easier
 *    comprehension and maintenance.
 *
 *    Instead of providing a number of global variables, or even a global
 *    structure, this structure is meant to consolidate all of the
 *    formerly-global variables into a data structure that is created in
 *    the main routine, and then passed to the functions that need it.
 */

typedef struct
{
   /**
    * Flag for absolute/relative cutoff frequency. 0 indicates a relative
    * cutoff frequency; 1 indicates an absolute cutoff.
    *
    * Selected by option --relative or -r.  The numeric parameter for this
    * option determines 'rel_cut_ratio', the log10 of the cutoff ratio
    * relative to the average, which defaults to 1.0.
    */

   wbool_t absolute_cutoff;               /* abs_flg     */

   /**
    * Flag for using patch file.
    */

   wbool_t use_patchfile;                 /* patch_flg   */

   /**
    * Provides the work area for the patch.
    */

   double * patch_array;                  /* pat[]       */

   /**
    * Provides the number of data items in the patch array.
    */

   int patch_array_size;                  /* npat        */

   /**
    * Holds the maximum power value.
    */

   double maximum_power;                  /* p0          */

   /**
    * Provides the frequency point of the maximum power value.
    */

   double maximum_power_freq;             /* if0         */

} analysis_scratchpad_t;

/*
 * Global functions for the analyse module.  These functions are
 * documented in the C file.
 */

extern wbool_t analysis_scratchpad_initialize
(
   analysis_scratchpad_t * parameters
);
extern void note_intensity
(
   double * p, double * fp,
   double cut_ratio, double rel_cut_ratio,
   int i0, int i1,
   double t0, char * intens,
   analysis_scratchpad_t * parameters
);
extern void average_FFT_into_midi
(
   int len, double samplerate,
   const double * amp2, const double * dphi, double * ave2
);
extern void pickup_notes
(
   double * amp2midi,
   double cut_ratio, double rel_cut_ratio,
   int i0, int i1,
   wbool_t abs_flg,
   char * intens
);
extern double patch_power
(
   double freq_ratio,
   analysis_scratchpad_t * parameters
);
extern void init_patch
(
   char * file_patch,
   int plen,
   filter_window_t nwin,
   analysis_scratchpad_t * aparms
);

#endif         /* WAONC_ANALYSE_H_ */

/*
 * analyse.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
