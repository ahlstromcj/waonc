#ifndef WAONC_AO_WRAPPER_H_
#define WAONC_AO_WRAPPER_H_

/* Header file for ao-wrapper.c
 *
 * Some wrappers for AO.
 *
 * Copyright (C) 2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: ao-wrapper.h,v 1.2 2007/10/14 06:20:17 kichiki Exp $
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
 * \file          ao-wrapper.h
 *
 *    This module provides functions to interact with libao.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-22
 * \version       $Revision$
 * \license       GNU GPL
 */

#include "macros.h"                    /* to define wbool_t                   */

extern ao_device * ao_init_16_stereo (int samplerate, wbool_t verbose);
extern void print_ao_driver_info_list (void);
extern int ao_write
(
   ao_device * device,
   double * left,
   double * right,
   int len
);

#endif         /* WAONC_AO_WRAPPER_H_ */

/*
 * ao-wrapper.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
