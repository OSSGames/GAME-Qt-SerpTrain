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

#ifndef SBOARDTUNNELSETTINGSWIDGET_H
#define SBOARDTUNNELSETTINGSWIDGET_H

#include <QtGui/QWidget>
#include "SColor.h"
#include "SBoardTunnel.h"
#include <QRadioButton>
#include "ui_SBoardTunnelSettingsWidget.h"

class SBoardTunnelSettingsWidget : public QWidget
{
    Q_OBJECT

public:
	SBoardTunnelSettingsWidget(const QQueue<STunnelQueue> &);
	~SBoardTunnelSettingsWidget();
    
	static QIcon GetBallIcon(QColor f_color);
	static QIcon GetTunnelIcon(QColor f_color);
	
	enum ConnectButtonMode { DISCONNECT_TUNNEL, CONNECT_TUNNEL_WITH, CONNECT_WITH_THIS_TUNNEL };
	void SetConnectButtonMode( enum ConnectButtonMode, QColor f_color = QColor() ); // f_color wird nur im Modus CONNECT_WITH_THIS_TUNNEL benötigt 
	
signals:
	void TunnelQueueChanged( const QQueue<STunnelQueue> & );
	void ConnectButtonPressed();
    
private slots:
	void on_addButton_clicked();
	void on_deleteButton_clicked();
	void on_conTunnelButton_clicked();
	void on_timeSpinBox_editingFinished();	
	void on_timeSpinBox_valueChanged() { on_timeSpinBox_editingFinished(); }; 
	void on_tableBalls_itemSelectionChanged();
	void radioButtons_clicked();

private:
	void SetTunnelQueue( const QQueue<STunnelQueue> & );

	int GetCurTableRow(); // return NO_ROW_SEL if no row seleted
	enum COLOR GetCheckedColor();
	void TableDataChanged();
	
	static const int NO_ROW_SEL = -1;
	Ui::BoardTunnelSettingsWidget ui;
	QHash<enum COLOR, QRadioButton *> m_radioButtons;
	QQueue<STunnelQueue> m_tunnelqueue;
};

#endif // SBOARDTUNNELSETTINGSWIDGET_H
