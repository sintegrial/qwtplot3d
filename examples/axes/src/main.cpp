/********************************************************************
    created:   2003/09/10
    filename:  main.cpp

    author:    Micha Bieber
*********************************************************************/

#include <qapplication.h>
#include "axesmainwindow.h"

#include <QGLFormat>


int main( int argc, char** argv )
{
	QApplication app( argc, argv );

  if ( !QGLFormat::hasOpenGL() )
	{
		qWarning( "This system has no OpenGL support. Exiting." );
		return -1;
  }

	AxesMainWindow mainwindow;
	mainwindow.show();
    mainwindow.plot->updateData();

	return app.exec();
}
