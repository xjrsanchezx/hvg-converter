/**
 * Copyright (c) 2013 xjrsanchezx@yahoo.es
 *
 * Use of this source code is governed by The MIT License (MIT) license 
 * that can be found in the LICENSE file.
 */

/** \file
 * \brief The main window
 *
 * \author xjrsanchezx@yahoo.es (Jairo R. Sánchez)
 * \copyright MIT License (MIT)
 */

#ifndef MAINDLG_H
#define MAINDLG_H

#include <QtWidgets/QMainWindow>
#include <qdir.h>
#include "ui_converter_main.h"

class MainDlg : public QMainWindow
{
	Q_OBJECT

public:
	MainDlg(QWidget *parent = 0);
	~MainDlg();

	bool setDatabaseFile(const QString& dbName);

private slots:
	void convert();						///< starts the conversion
	void romFolderExplore();			///< Opens a dialog to select the rom folder
	void infoExplore();					///< Opens a dialog to select the html with the description	
	void emulatorExplore();				///< Opens a dialog to select the emulator exe

private:
	bool getOriginalDatabaseColumn(const QString& column, QStringList& records);	///< Gets all the records from a given column

private:
	Ui::MainDlgClass ui;

	QDir _dbPath;						///< Path to the database directory
	QString _accessDBName;				///< Name of the original database
	QStringList _dbColumns;				///< Columns in the original database
};

#endif // MAINDLG_H
