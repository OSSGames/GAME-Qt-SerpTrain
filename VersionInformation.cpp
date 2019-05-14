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
 * $LastChangedDate: 2009-03-27 18:34:24 +0100 (Fr, 27. Mär 2009) $
 * $LastChangedRevision: 253 $
*/

#include "VersionInformation.h"

const qint64 VersionInformation::BuildNumber = Q_INT64_C(1508);
const QString VersionInformation::VersionNumber = "0.6.0"; // Muss immer im Format xx.yy.zz sein. Wenn zz ungerade ist, dann ist es eine Entwicklungsversion; sonst nicht!
const QString VersionInformation::CompileTime = __TIME__ " on " __DATE__;
const int VersionInformation::CurFormatNumber = 4; // Gibt an, mit in welchem Format der Editor abspeichert, bzw. bis zu welchen Formaten Levels geöffnet werden können.
const int VersionInformation::CurUpdateFormatNumber = 0; // Gibt an, welche Update-Dateien, das Programm lesen kann;
const QString VersionInformation::Md5Sum = "9213989ff6b82839cf86dd3a6b12db01  -";
