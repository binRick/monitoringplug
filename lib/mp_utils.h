/***
 * monitoringplug - mp_utils.h
 **
 *
 * Copyright (C) 2011 Marius Rieder <marius.rieder@durchmesser.ch>
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id$
 */

#ifndef MP_UTILS_H_
#define MP_UTILS_H_

#include <stdlib.h>

/**
 * Call malloc, call critical if faild.
 */
void *mp_malloc(size_t size);

/**
 * Call calloc, call critical if faild.
 */
void *mp_calloc(size_t nmemb, size_t size);

/**
 * Call realloc, call critical if faild.
 */
void *mp_realloc(void *ptr, size_t size);


#endif /* MP_UTILS_H_ */