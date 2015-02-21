/*
 * WaoN - a Wave-to-Notes transcriber : main processing function
 *
 * Copyright (C) 1998-2008,2011 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: main.c,v 1.12 2011/12/27 13:11:00 kichiki Exp $
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
 * \file          processing.c
 *
 *    This module provides functions for generating notes.
 *
 * \library       waonc application
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-23
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    This module was created by moving the main functionality of the waonc
 *    main() function into this module.
 */

#include <ctype.h>                     /* isdigit()                           */
#include <math.h>                      /* atoi()                              */
#include <stdio.h>                     /* printf(), fprintf(), strerror()     */
#include <sys/errno.h>                 /* errno                               */
#include <stdlib.h>                    /* exit()                              */
#include <string.h>                    /* strcat(), strcpy()                  */
#include <sndfile.h>                   /* libsndfile                          */

#include "memory-check.h"              /* CHECK_MALLOC() macro                */
#include "fft.h"                       /* waon FFT utility functions          */
#include "hc.h"                        /* HC array manipulation routines      */
#include "snd.h"                       /* wrapper for the libsndfile library  */
#include "midi.h"                      /* smf_...(), mid2freq[], get_note()   */
#include "analyse.h"                   /* note_intensity(), note_on_off(), ...*/
#include "notes.h"                     /* waon_notes_t                        */
#include "parameters.h"                /* waon_parameters_t                   */

wbool_t
processing
(
   waon_parameters_t * waon_parameters,
   analysis_scratchpad_t * analysis_scratchpad
)
{
   wbool_t result = not_nullptr(waon_parameters);
   if (result)
   {
      waon_notes_t * notes = WAON_notes_init();
      long fft_len = waon_parameters->fft_len;  /* this one is used a lot           */
      char vel[128];                      /* velocity at the current step     */
      int on_event[128];                  /* event index of waon_notes_t      */
      double * left  = (double *) malloc(sizeof(double) * fft_len);
      double * right = (double *) malloc(sizeof(double) * fft_len);
      double * x = nullptr;               /* wave data for FFT                */
      double * y = nullptr;               /* spectrum data for FFT            */
      double * p = nullptr;               /* power spectrum                   */
      double * p0 = nullptr;
      double * dphi = nullptr;
      double * ph0 = nullptr;
      double * ph1 = nullptr;
      double * pmidi = nullptr;
      SNDFILE * sf = nullptr;
      SF_INFO sfinfo;
      double t0;
      double den;
      int i0, i1;
      int i, sum;
#ifdef FFTW2
      rfftw_plan plan;
#else /* FFTW3 */
      fftw_plan plan;
#endif
      int icnt; /* counter  */
      long div;

      CHECK_MALLOC(notes, "main");
      CHECK_MALLOC(left,  "main");
      CHECK_MALLOC(right, "main");
      for (i = 0; i < 128; i ++)
      {
         vel[i]      = 0;
         on_event[i] = -1;
      }

#ifdef FFTW2
      x = (double *) malloc(sizeof(double) * fft_len);
      y = (double *) malloc(sizeof(double) * fft_len);
#else       /* FFTW3 */
      x = (double *) fftw_malloc(sizeof(double) * fft_len);
      y = (double *) fftw_malloc(sizeof(double) * fft_len);
#endif
      p = (double *) malloc(sizeof(double) * (fft_len/2 + 1));
      CHECK_MALLOC(x, "main");
      CHECK_MALLOC(y, "main");
      CHECK_MALLOC(p, "main");
      if (waon_parameters->flag_phase)
      {
         p0 = (double *) malloc(sizeof(double) * (fft_len/2 + 1));
         dphi = (double *) malloc(sizeof(double) * (fft_len/2 + 1));
         ph0 = (double *) malloc(sizeof(double) * (fft_len/2 + 1));
         ph1 = (double *) malloc(sizeof(double) * (fft_len/2 + 1));
         CHECK_MALLOC(p0, "main");
         CHECK_MALLOC(dphi, "main");
         CHECK_MALLOC(ph0, "main");
         CHECK_MALLOC(ph1, "main");
      }
      pmidi = (double *) malloc(sizeof(double) * 128);
      CHECK_MALLOC(pmidi, "main");
      if (is_nullptr(waon_parameters->file_midi))       /* MIDI output file */
      {
         waon_parameters->file_midi = (char *) malloc
         (
            sizeof(char) * (strlen("output.mid") + 1)
         );
         CHECK_MALLOC(waon_parameters->file_midi, "main");
         strcpy(waon_parameters->file_midi, "output.mid");
      }
      if (is_nullptr(waon_parameters->file_wav))        /* open input wav file */
      {
         waon_parameters->file_wav = (char *) malloc(sizeof(char) * 2);
         CHECK_MALLOC(waon_parameters->file_wav, "main");
         waon_parameters->file_wav[0] = '-';
      }

      /*
       * Yields "Conditional jump or move depends on uninitialised
       * value(s)" inside this function call in valgrind:
       */

      sf = sf_open(waon_parameters->file_wav, SFM_READ, &sfinfo);
      if (is_nullptr(sf))
      {
         fprintf
         (
            stderr, "Can't open input file %s: %s\n",
            waon_parameters->file_wav, strerror(errno)
         );
         exit(1);
      }
      sndfile_print_info(&sfinfo);
      if (sfinfo.channels != 2 && sfinfo.channels != 1)
      {
         errprint("Only mono and stereo inputs are supported");
         exit(1);
      }

      /*
       * -  t0 is the time-period for the FFT (inverse of smallest
       *    frequency).
       * -  den is the weight of the FFT window function.
       * -  i0 to i1 is the range to analyse (search notes) after 't0' is
       *    calculated.  i0 == 0 means a DC component (frequency == 0).
       */

      t0 = (double) fft_len / (double) sfinfo.samplerate;
      den = init_den(fft_len, waon_parameters->flag_window);
      i0 = (int)
      (
         g_midi_pitch_info.mp_mid2freq[waon_parameters->notelow] * t0 - 0.5
      );
      i1 = (int)
      (
         g_midi_pitch_info.mp_mid2freq[waon_parameters->notetop] * t0 - 0.5
      ) + 1;
      if (i0 <= 0)
         i0 = 1;

      if (i1 >= (fft_len/2))
         i1 = fft_len/2 - 1;

      init_patch
      (
         waon_parameters->file_patch, fft_len, waon_parameters->flag_window,
         analysis_scratchpad
      );

#ifdef FFTW2                     /* initialization plan for FFTW2 or FFTW3 */
      plan = rfftw_create_plan(fft_len, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
#else
      /*
       * Full valgrind check shows reachable "lost" block here:
       */

      plan = fftw_plan_r2r_1d(fft_len, x, y, FFTW_R2HC, FFTW_ESTIMATE);
#endif

      if (waon_parameters->shift_hop != fft_len) /* for first step */
      {
         /*
          * Full valgrind check shows reachable "lost" block in
          * sndfile_read() call.
          */

         if
         (
            sndfile_read
            (
               sf, sfinfo, left+waon_parameters->shift_hop,
               right+waon_parameters->shift_hop, fft_len-waon_parameters->shift_hop
            )
            != (fft_len - waon_parameters->shift_hop)
         )
         {
            fprintf (stderr, "No Wav Data!\n");
            exit(0);
         }
      }
      g_midi_pitch_info.mp_pitch_shift = 0.0;
      g_midi_pitch_info.mp_n_pitch = 0;
      for (icnt = 0; ; icnt++)                              /* MAIN LOOP      */
      {
         for (i = 0; i < fft_len - waon_parameters->shift_hop; i ++) /* shift       */
         {
            if (sfinfo.channels == 2)                       /* stereo         */
            {
               left[i] = left[i + waon_parameters->shift_hop];
               right[i] = right[i + waon_parameters->shift_hop];
            }
            else                                            /* mono           */
            {
               left[i] = left[i + waon_parameters->shift_hop];
            }
         }
         if
         (
            sndfile_read                              /* read from wav */
            (
               sf, sfinfo, left + (fft_len-waon_parameters->shift_hop),
               right + (fft_len-waon_parameters->shift_hop),
               waon_parameters->shift_hop
            )
            != waon_parameters->shift_hop
         )
         {
            /*
             * Happens under normal usage, no need to report it.
             *
             * errprint("WaoN: end of file");
             */

            break;
         }
         for (i = 0; i < fft_len; i ++)   /* set double table x[] for FFT */
         {
            if (sfinfo.channels == 2)                 /* stereo */
               x[i] = 0.5 * (left[i] + right[i]);
            else                                      /* mono */
               x[i] = left[i];
         }

         /**
          * Stage 1: calculate power spectrum
          */

         windowing(fft_len, x, waon_parameters->flag_window, 1.0, x);

#ifdef FFTW2
         rfftw_one(plan, x, y);
#else
         fftw_execute(plan); /* x[] -> y[] */
#endif

         if (waon_parameters->flag_phase == 0)
         {
            HC_to_amp2 (fft_len, y, den, p); /* no phase-vocoder correction   */
         }
         else                                /* with phase-vocoder correction */
         {
            HC_to_polar2(fft_len, y, 0, den, p, ph1);
            if (icnt == 0)                   /* first step, so no ph0[] yet   */
            {
               for (i = 0; i < (fft_len/2 + 1); ++i) /* full span             */
               {
                  dphi[i] = 0.0;             /* no correction                 */
                  p0[i] = p[i];              /* backup phase for next step    */
                  ph0[i] = ph1[i];
               }
            }
            else                       /* freq correction by phase difference */
            {
               for (i = 0; i < (fft_len/2 + 1); ++i) /* full span */
               {
                  double twopi = 2.0 * M_PI;
                  dphi[i] = ph1[i] - ph0[i] -
                     twopi * (double)i / (double) fft_len *
                     (double) waon_parameters->shift_hop;
                  for (; dphi[i] >= M_PI; dphi[i] -= twopi)
                     ;
                  for (; dphi[i] < -M_PI; dphi[i] += twopi)
                     ;

                  /*
                   * Frequency correction.  The frequency is
                   *
                   *    i / fft_len + dphi) * samplerate [Hz]
                   *
                   * Backup the phase for next step, then average the
                   * power for the analysis.
                   */

                  dphi[i] = dphi[i] / twopi / (double) waon_parameters->shift_hop;
                  p0[i] = p[i];
                  ph0[i] = ph1[i];
                  p[i] = 0.5 * (sqrt(p[i]) + sqrt(p0[i]));
                  p[i] = p[i] * p[i];
               }
            }
         }
         if (waon_parameters->psub_n != 0)              /* drum-removal process */
         {
            power_subtract_ave(fft_len, p, waon_parameters->psub_n, waon_parameters->psub_f);
         }
         if (waon_parameters->oct_f != 0.0)             /* octave-removal process */
         {
            power_subtract_octave(fft_len, p, waon_parameters->oct_f);
         }

         /**
          * Stage 2: pickup notes, new code:
          *
         if (flag_phase == 0)
         {
           average_FFT_into_midi (fft_len, (double)sfinfo.samplerate,
                   p, NULL,
                   pmidi);
         }
              else
         {
           average_FFT_into_midi (fft_len, (double)sfinfo.samplerate,
                   p, dphi,
                   pmidi);
         }
              pickup_notes (pmidi,
                cut_ratio, rel_cut_ratio,
                notelow, notetop,
                vel);
          *
          */

         /* old code */

         if (waon_parameters->flag_phase == 0) /* no phase-vocoder correction */
         {
            note_intensity
            (
               p, nullptr, waon_parameters->cut_ratio, waon_parameters->rel_cut_ratio,
               i0, i1, t0, vel, analysis_scratchpad
            );
         }
         else
         {
            /*
             * With phase-vocoder correction, make corrected frequency
             *
             *       i / fft_len + dphi) * samplerate [Hz]
             */

            for (i = 0; i < (fft_len/2 + 1); ++i)           /* full span */
            {
               dphi[i] = ((double) i / (double) fft_len + dphi[i]) *
                  (double) sfinfo.samplerate;
            }
            note_intensity
            (
               p, dphi, waon_parameters->cut_ratio, waon_parameters->rel_cut_ratio,
               i0, i1, t0, vel, analysis_scratchpad
            );
         }

         /**
          * Stage 3: check previous time for note-on/off
          */

         WAON_notes_check
         (
            notes, icnt, vel, on_event, 8, 0, waon_parameters->peak_threshold
         );
      }                                            /* MAIN LOOP      */

      /* Clean up the generated notes */

      WAON_notes_regulate(notes);
      WAON_notes_remove_shortnotes(notes, 1, 64);
      WAON_notes_remove_shortnotes(notes, 2, 28);
      WAON_notes_remove_octaves(notes);

      /*
       *
      g_midi_pitch_info.mp_pitch_shift /= (double) g_midi_pitch_info.mp_n_pitch;
      fprintf
      (
         stderr, "WaoN : difference of pitch = %f ( + %f )\n",
         -(g_midi_pitch_info.mp_pitch_shift - 0.5),
         g_midi_pitch_info.mp_adj_pitch
      );
       *
       */

      /*
       * div is the divisions for one beat (quarter-note).
       * Here we assume 120 BPM, that is, 1 beat is 0.5 sec.
       *
       * \note:
       *    (shift_hop / ft->rate) = duration for 1 step (sec)
      */

      div = (long)
      (
         0.5 * (double) sfinfo.samplerate / (double) waon_parameters->shift_hop
      );
      fprintf
      (
         stderr,
         "   Division:           %ld\n"
         "   WaoN # of events:   %d\n"
         "   Minimum note:       %d\n"
         "   Maximum note:       %d\n"
         ,
         div, notes->n, notes->minimum, notes->maximum
      );
      sum = 0;
      if (waon_parameters->dump_bins)
      {
         for (i = 0; i < MIDI_NOTE_COUNT; ++i)
         {
            if (notes->bin[i] > 0)
            {
               fprintf(stderr, "bin[%3d] = %5d\n", i, notes->bin[i]);
               sum += notes->bin[i];
            }
         }
         fprintf(stderr, "   Bin total:          %5d\n", sum);
      }
      if (waon_parameters->dump_events)
         WAON_notes_dump(notes);

      WAON_notes_output_midi(notes, div, waon_parameters->file_midi);

#ifdef FFTW2
      rfftw_destroy_plan(plan);
#else
      fftw_destroy_plan(plan);
#endif /* FFTW2 */

      WAON_notes_free(notes);

      if (not_nullptr(left))
         free(left);

      if (not_nullptr(right))
         free(right);

      if (not_nullptr(x))
         free(x);

      if (not_nullptr(y))
         free(y);

      if (not_nullptr(p))
         free(p);

      if (not_nullptr(p0))
         free(p0);

      if (not_nullptr(dphi))
         free(dphi);

      if (not_nullptr(ph0))
         free(ph0);

      if (not_nullptr(ph1))
         free(ph1);

      if (not_nullptr(pmidi))
         free(pmidi);

      parameters_free(waon_parameters);
      sf_close (sf);
   }
   return result;
}

/*
 * processing.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
