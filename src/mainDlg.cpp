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


#include "mainDlg.h"

#include <iostream>
#include <QtSql>

MainDlg::MainDlg(QWidget *parent /*= 0*/)
	: QMainWindow(parent)
{
	ui.setupUi(this);	

}

MainDlg::~MainDlg()
{
	
}

bool MainDlg::setDatabaseFile(const QString& dbName)
{
	_accessDBName = dbName;

	QSqlDatabase db;

	db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dbName);
	
	if (!db.open()) 
	{
		std::cerr << "GameListModel: Game database not found!" << std::endl;
		return false;
	}

	QString tableName = (db.tables())[0];			// Get the first table
	QSqlRecord record=db.record(tableName);			// Get its columns

	for(int i=0; i<record.count(); i++)
	{
		_dbColumns.push_back( record.fieldName(i) );	
	}
		
	// populate combo boxes	
	ui.nameComboBox->addItems( _dbColumns );
	ui.romComboBox->addItems( _dbColumns );


	return true;
}