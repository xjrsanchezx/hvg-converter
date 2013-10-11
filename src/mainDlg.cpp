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
#include <QProgressDialog>

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

	QSqlDatabase *db;

	db = new QSqlDatabase( QSqlDatabase::addDatabase("QODBC") );
	db->setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+dbName);
	
	if (!db->open()) 
	{
		std::cerr << "GameListModel: Game database not found!" << std::endl;
		return false;
	}

	QString tableName = (db->tables())[0];			// Get the first table
	QSqlRecord record=db->record(tableName);			// Get its columns

	QString connection = db->connectionName();
	db->close();
	delete db;

	QSqlDatabase::removeDatabase(connection);

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

	// read the rom name and files
	QString romName = ui.nameComboBox->currentText();		// name of the column that has the names of the rom
	QStringList romNameList;

	QString romFiles = ui.romComboBox->currentText();		// name of the column that has the roms
	QStringList romFileList;

	bool columnsRead = getOriginalDatabaseColumn(romName, romNameList);
	columnsRead &= getOriginalDatabaseColumn(romFiles, romFileList);

	if( !columnsRead || romNameList.size() != romFileList.size() )
	{
		QMessageBox::critical(0, "Error", "The input database is inconsistent.", QMessageBox::Ok);
		return;
	}

	// create the SQLite database
	QSqlDatabase *db;
	// Find QSLite driver and create the file
    db = new QSqlDatabase( QSqlDatabase::addDatabase("QSQLITE") );
	QString dbFileName = _dbPath.filePath("machine.sqlite");

	// delete previous databases
	std::cout << "writing database to file " << dbFileName.toStdString() << std::endl;

	if( QFile::exists(dbFileName) )
	{
		if( !QFile::remove(dbFileName) )
		{
			QMessageBox::critical(0, "Error", "The SQLite database is locked", QMessageBox::Ok);		
			return;
		}
	}

	db->setDatabaseName(dbFileName);

	// Open the database
	if (!db->open()) 
	{
		QMessageBox::critical(0, "Error", " Error creating SQLITE database", QMessageBox::Ok);
		return;
	}

	// create the table with the machine info	
	QSqlQuery query;
	query.exec("CREATE TABLE if not exists machine "
                   "(name varchar(64), "
                   "description varchar(1024))");

	QString queryString = "INSERT INTO machine values (:machineName, :machineDescription)";
	query.prepare(queryString);
	query.bindValue(":machineName", machineName);
	query.bindValue(":machineDescription", machineDescription);
	query.exec();
	query.clear();
	
	// create the table with the roms	
	query.exec("CREATE TABLE if not exists games "
                   "(id integer primary key autoincrement,"
				   "name varchar(128), "
                   "rom varchar(128))");
	query.clear();

	
	// open the progress dialog
	QProgressDialog progress("Converting the database...", QString(), 0, romNameList.size(), this);
    progress.setWindowModality(Qt::WindowModal);


	// insert the roms
	QSqlQuery batchQuery;
	queryString = "INSERT INTO games values (NULL, :romName, :romFile)";
	batchQuery.prepare(queryString);

	for(int i=0; i<romNameList.size(); i++)
	{
		batchQuery.bindValue(":romName", romNameList[i]);
		batchQuery.bindValue(":romFile", romFileList[i]);
		batchQuery.exec();
		progress.setValue(i+1);
	}


//	batchQuery.addBindValue(romNameList);
//	batchQuery.addBindValue(romFileList);
//	batchQuery.execBatch();
	batchQuery.clear();

	// close the database	
	QString connection = db->connectionName();
	db->close();
	delete db;
	
	QSqlDatabase::removeDatabase(connection);

	std::cout << "convert" << std::endl;
}

/**
\param[in] column The name of the column to retrive
\param[out] records All the entris of the column
\returns true if succeeded
*/
bool MainDlg::getOriginalDatabaseColumn(const QString& column, QStringList& records)
{
	QSqlDatabase *db;

	db = new QSqlDatabase( QSqlDatabase::addDatabase("QODBC") );
	db->setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+_accessDBName);
	
	if (!db->open()) 
	{
		std::cerr << "GameListModel: Game database not found!" << std::endl;
		return false;
	}

	QString tableName = (db->tables())[0];			// Get the first table

	QSqlQuery query;
	QString queryString = "SELECT " + column + " FROM "+ tableName;
	query.exec(queryString);	
	
	while(query.next() )
	{
		records.push_back( query.value(0).toString() );
	}

	query.clear();

	QString connection = db->connectionName();
	db->close();
	delete db;
	
	QSqlDatabase::removeDatabase(connection);

	return true;
}