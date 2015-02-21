#ifndef	WAONC_HC_H
#define	WAONC_HC_H

/*
 * Header file for hc.c
 *
 * Half-complex format routines
 *
 * Copyright (C) 2007 Kengo Ichiki <kichiki@users.sourceforge.net>
 * $Id: hc.h,v 1.3 2007/02/16 06:21:54 kichiki Exp $
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
 * \file          hc.h
 *
 *    This module provides half-complex format routines.
 *
 * \library       libwaonc
 * \author        Kengo Ichiki with modifications by Chris Ahlstrom
 * \date          2007-02-28
 * \updates       2013-12-23
 * \version       $Revision$
 * \license       GNU GPL
 */

#include "macros.h"                    /* wbool_t */

extern void HC_to_polar
(
   long len,
   const double * freq,
   wbool_t conjugate,
   double * amp,
   double * phs
);
extern void HC_to_polar2
(
   long len,
   const double * freq,
   wbool_t conjugate,
   double scale,
   double * amp2,
   double * phs
);
extern void HC_to_amp2
(
   long len,
   const double * freq,
   double scale,
   double * amp2
);
extern void polar_to_HC
(
   long len,
   const double * amp,
   const double * phs,
   wbool_t conjugate,
   double * freq
);
extern void polar_to_HC_scale
(
   long len,
   const double * amp,
   const double * phs,
   wbool_t conjugate,
   int scale,
   double * freq
);
extern void HC_mul
(
   long len, const double * x, const double * y, double * z
);
extern void HC_div (long len, const double * x, const double * y, double * z);
extern void HC_abs (long len, const double * x, double * z);
extern void HC_puckette_lock (long len, const double * y, double * z);
extern void HC_complex_phase_vocoder
(
   int len,
   const double * fs,
   const double * ft,
   const double * f_out_old,
   double * f_out
);

#endif         /* WAONC_HC_H */

/*
 * hc.h
 *
 * vim: sw=3 ts=3 wm=8 et ft=c
 */
