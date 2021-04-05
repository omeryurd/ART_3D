/* DTrackParse: C++ header file, A.R.T. GmbH 3.5.07-09.07.10
 *
 * Functions for processing data
 * Copyright (C) 2007-2010, Advanced Realtime Tracking GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Version v2.0
 *
 * $Id: DTrackParse.hpp,v 1.2 2010/07/09 09:03:20 sebastianz Exp $
 */

#ifndef _ART_DTRACKPARSE_H_
#define _ART_DTRACKPARSE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

// Parsing DTrack data

/**
 *	\brief	Search next line in buffer
 *	@param 	str		buffer (total)
 *	@param 	start	start position within buffer
 *	@param 	len		buffer length in bytes
 *	@return	begin of line, NULL if no new line in buffer
 */
char* string_nextline(char* str, char* start, int len);

/**
 *	\brief	Read next 'int' value from string
 *	@param[in] 	str		string
 *	@param[out] i		read value
 *	@return	pointer behind read value in str; NULL in case of error
 */
char* string_get_i(char* str, int* i);

/**
 *	\brief	Read next 'unsigned int' value from string
 *	@param[in] 	str		string
 *	@param[out]	ui		read value
 *	@return	pointer behind read value in str; NULL in case of error
 */
char* string_get_ui(char* str, unsigned int* ui);

/**
 *	\brief	Read next 'double' value from string
 *	@param[in] 	str		string
 *	@param[out] d		read value
 *	@return pointer behind read value in str; NULL in case of error
 */
char* string_get_d(char* str, double* d);

/**
 *	\brief	Read next 'float' value from string:
 *	@param[in] 	str		string
 *	@param[out] f 		read value
 *	@return pointer behind read value in str; NULL in case of error
 */
char* string_get_f(char* str, float* f);

/**
 *	\brief Process next block '[...]' in string
 *	@param[in] 	str		string
 *	@param[in] 	fmt		format string ('i' for 'int', 'f' for 'float')
 *	@param[out] idat	array for 'int' values (long enough due to fmt)
 *	@param[out] fdat	array for 'float' values (long enough due to fmt)
 *	@param[out] ddat	array for 'double' values (long enough due to fmt)
 *	@return pointer behind read value in str; NULL in case of error
 */
char* string_get_block(char* str, const char* fmt, int* idat = NULL, float* fdat = NULL, double *ddat = NULL);

/**
 *	\brief	Read next 'word' value from string
 *	@param[in] 	str		string
 *	@param[out] w		read value
 *	@return	pointer behind read value in str; NULL in case of error
 */
char* string_get_word(char* str, std::string& w);

/**
 *	\brief	Read next 'quoted text' value from string
 *	@param[in] 	str		string
 *	@param[out] qt		read value (without quotes)
 *	@return pointer behind read value in str; NULL in case of error
 */
char* string_get_quoted_text(char* str, std::string& qt);

/**
 *	\brief	Compare strings regarding DTrack2 parameter rules
 *	@param 	str		string
 *	@param 	p		parameter string
 *	@return pointer behind parameter in str; NULL in case of error
 */
char* string_cmp_parameter(char* str, const char* p);

#endif /* _ART_DTRACKPARSE_H_ */
