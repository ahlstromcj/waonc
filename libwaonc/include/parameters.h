#ifndef WAONC_PARAMETERS_H_
#define WAONC_PARAMETERS_H_

/*
 * WaoN - a Wave-to-Notes transcriber : parameters
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
 * \file          parameters.h
 *
 *    This module provides functions for setting waonc parameters.
 *
 * \library       waonc application
 * \author        Chris Ahlstrom
 * \date          2013-11-23
 * \updates       2013-11-29
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    This is an additional module to provide for management of the waonc
 *    parameters that can be referenced from more than one application.
 */

#include "macros.h"                    /* wbool_t and errprint() macros       */

/**
 *    Provides a structure for holding all of the waonc parameters, for
 *    easier maintenance and recognition.
 *
 * Corresponding short command-line arguments and fields:
 *
@verbatim
      -i          file_wav
      -o          file_midi
      -p          file_patch
      -c          cut_ratio
      -r          rel_cut_ratio (affects abs_flg as well)
      -t          note_top
      -b          note_low
      -w          flag_window (e.g. 3 for Hanning)
      -n          fft_len
      -s          shift_hop
      -k          peak_threshold
      --nophase   flag_phase (wbool_t)
      --psub-n    psub_n
      --psub-f    psub_f
      --oct       oct_f
      -a          g_midi_pitch_info.mp_adj_pitch
@endverbatim
 *
 * Others:
 *
@verbatim
      -h                show_help (affected by any bad option setting)
      -v                show_version
      --dump-bins       dump_bins (new wbool_t)
      --dump-events     dump_events (new wbool_t)
@endverbatim
 *
 */

typedef struct
{
   char * file_midi;       /*<< Holds the name of the output MIDI file.       */
   char * file_wav;        /*<< Holds the name of the input WAV file.         */
   char * file_patch;      /*<< Holds the name of the optional patch file.    */
   double cut_ratio;       /*<< Holds the absolute log10 cutoff-ratio value.  */
   double rel_cut_ratio;   /*<< Holds the relative log10 cutoff-ratio value.  */
   long fft_len;           /*<< Provides the length of the FFT window.        */
   int flag_window;        /*<< The type of FFT window (Hanning by default)   */
   int notelow;            /*<< Indicates the lowest MIDI note to be created. */
   int notetop;            /*<< Indicates the highest MIDI note to create.    */
   long shift_hop;         /*<< TBD.                                          */
   wbool_t flag_phase;     /*<< Indicates to use phase for TBD.               */
   int peak_threshold;     /*<< TBD.                                          */
   int psub_n;             /*<< TBD.                                          */
   double psub_f;          /*<< TBD.                                          */
   double oct_f;           /*<< TBD.                                          */
   double adj_pitch;       /*<< TBD.                                          */
   wbool_t abs_flg;        /*<< Indicates to use absolute/relative cutoff.    */
   wbool_t dump_bins;      /*<< Indicates to dump a count of each MIDI note.  */
   wbool_t dump_events;    /*<< Indicates to dump the events to the screen.   */
   wbool_t show_help;      /*<< Indicates to show the help text.              */
   wbool_t show_version;   /*<< Indicates to show the version text.           */

} waon_parameters_t;

/*
 * Global functions for the main module(s) to use.
 */

extern void print_version (void);
extern void print_usage ();
extern wbool_t parameters_initialize (waon_parameters_t * parameters);
extern void parameters_free (waon_parameters_t * parameters);
extern wbool_t parameters_parse
(
   waon_parameters_t * parameters,
   int argc,
   char * argv []
);

#endif         /* WAONC_PARAMETERS_H_ */

/*
 * parameters.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
