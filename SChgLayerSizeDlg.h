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

#ifndef SCHGLAYERSIZEDLG_H
#define SCHGLAYERSIZEDLG_H

#include <QtGui/QDialog>
#include "ui_SChgLayerSizeDlg.h"

class SChgLayerSizeDlg : public QDialog
{
    Q_OBJECT

public:
    SChgLayerSizeDlg(QWidget *f_parent, int f_rows, int f_cols);
    ~SChgLayerSizeDlg();
    
    bool AddCol();
    // Dürfen nur abgefragt werden, wenn AddCol()==true
    int AmountColToAdd();
    int AddColBefore(); // Achtung: 0 bedeutet vor der 1.; 6 bedeutet nach der 6.
    // Dürfen nur abgefragt werden, wenn AddCol()==false
    int DeleteColAt(); // Achtung: 0 bedeutet, dass die 1. gelöscht werden soll 
    
    bool AddRow();
    // Dürfen nur abgefragt werden, wenn AddRow()==true
    int AmountRowToAdd();
    int AddRowBefore(); // Achtung: 0 bedeutet vor der 1.; 6 bedeutet nach der 6.
    // Dürfen nur abgefragt werden, wenn AddRow()==false
    int DeleteRowAt(); // Achtung: 0 bedeutet, dass die 1. gelöscht werden soll 

private slots:
	void on_changeButton_clicked() { accept(); };
	void on_cancelButton_clicked() { reject(); };
    void on_radioAddCol_clicked() { SetAddColState(true); };
    void on_radioDelCol_clicked() { SetAddColState(false); };
    void on_radioAddRow_clicked() { SetAddRowState(true); };
    void on_radioDelRow_clicked() { SetAddRowState(false); };

private:
	void SetAddColState( bool f_state );
	void SetAddRowState( bool f_state );

    Ui::SChgLayerSizeDlgClass ui;
};

#endif // SCHGLAYERSIZEDLG_H
