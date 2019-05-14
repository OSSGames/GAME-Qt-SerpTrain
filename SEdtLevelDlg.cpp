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

#include "SEdtLevelDlg.h"
#include "SException.h"
#include <QMessageBox>
#include <QUuid>

SEdtLevelDlg::SEdtLevelDlg(QWidget *f_parent, QString f_name, QString f_desc,
	QString f_author, QString f_contact, int f_amountLayers, QString f_uuid, int f_version, int f_difficulty,
	bool f_readOnly)
    : QDialog(f_parent), m_readOnly(f_readOnly)
{
	ui.setupUi(this);
	
	ui.name->setText(f_name);
	ui.author->setText(f_author);
	ui.contact->setText(f_contact);
	ui.layers->setText(QString::number(f_amountLayers));
	ui.uuid->setText(f_uuid);
	ui.version->setText(QString::number(f_version));
	ui.difficulty->setCurrentIndex(f_difficulty);

	if ( f_readOnly )
		ui.desc->setHtml(f_desc);
	else
		// ToDo: Hier wäre HTML schöner, aber das erzeugt einen nervigen Overhead; deshalb ist hier die Unterscheidung nötig
		ui.desc->setPlainText(f_desc);
		
	// Dialog für Read-only anpassen
	if ( f_readOnly ) {
		ui.name->setReadOnly(true);
		ui.desc->setReadOnly(true);
		ui.author->setReadOnly(true);
		ui.contact->setReadOnly(true);
		
		for ( int i = ui.difficulty->count() - 1; i >= 0 ; i-- ) {
			if ( i != f_difficulty )
				ui.difficulty->removeItem(i);
		}
		
		ui.genUuidButton->setVisible(false);
		ui.saveButton->setVisible(false);
		ui.cancelButton->setText("&Close");
	}
}

SEdtLevelDlg::~SEdtLevelDlg()
{
}

void SEdtLevelDlg::on_saveButton_clicked(){
	accept();
}

void SEdtLevelDlg::on_cancelButton_clicked(){
	reject();
}

void SEdtLevelDlg::on_genUuidButton_clicked() {
	if ( QMessageBox::Yes == QMessageBox::warning(this, "Really generate new UUID?", "Do you really want to generate a new UUID?\n\nThe UUID is used to identify a certain level. For example if it needs to be updated. You only need to generate a new UUID if you used another level as template; so that your own level will have a different UUID.",
		QMessageBox::No | QMessageBox::Yes, QMessageBox::No ) ) {
		ui.uuid->setText(QUuid::createUuid().toString().remove('{').remove('}'));
	}
}

QString SEdtLevelDlg::GetName() {
	throwIfFalse(m_readOnly == false);
	return ui.name->text();
}

QString SEdtLevelDlg::GetDesc() {
	throwIfFalse(m_readOnly == false);
	return ui.desc->toPlainText();  // ToDo: Hier wäre HTML schöner, aber das erzeugt einen nervigen Overhead
}

QString SEdtLevelDlg::GetAuthor() {
	throwIfFalse(m_readOnly == false);
	return ui.author->text();
}

QString SEdtLevelDlg::GetContact() {
	throwIfFalse(m_readOnly == false);
	return ui.contact->text();
}

QString SEdtLevelDlg::GetUuid() {
	throwIfFalse(m_readOnly == false);
	return ui.uuid->text();
}

int SEdtLevelDlg::GetDifficulty() {
	throwIfFalse(m_readOnly == false);
	return ui.difficulty->currentIndex();
}
