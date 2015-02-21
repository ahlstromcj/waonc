/*
 * WaoN - a Wave-to-Notes transcriber : processing
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
 * \file          processing.h
 *
 *    This module provides functions for using waonc parameters to convert
 *    a wave file to a MIDI file.
 *
 * \library       waonc application
 * \author        Chris Ahlstrom
 * \date          2013-11-24
 * \updates       2013-11-28
 * \version       $Revision$
 * \license       GNU GPL
 *
 *    This module provides the code that was original in the main.c module
 *    for the waon command-line application.
 *
 *    The processing() function requires a new "scratchpad" parameter that
 *    takes the place of using global variables.
 */

/*
 * Global functions for the processing module.
 */

extern wbool_t processing
(
   waon_parameters_t * parameters,
   analysis_scratchpad_t * analysis_scratchpad
);

/*
 * processing.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
