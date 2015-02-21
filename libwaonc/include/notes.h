#ifndef WAONC_NOTES_H
#define WAONC_NOTES_H

/*
 * header file for notes.c --
 * routines for notes handling
 * Copyright (C) 2007,2011 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: notes.h,v 1.2 2011/12/27 13:00:56 kichiki Exp $
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
 * \file          notes.h
 *
 *    This module provides functions for saving notes.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-11-28
 * \version       $Revision$
 * \license       GNU GPL
 */

#include "macros.h"                    /* wbool_t and errprint() macros    */

/**
 *    Provides information on notes and events.
 *
 *    Changed from "struct WAON_notes" to this type definition for a
 *    slight increase in maintainability.
 *
 *    Also added "minimum" and "maximum" fields to keep track of the range
 *    of notes actually processed.
 *
 *    Also added a bin to hold the number of MIDI notes generated for each
 *    value (0 to 127) of MIDI pitch.
 */

typedef struct
{
  int n;             /*<< Holds the number of events in this structure.       */
  int * step;        /*<< The step for the events.                            */
  char * event;      /*<< The event type (0 == off, 1 == on).                 */
  char * note;       /*<< The MIDI note number (0-127).                       */
  char * vel;        /*<< The velocity of the note (for on) (0-127).          */
  int minimum;       /*<< Indicates the minimum note; starts out at 999.      */
  int maximum;       /*<< Indicates the maximum note; starts out at -1.       */
  int bin[MIDI_NOTE_COUNT];   /*<< Holds the occurrence count of each note.   */

} waon_notes_t;

/*
 * Global functions for notes module.
 */

extern waon_notes_t * WAON_notes_init (void);
extern void WAON_notes_free (waon_notes_t * notes);
extern void WAON_notes_append
(
   waon_notes_t * notes,
   int step,
   char event,
   char note,
   char vel
);
extern void WAON_notes_insert
(
   waon_notes_t * notes,
   int index,
   int step,
   char event,
   char note,
   char vel
);
extern void WAON_notes_remove_at (waon_notes_t * notes, int index);
extern void WAON_notes_regulate (waon_notes_t *notes);
extern void WAON_notes_remove_shortnotes
(
   waon_notes_t * notes,
   int min_duration,
   int min_vel
);
extern void WAON_notes_remove_longnotes
(
   waon_notes_t * notes,
   int max_duration,
   int min_vel
);
extern void WAON_notes_remove_smallnotes (waon_notes_t * notes, int min_vel);
extern void WAON_notes_remove_octaves (waon_notes_t * notes);
extern void WAON_notes_check
(
   waon_notes_t * notes,
   int step,
   char * vel,
   int * on_step,
   int on_threshold,
   int off_threshold,
   int peak_threshold
);
extern void WAON_notes_dump (waon_notes_t * notes);
extern void WAON_notes_dump2 (waon_notes_t * notes);

#endif         /* WAONC_NOTES_H */

/*
 * notes.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
