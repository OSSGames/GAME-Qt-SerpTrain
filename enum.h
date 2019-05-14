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

#ifndef ENUM_H_
#define ENUM_H_

namespace enumNamespace {
	enum BOARD_MODE { MODE_PLAY /*Der Level wird zum Spielen geöffnet*/, MODE_TEST /*Der Level wird zum Testen geöffnet*/, MODE_EDITOR /*Der Level wird im Editor geöffnet*/, MODE_EDT_SETTING /*Das Tile ist im Settingsdialog*/, MODE_EDT_CHANGE /*Das Tile ist im Changedialog*/, MODE_SAME /*Pseudomode zum Kopieren*/};
	
	enum BALL_ROUTE { ROUTE_NO, ROUTE_1, ROUTE_2, ROUTE_3, ROUTE_4, ROUTE_5, ROUTE_6, ROUTE_7, ROUTE_8, ROUTE_9 };  
	
	// DIR_(ABS_)SPECIAL ist eine Spezial-Richtung
	enum DIRECTION_ABSOLUTE { NORTH = 0, WEST, SOUTH, EAST, DIR_ABS_SPECIAL = 1000 };
	enum DIRECTION_RELATIVE { TOP = 0, LEFT, BOTTOM, RIGHT, DIR_SPECIAL = 1000};  
	
	enum TILES { TILE_STRAIGHT = 0, TILE_TURN, TILE_2TURN, TILE_TUNNEL, TILE_TUNNEL_MARKER, TILE_SWITCH_CONTROL, TILE_GOAL0, TILE_GOAL1, TILE_GOAL2, TILE_GOAL3, TILE_GOAL_STRAIGHT, TILE_GOAL_CONTINUE, TILE_EDITOR_SHEET, TILE_EDITOR_COLOR_SHEET};
	
	enum Z_MINOR {	Z_MINOR_BOTTOM = -4, Z_MINOR_LOWEST, Z_MINOR_LOWER, Z_MINOR_LOW,
					Z_MINOR_DEFAULT = 0,
					Z_MINOR_HIGH = 1, Z_MINOR_HIGHER, Z_MINOR_HIGHEST, Z_MINOR_TOP};
	enum Z_MAJOR {	Z_MAJOR_BACKGROUND = -20, Z_MAJOR_RAIL = -10, Z_MAJOR_BALL = 0,
					Z_MAJOR_HILLS = 10, Z_MAJOR_FOREGROUND = 20 , Z_MAJOR_OVERLAY = 30 };
	enum ROTATION { ROTATION_NO = 0, ROTATION_90, ROTATION_180, ROTATION_270 };
	enum ROTATION_DIR { ROTATION_RIGHT, ROTATION_LEFT };
	enum MIRROR { MIRROR_NO = 1, MIRROR_YES = -1 };
	enum BOARDTILES { STRAIGHT, TURN, SWITCH, TUNNEL, NONE, GOAL, DOUBLE_TURN, SWITCH_TURN };
};

#ifndef NO_ENUM_POLLUTION
	using namespace enumNamespace;
#endif

#endif /*ENUM_H_*/
