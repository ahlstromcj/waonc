#ifndef WAONC_MIDI_H
#define WAONC_MIDI_H

/*
 * Header file for midi.c --
 *
 * subroutines to write standard MIDI file
 * Copyright (C) 1998-2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: midi.h,v 1.5 2007/11/04 23:48:26 kichiki Exp $
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

#include "notes.h"                     /* waon_notes_t                        */

/**
 *    Provides a way to encapsulate application parameters for the MIDI
 *    module.
 */

typedef struct
{
   double mp_adj_pitch;
   double mp_pitch_shift;
   int mp_n_pitch;
   double mp_mid2freq[MIDI_NOTE_COUNT];

} midi_pitch_t;

/*
 * For estimating pitch shift.
 */

extern midi_pitch_t g_midi_pitch_info;

/*
 * General midi-frequency stuff.
 */

extern double midi_to_freq (int midi);
extern int freq_to_midi (double f);
extern double midi_to_logf (int midi);
extern int logf_to_midi (double logf);

/*
 * Get standard MIDI note from frequency.
 */

extern int get_note (double freq);
extern int smf_header_fmt
(
   int fd,
   unsigned short format,
   unsigned short tracks,
   unsigned short divisions
);
extern int smf_prog_change (int fd, char channel, char prog);
extern int smf_tempo (int fd, unsigned long tempo);
extern int smf_note_on
(
   int fd,
   long dtime,
   char note,
   char vel,
   char channel
);
extern int smf_note_off
(
   int fd,
   long dtime,
   char note,
   char vel,
   char channel
);
extern int smf_track_head (int fd, unsigned long size);
extern int smf_track_end (int fd);
extern int write_var_len (int fd, long value);
extern int read_var_len (int fd, long * value);
extern int wblong (int fd, unsigned long ul);
extern int wbshort (int fd, unsigned short us);
extern void WAON_notes_output_midi
(
   waon_notes_t * notes,
   double div,
   char * filename
);

#endif         /* WAONC_MIDI_H */

/*
 * midi.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
