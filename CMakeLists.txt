cmake_minimum_required ( VERSION 2.8.11 )

PROJECT( hvg_converter )

# Instruct CMake to run moc automatically when needed.
SET(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
 
FIND_PACKAGE(Qt5Widgets)


SET( HVG_CONVERTER_SRC
	src/main.cpp	
	src/mainDlg.cpp	
)

SET( HVG_CONVERTER_H	
	include/mainDlg.h	
)

SET( HVG_CONVERTER_FORMS
	ui/converter_main.ui
)

QT5_WRAP_UI(UIS_HDRS ${HVG_CONVERTER_FORMS} )

INCLUDE_DIRECTORIES( include )
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )

# used to keep from opening a console
SET(GUI_TYPE)

IF(WIN32)
	SET(GUI_TYPE WIN32)
ENDIF(WIN32)


ADD_EXECUTABLE (hvg_converter ${GUI_TYPE} ${HVG_CONVERTER_SRC} ${HVG_CONVERTER_H} ${UIS_HDRS}) 

qt5_use_modules(hvg_converter  Widgets Sql)
