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
 * $LastChangedDate: 2009-03-23 16:06:13 +0100 (Mo, 23. Mär 2009) $
 * $LastChangedRevision: 246 $
*/

#include "SLayer.h"
#include "SLayerGraphics.h"
#include "SGameBoard.h"
#include "SBoardGoal.h"
#include "SBoardTurn.h"
#include "SBoard2Turn.h"
#include "SBoardStraight.h"
#include "SBoardSwitch.h"
#include "SBoardTunnel.h"
#include "SException.h"
#include "SBoardNone.h"
#include "SConfig.h"
#include "SXMLConv.h"
#include "SBoardSwitchTurn.h"
#include "SEdtLayerDlg.h"
#include "SChgLayerSizeDlg.h"

/*static */ int SLayer::m_nextID = 0;

SLayer::SLayer(enum BOARD_MODE f_boardMode, int f_gameboardID ) : m_boardTiles ( NULL ), m_layerSize ( QSize(0,0) ), m_mainWindow ( NULL ),  
	m_goalToFinish ( 0 ), m_isGraphical ( false ), m_hasBeenInit ( false ),
	m_boardMode ( f_boardMode ), m_ID ( m_nextID++ ), m_gameboardID  ( f_gameboardID )
{ }

void SLayer::InitByXML(SXMLParse f_layer)
{	
	throwIfFalse ( m_hasBeenInit == false );
	m_hasBeenInit = true;

    m_invisible = f_layer.GetBoolAttribute("invisible", false);
    
    switch ( m_boardMode ) {
    	case MODE_PLAY:
    		m_isGraphical = ! m_invisible;
    		break;
    	case MODE_TEST:
    	case MODE_EDITOR:
    		m_isGraphical = true;
    		break;
    	default:
    		throwEx("Unvalid 'enum BOARD_MODE' value: " + QString::number( m_boardMode ) );
    }    	
    
 	m_layerSize = QSize(f_layer.GetIntAttribute("width", 0, 1), f_layer.GetIntAttribute("height", 0, 1));
 	
 	if ( m_layerSize.isEmpty() )
 		throwOwnEx(SXMLParseException, "The layer should not be empty!" ); 

	if ( m_isGraphical ) {
		m_mainWindow = new SLayerGraphics(m_invisible, m_boardMode);
	
		// Signale durchschleifen	
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(TestLevel()), this, SIGNAL(TestLevel())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(WindowClosed()), this, SIGNAL(WindowClosed())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(SaveLevel()), this, SIGNAL(SaveLevel())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(SaveLevelAs()), this, SIGNAL(SaveLevelAs())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(EditLevel()), this, SIGNAL(EditLevel())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(AddLayer()), this, SIGNAL(AddLayer())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(RestartLevel()), this, SIGNAL(RestartLevel())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(ShowLevel()), this, SIGNAL(ShowLevel())));
	
		// Schnittstellen von SLayerGraphics
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(EditLayer()), this, SLOT(EditLayer())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(ChangeLayerSize()), this, SLOT(ChangeLayerSize())));
		throwIfFalse(QObject::connect(m_mainWindow, SIGNAL(DelLayer()), this, SLOT(DelLayer())));
	}
    
	m_boardTiles = new SBoardTile * * [m_layerSize.width()];
	for ( int col = 0; col < m_layerSize.width(); col++ ) 
		m_boardTiles[col] = new SBoardTile * [m_layerSize.height()];
		
	SXMLParse line = f_layer.GetSubElement();

	for ( int row = 0; row < m_layerSize.height(); row++ ) {
		line.ToNextElement("line");
		
		SXMLParse boardtile = line.GetSubElement();
		for ( int col = 0; col < m_layerSize.width(); col++ ) {
			boardtile.ToNextElement("boardtile");
			
			{
				switch ( BOARDTILES result = boardtile.GetAttribute("type", SXMLConv::GetBoardtilesNames(), SXMLConv::GetBoardtilesValues(), NONE) )
				{
					case GOAL:
						m_boardTiles[col][row] = new SBoardGoal(m_boardMode, m_ID, m_gameboardID);
						break;
					case STRAIGHT:
						m_boardTiles[col][row] = new SBoardStraight(m_boardMode, m_ID, m_gameboardID);
						break;
					case NONE:
						m_boardTiles[col][row] = new SBoardNone(m_boardMode, m_ID, m_gameboardID);
						break;
					case TURN:
						m_boardTiles[col][row] = new SBoardTurn(m_boardMode, m_ID, m_gameboardID);
						break;
					case DOUBLE_TURN:
						m_boardTiles[col][row] = new SBoard2Turn(m_boardMode, m_ID, m_gameboardID);
						break;						
					case SWITCH_TURN:
						m_boardTiles[col][row] = new SBoardSwitchTurn(m_boardMode, m_ID, m_gameboardID);
						break;
					case SWITCH:
						m_boardTiles[col][row] = new SBoardSwitch(m_boardMode, m_ID, m_gameboardID);
						break;
					case TUNNEL:
						m_boardTiles[col][row] = new SBoardTunnel(m_boardMode, m_ID, m_gameboardID);
						break;
					default:
						throwEx("The result of SXMLParse.GetAttribute() is unvalid: " + QString::number(result)); 
				}
			}
			
			m_boardTiles[col][row]->InitByXML( boardtile.GetLocked());
			m_boardTiles[col][row]->SetPosition(QPoint(SConfig::FIELD_SIZE()*col, SConfig::FIELD_SIZE()*row));
			
			if ( m_boardMode == MODE_EDITOR ) {
				throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(ReplaceTileWith(SBoardTile *, SBoardTile *)), this, SLOT(ReplaceTileWith(SBoardTile *, SBoardTile *))));
			}				
			
			if ( m_isGraphical )
				m_mainWindow->AddToScene(m_boardTiles[col][row]);
		}
	}
	
	for ( int col = 0; col < m_layerSize.width(); col++ ) {
		for ( int row = 0; row < m_layerSize.height(); row++ ) {
	    	throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(ToNorth(SBall * )), m_boardTiles[col][AdjRow(row-1)], SLOT(FromSouth(SBall * ))));
	    	throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(ToSouth(SBall * )), m_boardTiles[col][AdjRow(row+1)], SLOT(FromNorth(SBall * ))));
	    	throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(ToWest (SBall * )), m_boardTiles[AdjCol(col-1)][row], SLOT(FromEast (SBall * ))));
	    	throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(ToEast (SBall * )), m_boardTiles[AdjCol(col+1)][row], SLOT(FromWest (SBall * ))));
		    	
			throwIfFalse(QObject::connect(m_boardTiles[col][row], SIGNAL(BallsCrashed (SBall *, SBall *)), this, SIGNAL(BallsCrashed (SBall *, SBall *))));
			
			// Ziele untereinander und mit SLayer verbinden
			SBoardGoal * goal = qobject_cast<SBoardGoal *>(m_boardTiles[col][row]);
			if ( goal != NULL ) {
				int colOther = col;
				int rowOther = row;
				enum DIRECTION_ABSOLUTE dirOtherNeeded;
				switch ( goal->GetAbsDirection(RIGHT) ) {
					case NORTH:
						rowOther = AdjRow(row - 1);
						dirOtherNeeded = SOUTH;
						break;
					case SOUTH:
						rowOther = AdjRow(row + 1);
						dirOtherNeeded = NORTH;
						break;
					case WEST:
						colOther = AdjCol(col - 1);
						dirOtherNeeded = EAST;
						break;
					case EAST:
						colOther = AdjCol(col + 1);
						dirOtherNeeded = WEST;
						break;
					default:
						throwEx("Unvalid Direction: " + QString::number(goal->GetAbsDirection(RIGHT)));
				}
				
				SBoardGoal * goalOther = qobject_cast<SBoardGoal *>(m_boardTiles[colOther][rowOther]);
				// Testen, ob das Zielstück auch richtig herum angeschlossen ist!
				if ( goalOther != NULL && goalOther->GetAbsDirection(LEFT) == dirOtherNeeded ) {
					goalOther->SetPassive();
					throwIfFalse(QObject::connect(goal, SIGNAL(GoalFull()), goalOther, SLOT(SetActive ())));
				} else {
					m_goalToFinish++;
					// letztes Ziel; melde 'Full' an SLayer
					
					throwIfFalse(QObject::connect(goal, SIGNAL(GoalFull()), this, SLOT(GoalFinished ())));
				}
			}
		}
	}
	
	if ( m_goalToFinish <= 0 ) {
		// Wenn's keine Ziele gibt, ist das Layer schon Ziel-fertig
		emit LayerFinished();
	}
	
	if ( m_isGraphical ) {
		m_mainWindow->Show();
	}	
}

SLayer::~SLayer()
{
	for ( int col = 0; col < m_layerSize.width(); col++ ) {
		for ( int row = 0; row < m_layerSize.height(); row++ ) {
			delete m_boardTiles[col][row];
		}
		delete[] m_boardTiles[col];
	}
	delete[] m_boardTiles;
	
	// Aufräumen
	delete m_mainWindow; //Scene (innerhalb SLayerGraphics) muss nach den boardTiles gelöscht werden, weil die Boardtiles, die auf einer Scene liegen sonnst auch gelöscht werden
}

void SLayer::Timestep(int f_timestep) {
 	for ( int col = 0; col < m_layerSize.width(); col++ ) {
		for ( int row = 0; row < m_layerSize.height(); row++ ) {
			if ( m_boardTiles[col][row] != NULL ) {
				m_boardTiles[col][row]->Timestep(f_timestep);
			}
		}
	}
	
	//CrashDetection
	for ( int col = 0; col < m_layerSize.width(); col++ ) {
		for ( int row = 0; row < m_layerSize.height(); row++ ) {
			CheckCrashs(m_boardTiles[col][row]->CrashDetection(f_timestep),
						m_boardTiles[AdjCol(col+1)][row]->CrashDetection(f_timestep),
						m_boardTiles[col][AdjRow(row+1)]->CrashDetection(f_timestep));
		}
	}
}

int SLayer::AdjCol(int f_col) {
	while ( f_col < 0 )
		f_col += m_layerSize.width();

	while ( f_col >= m_layerSize.width() )
		f_col -= m_layerSize.width();
		
	return f_col;
}

int SLayer::AdjRow(int f_row) {
	while ( f_row < 0 )
		f_row += m_layerSize.height();

	while ( f_row >= m_layerSize.height() )
		f_row -= m_layerSize.height();
	
	return f_row;
}

void SLayer::CheckCrashs(const QList<SCrashDetection> & f_tile, const QList<SCrashDetection> & f_right, const QList<SCrashDetection> & f_bottom ) {
	QPointF rightAdj = QPointF(SConfig::FIELD_SIZE_PATH(), 0);
	QPointF bottomAdj = QPointF(0, SConfig::FIELD_SIZE_PATH());
	
	for (int i = 0; i < f_tile.size(); ++i) {
		QPointF curPoint = f_tile.at(i).m_point;
		// Nach rechts schauen!
		for (int j=0; j < f_right.size(); ++j ) {
			QPointF abs = curPoint - ( f_right.at(j).m_point + rightAdj);
			if ( SCrashDetection::IsLenTooSmall(abs) ) {
				emit BallsCrashed(f_tile.at(i).m_ball, f_right.at(j).m_ball);
			}
		}
		// Nach unten
		for (int j=0; j < f_bottom.size(); ++j ) {
			QPointF abs = curPoint - ( f_bottom.at(j).m_point + bottomAdj);
			if ( SCrashDetection::IsLenTooSmall(abs) ) {
				emit BallsCrashed(f_tile.at(i).m_ball, f_bottom.at(j).m_ball);
			}
		}
	}	
}

void SLayer::GoalFinished() {
	m_goalToFinish--;
	throwIfFalse(m_goalToFinish >= 0);
	if ( m_goalToFinish == 0 )
		emit LayerFinished();
}

SXMLElement SLayer::ExportXmlInfo(SXMLDoc f_xmlDoc) {
	throwIfFalse(m_boardMode == MODE_EDITOR);
	
	SXMLElement xmlLayer = f_xmlDoc.CreateElement("layer");
	xmlLayer.setAttribute("width", QString::number(m_layerSize.width()));
	xmlLayer.setAttribute("height", QString::number(m_layerSize.height()));
	if ( m_invisible )
		xmlLayer.SetBoolAttribute("invisible", m_invisible);
	
	for ( int row = 0; row < m_layerSize.height(); row++ ) {
		SXMLElement line = f_xmlDoc.CreateElement("line");
		xmlLayer.appendChild(line);
		for ( int col = 0; col < m_layerSize.width(); col++ ) {
	    	line.appendChild(m_boardTiles[col][row]->ExportXmlInfo(f_xmlDoc));
		}
	}
	
	return xmlLayer; 
}
// Slot wird aufgerufen, wenn ein Tile im Editor gechanged werden soll
void SLayer::ReplaceTileWith(SBoardTile * f_old, SBoardTile * f_new) {
	throwIfFalse(m_boardMode == MODE_EDITOR);
	throwIfFalse(f_old != NULL);
	throwIfFalse(f_new != NULL);

	int row, col;
	throwIfFalse(GetTilePos(f_old, &row, &col));
	
	ReplaceTileWith(f_new, row, col);
}

void SLayer::ReplaceTileWith(SBoardTile * f_new, int f_row, int f_col) {
	delete m_boardTiles[f_col][f_row];
	m_boardTiles[f_col][f_row] = f_new;
	
	m_boardTiles[f_col][f_row]->SetPosition(QPoint(SConfig::FIELD_SIZE()*f_col, SConfig::FIELD_SIZE()*f_row));
	throwIfFalse(QObject::connect(m_boardTiles[f_col][f_row], SIGNAL(ReplaceTileWith(SBoardTile *, SBoardTile *)), this, SLOT(ReplaceTileWith(SBoardTile *, SBoardTile *))));
	m_mainWindow->AddToScene(m_boardTiles[f_col][f_row]);
}



// Sucht nach dem Tile und gibt (über f_row/f_col) seine Spalte und Zeile zurück
// Rückgabewert gibt an, ob es gefunden wurde
bool SLayer::GetTilePos(SBoardTile * f_tile, int * f_row, int * f_col) {
	throwIfFalse(f_row != NULL && f_col != NULL);
	
	for ( int row = 0; row < m_layerSize.height(); row++ ) {
		for ( int col = 0; col < m_layerSize.width(); col++ ) {
			if ( m_boardTiles[col][row] == f_tile ) {
				(*f_row) = row;
				(*f_col) = col;
				return true;
			}
		}
	}	
	
	return false;
}

QString SLayer::GetAdditionalInitErrors() {
	QString result;

	for ( int col = 0; col < m_layerSize.width(); col++ ) {
		for ( int row = 0; row < m_layerSize.height(); row++ ) {
			QString error = m_boardTiles[col][row]->GetAdditionalInitErrors();
			if ( ! error.isEmpty() ) {
				if ( ! result.isEmpty() )
					result += "; ";
					
				result += error;
			}
		}
	}
	
	return result;
}

void SLayer::SetEnabled(bool f_enable) {
	if ( m_isGraphical )
		m_mainWindow->setVisible(f_enable);
}

void SLayer::CloseWindow() {
	if ( m_isGraphical )
		m_mainWindow->close();
}

void SLayer::EditLayer() {
	throwIfFalse(m_mainWindow != NULL);
	SEdtLayerDlg dialog(m_mainWindow, m_invisible, m_layerSize.height(), m_layerSize.width());
	
	if ( QDialog::Accepted == dialog.exec() ) {
		m_invisible = dialog.IsLayerHidden();
		m_mainWindow->SetIsTunnelLayer(m_invisible);		
	}
} 

       
void SLayer::ChangeLayerSize() {
	throwIfFalse(m_mainWindow != NULL);
	SChgLayerSizeDlg dialog(m_mainWindow, m_layerSize.height(), m_layerSize.width());
	
	if ( QDialog::Accepted == dialog.exec() ) {
		// Spalten bearbeiten
		if ( dialog.AddCol() ) {
			// Spalten hinzufügen
			int amount = dialog.AmountColToAdd();
			int before = dialog.AddColBefore();
			int newWidth = amount + m_layerSize.width();
			SBoardTile * * * oldArray = m_boardTiles;
			m_boardTiles = new SBoardTile * * [newWidth];
			m_layerSize.rwidth() = newWidth;

			for ( int col = 0; col < newWidth; col++ ) {
				if ( col < before )
					m_boardTiles[col] = oldArray[col];
				else if ( col < before + amount ) {
					m_boardTiles[col] = new SBoardTile * [m_layerSize.height()];
					for ( int row = 0; row < m_layerSize.height(); row++ ) {
						m_boardTiles[col][row] = NULL;
						SBoardTile * newTile = new SBoardNone(m_boardMode, m_ID, m_gameboardID);
						ReplaceTileWith(newTile, row, col);
					}
				}
				else
					m_boardTiles[col] = oldArray[col-amount];
			}
				
			// altes Array löschen
			delete[] oldArray;
			oldArray = NULL;
		} else {
			// Spalten löschen
			int delIndex = dialog.DeleteColAt();
			m_layerSize.rwidth()--;
			
			// Spalte aus Array löschen 
			for ( int row = 0; row < m_layerSize.height(); row++ ) {
				delete m_boardTiles[delIndex][row];
				m_boardTiles[delIndex][row] = NULL;
			}
			delete[] m_boardTiles[delIndex];
			m_boardTiles[delIndex] = NULL; 
			
			for ( int col = delIndex; col < m_layerSize.width(); col++ ) {
				m_boardTiles[col] = m_boardTiles[col+1];
			}
		}
		
		// Zeilen bearbeiten
		if ( dialog.AddRow() ) {
			// Zeilen hinzufügen
			int amount = dialog.AmountRowToAdd();
			int before = dialog.AddRowBefore();
			int newHeight = amount + m_layerSize.height();
			m_layerSize.rheight() = newHeight;

			for ( int col = 0; col < m_layerSize.width(); col++ ) {
				SBoardTile * * oldArray = m_boardTiles[col];
				m_boardTiles[col] = new SBoardTile * [m_layerSize.height()];
				for ( int row = 0; row < m_layerSize.height(); row++ ) {
					if ( row < before )
						m_boardTiles[col][row] = oldArray[row];
					else if ( row < before + amount ) {
						m_boardTiles[col][row] = NULL;
						SBoardTile * newTile = new SBoardNone(m_boardMode, m_ID, m_gameboardID);
						ReplaceTileWith(newTile, row, col);
					}
					else
						m_boardTiles[col][row] = oldArray[row - amount];
				}
				
				// altes Array löschen
				delete[] oldArray;
				oldArray = NULL;
			}
		} else {
			// Zeilen löschen
			int delIndex = dialog.DeleteRowAt();
			m_layerSize.rheight()--;
			
			for ( int col = 0; col < m_layerSize.width(); col++ ) {
				// Das Zeilen-Element aus dieser Spalte löschen 
				delete m_boardTiles[col][delIndex];
				m_boardTiles[col][delIndex] = NULL;
				
				// Alle weiteren Elemente aufrücken lassen
				for ( int row = delIndex; row < m_layerSize.height(); row++ )
					m_boardTiles[col][row] = m_boardTiles[col][row+1];
			}
		}
		
		// Alle Boardtiles wieder an die richtige Stelle setzen
		for ( int col = 0; col < m_layerSize.width(); col++ )
			for ( int row = 0; row < m_layerSize.height(); row++ ) {
				throwIfFalse(m_boardTiles[col][row] != NULL);	
				m_boardTiles[col][row]->SetPosition(QPoint(SConfig::FIELD_SIZE()*col, SConfig::FIELD_SIZE()*row));
		}
		
		m_mainWindow->AdjustLayerSize();
	}
} 

void SLayer::DelLayer() {
	emit DeleteThisLayer(this);
} 

QWidget * SLayer::Widget() {
	return qobject_cast<QWidget * >(m_mainWindow);
}

void SLayer::SetIsClosable(bool f_closable) {
	if ( m_isGraphical ) {
		m_mainWindow->SetIsClosable(f_closable);
	}
}
