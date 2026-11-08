/********************************************************************
    created:   2003/09/09
    filename:  main.cpp

    author:    Micha Bieber
*********************************************************************/

#include <qapplication.h>
#include "meshmainwindow.h"

#include <QtGlobal>
#include <utilities/hasOpenGL.h>

int main( int argc, char **argv )
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setColorSpec( QApplication::CustomColor );
#endif

  QApplication app(argc,argv);

  if ( !Qwt3D::hasOpenGL() )
	{
		qWarning( "This system has no OpenGL support. Exiting." );
		return -1;
  }

  MeshMainWindow mainwindow;
  mainwindow.resize(1024,768);
	mainwindow.show();
    mainwindow.dataWidget->updateData();

	return app.exec();
}
