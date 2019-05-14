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
 * $LastChangedDate: 2009-03-09 12:52:29 +0100 (Mo, 09. Mär 2009) $
 * $LastChangedRevision: 235 $
*/

#include "SBoardTile.h"
#include "SBoardGoal.h"
#include "SBoardTurn.h"
#include "SBoardStraight.h"
#include "SBoardSwitch.h"
#include "SBoardSwitchTurn.h"
#include "SBoard2Turn.h"
#include "SBoardTunnel.h"
#include "SException.h"
#include "SBoardNone.h"
#include "SConfig.h"
#include "SBall.h"
#include "SPath.h"
#include "SGraphicsEdtColors.h"
#include "SGraphicsTile.h"
#include "SGraphicsEdtSheet.h"
#include "SEdtSettingsDlg.h"
#include "SXMLConv.h"
#include <math.h>
#include <QMessageBox>
#include <QtGui>

SBoardTile::SBoardTile(enum BOARD_MODE f_boardMode, int f_layerID, int f_gameboardID) : m_rotation ( ROTATION_NO ) , m_mirror ( MIRROR_NO ),
	m_pos ( 0, 0 ), m_timestepLastCrashDetection ( 0 ), m_isGraphical ( false ), m_scene ( NULL ),
	m_hasBeenInit ( false ), m_boardMode ( f_boardMode ), m_layerID ( f_layerID ), m_gameboardID ( f_gameboardID )
{
	m_colors[TOP] = BLACK;
	m_colors[RIGHT] = BLACK;
	m_colors[BOTTOM] = BLACK;
	m_colors[LEFT] = BLACK;
}

SBoardTile::~SBoardTile()
{
	// Achtung; die Kugel/Tiles müssen umständlich über deleteLater gelöscht werden, da sie evtl.
	// aus anderen Threads kommen; außerdem werden alle Tiles/Bälle gelöscht!
 	QHash<int, SGraphicsTile *>::iterator tile;
	for (tile = m_tiles.begin(); tile != m_tiles.end(); tile++) {
		throwIfFalse((*tile) != NULL );
		(*tile)->RemoveFromScene();
		(*tile)->deleteLater();
	}
 	m_tiles.clear();
 	
 	while (!m_balls.isEmpty()) {
 		SBall * ball = m_balls.takeFirst();
 		if ( m_isGraphical )
 			ball->RemoveFromScene();
 		ball->deleteLater();
 	}
}

void SBoardTile::SetPosition(QPoint f_pos, enum ROTATION f_rotation, enum MIRROR f_mirror) {
	m_rotation = f_rotation;
	m_mirror = f_mirror;
	m_pos = f_pos;
	
	UpdatePosition();
}

void SBoardTile::SetPosition(QPoint f_pos) {
	m_pos = f_pos;
	
	UpdatePosition();
}

void SBoardTile::SetRotation(enum ROTATION f_rotation) {
	m_rotation = f_rotation;
	
	UpdatePosition();
}

void SBoardTile::SetMirror(enum MIRROR f_mirror) {
	m_mirror = f_mirror;
	
	UpdatePosition();
}

// Rechnet die Absolute Richtung in die Relative um
enum DIRECTION_RELATIVE SBoardTile::GetRelDirection ( enum DIRECTION_ABSOLUTE f_dirAbs) const {
	return GetRelDirection(f_dirAbs, m_rotation, m_mirror);
}

/*static */ enum DIRECTION_RELATIVE SBoardTile::GetRelDirection ( enum DIRECTION_ABSOLUTE f_dirAbs, enum ROTATION f_rotation, enum MIRROR f_mirror){
	if ( f_dirAbs == DIR_ABS_SPECIAL )
		return DIR_SPECIAL;

	return (enum DIRECTION_RELATIVE) ( ( (f_dirAbs - f_rotation) * f_mirror + 4 ) % 4);
}

enum DIRECTION_ABSOLUTE SBoardTile::GetAbsDirection ( enum DIRECTION_RELATIVE f_dirRel ) const {
	return GetAbsDirection(f_dirRel, m_rotation, m_mirror);
}

/*static*/ enum DIRECTION_ABSOLUTE SBoardTile::GetAbsDirection ( enum DIRECTION_RELATIVE f_dirRel, enum ROTATION f_rotation, enum MIRROR f_mirror ){
	if ( f_dirRel == DIR_SPECIAL )
		return DIR_ABS_SPECIAL;
	
	return (enum DIRECTION_ABSOLUTE) ( ( f_dirRel * f_mirror + f_rotation + 4 ) % 4);
}

void SBoardTile::ReceiveBallFrom(SBall * f_ball, enum DIRECTION_ABSOLUTE f_dirAbs) {
	if ( IsDirectionCon ( f_dirAbs ) ) {
		m_balls << f_ball;
		ReceiveBall(f_ball, GetRelDirection(f_dirAbs));
		DoTimestep(f_ball);
	}
	else {
		// Ball zurückschicken, damit er noch innerhalb eines Boardtiles registriert ist!
		// Dort soll er aber nicht mehr weiterfahren!
		f_ball->m_steps = 0;
		SendBallAbs( f_ball, f_dirAbs );
		
		// Crash melden!
		emit BallsCrashed(f_ball);
	}
}

bool SBoardTile::IsDirectionCon ( enum DIRECTION_ABSOLUTE f_dirAbs ) const {
	return IsRelDirectionCon(GetRelDirection ( f_dirAbs ) ); 
}

void SBoardTile::SendBallAbs( SBall * f_ball, enum DIRECTION_ABSOLUTE f_dirAbs ){
	switch ( f_dirAbs ) {
		case NORTH:
			emit ToNorth( f_ball );
			break;
		case SOUTH:
			emit ToSouth( f_ball );
			break;
		case WEST:
			emit ToWest( f_ball );
			break;
		case EAST:
			emit ToEast( f_ball );
			break;
		default:
			throwEx("Unvalid 'enum DIRECTION_ABSOLUTE' value: " + QString::number( f_dirAbs ) );
	}
}

void SBoardTile::SendBall( SBall * f_ball, enum DIRECTION_RELATIVE f_dirRel ) {
	if ( ! IsRelDirectionCon( f_dirRel ) )
		throwOwnEx(SDirNotConException, "This tile has no connection to this direction!");
		
	throwIfFalse(m_balls.removeAll(f_ball) == 1);
	
	SendBallAbs ( f_ball, GetAbsDirection( f_dirRel ) );
}

void SBoardTile::Timestep ( int f_timestepNr ) {
	foreach (SBall * ball, m_balls ) {
		if ( ball->m_lastTimestep + 1 == f_timestepNr ) {
			// f_timestepNr ist der nächste nötige Zeitschritt	
			DoTimestep ( ball );
		}
		else {
			// Ansonsten darf es höchstens sein, dass dieser Zeitschritt gerade schon ausgeführt wurde
			throwIfFalse ( ball->m_lastTimestep == f_timestepNr );
		}
	}
}

void SBoardTile::DoTimestep ( SBall * f_ball ) {
	const SPath * path = GetBallPath( f_ball );

	f_ball->m_steps += GetBallSpeed( f_ball );
	
	if ( f_ball->m_steps > path->GetPathLen() ) {
		f_ball->m_steps -= path->GetPathLen();
		
		SendBall(f_ball, path->GetExit());
	}
	else {
		// Zeitschritt fertig!
		f_ball->m_lastTimestep++;
		
		
		if ( m_isGraphical ) {
			QPointF relPos = path->GetGraphicalPosOnPath(f_ball->m_steps);
			f_ball->SetRelativePosition(m_pos, relPos, m_rotation, m_mirror);
		}
	}
} 

int SBoardTile::GetBallSpeed ( SBall * ) {
	return SConfig::DEFAULT_PATH_SPEED();
}

void SBoardTile::AddToScene( QGraphicsScene * f_scene) {
	if ( m_isGraphical )
		throwEx("SBoardTile is already added to a scene!" );
	else {
		m_isGraphical = true;
		m_scene = f_scene;
		InitGraphic();
		
		// Im Editor wird einfach das Config-Menü darüber gelegt
		if ( m_boardMode == MODE_EDITOR ) {
			SGraphicsEdtSheet * edtSheet = new SGraphicsEdtSheet();
			m_tiles[TILE_EDITOR_SHEET] = edtSheet;
			
			throwIfFalse(QObject::connect(edtSheet, SIGNAL(EdtRotate(enum ROTATION_DIR)), this, SLOT(EdtRotate(enum ROTATION_DIR)))); 
			throwIfFalse(QObject::connect(edtSheet, SIGNAL(EdtMirror()), this, SLOT(EdtMirror()))); 
			throwIfFalse(QObject::connect(edtSheet, SIGNAL(EdtChange()), this, SLOT(EdtChange()))); 
			throwIfFalse(QObject::connect(edtSheet, SIGNAL(EdtSettings()), this, SLOT(EdtSettings()))); 
		} else if ( m_boardMode == MODE_EDT_SETTING) {
			SGraphicsEdtColors * edtColors = new SGraphicsEdtColors();
			m_tiles[TILE_EDITOR_COLOR_SHEET] = edtColors;
			
			SendColorInfosToEdtSheet();
			
			throwIfFalse(QObject::connect(edtColors, SIGNAL(ColorChanged(SColor, DIRECTION_ABSOLUTE)), this, SLOT(ChangeColor(SColor, DIRECTION_ABSOLUTE)))); 
		}	
		
		
 		// Alle zur f_scene hinzufügen 
 		QHash<int, SGraphicsTile *>::iterator tile;
 		for (tile = m_tiles.begin(); tile != m_tiles.end(); tile++) {
 			throwIfFalse((*tile) != NULL );
 			(*tile)->AddToScene(f_scene);
 		}
		
		UpdatePosition();
		UpdateColorsInfo();
	}
}
	
SBoardTile * SBoardTile::Copy(enum BOARD_MODE f_boardMode) const {
	if ( f_boardMode == MODE_SAME )
		f_boardMode = m_boardMode;
	
	SBoardTile * result = DoCopy(f_boardMode);
	
	// Kopiere die relevaten SBoardTile-Informationen
	result->m_rotation = m_rotation;
	result->m_mirror = m_mirror;
	result->m_colors = m_colors;
	result->m_hasBeenInit = m_hasBeenInit;
		
	return result;
}

void SBoardTile::UpdatePosition() {
	if ( m_isGraphical ) {
 		QHash<int, SGraphicsTile *>::iterator tile;
 		for (tile = m_tiles.begin(); tile != m_tiles.end(); tile++) {
 			throwIfFalse((*tile) != NULL );
 			(*tile)->SetPosition(m_pos, m_rotation, m_mirror);
 		}
	}
}

const QList<SCrashDetection> & SBoardTile::CrashDetection(int f_timestep) {
	if ( f_timestep != m_timestepLastCrashDetection ) {
		m_possibleCrashs.clear();
		
		SCrashDetection crashDet;
		// Crashs Suchen
		for (int i = 0; i < m_balls.size(); ++i) {
			SBall * ball1 = m_balls.at(i); 
			QPointF point = GetBallPath( ball1 )->GetPosOnPath(ball1->m_steps);
			for (int j = i + 1; j < m_balls.size(); ++j) {
				SBall * ball2 = m_balls.at(j);
				QPointF abs = GetBallPath( ball2 )->GetPosOnPath(ball2->m_steps) - point;

				if ( SCrashDetection::IsLenTooSmall(abs) ) {
					emit BallsCrashed(ball1, ball2);
				}
			}
			
			if ( CanCrashOutsideHappen(ball1, point) )
			{
				// Crash possible
				// Die Koordinaten von außer betrachtet berechnen
				crashDet.m_point = SGraphicsItem::TransformRelPosition(point, m_rotation, m_mirror);
				crashDet.m_ball = ball1;
				m_possibleCrashs << crashDet;
			}
		}
		
		m_timestepLastCrashDetection = f_timestep; 
	}
	
	return m_possibleCrashs;
}

bool SCrashDetection::IsLenTooSmall(QPointF f_abs)
{
	return ( sqrt(pow(f_abs.x(),2) + pow(f_abs.y(),2)) < SConfig::RAIL_WIDTH_PATH() );
}

bool SBoardTile::CanCrashOutsideHappen(SBall * /*ball1*/, QPointF point) {
	int outerLimit = SConfig::FIELD_SIZE_PATH() / 2 - SConfig::RAIL_WIDTH_PATH();
	
	return ( qAbs(point.x()) >= outerLimit || qAbs(point.y()) >= outerLimit );
}

void SBoardTile::InitByXML(SXMLParse f_boardtile) 
{
	throwIfFalse ( m_hasBeenInit == false );
	m_hasBeenInit = true;

	SpecialInitByXML(f_boardtile.GetLocked());

	// Rotation
	{
		enum ROTATION rotation = f_boardtile.GetAttribute("rotation", SXMLConv::GetRotationNames(), SXMLConv::GetRotationValues(), ROTATION_NO);
		this->SetRotation(rotation);
	}
	

	// Mirror
	{
		enum MIRROR mirror = f_boardtile.GetAttribute("mirror", SXMLConv::GetMirrorNames(), SXMLConv::GetMirrorValues(), MIRROR_NO);
		this->SetMirror(mirror);
	}

	// Farben
	{
		SColor colorTop 	= f_boardtile.GetAttribute("colorTop",    SXMLConv::GetColorNames(), SXMLConv::GetColorValues(), SColor(BLACK) );
		SColor colorRight 	= f_boardtile.GetAttribute("colorRight",  SXMLConv::GetColorNames(), SXMLConv::GetColorValues(), SColor(BLACK) );
		SColor colorBottom	= f_boardtile.GetAttribute("colorBottom", SXMLConv::GetColorNames(), SXMLConv::GetColorValues(), SColor(BLACK) );
		SColor colorLeft	= f_boardtile.GetAttribute("colorLeft",   SXMLConv::GetColorNames(), SXMLConv::GetColorValues(), SColor(BLACK) );
		
		if ( this->SetColor(colorTop, TOP) == false )
			f_boardtile.AddError("The color 'colorTop' is somehow silly!");
		if ( this->SetColor(colorRight, RIGHT) == false )
			f_boardtile.AddError("The color 'colorRight' is somehow silly!");
		if ( this->SetColor(colorBottom, BOTTOM) == false )
			f_boardtile.AddError("The color 'colorBottom' is somehow silly!");
		if ( this->SetColor(colorLeft, LEFT) == false )
			f_boardtile.AddError("The color 'colorLeft' is somehow silly!");
	}
}

void SBoardTile::EdtRotate(enum ROTATION_DIR f_rotationDir) {
	throwIfFalse(m_boardMode == MODE_EDITOR);
	
	if ( f_rotationDir == ROTATION_LEFT ) {
		switch ( m_rotation ) {
			case ROTATION_NO:
				SetRotation(ROTATION_270);
				break;
			case ROTATION_90:
				SetRotation(ROTATION_NO);
				break;
			case ROTATION_180:
				SetRotation(ROTATION_90);
				break;
			case ROTATION_270:
				SetRotation(ROTATION_180);
				break;
			default:
				throwEx("Unvalid 'enum ROTATION' value: " + QString::number( m_rotation ) );
		}
	} else if ( f_rotationDir == ROTATION_RIGHT ) {
		switch ( m_rotation ) {
			case ROTATION_NO:
				SetRotation(ROTATION_90);
				break;
			case ROTATION_90:
				SetRotation(ROTATION_180);
				break;
			case ROTATION_180:
				SetRotation(ROTATION_270);
				break;
			case ROTATION_270:
				SetRotation(ROTATION_NO);
				break;
			default:
				throwEx("Unvalid 'enum ROTATION' value: " + QString::number( m_rotation ) );
		}
	} else
		throwEx("Unvalid 'enum ROTATION_DIR' value: " + QString::number( f_rotationDir ) );
}

void SBoardTile::EdtMirror() {
	throwIfFalse(m_boardMode == MODE_EDITOR);

	// Die Spiegelung toggeln
	SetMirror((MIRROR) (m_mirror * -1));
	
	// Zusätzlich noch so drehen, dass es aussieht, als wäre an der momentanen Y-Achse gespiegelt worden
	if ( m_rotation == ROTATION_90 )
		SetRotation(ROTATION_270);
	else if ( m_rotation == ROTATION_270 )
		SetRotation(ROTATION_90);
}

void SBoardTile::EdtChange(){
	throwIfFalse(m_boardMode == MODE_EDITOR);
	
	// ToDo: Das Ganze in eine Dialog-Klasse einbauen! 
	QMessageBox msgBox;
	
	#define AddButton(TYPE, NAME, ICON, HOTKEY, BOARDTILE)							\
		QPushButton * TYPE##Button = msgBox.addButton("", QMessageBox::ActionRole);	\
		TYPE##Button->setIconSize(QSize(60, 60));									\
		QIcon TYPE##Icon = QIcon(ICON);												\
		TYPE##Button->setIcon(TYPE##Icon);											\
		TYPE##Button->setToolTip(NAME "\nHotkey: " HOTKEY);							\
		TYPE##Button->setShortcut(QKeySequence(HOTKEY));							\
		if ( qobject_cast<BOARDTILE * >(this) != NULL )								\
			msgBox.setEscapeButton(TYPE##Button);
	
	AddButton(none, "None", ":/icons/none.png", "1", SBoardNone);
	AddButton(straight, "Straight", ":/icons/straight.png", "2", SBoardStraight);
	AddButton(turn, "Turn", ":/icons/turn.png", "3", SBoardTurn);
	AddButton(doubleTurn, "DoubleTurn", ":/icons/doubleTurn.png", "4", SBoard2Turn);
	AddButton(switch, "Switch", ":/icons/switch.png", "5", SBoardSwitch);
	AddButton(switchTurn, "SwitchTurn", ":/icons/switchTurn.png", "6", SBoardSwitchTurn);
	AddButton(tunnel, "Tunnel", ":/icons/tunnel.png", "7", SBoardTunnel);
	AddButton(goal, "Goal", ":/icons/goal.png", "8", SBoardGoal);
			
	msgBox.setWindowTitle("Change Tile");
	msgBox.setText("What tile type do you want?");
	
	msgBox.exec();
	
	SBoardTile * newBoardtile = NULL;
	
	#define CheckButton(TYPE, BOARDTILE)												\
		if (msgBox.clickedButton() == TYPE##Button) {									\
			if ( qobject_cast<BOARDTILE * >(this) == NULL ) {							\
				newBoardtile = new BOARDTILE(m_boardMode, m_layerID, m_gameboardID);	\
			}																			\
		} else 
	
	CheckButton(none, SBoardNone)
	CheckButton(straight, SBoardStraight)
	CheckButton(turn, SBoardTurn)
	CheckButton(doubleTurn, SBoard2Turn)
	CheckButton(switch, SBoardSwitch)
	CheckButton(switchTurn, SBoardSwitchTurn)
	CheckButton(tunnel, SBoardTunnel)
	CheckButton(goal, SBoardGoal)
		throwEx("Unknown Button pressed!");
		
	if ( newBoardtile != NULL )
		emit ReplaceTileWith(this, newBoardtile);
}

void SBoardTile::EdtSettings(){
	throwIfFalse(m_boardMode == MODE_EDITOR);
	
	SBoardTile * boardtile = this->Copy(MODE_EDT_SETTING);
	
	//Settingsdialog erstellen und initialisieren
	SEdtSettingsDlg settingsDlg(boardtile);	
	
	//Anzeigen und bei Änderungen, das Boardtile austauschen
	if ( settingsDlg.exec() == QDialog::Accepted ) {
		emit ReplaceTileWith(this, boardtile->Copy(m_boardMode));
	}
}

SXMLElement SBoardTile::ExportXmlInfo(SXMLDoc f_xmlDoc) {
	throwIfFalse(m_boardMode == MODE_EDITOR);
	QString shouldBeSetByAdjustXmlInfo = "ShouldBeSetBy AdjustXmlInfo";
	
	SXMLElement xmlBoardtile = f_xmlDoc.CreateElement("boardtile");
	xmlBoardtile.setAttribute("type", shouldBeSetByAdjustXmlInfo);
	xmlBoardtile.SetAttribute("rotation",    m_rotation,    SXMLConv::GetRotationNames(), SXMLConv::GetRotationValues());
	xmlBoardtile.SetAttribute("mirror",      m_mirror,      SXMLConv::GetMirrorNames(),   SXMLConv::GetMirrorValues());
	
	if ( m_colors[TOP] != BLACK )
		xmlBoardtile.SetAttribute("colorTop",    m_colors[TOP],    SXMLConv::GetColorNames(),    SXMLConv::GetColorValues());
	if ( m_colors[RIGHT] != BLACK )
		xmlBoardtile.SetAttribute("colorRight",  m_colors[RIGHT],  SXMLConv::GetColorNames(),    SXMLConv::GetColorValues());
	if ( m_colors[BOTTOM] != BLACK )
		xmlBoardtile.SetAttribute("colorBottom", m_colors[BOTTOM], SXMLConv::GetColorNames(),    SXMLConv::GetColorValues());
	if ( m_colors[LEFT] != BLACK )
		xmlBoardtile.SetAttribute("colorLeft",   m_colors[LEFT],   SXMLConv::GetColorNames(),    SXMLConv::GetColorValues());
		
	// Die Unterklassen sollen die Infos vervollständigen
	AdjustXmlInfo(xmlBoardtile, f_xmlDoc);
	
	throwIfFalse(xmlBoardtile.attribute("type", shouldBeSetByAdjustXmlInfo) != shouldBeSetByAdjustXmlInfo );
	
	return xmlBoardtile; 
}

void SBoardTile::ChangeColor( SColor f_color,  DIRECTION_ABSOLUTE f_absDir ) {
	ChangeColor( f_color, GetRelDirection(f_absDir) );
}

void SBoardTile::ChangeColor( SColor f_color, DIRECTION_RELATIVE f_relDir ) {
	switch ( f_relDir ) {
		case TOP:
			m_colors[TOP] = f_color;
			break;
		case RIGHT:
			m_colors[RIGHT] = f_color;
			break;
		case BOTTOM:
			m_colors[BOTTOM] = f_color;
			break;
		case LEFT:
			m_colors[LEFT] = f_color;
			break;
		default:
			throwEx("Unvalid DIRECTION_RELATIVE-value: " + QString::number(f_relDir));
	}
	
	UpdateColorsInfo();
	
	if ( m_boardMode == MODE_EDT_SETTING) 
		SendColorInfosToEdtSheet();
}

bool SBoardTile::SetColor(SColor f_color, DIRECTION_ABSOLUTE f_absDir) {
	return SetColor(f_color, GetRelDirection(f_absDir));
}

bool SBoardTile::SetColor(SColor f_color, DIRECTION_RELATIVE f_relDir ) {
	if ( IsColorOk(f_color, f_relDir ) ) {
		ChangeColor(f_color, f_relDir);
		return true;
	}
	
	return false;
}

bool SBoardTile::IsColorOk(SColor f_color, DIRECTION_ABSOLUTE f_absDir ) {
	return IsColorOk(f_color, GetRelDirection(f_absDir));
}

void SBoardTile::SendColorInfosToEdtSheet() {
	throwIfFalse(m_boardMode == MODE_EDT_SETTING);
	
	SGraphicsEdtColors * edtColors = qensure_cast(SGraphicsEdtColors *, m_tiles[TILE_EDITOR_COLOR_SHEET]);

	SAllowedColors allowedColors;
	
	#define SEND_COLOR_INFOS(ABS_DIR)												\
		for ( SColor color = FIRST_COLOR; color <= LAST_COLOR; ++color ) {			\
			allowedColors.SetAllowedColor(color, IsColorOk(color, ABS_DIR));		\
		}																			\
		allowedColors.SetActiveColor(m_colors[GetRelDirection(ABS_DIR)]);			\
		edtColors->SetAllowedColors(ABS_DIR, allowedColors);
		

	SEND_COLOR_INFOS(NORTH);
	SEND_COLOR_INFOS(EAST);
	SEND_COLOR_INFOS(SOUTH);
	SEND_COLOR_INFOS(WEST);
	
	#undef SEND_COLORS
}

QWidget * SBoardTile::CreateCustumSettingsWidget() {
	throwIfFalse(m_boardMode == MODE_EDT_SETTING);
	
	return NULL;
};

QString SBoardTile::GetAdditionalInitErrors() {
	return QString();
}
