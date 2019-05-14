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

#ifndef SBOARDTILE_H_
#define SBOARDTILE_H_

#include <QObject>
#include <QList>
#include <QHash>
#include <QGraphicsScene>

#include "enum.h"
#include "SColor.h"
#include "SXMLParse.h"
#include "SXMLDoc.h"

class SBall;
class SPath;
class SGraphicsTile;

struct SCrashDetection {
	static bool IsLenTooSmall(QPointF f_abs);
	bool IsLenTooSmall() { return IsLenTooSmall(m_point); };
public:
	QPointF m_point;
	SBall * m_ball;
};

class SBoardTile : public QObject
{
	Q_OBJECT
	
public:
	SBoardTile(enum BOARD_MODE, int f_layerID, int f_gameboardID);
	virtual ~SBoardTile();
	
	void InitByXML(SXMLParse f_xmlparse);	
	
	virtual QString GetAdditionalInitErrors();

	enum DIRECTION_RELATIVE GetRelDirection ( enum DIRECTION_ABSOLUTE ) const;
	enum DIRECTION_ABSOLUTE GetAbsDirection ( enum DIRECTION_RELATIVE ) const;

	static enum DIRECTION_RELATIVE GetRelDirection ( enum DIRECTION_ABSOLUTE, enum ROTATION, enum MIRROR );
	static enum DIRECTION_ABSOLUTE GetAbsDirection ( enum DIRECTION_RELATIVE, enum ROTATION, enum MIRROR );
	
	void SetPosition(QPoint f_pos, enum ROTATION, enum MIRROR);
	void SetPosition(QPoint f_pos);
	void SetRotation(enum ROTATION);
	void SetMirror(enum MIRROR);
		
	bool IsDirectionCon ( enum DIRECTION_ABSOLUTE ) const;
	
	virtual void Timestep ( int f_timestepNr );
	void AddToScene( QGraphicsScene * );
	
	SBoardTile * Copy(enum BOARD_MODE f_boardMode = MODE_SAME) const;
	
	const QList<SCrashDetection> & CrashDetection(int f_timestep);
	
	// Setzt die Farbe und überprüft, ob sie ok ist.
	bool SetColor(SColor f_color, DIRECTION_ABSOLUTE );
	bool SetColor(SColor f_color, DIRECTION_RELATIVE );
	
	bool IsColorOk(SColor, DIRECTION_ABSOLUTE);
	virtual bool IsColorOk(SColor, DIRECTION_RELATIVE ) = 0;
	SXMLElement ExportXmlInfo(SXMLDoc f_xmlDoc);
	
	//Nur im Mode MODE_EDT_SETTING genutzt!
	//Erstellt ein CustomWidget für den Settingsdialog
	//Das QWidget liegt dann auf dem Heap und muss vom Aufrufer gelöscht werden
	virtual QWidget * CreateCustumSettingsWidget(); 
			
public slots:
	void FromNorth(SBall * f_ball) { ReceiveBallFrom( f_ball, NORTH ); };
	void FromWest (SBall * f_ball) { ReceiveBallFrom( f_ball, WEST  ); };
	void FromSouth(SBall * f_ball) { ReceiveBallFrom( f_ball, SOUTH ); };
	void FromEast (SBall * f_ball) { ReceiveBallFrom( f_ball, EAST  ); };
	
private slots:
	void ChangeColor( SColor f_color, DIRECTION_ABSOLUTE );
	void ChangeColor( SColor f_color, DIRECTION_RELATIVE );
	
	void EdtRotate(enum ROTATION_DIR);
	void EdtMirror();
	void EdtChange();
	void EdtSettings();
	
signals:
	void ToNorth(SBall * );
	void ToWest (SBall * );
	void ToSouth(SBall * );
	void ToEast (SBall * );
	
	void BallsCrashed(SBall * f_firstBall, SBall * f_secondBall = NULL); // f_secondBall kann NULL sein!
	
	// Signal wird aufgerufen, wenn das Tile im Editor gechanged werden soll
	void ReplaceTileWith(SBoardTile * f_old, SBoardTile * f_new);
	
protected:
	virtual void UpdateColorsInfo() = 0; // Muss die Farben m_colors[RIGHT]/Left... grafisch und spiel-logisch umsetzten (

	virtual void SpecialInitByXML(SXMLParse f_xmlparse) = 0;	

	virtual void AdjustXmlInfo(SXMLElement f_element, SXMLDoc f_xmlDoc) = 0;
		
	virtual void ReceiveBall(SBall *, enum DIRECTION_RELATIVE) = 0;
	virtual void SendBall( SBall *, enum DIRECTION_RELATIVE );
	virtual void SendBallAbs( SBall *, enum DIRECTION_ABSOLUTE );
	
	virtual bool IsRelDirectionCon ( enum DIRECTION_RELATIVE ) const = 0;
	virtual const SPath * GetBallPath( SBall * ) const = 0;
	virtual bool CanCrashOutsideHappen(SBall * ball1, QPointF point); // Überprüft, ob außerhalb ein Crash stattfinden kann. Wenn ja, wird der Ball mit der Bälle der Umliegenden Felder verglichen! 
	
	void SendColorInfosToEdtSheet();

	void DoTimestep( SBall * );
	virtual void InitGraphic( ) = 0;
	virtual SBoardTile * DoCopy(enum BOARD_MODE) const = 0;
	void UpdatePosition();
	int GetBallSpeed ( SBall * );	
	void ReceiveBallFrom(SBall *, enum DIRECTION_ABSOLUTE);
	
	enum ROTATION m_rotation;
	enum MIRROR m_mirror;
	QPoint m_pos;
	
	QList<SBall *> m_balls;
	QList<SCrashDetection> m_possibleCrashs;
	int m_timestepLastCrashDetection; 
	QHash<int, SGraphicsTile *> m_tiles;
	bool m_isGraphical;
	QGraphicsScene * m_scene;
	
	QHash<DIRECTION_RELATIVE, SColor> m_colors;
	
	bool m_hasBeenInit; // Has been InitByXML

	enum BOARD_MODE m_boardMode;
	
	int m_layerID;
	int m_gameboardID;
};

#endif /*SBOARDTILE_H_*/
