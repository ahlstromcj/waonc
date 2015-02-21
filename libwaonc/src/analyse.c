/*
 * Routines to analyse power spectrum and output notes
 *
 * Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: analyse.c,v 1.8 2007/11/04 23:45:39 kichiki Exp $
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**
 * \file          analyse.c
 *
 *    This module provides functions for analysing notes.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-22
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    This module provides functions for stage 2 of wave-to-MIDI processing:
 *    the note selection process.
 */

#include <stdio.h>                     /* fprintf()                        */
#include <stdlib.h>                    /* malloc()                         */
#include <string.h>                    /* memset()                         */
#include <math.h>                      /* log10()                          */
#include <sys/errno.h>                 /* errno                            */
#include <sndfile.h>                   /* libsndfile                       */

#include "macros.h"                    /* MIDI manifest constants          */
#include "analyse.h"
#include "memory-check.h"              /* CHECK_MALLOC() macro             */
#include "midi.h"                      /* get_note()                       */
#include "snd.h"

/**
 *    Initializes an analysis_scratchpad_t structure.
 *
 * \param parameters
 *    This structure provides the parameters of WaoN processing that used
 *    to be global variables.  Since we've refactored the waon application
 *    into a new application, waonc, that is now distributed as a main
 *    module and a library, it is no longer wise to use global
 *    variables.
 *
 * \return
 *    Returns true (wtrue) if the parameters structure pointer was not
 *    null.
 */

wbool_t
analysis_scratchpad_initialize (analysis_scratchpad_t * parameters)
{
   wbool_t result = not_nullptr(parameters);
   if (result)
   {
      parameters->absolute_cutoff = DEFAULT_USE_ABSOLUTE_CUTOFF;
      parameters->use_patchfile = wfalse;
      parameters->patch_array = nullptr;
      parameters->patch_array_size = 0;
      parameters->maximum_power = 0.0;
      parameters->maximum_power_freq = 0.0;
   }
   return result;
}

/**
 *    Gets the intensity of notes from the power spectrum.
 *
 * \param p
 *    Provides the power spectrum array.
 *
 * \param fp
 *    Provides frequencies for each bin, in an array.  If null, the
 *    center frequencies are used.
 *
 * \param cut_ratio
 *    Provides the log10 of cutoff ratio to use to scale the velocity.
 *
 * \param rel_cut_ratio
 *    Provides the log10 of cutoff ratio relative to the average.  0 means
 *    that the cutoff is equal to the average.
 *
 * \param i0,
 *    The beginning of the frequency range.
 *
 * \param i1
 *    The end of the frequency range.
 *
 * \param t0
 *    Center frequency???????????????????
 *
 * \param [out] intens
 *    Provides the intensity [0,128) for each MIDI note [0, 128).
 *
 * \param aparms
 *    Provides (among other values) the flag for the absolute/relative
 *    cutoff parameter.  Values:
 *       -  0 (wfalse) for relative
 *       -  1 (wtrue) for absolute
 *    Also provide a variable that indicates whether a patch file is used
 *    or not.
 */

void
note_intensity
(
   double * p,
   double * fp,
   double cut_ratio,
   double rel_cut_ratio,
   int i0,
   int i1,
   double t0,
   char * intens,
   analysis_scratchpad_t * aparms
)
{
   int i;
   int imax;
   double max;
   double x;
   double freq;                           /* frequency of the peak in power   */
   double f;
   int in;
   double av = 1.0;                       /* assume absolute cutoff at first  */
   for (i = 0; i < MIDI_NOTE_COUNT; i++)  /* clear the intensity array        */
      intens[i] = 0;                      /* \optimization use memset()       */

   /**
    * If using the relative-cutoff option, obtain the average power over
    * the [i0, i1) frequency range.
    */

   if (! aparms->absolute_cutoff)      /* see the "for (;;)" loop below       */
   {
      av = 0.0;
      for (i = i0; i < i1; i++)
         av += p[i];

      av /= (double)(i1 - i0);         /* average power over frequency range  */
   }

   /**
    * Next, search for the intensity peak.  Set the threshold to the
    * average.  If absolute-cutoff is used, set the maximum to
    * 10^cut_ratio.  Otherwise, set the maximum to the average power times
    * 10^rel_cut_ratio.
    */

   for (;;)
   {
      if (aparms->absolute_cutoff)
         max = pow(10.0, cut_ratio);
      else
         max = av * pow(10.0, rel_cut_ratio);

      imax = -1;
      for (i = i0; i < i1; i++)
      {
         if (p[i] > max)
         {
            max = p[i];
            imax = i;
         }
      }
      if (imax == -1)                  /* no peak over average was found      */
         break;

      /**
       * Then get the MIDI note number from imax (the FFT frequency index).
       */

      if (is_nullptr(fp))              /* use the center frequencies          */
         freq = (double) imax / t0;
      else
         freq = fp[imax];              /* use specified frequency bins        */

      in = get_note(freq);             /* midi note number                    */
      if (in >= i0 && in <= i1)        /* check  the range of the note        */
      {
         /**
          * For the first time on each note, scale the intensity
          * (velocity) of the peak power range from 10^cut_ratio to 10^0.
          * Intensity is clipped to the range [0, 128).
          */

         if (intens[in] == 0)          /* if first time on the note...        */
         {
            x = 127.0 / (double) (-cut_ratio) *
               (log10(p[imax]) - (double) cut_ratio);

            if (x >= 128.0)
               intens[in] = 127;       /* clip intensity to legal MIDI range  */
            else if (x > 0)
               intens[in] = (int) x;
         }
      }

      /**
       * Next, this function subtracts the  peak up to minimum in both sides.
       * Optionally, a patch file is used.  [NEED TO LEARN THE EXACT
       * PURPOSE OF THE PATCH FILE.]
       */

      if (aparms->use_patchfile)
      {
         for (i = i0; i < i1; i++)
         {
            if (is_nullptr(fp))        /* use the center frequencies          */
               f = (double) i / t0;
            else
               f = fp[i];              /* use specified frequency bins        */

            p[i] -= max * patch_power(f/freq, aparms);
            if (p[i] < 0)
               p[i] = 0;
         }
      }
      else
      {
         p[imax] = 0.0;
         for                           /* right side */
         (
            i = imax + 1; p[i] != 0.0 && i < (i1-1) && p[i] >= p[i+1]; i++
         )
         {
            p[i] = 0.0;
         }
         if (i == i1-1)
            p[i] = 0.0;

         for                           /* left side */
         (
            i = imax - 1; p[i] != 0.0 && i > i0 && p[i - 1] <= p[i]; i--
         )
         {
            p[i] = 0.0;
         }
         if (i == i0)
            p[i] = 0.0;
      }
   }
}

/**
 *    Converts FFT information into MIDI.
 *
 * \note
 *    The value len/2 + 1 is the center of the buffer.
 *
 * \param len
 *    Provides the length of the power (amplitude-squared) and PV
 *    frequency correction arrays.
 *
 * \param samplerate
 *    Provides the sampling rate in cycles per second.
 *
 * \param amp2 [(len/2)+1]
 *    Provides the power spectrum (amp^2, amplitude squared)
 *
 * \param dphi [(len/2)+1]
 *    Provides the PV frequency correction factor defined by
 *    (1/2pi hop)principal(phi - phi0 - Omega).
 *    Therefore, the corrected frequency is (k/len +
 *    dphi[k])*samplerate [Hz].  If this parameter is
 *    null, then the plain FFT power spectrum has been provided.
 *
 * \param [out] ave2 [128]
 *    Provides the averaged amplitude-squared value for each MIDI note.
 */

void
average_FFT_into_midi
(
   int len,
   double samplerate,
   const double * amp2,
   const double * dphi,
   double * ave2
)
{
   int k;
   int midi;
   double f;                           /* corrected frequency                 */
   int * n = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   CHECK_MALLOC(n, "average_FFT_into_midi");
   for (midi = 0; midi < MIDI_NOTE_COUNT; midi++)
   {
      ave2[midi] = 0.0;
      n[midi] = 0;
   }
   for (k = 1; k < (len+1)/2; k++)
   {
      if (is_nullptr(dphi))            /* plain FFT power spectrum            */
         f = (double) k / (double) len * samplerate;
      else
         f = ((double) k / (double) len + dphi[k]) * samplerate;

      midi = freq_to_midi(f);
      if (midi >= 0 && midi < MIDI_NOTE_COUNT)
      {
         ave2[midi] += sqrt(amp2[k]);
         n[midi]++;
      }

      /*
       * Any need to report or note out-of-range values?
       */
   }
   for (midi = 0; midi < MIDI_NOTE_COUNT; midi ++)    /* average and square   */
   {
      if (n[midi] > 0)
      {
         ave2[midi] = ave2[midi] / (double) n[midi];  /* average              */
         ave2[midi] = ave2[midi] * ave2[midi];        /* square               */
      }
   }
   free(n);
}

/**
 *    Picks up notes and its power from a table of power for each MIDI note.
 *
 * \param amp2midi [128]
 *    Provides the amplitude-squared (power) value for each MIDI note.
 *
 * \param cut_ratio
 *    Provides the log10 of the cutoff ratio to scale velocity.
 *
 * \param rel_cut_ratio
 *    Provides the log10 of the cutoff ratio relative to average.  0 means
 *    cutoff is equal to average
 *
 * \param i0
 *    Provides the beginning of the MIDI note range.  It is <i> not </i> a
 *    frequency index.
 *
 * \param i1
 *    Provides the end of the MIDI note range.  It is <i> not </i> a
 *    frequency index.
 *
 * \param abs_flag
 *    Provides the flag for the absolute versus relative cutoff.  0
 *    (wfalse) for relative, 1 (wtrue) for absolute.  This is a new
 *    parameter to replace the usage of a global variable.
 *
 * \param [out] intens[]
 *    Provides the output intensity with 127 [128!] elements (the number
 *    of possible MIDI notes).
 */

void
pickup_notes
(
   double * amp2midi,
   double cut_ratio,
   double rel_cut_ratio,
   int i0,
   int i1,
   wbool_t abs_flg,                    /* NEW PARAMETER */
   char * intens
)
{
   double oct_fac = 0.0;               /* = 0.5;  octave harmonics factor */
   int imax;
   double max;
   double x;
   int in;
   double av = 1.0;
   int i;
   for (i = 0; i < MIDI_NOTE_COUNT; i++)           /* clear */
      intens[i] = 0;

   if (! abs_flg)
   {
      /*
       * This could be a candidate for a function.
       */

      av = 0.0;
      for (i = i0; i < i1; i++)
         av += amp2midi[i];            /* calc average power */

      av /= (double) (i1 - i0);
   }
   for (;;)
   {
      /*
       * Search peak, set the threshold to the average.
       */

      if (! abs_flg)
         max = av * pow(10.0, rel_cut_ratio);
      else
         max = pow(10.0, cut_ratio);

      imax = -1;
      for (i = i0; i < i1; i++)
      {
         if (amp2midi[i] > max)
         {
            max = amp2midi[i];
            imax = i;
         }
      }
      if (imax == -1)                  /* no peak found                       */
         break;

      in = imax;                       /* so that imax is THE midi note       */
      if (intens[in] == 0)             /* if second time on same note, skip   */
      {
         /*
          * Scale intensity (velocity) of the peak.  The power range from
          * 10^cut_ratio to 10^0 is scaled.
          */

         x = (double) MIDI_NOTE_MAX / (double) (-cut_ratio) *
            (log10(amp2midi[in]) - (double) cut_ratio);

         if (x >= (double) MIDI_NOTE_COUNT)
            intens[in] = MIDI_NOTE_MAX;
         else if (x > 0)
            intens[in] = (int) x;

         if (oct_fac > 0.0)            /* octave harmonics reduction          */
         {
            /*
             * Chris:  this loop looks incorrect.   As is, it operates one
             * the same note (in+12) a few times.  The change doesn't
             * affect the default output of processing ca_doremi.wav with
             * waonc.
             */

#ifdef USE_ORIGINAL_CODE
            for (i = in + 12; in < 128; in += 12)
#else
            for
            (
               i = in + MIDI_NOTE_OCTAVE;
               i < MIDI_NOTE_COUNT;
               i += MIDI_NOTE_OCTAVE
            )
#endif
            {
               amp2midi[i] = sqrt(amp2midi[i]) -
                  oct_fac * sqrt(amp2midi[i-MIDI_NOTE_OCTAVE]);

               if (amp2midi[i] < 0.0)
                  amp2midi[i] = 0.0;
               else
                  amp2midi[i] = amp2midi[i] * amp2midi[i];
            }
         }
      }
      amp2midi[imax] = 0.0;            /* subtract the peak bin */
   }
}

/**
 *    Return power of patch relative to its maximum at the freqency where
 *    the ratio to the maximum is 'freq_ratio'.
 *
 * \param freq_ratio
 *    Provides the desired ratio.
 *
 * \param aparms
 *    Provides (among other values) the maximum power, the  maximum power
 *    frequency, the patch array, and the patch array-size.
 *
 * \return
 *    Returns the relative power of the patch.
 */

double
patch_power
(
   double freq_ratio,
   analysis_scratchpad_t * aparms
)
{
   double f = (double) aparms->maximum_power_freq * freq_ratio;
   int i0 = (int) f;
   int i1 = i0 + 1;
   if (i0 < 1 || i1 > aparms->patch_array_size)
      return 0.0;
   else
   {
      double dpdf = aparms->patch_array[i1] - aparms->patch_array[i0];
      double p = aparms->patch_array[i0] + dpdf * (f - (double) i0);
      return p / aparms->maximum_power;
   }
}

/**
 *    Initializes the patch.
 *
 * \param file_patch
 *    Provide the filename of the patch-file.
 *
 * \param plen
 *    Provides the number of data points in the patch (wav).
 *
 * \param nwin
 *    Provides the index of windowing function to use.
 *
 * \param aparms
 *    Provides output for the the following values:
 *
 *       -  Formerly pat[], now provided by patch_array, is the power of patch.
 *       -  Formerly npat, now provided by patch_array_size, is the number
 *          of datum elements in pat[] [plen/2+1].
 *       -  Formerly p0, now provided by maximum_power, the maximum of
 *          the power.
 *       -  Formerly if0, now provided by maximum_power_freq, the
 *          frequency of maximum power.
 */

void
init_patch
(
   char * file_patch,
   int plen,
   filter_window_t nwin,
   analysis_scratchpad_t * aparms
)
{
   if (is_nullptr(file_patch))         /* prepare patch  */
   {
      aparms->use_patchfile = wfalse;
      return;
   }
   else
   {
      double * x = nullptr;
      double * xx = nullptr;
      double * y = nullptr;
      SNDFILE * sf = nullptr;
      SF_INFO sfinfo;
      double den;
      int i;

#ifdef FFTW2
      rfftw_plan plan;
#else
      fftw_plan plan;
#endif

      /* allocate pat[]  */

      if (not_nullptr(aparms->patch_array))
      {
         infoprint("patch array already allocated, freeing it");
         free(aparms->patch_array);
      }
      aparms->patch_array = (double *) malloc(sizeof(double) * (plen/2 + 1));
      if (is_nullptr(aparms->patch_array))
      {
         fprintf(stderr, "? cannot allocate pat[%d]\n", (plen/2 + 1));
         aparms->use_patchfile = wfalse;
         return;
      }
      x  = (double *) malloc(sizeof(double) * plen);
      xx = (double *) malloc(sizeof(double) * plen);
      if (is_nullptr(x) || is_nullptr(xx))
      {
         fprintf(stderr, "? cannot allocate x[%d]\n", plen);
         aparms->use_patchfile = wfalse;
         return;
      }
      y = (double *) malloc(sizeof(double) * plen);      /* spectrum for FFT  */
      if (is_nullptr(y))
      {
         fprintf(stderr, "? cannot allocate y[%d]\n", plen);
         aparms->use_patchfile = wfalse;
         free(x);
         free(xx);
         return;
      }
      sf = sf_open(file_patch, SFM_READ, &sfinfo);       /* open patch file   */
      if (is_nullptr(sf))
      {
         fprintf
         (
            stderr, "? cannot open patch file %s, aborting: %s\n",
            file_patch, strerror(errno)
         );
         exit(1);
      }
      if (sndfile_read(sf, sfinfo, x, xx, plen) != plen) /* read patch wav    */
      {
         fprintf(stderr, "no patch data");
         aparms->use_patchfile = wfalse;
         free(x);
         free(xx);
         free(y);
         return;
      }
      if (sfinfo.channels == 2)
      {
         for (i = 0; i < plen; i ++)
            x[i] = 0.5 * (x[i] + xx[i]);
      }
      den = init_den(plen, nwin);                        /* calc patch power  */

#ifdef FFTW2
      plan = rfftw_create_plan(plen, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
#else
      plan = fftw_plan_r2r_1d(plen, x, y, FFTW_R2HC, FFTW_ESTIMATE);
#endif

      power_spectrum_fftw(plen, x, y, aparms->patch_array, den, nwin, plan);
      fftw_destroy_plan(plan);
      free(x);
      free(xx);
      free(y);
      sf_close(sf);
      aparms->maximum_power = 0.0;
      aparms->maximum_power_freq = (double) WAON_UNINITIALIZED;
      for (i = 0; i < plen / 2; i++)                     /* find maximum      */
      {
         if (aparms->patch_array[i] > aparms->maximum_power)
         {
            aparms->maximum_power = aparms->patch_array[i];
            aparms->maximum_power_freq = (double) i;
         }
      }
      if (aparms->maximum_power_freq == (double) WAON_UNINITIALIZED)
         aparms->use_patchfile = wfalse;

      aparms->patch_array_size = plen / 2;
      aparms->use_patchfile = wtrue;
   }
}

/*
 * analyse.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
