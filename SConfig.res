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

//Über NEW_CONFIG(TYPE, NAME, STD_VALUE, DESC) sollen hier die Konfigurationswerte erstellt werden
//Nicht von außen zu ändernde Werte können über NEW_CONST(TYPE, NAME, STD_VALUE, DESC) erstellt werden

//In dieser Reihenfolge werden die Werte auch in der Konfiguration angezeigt
//Neue (Sinn)-Abschnitte können mit NEW_SECTION(DESC) begonnen werden

//Dabei wird DESC grundsätzlich als Zeichenkette angegeben!

NEW_SECTION( "&Save options" )
NEW_CONFIG( bool, SAVE_BEFORE_TEST, false, "automatic save before testing Level" )
NEW_CONST ( QString, SERPTRAIN_HOME, QDir::homePath() + "/.SerpTrain", "Homedirectory of SerpTrain" )
NEW_CONFIG( QString, LEVEL_DIR, SERPTRAIN_HOME() + "/Levels", "Directory of Levels" )
NEW_CONFIG( QString, OWN_LEVEL_DIR, SERPTRAIN_HOME() + "/OwnLevels", "Directory of own Levels" )
NEW_CONFIG( QString, LAST_EDITED_LEVEL, SERPTRAIN_HOME() + "/OwnLevels/LastEditedLevel.xml", "Last Edited Level" )

NEW_SECTION( "&Update" )
NEW_CONFIG( QString, UPDATE_LEVEL_URL, "http://serpedon.de/SerpTrain/LevelList.xml", "Update URL" )

NEW_SECTION( "&Ball Colors" )
NEW_CONFIG( QColor, BALL_COLOR_RED, Qt::red, "red" )
NEW_CONFIG( QColor, BALL_COLOR_ORANGE, QColor(255,140,0), "orange" )
NEW_CONFIG( QColor, BALL_COLOR_GREEN, Qt::green, "green" )
NEW_CONFIG( QColor, BALL_COLOR_WHITE, Qt::white, "white" )
NEW_CONFIG( QColor, BALL_COLOR_YELLOW, Qt::yellow, "yellow" )
NEW_CONFIG( QColor, BALL_COLOR_BLUE, Qt::blue, "blue" )

NEW_SECTION( "&Colors" )
NEW_CONFIG( QColor, STD_RAIL_COLOR, Qt::black, "active rail")
NEW_CONFIG( QColor, PASSIVE_RAIL_COLOR, Qt::darkGray, "deactive rail")
NEW_CONFIG( QColor, HILL_COLOR, QColor(40,25,0), "tunnel")
NEW_CONFIG( QColor, TUNNEL_TEXT_COLOR, Qt::white, "text on Tunnel" )
NEW_CONFIG( QColor, EDIT_ROTATE_COLOR, QColor(140,140,160), "rotate arrow" )
NEW_CONFIG( QColor, EDIT_SHINY_WHITE, QColor(255,255,255,50), "shiny circle in editor" )
NEW_CONFIG( QColor, LAYER_ACTIVE_2, QColor(0, 77, 0), "layer (minor)")
NEW_CONFIG( QColor, LAYER_ACTIVE_1, QColor(0, 192, 0), "layer (main)")
NEW_CONFIG( QColor, LAYER_INACTIVE_1, QColor(128, 64, 0), "tunnel layer (main)")
NEW_CONFIG( QColor, LAYER_INACTIVE_2, QColor(52, 26, 0), "tunnel layer (minor)")
NEW_CONFIG( QColor, SWITCH_CONTROL_COLOR, HILL_COLOR(), "switch control device" )

NEW_SECTION( "&Debug" )
NEW_CONFIG( bool, LOG_EXCEPTIONS, true, "Log Exceptions")
NEW_CONFIG( QString, LOG_EXCEPTIONS_FILE, SERPTRAIN_HOME() + "/exceptions.csv", "Logfile for Exceptions" )

NEW_SECTION( "&Internals" )
NEW_CONFIG( int, FIELD_SIZE, 50, "size of one field" )
NEW_CONST ( int, RAIL_WIDTH, FIELD_SIZE() / 5, "ball diameter" )
NEW_CONFIG( int, TUNNEL_MARKER_WIDTH, FIELD_SIZE() / 10, "width of tunnel marker" )
NEW_CONFIG( int, LOGS_OVERHEAD, FIELD_SIZE() / 15, "overhead of logs" )
NEW_CONFIG( int, LOGS_ON_STRAIGHT, 7, "logs on straight" )
NEW_CONFIG( int, LOGS_ON_TURN, LOGS_ON_STRAIGHT(), "logs on turn" )
NEW_CONFIG( int, RAIL_WIDTH_GRAPHICAL, FIELD_SIZE() / 25, "Rail width" )
NEW_CONFIG( int, SWITCH_CONTROL_WIDTH_GRAPHICAL, RAIL_WIDTH_GRAPHICAL(), "switch control device margin" )
NEW_CONFIG( int, TIMESTEPS_PER_SECONDS, 30, "timesteps per second" )
NEW_CONST ( int, FIELD_SIZE_PATH, 1000, "Path size per field" )
NEW_CONST ( int, RAIL_WIDTH_PATH, FIELD_SIZE_PATH() * RAIL_WIDTH() / FIELD_SIZE(), "Ball diameter in Path" )
NEW_CONST ( int, DEFAULT_PATH_SPEED, 50, "default path speed" )
NEW_CONST ( int, CON_POINT_1, ( FIELD_SIZE()-RAIL_WIDTH() ) / 2, "connection point 1" )        // Gibt an, wo die Schienen sich treffen müssen 
NEW_CONST ( int, CON_POINT_2, CON_POINT_1() + RAIL_WIDTH(), "connection point 2" )
NEW_CONST ( int, CON_POINT_1_LOGS, CON_POINT_1()-LOGS_OVERHEAD(), "connection point 1 of logs" )
NEW_CONST ( int, CON_POINT_2_LOGS, CON_POINT_2()+LOGS_OVERHEAD(), "connection point 2 of logs" )
NEW_CONFIG( QString, TUNNEL_TEXT_OVERFLOW, "+", "character shown on a tunnel if there are too many balls waiting")
NEW_CONFIG( int, TUNNEL_NUMBER_OVERFLOW, 5, "maximal number of balls shown on a tunnel")
NEW_CONST ( int, CONFIG_INT_MINIMUM, 0, "minimum for an integer in the configuration" )
NEW_CONST ( int, CONFIG_INT_MAXIMUM, 99999, "maximum for an integer in the configuration" )

#undef NEW_CONFIG
#undef NEW_CONST
#undef NEW_SECTION
