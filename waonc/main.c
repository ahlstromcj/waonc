/*
 * WaoN - a Wave-to-Notes transcriber : main
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
 * \file          main.c
 *
 *    This module provides functions for generating notes.
 *
 * \library       waonc application
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-11-28
 * \version       $Revision$
 * \license       GNU GPL
 */

#include "analyse.h"                   /* note_intensity(), note_on_off(), ...*/
#include "midi.h"                      /* g_midi_pitch_info NEW NEW NEW       */
#include "parameters.h"                /* waon_parameters_t                   */
#include "processing.h"                /* processing()                        */

/**
 *    Provides the entry-point for the waonc program.
 *
 * @param argc
 *    Provides the standard count of the number of command-line arguments,
 *    including the name of the program.
 *
 * @param argv
 *    Provides the command-line arguments as an array of pointers.
 *
 * @return
 *    Returns a 0 value if the application succeeds, and a non-zero value
 *    otherwise.
 */

int
main (int argc, char * argv [])
{
   wbool_t parse_good = wtrue;
   waon_parameters_t waon_parameters;
   analysis_scratchpad_t analysis_scratchpad;
   wbool_t result = parameters_initialize(&waon_parameters);
   if (result)
      result = parameters_parse(&waon_parameters, argc, argv);

   if (result)
      result = analysis_scratchpad_initialize(&analysis_scratchpad);

   if (result)
   {
       analysis_scratchpad.absolute_cutoff = waon_parameters.abs_flg;
       if (not_nullptr(waon_parameters.file_patch))
          analysis_scratchpad.use_patchfile = wtrue;

       /* NEW as of 12-01 */

       g_midi_pitch_info.mp_adj_pitch = waon_parameters.adj_pitch;
   }
   if (waon_parameters.show_help)
   {
      print_usage();
      parse_good = wfalse;
   }
   else if (waon_parameters.show_version)
   {
      print_version();
      parse_good = wfalse;
   }
   if (result && parse_good)
   {
      parse_good = processing(&waon_parameters, &analysis_scratchpad);
      parameters_free(&waon_parameters);
   }
   return parse_good ? 0 : 1 ;
}

/*
 * main.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
