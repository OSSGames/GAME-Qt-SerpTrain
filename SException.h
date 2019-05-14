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
 * $LastChangedDate: 2009-03-25 14:11:18 +0100 (Mi, 25. Mär 2009) $
 * $LastChangedRevision: 250 $
*/

#ifndef SEXCEPTION_H_
#define SEXCEPTION_H_

#include <stdexcept>
#include <QString>
#include <QObject>

class SException : public std::exception
{
public:
	SException(QString f_msg);
	virtual ~SException() throw();
	virtual const char * what() const throw();

	static void Report(std::exception & e);
private:
	QString m_msg;
};

#define throwEx(f_errorMsg) throw SCriticalException((QString)f_errorMsg + "\nFile: " + __FILE__ ", Line: " + QString::number(__LINE__)) 
#define throwOwnEx(exception, f_errorMsg) throw exception((QString)f_errorMsg + "\nFile: " + __FILE__ ", Line: " + QString::number(__LINE__))

#define throwIfFalse(expression) 												\
		if ( expression )														\
			;																	\
		else																	\
			throwEx("The term '" #expression "' evaluated to false!" )
			

#define DefineNewException(exception) 					\
	class exception : public SException					\
	{													\
	public:												\
		exception(QString f_msg);						\
		virtual ~exception() throw();					\
	};
 
DefineNewException(SCriticalException)
DefineNewException(SDirNotConException)
DefineNewException(SXMLParseException)

#define qensure_cast(TYPE, POINTER)	get_ensure_cast_pointer(POINTER, (TYPE) NULL, #TYPE, #POINTER, __FILE__, QString::number(__LINE__))

template <typename T>
T get_ensure_cast_pointer(QObject * f_pointer, T, QString type, QString pointer, QString file, QString line ) {
	T tempPointer = qobject_cast<T>(f_pointer);
	if ( tempPointer == NULL )
		throw SCriticalException((QString)pointer + " is not a valid '" + type + "'-pointer!" + "\nFile: " + file + ", Line: " + line);
		
	return tempPointer;
}

#endif /*SEXCEPTION_H_*/

