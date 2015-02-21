/*
 * Routines for notes handling
 *
 * Copyright (C) 2007,2011 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: notes.c,v 1.2 2011/12/27 13:00:20 kichiki Exp $
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
 * \file          notes.c
 *
 *    This module provides functions for generating notes.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-24
 * \version       $Revision$
 * \license       GNU GPL
 */

#include <stdio.h>                     /* fprintf()                        */
#include <stdlib.h>                    /* malloc()                         */
#include <string.h>                    /* memset()                         */
#include <sys/errno.h>                 /* errno                            */

#include "memory-check.h"              /* CHECK_MALLOC() macro             */
#include "notes.h"                     /* this module's functions          */

/**
 *    Allocates and initializes a waon_notes_t (struct WAON_notes)
 *    structure.
 *
 * \return
 *    Returns a pointer to the structure.  It is important to use (only)
 *    WAON_notes_free() to free this structure when finished with it.
 */

waon_notes_t *
WAON_notes_init (void)
{
   waon_notes_t * notes = (waon_notes_t *) malloc(sizeof(waon_notes_t));
   CHECK_MALLOC(notes, "WAON_notes_init");
   notes->n = 0;
   notes->step  = nullptr;
   notes->event = nullptr;
   notes->note  = nullptr;
   notes->vel   = nullptr;
   notes->minimum = MIDI_NOTE_MIN_INITIAL;      /* 999 */
   notes->maximum = MIDI_NOTE_MAX_INITIAL;      /*  -1 */
   (void) memset(notes->bin, 0, sizeof(notes->bin));
   return notes;
}

/**
 *    Frees a waon_notes_t structure and each of the entities it points
 *    to.
 *
 * \param notes
 *    The structure to be freed.  The pointers are all checked, for safety.
 */

void
WAON_notes_free (waon_notes_t * notes)
{
   if (not_nullptr(notes))
   {
      if (not_nullptr(notes->step))
         free(notes->step);

      if (not_nullptr(notes->event))
         free(notes->event);

      if (not_nullptr(notes->note))
         free(notes->note);

      if (not_nullptr(notes->vel))
         free(notes->vel);

      free(notes);
   }
}

/**
 *    Appends a note to the notes buffer.
 *
 *    This function also adjusts the minimum/maximum note values.
 *
 * \note
 *    Potentially inefficient due to the realloc() calls.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param step
 *    I currently believe this value is the ordinal value of the current
 *    window-buffer.
 *
 * \param event
 *    Indicates what kind of event, usually a note-on or note-off event.
 *    These events are denoted by the macros MIDI_EVENT_NOTE_OFF
 *    MIDI_EVENT_NOTE_ON.
 *
 * \param note
 *    Provides the note value, ranging from MIDI_NOTE_MIN (0) to
 *    MIDI_NOTE_MAX (127).
 *
 * \param vel
 *    Provides the note velocity value, ranging from MIDI_VELOCITY_MIN (0)
 *    to MIDI_VELOCITY_MAX (127).
 */

void
WAON_notes_append
(
   waon_notes_t * notes,
   int step,
   char event,
   char note,
   char vel
)
{
   int i = notes->n;                   /* index of the last element           */
   notes->n++;
   notes->step  = (int *) realloc(notes->step, sizeof(int) * (notes->n));
   notes->event = (char *) realloc(notes->event, sizeof(char) * (notes->n));
   notes->note  = (char *) realloc(notes->note, sizeof(char) * (notes->n));
   notes->vel   = (char *) realloc(notes->vel, sizeof(char) * (notes->n));
   CHECK_MALLOC(notes->step,  "WAON_notes_append");
   CHECK_MALLOC(notes->event, "WAON_notes_append");
   CHECK_MALLOC(notes->note,  "WAON_notes_append");
   CHECK_MALLOC(notes->vel,   "WAON_notes_append");
   notes->step[i] = step;
   notes->event[i] = event;
   notes->note[i] = note;
   notes->vel[i] = vel;
   if (event == MIDI_EVENT_NOTE_ON)    /* we want to keep stats on the notes  */
   {
      ++notes->bin[(int) note];        /* increment our bin count for note    */
      if (note > notes->maximum)       /* adjust the max and min note values  */
         notes->maximum = note;
      else if (note < notes->minimum)
         notes->minimum = note;
   }
}

/**
 *    Inserts a note at the given point in the notes buffer.
 *
 *    This function also adjusts the minimum/maximum note values.
 *
 * \note
 *    Potentially inefficient due to the realloc() calls.  It also has to
 *    move all the events in order to insert the event.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param index
 *    Indicates where in the buffer to insert the note.
 *
 * \param step
 *    I currently believe this value is the ordinal value of the current
 *    window-buffer.
 *
 * \param event
 *    Indicates what kind of event, usually a note-on or note-off event.
 *    These events are denoted by the macros MIDI_EVENT_NOTE_OFF
 *    MIDI_EVENT_NOTE_ON.
 *
 * \param note
 *    Provides the note value, ranging from MIDI_NOTE_MIN (0) to
 *    MIDI_NOTE_MAX (127).
 *
 * \param vel
 *    Provides the note velocity value, ranging from MIDI_VELOCITY_MIN (0)
 *    to MIDI_VELOCITY_MAX (127).
 */

void
WAON_notes_insert
(
   waon_notes_t * notes,
   int index,
   int step,
   char event,
   char note,
   char vel
)
{
   int i;
   notes->n++;
   notes->step  = (int *) realloc(notes->step, sizeof(int) * (notes->n));
   notes->event = (char *) realloc(notes->event, sizeof(char) * (notes->n));
   notes->note  = (char *) realloc(notes->note, sizeof(char) * (notes->n));
   notes->vel   = (char *) realloc(notes->vel, sizeof(char) * (notes->n));
   CHECK_MALLOC(notes->step,  "WAON_notes_insert");
   CHECK_MALLOC(notes->event, "WAON_notes_insert");
   CHECK_MALLOC(notes->note,  "WAON_notes_insert");
   CHECK_MALLOC(notes->vel,   "WAON_notes_insert");

   /*
    * copy elements (index, ..., n-2) into (index+1, ..., n-1)
    */

   for (i = notes->n - 1; i > index; --i)
   {
      notes->step[i] = notes->step[i-1];
      notes->event[i] = notes->event[i-1];
      notes->note[i] = notes->note[i-1];
      notes->vel[i] = notes->vel[i-1];
   }
   notes->step[index] = step;
   notes->event[index] = event;
   notes->note[index] = note;
   notes->vel[index] = vel;
   if (event == MIDI_EVENT_NOTE_ON)          /* 2nd occurrence of this code   */
   {
      ++notes->bin[(int) note];              /* count the note                */
      if (note > notes->maximum)             /* update the statistics         */
         notes->maximum = note;
      else if (note < notes->minimum)
         notes->minimum = note;
   }
}

/**
 *    Removes a note at the given point in the notes buffer.
 *
 *    This function does <i> not </i> adjust the minimum/maximum note values.
 *
 * \note
 *    Potentially inefficient due to the realloc() calls.  Not sure why it
 *    bothers to reallocate.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param index
 *    Indicates where in the buffer to insert the note.
 */

void
WAON_notes_remove_at (waon_notes_t * notes, int index)
{
   /* copy elements (index+1, ..., n-1) into (index, ..., n-2) */

   int i;
   --notes->bin[(int)(notes->note[index])];  /* un-count the note */
   for (i = index + 1; i < notes->n; ++i)
   {
      notes->step[i-1] = notes->step[i];
      notes->event[i-1] = notes->event[i];
      notes->note[i-1] = notes->note[i];
      notes->vel[i-1] = notes->vel[i];
   }
   notes->n--;
   if (notes->n > 0)                         /* resize the arrays */
   {
      notes->step  = (int *) realloc (notes->step,  sizeof(int) * (notes->n));
      notes->event = (char *) realloc (notes->event, sizeof(char) * (notes->n));
      notes->note  = (char *) realloc (notes->note,  sizeof(char) * (notes->n));
      notes->vel   = (char *) realloc (notes->vel,   sizeof(char) * (notes->n));
      CHECK_MALLOC(notes->step,  "WAON_notes_remove_at [step]");
      CHECK_MALLOC(notes->event, "WAON_notes_remove_at [event]");
      CHECK_MALLOC(notes->note,  "WAON_notes_remove_at [note]");
      CHECK_MALLOC(notes->vel,   "WAON_notes_remove_at [vel]");
   }
   else
   {
      errprint
      (
         "WAON_notes_remove_at(): no note found (top/bottom range too small?)"
      );
   }
}

/**
 *    Shift indices in on_index[] larger than i_rm where i_rm is the
 *    removed index.
 *
 *    For each of the 128 MIDI note values (0 to 127), if on_index[i] is
 *    greater than i_rm, then on_index[i] is decremented.
 *
 * \param on_index
 *    TBD
 *
 * \param i_rm
 *    Provides the target value for the removed index.
 */

void
check_on_index_for_remove (int * on_index, int i_rm)
{
   int i;
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
   {
      if (on_index[i] > i_rm)
         on_index[i]--;
   }
}

/**
 *    For each each the n notes in the waon_notes_t structure:
 *
 *       -  Obtain the MIDI note value for that note.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 */

void
WAON_notes_regulate (waon_notes_t * notes)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int index;
   int last_step;
   CHECK_MALLOC(on_step,  "WAON_notes_remove_regulate");
   CHECK_MALLOC(on_index, "WAON_notes_remove_regulate");
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;

   for (index = 0; index < notes->n; index++)
   {
      int note = (int) notes->note[index];
      if (notes->event[index] == MIDI_EVENT_NOTE_OFF)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /*
             * No On event on the note so remove this orphaned Off event.
             */

            WAON_notes_remove_at(notes, index);
            index--;
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[index] == MIDI_EVENT_NOTE_ON)
      {
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            /*
             * The note is already on, so insert Off event here.
             */

            WAON_notes_insert
            (
               notes, index, notes->step[index], MIDI_EVENT_NOTE_OFF, note,
               MIDI_VELOCITY_HALF
            );
            index++;
         }
         on_step[note] = notes->step[index];
         on_index[note] = index;
      }
      else
         fprintf(stderr, "? invalid event type %d\n", notes->event[index]);
   }
   if (notes->n > 0)                   /* check if any On notes left */
   {
      last_step = notes->step[notes->n - 1];
      for (i = 0; i < MIDI_NOTE_COUNT; ++i)
      {
         if (on_step[i] < 0)
            continue;

         WAON_notes_append
         (
            notes, last_step + 1, MIDI_EVENT_NOTE_OFF, MIDI_NOTE(i),
            MIDI_VELOCITY_HALF
         );
      }
   }
   else
   {
      errprint
      (
         "WAON_notes_regulate: no note found (top/bottom range too small?)"
      );
   }
   free(on_step);
   free(on_index);
}

/**
 *    Removes notes that are short and less than a minimum velocity.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param min_duration
 *    The minimum duration for removing a note.
 *
 * \param min_vel
 *    The minimum velocity for removing a note.
 */

void
WAON_notes_remove_shortnotes
(
   waon_notes_t * notes, int min_duration, int min_vel
)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int index;
   CHECK_MALLOC(on_step,  "WAON_notes_remove_shortnotes");
   CHECK_MALLOC(on_index, "WAON_notes_remove_shortnotes");
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;

   for (index = 0; index < notes->n; ++index)
   {
      int note = (int) notes->note[index];
      if (notes->event[index] == 0)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /*
             * No On event for the note, so remove this orphaned Off event.
             */

            WAON_notes_remove_at(notes, index);
            index--;
         }
         else
         {
            int vel = (int) notes->vel[on_index[note]];
            int duration = notes->step[index] - on_step[note];
            if (duration <= min_duration && vel <= min_vel)
            {
               int index_on;

               /* remove these On and Off events for the note */

               WAON_notes_remove_at(notes, index);
               --index;
               index_on = on_index[note];
               WAON_notes_remove_at(notes, index_on);
               --index;

               /* need to shift indices on on_index[] */

               check_on_index_for_remove(on_index, index_on);
            }
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[index] == 1)
      {
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            WAON_notes_insert       /* note already on so insert Off event    */
            (
               notes, index, notes->step[index], MIDI_EVENT_NOTE_OFF, note,
               MIDI_VELOCITY_HALF
            );
            index++;
         }
         on_step[note] = notes->step[index];
         on_index[note] = index;
      }
      else
         fprintf(stderr, "invalid event type %d\n", notes->event[index]);
   }
   free(on_step);
   free(on_index);
}

/**
 *    Removes notes that are long and less than a minimum velocity.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param max_duration
 *    The maximum duration for removing a note.
 *
 * \param min_vel
 *    The minimum velocity for removing a note.
 */

void
WAON_notes_remove_longnotes
(
   waon_notes_t * notes,
   int max_duration,
   int min_vel
)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int index;
   CHECK_MALLOC(on_step,  "WAON_notes_remove_longnotes");
   CHECK_MALLOC(on_index, "WAON_notes_remove_longnotes");
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
   {
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;
   }
   for (index = 0; index < notes->n; index++)
   {
      int note = (int) notes->note[index];
      if (notes->event[index] == 0)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /*
             * No On event for the note, so remove this orphaned Off event.
             */

            WAON_notes_remove_at(notes, index);
            index--;
         }
         else
         {
            int vel = (int) notes->vel[on_index[note]];
            int duration = notes->step[index] - on_step[note];
            if (duration >= max_duration && vel <= min_vel)
            {
               int index_on;

               /* remove these On and Off events for the note */

               WAON_notes_remove_at(notes, index);
               index--;
               index_on = on_index[note];
               WAON_notes_remove_at(notes, index_on);
               index--;

               /* need to shift indices on on_index[] */

               check_on_index_for_remove(on_index, index_on);
            }
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[index] == 1)
      {
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            WAON_notes_insert       /* note already on so insert off event */
            (
               notes, index, notes->step[index], MIDI_EVENT_NOTE_OFF, note,
               MIDI_VELOCITY_HALF
            );
            index++;
         }
         on_step[note] = notes->step[index];
         on_index[note] = index;
      }
   }
   free(on_step);
   free(on_index);
}

/**
 *    Removes notes that are less than a minimum velocity.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 *
 * \param min_vel
 *    The minimum velocity for removing a note.
 */

void
WAON_notes_remove_smallnotes (waon_notes_t * notes, int min_vel)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int index;
   CHECK_MALLOC(on_step,  "WAON_notes_remove_smallnotes");
   CHECK_MALLOC(on_index, "WAON_notes_remove_smallnotes");
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
   {
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;
   }
   for (index = 0; index < notes->n; index++)
   {
      int note = (int) notes->note[index];
      if (notes->event[index] == 0)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /*
             * no on event on the note so remove this orphaned off event.
             */

            WAON_notes_remove_at(notes, index);
            index--;
         }
         else
         {
            int vel = (int) notes->vel[on_index[note]];
            if (vel <= min_vel)     /* remove on and off events for the note */
            {
               int index_on;
               WAON_notes_remove_at(notes, index);
               index--;
               index_on = on_index[note];
               WAON_notes_remove_at(notes, index_on);
               index--;

               /* need to shift indices on on_index[] */

               check_on_index_for_remove(on_index, index_on);
            }
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[index] == 1)
      {
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            WAON_notes_insert       /* note already on so insert Off event */
            (
               notes, index, notes->step[index], MIDI_EVENT_NOTE_OFF, note,
               MIDI_VELOCITY_HALF
            );
            index++;
         }
         on_step[note] = notes->step[index];
         on_index[note] = index;
      }
   }
   free(on_step);
   free(on_index);
}

/**
 *    Removes notes that are an octave away from ....????
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 */

void
WAON_notes_remove_octaves (waon_notes_t * notes)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * flag_remove = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int index;
   CHECK_MALLOC(on_step, "WAON_notes_remove_octaves");
   CHECK_MALLOC(on_index, "WAON_notes_remove_octaves");
   CHECK_MALLOC(flag_remove, "WAON_notes_remove_octaves");

   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
   {
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;
      flag_remove[i] = 0;                 /* false */
   }
   for (index = 0; index < notes->n; index++)
   {
      int note = (int)notes->note[index];
      if (notes->event[index] == 0)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /*
             * No On event for the note, so remove this orphaned Off event.
             */

            WAON_notes_remove_at(notes, index);
            index--;
         }
         else
         {
            if (flag_remove[note] > 0) /* remove on and off events for note */
            {
               int index_on;
               WAON_notes_remove_at(notes, index);
               index--;
               index_on = on_index[note];
               WAON_notes_remove_at(notes, index_on);
               index--;

               /* need to shift indices on on_index[] */

               check_on_index_for_remove (on_index, index_on);
            }
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[index] == 1)
      {
         int note_down;
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            /*
             * the note is already on so, insert off event here.
             */

            WAON_notes_insert
            (
               notes, index, notes->step[index], MIDI_EVENT_NOTE_OFF, note,
               MIDI_VELOCITY_HALF
            );
            index++;
         }
         on_step[note] = notes->step[index];
         on_index[note] = index;
         flag_remove[note] = 0;              /* false */
         note_down = note - 12;
         if (note_down < 0)
            continue;

         if (on_step[note_down] >= 0 && on_index[note_down] >= 0)
         {
            if (notes->vel[index] < notes->vel[on_index[note_down]])
            {
               flag_remove[note] = 1; /* true */
            }
         }
      }
   }
   free(flag_remove);                  /* \change ca 2013-11-17 was leaked! */
   free(on_index);
   free(on_step);
}

/**
 *    Provides a processing step for stage 3, where a time-difference
 *    check for note-on/off is made.
 *
 *    This function checks on and off events for each note, comparing
 *    vel[] and on_vel[].
 *
 * \param [out] notes
 *    Provides the note-management structure. event(s) are appended if the
 *    check succeeds.
 *
 * \param step
 *    Provides the present step
 *
 * \param vel[128]
 *    Provides velocity at the present step
 *
 * \param on_event[128]
 *    Provides index of notes at the last on-event for each note.  (-1)
 *    means the note is off at the last step.
 *
 * \param on_threshold
 *    Provides the "on" threshold.  The note turns on if vel[i] >
 *    on_threshold.
 *
 * \param off_threshold
 *    Provides the "off" threshold.  The note turns off if vel[i] <=
 *    off_threshold.
 *
 * \param peak_threshold
 *    Provides the "peak" threshold.  The note turns off and on if vel[i]
 *    >= (on_vel[i] + peak_threshold)
 */

void
WAON_notes_check
(
   waon_notes_t * notes,
   int step,
   char * vel,
   int * on_event,
   int on_threshold,
   int off_threshold,
   int peak_threshold
)
{
   int i;
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)  /* loop for notes                   */
   {
      if (on_event[i] < 0)                /* off at last step                 */
      {
         if (vel[i] > on_threshold)       /* check note-on by on_threshold    */
         {
            WAON_notes_append
            (
               notes, step, MIDI_EVENT_NOTE_ON, MIDI_NOTE(i), vel[i]
            );
            on_event[i] = notes->n - 1;   /* event index of notes             */
         }
      }
      else                                /* on at last step                  */
      {
         if (vel[i] <= off_threshold)     /* check note-on by off_threshold   */
         {
            WAON_notes_append             /* off                              */
            (
               notes, step, MIDI_EVENT_NOTE_OFF, MIDI_NOTE(i),
               MIDI_VELOCITY_HALF
            );
            on_event[i] = WAON_UNINITIALIZED;
         }
         else                             /* note is over off_threshold       */
         {
            if (vel[i] >= (notes->vel[on_event[i]] + peak_threshold))
            {
               WAON_notes_append          /* off                              */
               (
                  notes, step, MIDI_EVENT_NOTE_OFF, MIDI_NOTE(i),
                  MIDI_VELOCITY_HALF
               );
               WAON_notes_append          /* on                               */
               (
                  notes, step, MIDI_EVENT_NOTE_ON, MIDI_NOTE(i), vel[i]
               );
               on_event[i] = notes->n - 1;         /* event index of notes    */
            }
            else if (vel[i] > notes->vel[on_event[i]])
               notes->vel[on_event[i]] = vel[i];   /* overwrite velocity      */
         }
      }
   }
}

/**
 *    Dumps the notes to standard output.
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 */

void
WAON_notes_dump (waon_notes_t * notes)
{
   int last_step = WAON_UNINITIALIZED;
   int i;
   for (i = 0; i < notes->n; ++i)
   {
      fprintf(stdout, "[%5d] ", i);
      if (notes->step[i] > last_step)
      {
         fprintf(stdout, "step %5d: ", notes->step[i]);
         last_step = notes->step[i];
      }
      else
         fprintf(stdout, "          : ");

      fprintf(stdout, (notes->event[i] == 0) ? "off" : "on ");
      fprintf(stdout, "%3d %3d\n", notes->note[i], notes->vel[i]);
   }
}

/**
 *    Dumps the notes to standard output.
 *
 *    This function is different from WAON_notes_dump() in that it....
 *
 * \param notes
 *    Provides the management structure for MIDI notes.
 */

void
WAON_notes_dump2 (waon_notes_t * notes)
{
   int * on_step  = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int * on_index = (int *) malloc(sizeof(int) * MIDI_NOTE_COUNT);
   int i;
   int last_step;
   CHECK_MALLOC(on_step,  "WAON_notes_dump2");
   CHECK_MALLOC(on_index, "WAON_notes_dump2");
   for (i = 0; i < MIDI_NOTE_COUNT; ++i)
   {
      on_step[i] = on_index[i] = WAON_UNINITIALIZED;
   }
   for (i = 0; i < notes->n; ++i)
   {
      int note = (int) notes->note[i];
      if (notes->event[i] == 0)
      {
         if (on_step[note] < 0 || on_index[note] < 0) /* off event */
         {
            /* orphaned off event */
         }
         else
         {
            int step = notes->step[on_index[note]];
            int duration = notes->step[i] - on_step[note];
            int vel = (int)notes->vel[on_index[note]];
            fprintf
            (
               stdout,
               "%5d : note %3d, duration %3d, vel %3d\n",
               step, note, duration, vel
            );
         }
         on_step[note] = on_index[note] = WAON_UNINITIALIZED;
      }
      else if (notes->event[i] == 1)
      {
         if (on_step[note] >= 0 && on_index[note] >= 0) /* on event */
         {
            /* the note is already on */

            int step = notes->step[on_index[note]];
            int duration = notes->step[i] - on_step[note];
            int vel = (int)notes->vel[on_index[note]];
            fprintf
            (
               stdout,
               "%5d : note %3d, duration %3d, vel %3d (* no-off)\n",
               step, note, duration, vel
            );
         }
         on_step[note] = notes->step[i];
         on_index[note] = i;
      }
   }
   if (notes->n > 0)
   {
      last_step = notes->step[notes->n - 1];       /* check if on note left */
      for (i = 0; i < MIDI_NOTE_COUNT; ++i)
      {
         if (on_step[i] >= 0)
         {
            int step = notes->step[on_index[i]];
            int duration = last_step + 1 - on_step[i];
            int vel = (int)notes->vel[on_index[i]];
            fprintf
            (
               stdout,
               "%5d : note %3d, duration %3d, vel %3d (* no-off at the end)\n",
               step, i, duration, vel
            );
         }
      }
   }
   else
   {
      errprint("WAON_notes_dump2: no note found (top/bottom range too small?)");
   }
   free(on_step);
   free(on_index);
}

/*
 * notes.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
