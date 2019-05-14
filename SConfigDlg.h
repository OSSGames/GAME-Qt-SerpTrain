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
 * $LastChangedDate: 2009-03-27 00:22:56 +0100 (Fr, 27. Mär 2009) $
 * $LastChangedRevision: 251 $
*/

#ifndef SCONFIGDLG_H
#define SCONFIGDLG_H

#include "ui_SConfigDlg.h"
#include "SConfig.h"
#include <QtGui/QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>

#define NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC)									\
	protected: static void Set##NAME ( TYPE f_value, bool f_default ) {			\
		m_##NAME = f_value;														\
		m_##NAME##default = f_default;											\
	}
	
#define NEW_CONST(TYPE, NAME, VALUE, DESC) ;	
#define NEW_SECTION(NAME) ;

class SConfigDlg : public QDialog, protected SConfig
{
    Q_OBJECT

public:
    SConfigDlg(QWidget *parent = 0);
    ~SConfigDlg();
    
    #include "SConfig.res"
   
protected slots:
	void on_saveButton_clicked() { accept(); };
	void on_cancelButton_clicked() { reject(); };
	
protected:
	void FinishTab( QWidget * curTab, QGridLayout * curLayout );
    
private:
    Ui::SConfigDlgClass ui;
};

/*
 * Template für einen Typ <T>
 * 
class SConfigItem_<T> : public QWidget<T>
{
    Q_OBJECT

public:
    SConfigItem_<T>(QWidget * f_parent, const <T> & f_curValue, bool f_default, const <T> & f_stdValue,
    				void (*f_saveFunc)(<T>, bool) ); // To Implement
    virtual ~SConfigItem_<T>() {};
    
public slots:
	void SetDefault(bool f_default) { m_default = f_default; if ( m_default ) SetValue( m_stdValue ); };
	void Save() { m_saveFunc( GetCurValue(), m_default ); };
    
signals:
	void ValueChanged(bool); // Das Signal emitiert immer 'false', da es mit der QCheckBox verbunden werden soll!
	
protected:
	<T> GetCurValue(); // To Implement
	void SetValue( <T> f_value ); // To Implement
	
private:
	<T> m_stdValue;
	bool m_default;
	void (*m_saveFunc)(<T>, bool);
};
*/

class SConfigItem_QString : public QLineEdit
{
    Q_OBJECT

public:
    SConfigItem_QString(QWidget * f_parent, const QString & f_curValue, bool f_default, const QString & f_stdValue, void (*f_saveFunc)(QString, bool) );
    virtual ~SConfigItem_QString() {};
    
public slots:
	void SetDefault(bool f_default) { m_default = f_default; if ( m_default ) SetValue( m_stdValue ); };
	void Save() { m_saveFunc( GetCurValue(), m_default ); };
    
signals:
	void ValueChanged(bool); // Das Signal emitiert immer 'false', da es mit der QCheckBox verbunden werden soll!
	
protected:
	QString GetCurValue() { return this->text(); };
	void SetValue( QString f_value ) { this->setText( f_value ); };
	
private slots:
	void QLineEditValueChanged() { if ( GetCurValue() != m_stdValue ) emit ValueChanged( false ); };
	
private:
	QString m_stdValue;
	bool m_default;
	void (*m_saveFunc)(QString, bool);
};

class SConfigItem_int : public QSpinBox
{
    Q_OBJECT

public:
    SConfigItem_int(QWidget * f_parent, const int & f_curValue, bool f_default, const int & f_stdValue,
    				void (*f_saveFunc)(int, bool) );
    
public slots:
	void SetDefault(bool f_default) { m_default = f_default; if ( m_default ) SetValue( m_stdValue ); };
	void Save() { m_saveFunc( GetCurValue(), m_default ); };
    
signals:
	void ValueChanged(bool); // Das Signal emitiert immer 'false', da es mit der QCheckBox verbunden werden soll!
	
protected:
	int GetCurValue() { return this->value(); };
	void SetValue( int f_value ) { this->setValue(f_value); };
	
private slots:
	void QSpinBoxValueChanged() { if ( GetCurValue() != m_stdValue ) emit ValueChanged( false ); };

private:
	int m_stdValue;
	bool m_default;
	void (*m_saveFunc)(int, bool);
};

class SConfigItem_bool : public QCheckBox
{
    Q_OBJECT

public:
    SConfigItem_bool(QWidget * f_parent, const bool & f_curValue, bool f_default, const bool & f_stdValue,
    				void (*f_saveFunc)(bool, bool) );
    
public slots:
	void SetDefault(bool f_default) { m_default = f_default; if ( m_default ) SetValue( m_stdValue ); };
	void Save() { m_saveFunc( GetCurValue(), m_default ); };
    
signals:
	void ValueChanged(bool); // Das Signal emitiert immer 'false', da es mit der QCheckBox verbunden werden soll!
	
protected:
	bool GetCurValue() { return this->isChecked(); };
	void SetValue( bool f_value ) { this->setChecked(f_value); };
	
private slots:
	void QCheckBoxValueChanged() { if ( GetCurValue() != m_stdValue ) emit ValueChanged( false ); };

private:
	bool m_stdValue;
	bool m_default;
	void (*m_saveFunc)(bool, bool);
};

class SConfigItem_QColor : public QPushButton
{
    Q_OBJECT

public:
    SConfigItem_QColor(QWidget * f_parent, const QColor & f_curValue, bool f_default, const QColor & f_stdValue,
    				void (*f_saveFunc)(QColor, bool) ); // To Implement
    virtual ~SConfigItem_QColor() {};
    
public slots:
	void SetDefault(bool f_default) { m_default = f_default; if ( m_default ) SetValue( m_stdValue ); };
	void Save() { m_saveFunc( GetCurValue(), m_default ); };
    
signals:
	void ValueChanged(bool); // Das Signal emitiert immer 'false', da es mit der QCheckBox verbunden werden soll!
	
protected:
	QColor GetCurValue() { return m_curValue; };
	void SetValue( QColor f_value );
	
protected slots:
	void QPushButtonClicked();
	
private:
	QColor m_curValue;
	QColor m_stdValue;
	bool m_default;
	void (*m_saveFunc)(QColor, bool);
};
#endif // SCONFIGDLG_H
