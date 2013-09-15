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
#include <QMessageBox>

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
	_dbPath = QFileInfo(dbName).absoluteDir();

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

	// connect the button
	connect(ui.convertButton, SIGNAL(clicked()), this, SLOT(convert()));

	return true;
}

void MainDlg::convert()
{
	QString machineName = ui.machineNameEdit->text();
	QString machineDescription = ui.descriptionEdit->toPlainText();

	// The machine name is required
	if(machineName == "")
	{
		std::cerr << "The machine name is required" << std::endl;

		QMessageBox::critical(0, "Error", "The machine name is required", QMessageBox::Ok);		
		return;
	}

	QSqlDatabase db;

	// Find QSLite driver and create the file
    db = QSqlDatabase::addDatabase("QSQLITE");
	QString dbFileName = _dbPath.filePath("machine.sqlite");
	db.setDatabaseName(dbFileName);

	// Open the database
	if (!db.open()) 
	{
		QMessageBox::critical(0, "Error", " Error creating SQLITE database", QMessageBox::Ok);
		return;
	}

	std::cout << "convert" << std::endl;
}