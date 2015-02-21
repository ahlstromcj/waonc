/*
 * Some wrappers for ao
 *
 * Copyright (C) 2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: ao-wrapper.c,v 1.4 2007/10/14 06:19:36 kichiki Exp $
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */

/**
 * \file          ao-wrapper.c
 *
 *    This module provides wrapper functions for libao.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-22
 * \version       $Revision$
 * \license       GNU GPL
 */

#include <stdlib.h>
#include <unistd.h>                    /* write()                             */
#include <ao/ao.h>

#include "macros.h"                    /* wbool_t, errprint() macros          */
#include "memory-check.h"              /* CHECK_MALLOC() macro                */

/**
 *    Shows some information about the AO processing.  This is an internal
 *    function.
 *
 * \param info
 *    Provides the information to be shown.
 *
 * \param label
 *    Provides a context label for the information.
 */

static void
print_ao_info (const ao_info * info, const char * label)
{
   fprintf
   (
      stdout,
      "%s:\n"
      "   type:            %d\n"
      "   name:            %s\n"
      "   short name:      %s\n"
      "   driver comment:  %s\n",
      label, info->type, info->name, info->short_name, info->comment
   );
}

/**
 *    Opens the ao device.  The mode of the device is 16-bits, stereo.
 *
 * \param samplerate
 *    Provides the samplerate in cycles per second.
 *
 * \param verbose
 *    Provides the verbosity level:
 *    -  0 (wfalse) == quiet
 *    -  1 (wtrue) == print information
 *
 * \return
 *    Returns a pointer to the AO device.  If the device cannot be opened,
 *    a null pointer is returned.
 */

ao_device *
ao_init_16_stereo (int samplerate, wbool_t verbose)
{
   ao_device * device;
   ao_sample_format format;
   int default_driver;
   ao_initialize();
   default_driver = ao_default_driver_id();
   format.bits = 16;
   format.channels = 2;
   format.rate = samplerate;
   format.byte_format = AO_FMT_LITTLE;
   device = ao_open_live(default_driver, &format, NULL /* no options */);
   if (is_nullptr(device))
   {
      errprint("error opening device");
      return nullptr;
   }
   if (verbose)
   {
      ao_info * info = ao_driver_info(default_driver);
      print_ao_info(info, "[ao]");
   }
   return device;
}

/**
 *    Prints a list of the libao driver information.
 */

void
print_ao_driver_info_list (void)
{
   int driver_count;
   ao_info ** info_list;
   int i;
   char label[132];
   info_list = ao_driver_info_list(&driver_count);
   for (i = 0; i < driver_count; i ++)
   {
      sprintf(label, "%d: ", i);             /* should use a SAFE sprintf()!  */
      print_ao_info(info_list [i], label);
   }
}

/**
 *    Writes stereo data to the given AO device.
 *
 * \param device
 *    Provides a pointer to the device.  This pointer must be valid; it is
 *    not checked.
 *
 * \param left
 *    Provides the data for the left channel.  This data is scaled by
 *    32768 (the maximum value of a 16-bit integer, plus 1).
 *
 * \param left
 *    Provides the data for the right channel.  This data is scaled by
 *    32768.
 *
 */

int
ao_write
(
   ao_device * device,
   double * left,
   double * right,
   int len
)
{
   int i;
   int status;
   short sl;
   short sr;
   char * buffer = (char *) malloc(len * 4 * sizeof(char));
   CHECK_MALLOC(buffer, "ao_write");
   for (i = 0; i < len; i++)
   {
      sl = (short) (left[i] * 32768.0);
      sr = (short) (right[i] * 32768.0);
      buffer[i*4 + 0] =  sl       & 0xff;
      buffer[i*4 + 1] = (sl >> 8) & 0xff;
      buffer[i*4 + 2] =  sr       & 0xff;
      buffer[i*4 + 3] = (sr >> 8) & 0xff;
   }
   status = ao_play(device, buffer, len * 4);
   if (status != 0)
      status = len * 4;
   else
      errprint("ao_write() returned a zero status");

   free(buffer);
   return status;
}

/*
 * ao-wrapper.c
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
