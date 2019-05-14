/* SerpTrain (a game with balls on rails)
 * Copyright (C) 2008, 2009  Michael \"Serpedon\" Walz (http://serpedon.de)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 * 
 * $LastChangedDate: 2009-03-04 12:14:03 +0100 (Mi, 04. Mär 2009) $
 * $LastChangedRevision: 222 $
*/

#ifndef SARRAY_H
#define SARRAY_H

#include "SException.h"

// This class should be used for fixed sized arrays
// They just behave like regular arrays, but can be copied by =
template<typename T, int f_size>
class SArray
{
public:
	SArray() : size(f_size) {
	};
	
	SArray(const SArray<T, f_size> & f_arrayToCopy) : size(f_size) {
		*this = f_arrayToCopy;
	};
	

	SArray<T, f_size> & operator=(const SArray<T, f_size> & f_arrayToCopy)
    {
		for ( int i = 0; i < size; i++ )
			m_array[i] = f_arrayToCopy.m_array[i];

        return *this;
    };

	T & operator[] ( int f_index) {
        throwIfFalse(f_index >= 0 && f_index < size);
        return m_array[f_index];
    };
	
	const T & operator[] ( int f_index) const {
        throwIfFalse(f_index >= 0 && f_index < size);
        return m_array[f_index];
    };
    
    const int size;

private:
	T m_array[f_size];
};

#endif /*SARRAY_H*/
