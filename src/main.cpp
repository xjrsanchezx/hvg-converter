/**
 * Copyright (c) 2013 xjrsanchezx@yahoo.es
 *
 * Use of this source code is governed by The MIT License (MIT) license 
 * that can be found in the LICENSE file.
 */

/** \file 
 * \author xjrsanchezx@yahoo.es (Jairo R. Sánchez)
 * \copyright MIT License (MIT)
 */


#include <QtWidgets/QApplication>
#include <QFileDialog>
#include "mainDlg.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainDlg dlg;

	// open the mdb file
	QString fileName = QFileDialog::getOpenFileName(&dlg, "Open Database File", "", "Access Databases (*.mdb)");

	if(fileName == NULL)
		return -1;

	if( dlg.setDatabaseFile(fileName) )
		dlg.show();
	else
		return -1;

	return a.exec();
}
