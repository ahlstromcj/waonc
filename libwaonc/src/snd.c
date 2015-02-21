/* some wrapper for libsndfile
 * Copyright (C) 2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: snd.c,v 1.4 2007/10/21 04:03:30 kichiki Exp $
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * \file          snd.c
 *
 *    This module provides ...
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-11-17
 * \version       $Revision$
 * \license       GNU GPL
 */

#include <stdlib.h>
#include <string.h>                    /* memset()                      */
#include <sndfile.h>

#include "macros.h"                    /* wbool_t and errprint() macros */
#include "memory-check.h"              /* CHECK_MALLOC() macro          */
#include "snd.h"                       /* this module's functions       */

long sndfile_read
(
   SNDFILE * sf,
   SF_INFO sfinfo,
   double * left,
   double * right,
   int len
)
{
   static double * buf = NULL;
   static int nbuf = 0;
   sf_count_t status;
   if (buf == NULL)
   {
      buf = (double *)malloc (sizeof (double) * len * sfinfo.channels);
      CHECK_MALLOC (buf, "sndfile_read");
      nbuf = len * sfinfo.channels;
   }
   if (len * sfinfo.channels > nbuf)
   {
      buf = (double *)realloc (buf, sizeof (double) * len * sfinfo.channels);
      CHECK_MALLOC (buf, "sndfile_read");
      nbuf = len * sfinfo.channels;
   }
   if (sfinfo.channels == 1)
   {
      status = sf_readf_double (sf, left, (sf_count_t)len);
   }
   else
   {
      int i;
      status = sf_readf_double (sf, buf, (sf_count_t)len);
      for (i = 0; i < len; i ++)
      {
         left  [i] = buf [i * sfinfo.channels];
         right [i] = buf [i * sfinfo.channels + 1];
      }
   }

   return ((long) status);
}

long sndfile_read_at
(
   SNDFILE * sf,
   SF_INFO sfinfo,
   long start,
   double * left,
   double * right,
   int len
)
{
   sf_count_t status;
   if (start < 0 || start >= sfinfo.frames)        /* check the range */
      return 0;

   status = sf_seek(sf, (sf_count_t) start, SEEK_SET);   /* start point */
   if (status == -1)
   {
      errprint("seek error");
      exit(1);
   }
   return sndfile_read(sf, sfinfo, left, right, len);
}

/**
 * print sfinfo
 */

void sndfile_print_info (SF_INFO * sfinfo)
{
   /* Major types */

   switch (sfinfo->format & SF_FORMAT_TYPEMASK)
   {
   case SF_FORMAT_WAV:
      infoprint("Format: Microsoft WAV format (little endian default).");
      break;

   case SF_FORMAT_AIFF:
      infoprint("Format: Apple/SGI AIFF format (big endian).");
      break;

   case SF_FORMAT_AU:
      infoprint("Format: Sun/NeXT AU format (big endian).");
      break;

   case SF_FORMAT_RAW:
      infoprint("Format: RAW PCM data.");
      break;

   case SF_FORMAT_PAF:
      infoprint("Format: Ensoniq PARIS file format.");
      break;

   case SF_FORMAT_SVX:
      infoprint("Format: Amiga IFF / SVX8 / SV16 format.");
      break;

   case SF_FORMAT_NIST:
      infoprint("Format: Sphere NIST format.");
      break;

   case SF_FORMAT_VOC:
      infoprint("Format: VOC files.");
      break;

   case SF_FORMAT_IRCAM:
      infoprint("Format: Berkeley/IRCAM/CARL");
      break;

   case SF_FORMAT_W64:
      infoprint("Format: Sonic Foundry's 64 bit RIFF/WAV");
      break;

   case SF_FORMAT_MAT4:
      infoprint("Format: Matlab (tm) V4.2 / GNU Octave 2.0");
      break;

   case SF_FORMAT_MAT5:
      infoprint("Format: Matlab (tm) V5.0 / GNU Octave 2.1");
      break;

   case SF_FORMAT_PVF:
      infoprint("Format: Portable Voice Format");
      break;

   case SF_FORMAT_XI:
      infoprint("Format: Fasttracker 2 Extended Instrument");
      break;

   case SF_FORMAT_HTK:
      infoprint("Format: HMM Tool Kit format");
      break;

   case SF_FORMAT_SDS:
      infoprint("Format: Midi Sample Dump Standard");
      break;

   case SF_FORMAT_AVR:
      infoprint("Format: Audio Visual Research");
      break;

   case SF_FORMAT_WAVEX:
      infoprint("Format: MS WAVE with WAVEFORMATEX");
      break;

   case SF_FORMAT_SD2:
      infoprint("Format: Sound Designer 2");
      break;

   case SF_FORMAT_FLAC:
      infoprint("Format: FLAC lossless file format");
      break;

   case SF_FORMAT_CAF:
      infoprint("Format: Core Audio File format");
      break;

   default :
      errprint("unknown Subtype");
      exit(1);
   }

   /* Subtypes */

   switch (sfinfo->format & SF_FORMAT_SUBMASK)
   {
   case SF_FORMAT_PCM_S8:
      infoprint("Subtype: Signed 8 bit data");
      break;

   case SF_FORMAT_PCM_16:
      infoprint("Subtype: Signed 16 bit data");
      break;

   case SF_FORMAT_PCM_24:
      infoprint("Subtype: Signed 24 bit data");
      break;

   case SF_FORMAT_PCM_32:
      infoprint("Subtype: Signed 32 bit data");
      break;

   case SF_FORMAT_PCM_U8:
      infoprint("Subtype: Unsigned 8 bit data (WAV and RAW only)");
      break;

   case SF_FORMAT_FLOAT:
      infoprint("Subtype: 32 bit float data");
      break;

   case SF_FORMAT_DOUBLE:
      infoprint("Subtype: 64 bit float data");
      break;

   case SF_FORMAT_ULAW:
      infoprint("Subtype: U-Law encoded.");
      break;

   case SF_FORMAT_ALAW:
      infoprint("Subtype: A-Law encoded.");
      break;

   case SF_FORMAT_IMA_ADPCM:
      infoprint("Subtype: IMA ADPCM.");
      break;

   case SF_FORMAT_MS_ADPCM:
      infoprint("Subtype: Microsoft ADPCM.");
      break;

   case SF_FORMAT_GSM610:
      infoprint("Subtype: GSM 6.10 encoding.");
      break;

   case SF_FORMAT_VOX_ADPCM:
      infoprint("Subtype: Oki Dialogic ADPCM encoding.");
      break;

   case SF_FORMAT_G721_32:
      infoprint("Subtype: 32kbs G721 ADPCM encoding.");
      break;

   case SF_FORMAT_G723_24:
      infoprint("Subtype: 24kbs G723 ADPCM encoding.");
      break;

   case SF_FORMAT_G723_40:
      infoprint("Subtype: 40kbs G723 ADPCM encoding.");
      break;

   case SF_FORMAT_DWVW_12:
      infoprint("Subtype: 12 bit Delta Width Variable Word encoding.");
      break;

   case SF_FORMAT_DWVW_16:
      infoprint("Subtype: 16 bit Delta Width Variable Word encoding.");
      break;

   case SF_FORMAT_DWVW_24:
      infoprint("Subtype: 24 bit Delta Width Variable Word encoding.");
      break;

   case SF_FORMAT_DWVW_N:
      infoprint("Subtype: N bit Delta Width Variable Word encoding.");
      break;

   case SF_FORMAT_DPCM_8:
      infoprint("Subtype: 8 bit differential PCM (XI only)");
      break;

   case SF_FORMAT_DPCM_16:
      infoprint("Subtype: 16 bit differential PCM (XI only)");
      break;

   default :
      errprint("unknown Subtype");
      exit(1);
   }

   /* Endian */

   switch (sfinfo->format & SF_FORMAT_ENDMASK)
   {
   case SF_ENDIAN_FILE:
      infoprint("Endian type: Default file endian-ness.");
      break;

   case SF_ENDIAN_LITTLE:
      infoprint("Endian type: Force little endian-ness.");
      break;

   case SF_ENDIAN_BIG:
      infoprint("Endian type: Force big endian-ness.");
      break;

   case SF_ENDIAN_CPU:
      infoprint("Endian type: Force CPU endian-ness.");
      break;

   default :
      errprint("unknown Endian type");
      exit(1);
   }
   fprintf
   (
      stdout,
      "   Frames:             %d\n"
      "   Samplerate:         %d\n"
      "   Channels:           %d\n"
      "   Sections:           %d\n"
      "   Seekable:           %d\n"
      ,
      (int) sfinfo->frames, sfinfo->samplerate, sfinfo->channels,
      sfinfo->sections, sfinfo->seekable
   );
}

/**
 * OUTPUT (returned value)
 *
 *  -1 : no extension or unsupported format
 *  0  : wav
 *  1  : flac
 */

int
check_filetype_by_extension (const char * file)
{
   int len = strlen (file);
   int i;
   for (i = len - 1; i >= 0; i--)
   {
      if (file [i] == '.') break;
   }

   if (file [i] != '.')
      return -1; /* no extension */
   else if (strcmp (file + i + 1, "wav") == 0)
      return 0; /* wav */
   else if (strcmp (file + i + 1, "flac") == 0)
      return 1; /* flac */
   else
      return -1; /* unsupported format */
}

/**
 * output functions
 */

SNDFILE *
sndfile_open_for_write
(
   SF_INFO * sfinfo,
   const char * file,
   int samplerate,
   int channels
)
{
   SNDFILE * sf = NULL;
   int mode = check_filetype_by_extension(file);
   if (mode < 0)
   {
      fprintf
      (
         stderr, "unknown or unsupported format %s, wav format forced.\n", file
      );
      mode = 0;                        /* wav */
   }
   memset(sfinfo, 0, sizeof (*sfinfo));
   switch (mode)                       /* format */
   {
   case 0:                             /* wav */
      sfinfo->format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_FILE;
      break;

   case 1:                             /* flac */
      sfinfo->format = SF_FORMAT_FLAC | SF_FORMAT_PCM_16 | SF_ENDIAN_FILE;
      break;

   default:
      errprintf("? invalid mode %d\n", mode);
      exit(1);
   }

   /* sfinfo->frames = 0; */

   sfinfo->samplerate = samplerate;
   sfinfo->channels = channels;
   sfinfo->sections = 1;
   sfinfo->seekable = 1;
   sf = sf_open(file, SFM_WRITE, sfinfo);
   if (sf == NULL)
   {
      errprintf("failed to open %s\n", file);
      exit(1);
   }
   return sf;
}

long
sndfile_write
(
   SNDFILE * sf,
   SF_INFO sfinfo,
   double * left,
   double * right,
   int len
)
{
   sf_count_t status;
   if (sfinfo.channels == 1)
   {
      status = sf_writef_double (sf, left, (sf_count_t)len);
   }
   else
   {
      int i;
      double * buf = (double *) malloc(sizeof (double) * len * sfinfo.channels);
      CHECK_MALLOC(buf, "sndfile_write");
      for (i = 0; i < len; i ++)
      {
         buf[i * sfinfo.channels]     = left[i];
         buf[i * sfinfo.channels + 1] = right[i];
      }
      status = sf_writef_double(sf, buf, (sf_count_t)len);
      free(buf);
   }
   return (long) status;
}

/*
 * snd.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
