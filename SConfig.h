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

#ifndef SCONFIG_H_
#define SCONFIG_H_

#include "SException.h"
#include <QColor>
#include <QSettings>
#include <QDir>

#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) 			\
	public:  static TYPE NAME() { return m_##NAME; }		\
	protected: static TYPE m_##NAME; 						\
	protected: static bool m_##NAME##default;		
	
#define NEW_CONST(TYPE, NAME, VALUE, DESC)					\
	public:  static TYPE NAME() { return VALUE; }
	
#define NEW_SECTION(NAME) ;

class SConfig
{
public:
	virtual ~SConfig();
	
	static void Save();
	static void Load();
protected:
	SConfig();
	
	template <typename T>
	static void SetValue ( const QString & f_name, T f_value ) { m_settings->setValue( f_name, f_value ); };

	template <typename T>
	static bool GetValue ( const QString & f_name, T * f_result ) {			
		throwIfFalse( f_result != NULL );									
		QVariant variant = m_settings->value( f_name );
		if ( variant.isValid() && ! variant.isNull() && variant.canConvert<T>() ) { // && variant.convert<T>() ) {
     		( *f_result ) = variant.value<T>();
     		return true;
		}
		
		return false;
	};

	static QSettings * m_settings; 
	
	#include "SConfig.res"
};

const double Pi = 3.14159265358979323846264338327950288419717;

#endif /*SCONFIG_H_*/
