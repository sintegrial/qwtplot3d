//-----------------------------------------------------------------
//              curveplot.cpp
//
//      A simple example which shows how to use CurvePlot
//-----------------------------------------------------------------

#include <math.h>
#include <qapplication.h>
#include <qwt3d_curveplot.h>

using namespace Qwt3D;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    CurvePlot plot;
    plot.createDataset();
    plot.resize(800,600);
    plot.show();
    return a.exec();
}

