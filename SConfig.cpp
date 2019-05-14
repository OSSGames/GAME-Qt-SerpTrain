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
 * $LastChangedDate: 2009-03-27 15:47:24 +0100 (Fr, 27. Mär 2009) $
 * $LastChangedRevision: 252 $
*/

#include "SConfig.h"

SConfig::SConfig()
{
}

SConfig::~SConfig()
{
}

#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) TYPE SConfig::m_##NAME = STD_VALUE; bool SConfig::m_##NAME##default = true;		
#define NEW_CONST(TYPE, NAME, VALUE, DESC) ;
#define NEW_SECTION(NAME) ;

#include "SConfig.res"

/*static*/ QSettings * SConfig::m_settings = NULL; 

/*static*/ void SConfig::Save() {
	if ( m_settings == NULL )
		m_settings = new QSettings();
	
	#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) {						\
		SetValue("Config/" #NAME "_isDefault", m_##NAME##default );			\
		if ( ! m_##NAME##default )											\
			SetValue("Config/" #NAME, m_##NAME);							\
	}
	
	#define NEW_CONST(TYPE, NAME, VALUE, DESC)	;
	#define NEW_SECTION(NAME) ;
	
	#include "SConfig.res"
	
	m_settings->sync();	
}

/*static*/ void SConfig::Load() {
	if ( m_settings == NULL )
		m_settings = new QSettings();
		
	#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) {								\
		if ( ! GetValue("Config/" #NAME "_isDefault", & m_##NAME##default ) )		\
			m_##NAME##default = true;												\
		if ( ! m_##NAME##default ) {												\
			if ( ! GetValue("Config/" #NAME, & m_##NAME) ) {						\
				m_##NAME##default = true;											\
				m_##NAME = STD_VALUE;												\
			}																		\
		}																			\
	}
	
	#define NEW_CONST(TYPE, NAME, VALUE, DESC)	;
	#define NEW_SECTION(NAME) ;
	
	#include "SConfig.res"
	
}
