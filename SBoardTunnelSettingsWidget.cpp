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

#include "SBoardTunnelSettingsWidget.h"
#include "SColor.h"
#include "SException.h"
#include "SConfig.h"
#include <QPixmap>
#include <QPainter>

class STableTunnelQueueItem : public QTableWidgetItem
{
public:
	STableTunnelQueueItem(SColor f_ballColor = FIRST_BALL_COLOR, int f_creatingTime = 0) {
		setData(CREATING_TIME, f_creatingTime);
		setData(BALL_COLOR, (int) ((enum COLOR) f_ballColor));
		
	}
	
	virtual ~STableTunnelQueueItem() {};
	
	bool operator<( const QTableWidgetItem & f_other) const {
		return data(CREATING_TIME).toInt() <  f_other.data(CREATING_TIME).toInt();
	}
	
	void setData( int f_role, const QVariant & f_value ) {
		QTableWidgetItem::setData(f_role, f_value);
		
		switch ( f_role ) {
			case CREATING_TIME:
				setText(f_value.toString());
				break;
			case BALL_COLOR:				
				setIcon(SBoardTunnelSettingsWidget::GetBallIcon(GetColor().GetQColor()));
				break;
		}
	}
	
	SColor GetColor() {
		return ((enum COLOR) data(BALL_COLOR).toInt() );	
	}
	
	const static enum Qt::ItemDataRole CREATING_TIME = Qt::UserRole;
	const static enum Qt::ItemDataRole BALL_COLOR = (Qt::ItemDataRole) ( Qt::UserRole + 1 );
};


SBoardTunnelSettingsWidget::SBoardTunnelSettingsWidget(const QQueue<STunnelQueue> & f_queue)
    : QWidget(NULL)
{
	ui.setupUi(this);
	
	for ( SColor ballColor = FIRST_BALL_COLOR; ballColor <= LAST_BALL_COLOR; ++ballColor) {
		QRadioButton * button = new QRadioButton(this);
		button->setIcon(GetBallIcon(ballColor));
		ui.colorBallLayout->addWidget(button);
		
		throwIfFalse(QObject::connect(button, SIGNAL(clicked()), this, SLOT(radioButtons_clicked())));
				
		m_radioButtons.insert(ballColor, button);
	}
	
	m_radioButtons[(enum COLOR) FIRST_BALL_COLOR]->setChecked(true);
	
	ui.tableBalls->setItemPrototype(new STableTunnelQueueItem());
	
	SetTunnelQueue(f_queue);
}

SBoardTunnelSettingsWidget::~SBoardTunnelSettingsWidget()
{

}

QIcon SBoardTunnelSettingsWidget::GetBallIcon(QColor f_color) {
	QPixmap pixmap(16,16);
	pixmap.fill(SConfig::LAYER_ACTIVE_1());
	QPainter painter(&pixmap);
	painter.setBrush(f_color);
	painter.setPen(Qt::NoPen);
	QRect rect = QRect(	( pixmap.width() - SConfig::RAIL_WIDTH() ) / 2,
						( pixmap.height() - SConfig::RAIL_WIDTH() ) / 2,
						SConfig::RAIL_WIDTH(), SConfig::RAIL_WIDTH() );
	painter.drawEllipse( rect );
	return pixmap;	
}

QIcon SBoardTunnelSettingsWidget::GetTunnelIcon(QColor f_color) {
	QPixmap pixmap(16,16);
	pixmap.fill(f_color);
	return pixmap;	
};

void SBoardTunnelSettingsWidget::on_addButton_clicked() {
	int row = ui.tableBalls->rowCount();
	ui.tableBalls->setRowCount(row + 1);
	ui.tableBalls->setItem(0, row, new STableTunnelQueueItem(GetCheckedColor(), ui.timeSpinBox->value()));
	TableDataChanged();
}

void SBoardTunnelSettingsWidget::on_deleteButton_clicked() {
	int row = GetCurTableRow();
	if ( row != NO_ROW_SEL ) {
		ui.tableBalls->removeRow(row);
		TableDataChanged();
	}
}

void SBoardTunnelSettingsWidget::on_conTunnelButton_clicked() {
	emit ConnectButtonPressed();
}

void SBoardTunnelSettingsWidget::on_timeSpinBox_editingFinished() {
	int time = ui.timeSpinBox->value();
	int row = GetCurTableRow();
	if ( row != NO_ROW_SEL ) {
		ui.tableBalls->item(row, 0)->setData(STableTunnelQueueItem::CREATING_TIME, time);
		TableDataChanged();
	}
}

int SBoardTunnelSettingsWidget::GetCurTableRow() {
	QList<QTableWidgetSelectionRange> ranges = ui.tableBalls->selectedRanges();
	int row;
	if ( ranges.size() == 1 && ( row = ranges[0].bottomRow() ) == ranges[0].topRow() )
		return row;
	
	return NO_ROW_SEL;
} 

void SBoardTunnelSettingsWidget::TableDataChanged() {
	ui.tableBalls->sortByColumn(0, Qt::AscendingOrder);
	
	m_tunnelqueue.clear();
	
	int curTime = 0;
	
	for ( int row = 0; row < ui.tableBalls->rowCount(); row++ ) {
		int time = ui.tableBalls->item(row, 0)->data(STableTunnelQueueItem::CREATING_TIME).toInt();
		enum COLOR color =(enum COLOR) ui.tableBalls->item(row, 0)->data(STableTunnelQueueItem::BALL_COLOR).toInt();
		
		if ( time <= curTime ) {
			// Kein Waitobjekt nötig
		} else {
			STunnelQueue wait; curTime++; //++, da das Löschen jeweils auch eine Zeitschritt benötigt
			wait.type = STunnelQueue::WAIT;
			wait.data.wait.timesteps = time - curTime; curTime = time;
			m_tunnelqueue.enqueue(wait);
		}
	
		STunnelQueue ball; curTime++; //++, da das Löschen jeweils auch eine Zeitschritt benötigt
		ball.type = STunnelQueue::BALL;
		ball.data.ball.ballColor = color;
		
		m_tunnelqueue.enqueue(ball);
	}
	
	
	emit TunnelQueueChanged( m_tunnelqueue );
}

void SBoardTunnelSettingsWidget::on_tableBalls_itemSelectionChanged() {
	int row = GetCurTableRow();
	if ( row != NO_ROW_SEL ) {
		QTableWidgetItem * item = ui.tableBalls->item(GetCurTableRow(), 0);
		
		// Time setzen
		int time = item->data(STableTunnelQueueItem::CREATING_TIME).toInt();
		ui.timeSpinBox->setValue(time);
		
		// Farben setzen
		enum COLOR color = (enum COLOR) item->data(STableTunnelQueueItem::BALL_COLOR).toInt();
		throwIfFalse(m_radioButtons[color] != NULL );
		m_radioButtons[color]->setChecked(true);
	}
}

void SBoardTunnelSettingsWidget::radioButtons_clicked() {
	int row = GetCurTableRow();
	if ( row != NO_ROW_SEL ) {	 
		ui.tableBalls->item(row, 0)->setData(STableTunnelQueueItem::BALL_COLOR, (int) GetCheckedColor());
		TableDataChanged();
	}
}

enum COLOR SBoardTunnelSettingsWidget::GetCheckedColor() {
	QHashIterator<enum COLOR, QRadioButton *> i(m_radioButtons);
	while (i.hasNext()) {
		i.next();
		if ( i.value()->isChecked() )
			return i.key();
	}
	
	throwEx("No color is checked!");
	return BLACK;
}

void SBoardTunnelSettingsWidget::SetTunnelQueue( const QQueue<STunnelQueue> & f_queue) {
	int curTime = 0;
	for (int queueNr = 0; queueNr < f_queue.size(); queueNr++) {
		STunnelQueue curQueue = f_queue.at(queueNr);
		switch ( curQueue.type ) {
			case STunnelQueue::BALL:
			{
				
				int row = ui.tableBalls->rowCount();
				ui.tableBalls->setRowCount(row + 1);
				ui.tableBalls->setItem(0, row, new STableTunnelQueueItem(curQueue.data.ball.ballColor, curTime));
				break;
			}
			case STunnelQueue::WAIT: 
			{
				curTime += curQueue.data.wait.timesteps;
				break;
			}
			default:
				throwEx("The value of 'curQueue.type' is invalid: " + QString::number(curQueue.type));
		}
		curTime++; // Da das löschen der Objekte aus der Queue auch eine Zeitschritt dauert!
	}

	TableDataChanged();
}

void SBoardTunnelSettingsWidget::SetConnectButtonMode( enum ConnectButtonMode f_mode, QColor f_color /* = QColor()*/ ) {
	switch ( f_mode ) {
		case DISCONNECT_TUNNEL:
			ui.conTunnelButton->setText("Disconnect &tunnel");
			ui.conTunnelButton->setIcon(QIcon ()); 
			break;
		case CONNECT_TUNNEL_WITH:
			ui.conTunnelButton->setText("Connect &tunnel with ...");
			ui.conTunnelButton->setIcon(QIcon ()); 
			break;
		case CONNECT_WITH_THIS_TUNNEL:
			ui.conTunnelButton->setText("Connect colored &tunnel with this one");
			ui.conTunnelButton->setIcon(GetTunnelIcon(f_color)); 
			break;
		default:
			throwEx("Unvalid 'enum ConnectButtonMode' value: " + QString::number(f_mode));
	}
}
