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
 * $LastChangedDate: 2009-03-27 15:47:24 +0100 (Fr, 27. Mär 2009) $
 * $LastChangedRevision: 252 $
*/

#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>

#include "VersionInformation.h"
#include "SException.h"
#include "SGameBoard.h"
#include "SConfig.h"
#include "SOpenLevel.h"
#include "SConfigDlg.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("serpedon");
	QCoreApplication::setOrganizationDomain("serpedon.de");
	QCoreApplication::setApplicationName("SerpTrain");
	SConfig::Load();
     
    int result = 0;
	try {
		// ToDo: Das Ganze in eine Dialog-Klasse einbauen! 
 		QMessageBox msgBox;
 		QPushButton *playButton = msgBox.addButton("Play", QMessageBox::ActionRole);
 		QPushButton *tutButton = msgBox.addButton("Tutorial", QMessageBox::ActionRole);
 		QPushButton *editorButton = msgBox.addButton("Editor", QMessageBox::ActionRole);
 		QPushButton *configButton = msgBox.addButton("Configuration", QMessageBox::ActionRole);
 		QPushButton *closeButton = msgBox.addButton("Exit", QMessageBox::ActionRole);
 		msgBox.setEscapeButton(closeButton);
 		msgBox.setWindowTitle("SerpTrain " + VersionInformation::VersionNumber);
 		
		do {	//Config/{$VARNAME}_isDefault					bool		Speichert, ob die Variable noch den Standardwert besitzt
	//Config/{$VARNAME}							<generic>	Falls die Variable nicht mehr den Standardwert besitzt, steht hier der richtige drin!
	
			
	 		msgBox.exec();
	
			bool end = false;
	 		if (msgBox.clickedButton() == playButton) {
	 			do {
			 		SOpenLevel dialog;
			 		if ( dialog.exec() == QDialog::Accepted ) {
				 		// ToDo: Nur eine Quick & dirty methode zum Spiel wiederholen
				 		QString filename = dialog.GetLevel();
						QFile file(filename);
				    	SGameBoard::StartLevel(&file, MODE_PLAY);
			 		} else
			 			end = true;
	 			} while ( ! end );		 			  		
	 		} else if (msgBox.clickedButton() == tutButton) {
		 		// ToDo: Nur eine Quick & dirty methode zum Spiel wiederholen
		 		QMessageBox tutBox;
 				QPushButton *tut1Button = tutBox.addButton("Tutorial 1", QMessageBox::ActionRole);
 				QPushButton *tut2Button = tutBox.addButton("Tutorial 2", QMessageBox::ActionRole);
 				QPushButton *tut3Button = tutBox.addButton("Tutorial 3", QMessageBox::ActionRole);
 				QPushButton *tut4Button = tutBox.addButton("Tutorial 4", QMessageBox::ActionRole);
 				QPushButton *backButton = tutBox.addButton("Back", QMessageBox::ActionRole);
 				tutBox.setEscapeButton(backButton);
 				tutBox.setWindowTitle("SerpTrain " + VersionInformation::VersionNumber + " - Tutorials");
 				
 				do {
					QString filename;
					tutBox.exec();
	
	 				if (tutBox.clickedButton() == tut1Button)
	 					filename = ":/levels/tut1.xml";
	 				else if (tutBox.clickedButton() == tut2Button)
	 					filename = ":/levels/tut2.xml";
	 				else if (tutBox.clickedButton() == tut3Button)
	 					filename = ":/levels/tut3.xml";
	 				else if (tutBox.clickedButton() == tut4Button)
	 					filename = ":/levels/tut4.xml";
	 				else if (tutBox.clickedButton() != backButton)
	 					throwEx("Unknown Button pressed!");
	 				
	 				if ( ! filename.isEmpty() ) {
 						QFile file(filename);
			    		SGameBoard::StartLevel(&file, MODE_PLAY);
	 				}
	 				     		
 				} while ( tutBox.clickedButton() != backButton );
	 		} else if (msgBox.clickedButton() == editorButton) {
	 			QMessageBox editorBox;
 				QPushButton *loadLevelButton = editorBox.addButton("Load Level", QMessageBox::ActionRole);
 				QPushButton *newLevelButton = editorBox.addButton("Create New Level", QMessageBox::ActionRole);
 				QPushButton *backButton = editorBox.addButton("Back", QMessageBox::ActionRole);
 				editorBox.setEscapeButton(backButton);
 				editorBox.setWindowTitle("SerpTrain " + VersionInformation::VersionNumber + " - Editor");
 				
 				do {
					editorBox.exec();
					
					QString filename;	
	 				if (editorBox.clickedButton() == loadLevelButton) {
	 					QDir dir;
 						dir.mkpath(SConfig::OWN_LEVEL_DIR());
 				
						filename = QFileDialog::getOpenFileName(NULL, QObject::tr("Open Level"),
                                                 SConfig::OWN_LEVEL_DIR(),
                                                 QObject::tr("Levels (*.xml)"));
	 				} else if (editorBox.clickedButton() == newLevelButton) {
	 					filename = ":/levels/emptyLevel.xml";
	 				} else if (editorBox.clickedButton() != backButton) {
	 					throwEx("Unknown Button pressed!");
	 				}
	 					
	 				if ( ! filename.isEmpty() ) {
		 				// Starte den Editor
		        		QFile file(filename);
		    			SGameBoard::StartLevel(&file, MODE_EDITOR);
	 				}	 				
 				} while ( editorBox.clickedButton() != backButton );
	 		} else if ( msgBox.clickedButton() == configButton ) {
	 			SConfigDlg configDlg;
	 			configDlg.exec();
	 		} else if ( msgBox.clickedButton() != closeButton)
	 			throwEx("Unknown Button pressed!");
		}
		while ( msgBox.clickedButton() != closeButton ); 
	}
	catch ( std::exception & e)
	{
		SException::Report(e);
	}
	
	SConfig::Save();
	return result;
}
