/*
 * WaoN - a Wave-to-Notes transcriber : parameters
 *
 * Copyright (C) 1998-2008,2011 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: main.c,v 1.12 2011/12/27 13:11:00 kichiki Exp $
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
 * \file          parameters.c
 *
 *    This module provides functions for setting waonc parameters.
 *
 * \library       waonc application
 * \author        Chris Ahlstrom
 * \date          2013-11-23
 * \updates       2013-11-29
 * \version       $Revision$
 * \license       GNU GPL
 */

#include <ctype.h>                     /* isdigit()                           */
#include <math.h>                      /* atoi()                              */
#include <stdlib.h>                    /* atoi(), atof(), ...                 */
#include <string.h>                    /* strcat(), strcpy()                  */

#include "fft.h"                       /* filter-window enumeration           */
#include "memory-check.h"              /* CHECK_MALLOC() macro                */
#include "midi.h"                      /* MIDI-related macros                 */
#include "parameters.h"                /* declares functions for this module  */
#include "VERSION.h"

/**
 *    Provides a local short-hand notation for checking if the current
 *    command-line argument matches the given long and short option
 *    strings.
 *
 * Usage example:
 *
\verbatim
         if (OPTION_MATCH("--peak", "-k"))
         {
            // handle the option
         }
\endverbatim
 */

#define OPTION_MATCH(longopt, shortopt) \
   (strcmp(argv[i], longopt) == 0) || (strcmp(argv[i], shortopt) == 0)

/**
 *    Prints an error message and a little advice.
 *
 * \param msg
 *    The error message to print.
 */

static void
print_error (const char * msg)
{
   errprintf("? %s\n", msg);
   fprintf(stderr, "Run 'waonc --help' for help with program options.\n");
}

/**
 *    Prints the version and authorship information.
 */

void
print_version (void)
{
   fprintf
   (
      stdout,
      "WaoN - a Wave-to-MIDI transcriber, version %s\n\n"
      "Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>\n"
      "Modified 2013 Chris Ahlstrom <ahlstromcj@users.sourceforge.net>\n"
      "waon on Web: http://waon.sourceforge.net/\n\n" ,
      WAON_VERSION
   );
}

/**
 *    Provides the help string, formatted for an 80-column console screen.
 *    Broken into pieces because of string-size limitations.
 *    Part 1.
 */

static const char * const s_waon_helptext_1 =

"WaoN is a Wave-to-Notes transcriber, a converter from a sound file to MIDI\n"
"file. This version, waonc, is an extension of the original program.\n"
"\n"
"Usage: waonc [option ...]\n"
"\n"
"HELP OPTIONS:\n"
"  -h --help         Print this help.\n"
"  -v, --version     Print version information.\n"
"\n"
;

/**
 *    Help string part 2.
 */

static const char * const s_waon_helptext_2 =

"FILE OPTIONS:\n"
"  -i --input        Input WAV file ('-' for default) [Default: stdin].\n"
"  -o --output       Output MID file ('-' for default) [Default: 'output.mid'].\n"
"  -p --patch        Patch file [Default: no patch].\n"
"\n"
;

/**
 *    Help string part 3.
 */

static const char * const s_waon_helptext_3 =

"FFT OPTIONS:\n"
"  -n --window-length Sampling number for WAV in 1 step (default: 2048).\n"
"  -w --window       Selects the FFT window function.  Either the digit or\n"
"                    name (at least 3 characters) can be provided.\n"
"\n"
"     0 No window ('none')\n"
"     1 'Parzen' window\n"
"     2 'Welch' window\n"
"     3 'Hanning' window [default]\n"
"     4 'Hamming' window\n"
"     5 'Blackman' window\n"
"     6 'Steeper' 30-dB/octave rolloff window\n"
"\n"
;

/**
 *    Help string part 4.
 */

static const char * const s_waon_helptext_4 =

"READING WAV OPTIONS:\n"
"  -s --shift        Shift number of WAV in 1 step [Default: 1/4 of -n option].\n"
"\n"
"PHASE-VOCODER OPTIONS:\n"
"  --no-phase        Don't use phase difference to improve frequency estimates.\n"
"\n"
;

/**
 *    Help string part 5.
 */

static const char * const s_waon_helptext_5 =

"NOTE SELECTION OPTIONS:\n"
"  -c --cutoff       log10 of cut-off ratio to scale velocity [Default: -5.0].\n"
"  -r --relative     log10 of cut-off ratio relative to the average.\n"
"                    [Default: absolute cutoff with the value in -c option].\n"
"  -k --peak         Peak threshold for note-on, range [0,127]. [Default: 128\n"
"                    = no peak-search = search only first on-event].\n"
;

static const char * const s_waon_helptext_6 =

"  -t --top          Top note [MIDI number]. [Default: 103 = G7]\n"
"  -b --bottom       Bottom note [MIDI number]. [Default: 28 = E1]\n"
"                    Middle C (261 Hz) = C4 = MIDI 60. Midi range [0,127].\n"
"  -a --adjust       Pitch-adjustment suggested by WaoNc after analysis.\n"
"                    Unit is half-note: +1 is half-note up, -0.5 is\n"
"                    quarter-note down. [Default: 0]\n"
"\n"
;

static const char * const s_waon_helptext_7 =

"DRUM-REMOVAL OPTIONS:\n"
"  --psub-n          Number of averaging bins in one side (i-n,...,i,...,i+n).\n"
"  --psub-f          Factor for the average, where the power is modified as\n"
"                    p[i] = [SQRT(p[i]) - f * SQRT(ave[i])]^2 (default: 0.0)\n"
"\n"
"OCTAVE-REMOVAL OPTIONS:\n"
"  --oct             Factor for the octave removal, where the power is modified\n"
"                    as p[i] = [SQRT(p[i]) - f * SQRT(oct[i])]^2 (default: 0.0)\n"
"\n"
;

static const char * const s_waon_helptext_8 =

"VISIBILITY OPTIONS:\n"
"  --quiet           Show no output (TODO).\n"
"  --dump-bins       Show the MIDI note histogram data.\n"
"  --dump-events     Show the MIDI event data.\n"
;

/**
 *    Prints the waonc usage/help information.
 */

void
print_usage ()
{
   print_version();
   fprintf
   (
      stdout, "%s%s%s%s%s%s%s%s",
      s_waon_helptext_1,
      s_waon_helptext_2,
      s_waon_helptext_3,
      s_waon_helptext_4,
      s_waon_helptext_5,
      s_waon_helptext_6,
      s_waon_helptext_7,
      s_waon_helptext_8
   );
}

/**
 *    Initializes a waonc parameters structure to a known, default state.
 *
 * \param parameters
 *    Provides the structure to be initialized.  The pointer is checked.
 *
 * \return
 *    Returns true if the pointer was valid.
 */

wbool_t
parameters_initialize (waon_parameters_t * parameters)
{
   wbool_t result = not_nullptr(parameters);
   if (result)
   {
      parameters->file_midi = nullptr;
      parameters->file_wav = nullptr;
      parameters->file_patch = nullptr;
      parameters->cut_ratio = DEFAULT_CUTOFF_RATIO;
      parameters->rel_cut_ratio = DEFAULT_RELATIVE_CUTOFF_RATIO;
      parameters->fft_len = DEFAULT_FFT_LENGTH;
      parameters->flag_window = DEFAULT_FFT_WINDOW_TYPE;   /* Hanning window */
      parameters->notelow = DEFAULT_NOTE_BOTTOM;
      parameters->notetop = DEFAULT_NOTE_TOP;
      parameters->shift_hop = 0;
      parameters->flag_phase = DEFAULT_USE_PHASE;
      parameters->peak_threshold = DEFAULT_PEAK_THRESHOLD_DISABLED;
      parameters->psub_n = 0;
      parameters->psub_f = 0.0;
      parameters->oct_f = 0.0;
      parameters->adj_pitch = 0.0;
      parameters->abs_flg = DEFAULT_USE_ABSOLUTE_CUTOFF;
      parameters->dump_bins = wfalse;
      parameters->dump_events = wfalse;
      parameters->show_help = wfalse;
      parameters->show_version = wfalse;
   }
   return result;
}

/**
 *    Frees up any of the pointers that were allocated in the waonc
 *    parameters structure.
 */

void
parameters_free (waon_parameters_t * parameters)
{
   wbool_t result = not_nullptr(parameters);
   if (result)
   {
      if (not_nullptr(parameters->file_midi))
      {
         free(parameters->file_midi);
         parameters->file_midi = nullptr;
      }
      if (not_nullptr(parameters->file_wav))
      {
         free(parameters->file_wav);
         parameters->file_wav = nullptr;
      }
      if (not_nullptr(parameters->file_patch))
      {
         free(parameters->file_patch);
         parameters->file_patch = nullptr;
      }
   }
}

/**
 *    Parses the command-line options in order to change the setting of
 *    the waonc parameters structure.
 *
 * \param parameters
 *    Provides the structure to be altered.  The pointer is checked.
 *
 * \return
 *    Returns true if all of the options supplied were for settings, and
 *    all settings succeeded.  Returns false if the show_help or
 *    show_version fields were set.  Also returns false if the parameters
 *    pointer was invalid.
 */

wbool_t
parameters_parse
(
   waon_parameters_t * parameters,
   int argc,
   char * argv []
)
{
   wbool_t result = not_nullptr(parameters);
   if (result)
   {
      int i;
      for (i = 1; i < argc; i++)
      {
         if (OPTION_MATCH("--input", "-i"))
         {
            if (i+1 < argc)
            {
               parameters->file_wav = (char *) malloc
               (
                  sizeof(char) * (strlen(argv[++i]) + 1)
               );
               CHECK_MALLOC(parameters->file_wav, "main");
               strcpy(parameters->file_wav, argv[i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--output", "-o"))
         {
            if (i+1 < argc)
            {
               parameters->file_midi = (char *) malloc
               (
                  sizeof(char) * (strlen(argv[++i]) + 1)
               );
               CHECK_MALLOC(parameters->file_midi, "main");
               strcpy(parameters->file_midi, argv[i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--cutoff", "-c"))
         {
            if (i+1 < argc)
            {
               parameters->cut_ratio = atof(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--top", "-t"))
         {
            if (i+1 < argc)
            {
               parameters->notetop = atoi(argv[++i]);
               if
               (
                  parameters->notetop < 0 ||
                  parameters->notetop >= MIDI_NOTE_MAX
               )
               {
                  errprint("--top value outside of 0 to 127 range");
                  result = wfalse;
                  break;
               }
               else if (parameters->notetop < parameters->notelow)
               {
                  errprint("--top and --bottom need to be reversed");
                  result = wfalse;
               }
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--bottom", "-b"))
         {
            if (i+1 < argc)
            {
               parameters->notelow = atoi(argv[++i]);
               if
               (
                  parameters->notelow < 0 ||
                  parameters->notelow >= MIDI_NOTE_MAX
               )
               {
                  errprint("--bottom value outside of 0 to 127 range");
                  result = wfalse;
                  break;
               }
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--window", "-w"))
         {
            if (i+1 < argc)
            {
               const char * value = argv[++i];
               if (isdigit(value[0]))
                  parameters->flag_window = atoi(value);
               else
                  parameters->flag_window = (int) fft_window_value(value);

               if (parameters->flag_window >= FILTER_WINDOW_MAX)
               {
                  print_error("no such filter-window");
                  parameters->show_help = wtrue;
                  result = wfalse;
                  break;
               }
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--window-length", "-n"))
         {
            if (i+1 < argc)
            {
               parameters->fft_len = atoi(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--shift", "-s"))
         {
            if (i+1 < argc)
            {
               parameters->shift_hop = atoi(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--patch", "-p"))
         {
            if (i+1 < argc)
            {
               parameters->file_patch = argv[++i];
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--relative", "-r"))
         {
            parameters->abs_flg = wfalse;
            if (i+1 < argc)
            {
               parameters->rel_cut_ratio = atof(argv[++i]);
            }
            else
               parameters->rel_cut_ratio = DEFAULT_RELATIVE_CUTOFF_RATIO;

            /*
             * If no parameter is given, this means use the default
             * relative ratio.  It is not an error.
             *
             * else
             * {
             *    parameters->show_help = wtrue;
             *    result = wfalse;
             *    break;
             * }
             */
         }
         else if (OPTION_MATCH("--peak", "-k"))
         {
            if (i+1 < argc)
            {
               parameters->peak_threshold = atoi(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--adjust", "-a"))
         {
            if (i+1 < argc)
            {
               parameters->adj_pitch = atof(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--help", "-h"))
         {
            parameters->show_help = wtrue;
            result = wfalse;
            break;
         }
         else if (OPTION_MATCH("--no-phase", "--nophase"))
         {
            parameters->flag_phase = wfalse;
         }
         else if (strcmp(argv[i], "--psub-n") == 0)
         {
            if (i+1 < argc)
            {
               parameters->psub_n = atoi(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (strcmp(argv[i], "--psub-f") == 0)
         {
            if (i+1 < argc)
            {
               parameters->psub_f = atof(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (strcmp(argv[i], "--oct") == 0)
         {
            if (i+1 < argc)
            {
               parameters->oct_f = atof(argv[++i]);
            }
            else
            {
               parameters->show_help = wtrue;
               result = wfalse;
               break;
            }
         }
         else if (OPTION_MATCH("--version", "-v"))
         {
            parameters->show_version = wtrue;
            result = wfalse;
         }
         else if (strcmp(argv[i], "--dump-bins") == 0)
         {
            parameters->dump_bins = wtrue;
         }
         else if (strcmp(argv[i], "--dump-events") == 0)
         {
            parameters->dump_events = wtrue;
         }
         else
         {
            parameters->show_help = wtrue;
            result = wfalse;
         }
      }
      if (result)
      {
         if (parameters->shift_hop == 0)
            parameters->shift_hop = parameters->fft_len / 4;

         if (parameters->psub_n == 0)
            parameters->psub_f = 0.0;

         if (parameters->psub_f == 0.0)
            parameters->psub_n = 0;
      }
   }
   return result;
}

/*
 * parameters.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
