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

    GLdouble ctrlpoints[9][3] = {{-47.343, 23.057, 42}, {-46.804, 23.052, 42},
                                {-45.655, 23.146, 42}, {-44.004, 23.840, 42},
                                {-42.756, 24.882, 42}, {-41.930, 25.988, 42},
                                {-41.357, 27.256, 42}, {-41.178, 28.209, 42},
                                {-41.149, 28.720, 42}}; //控制点

    GLdouble knots[13] = {0,        0,        0, 0, 0.174058, 0.386051, 0.551328,
                         0.693068, 0.834781, 1, 1, 1,        1};
    plot.setNurbsData(13, knots, 3, &ctrlpoints[0][0], 4);

    TripleVector ap;
    for (int i = 0; i < 9; ++i) {
        ap.push_back(Triple(ctrlpoints[i][0], ctrlpoints[i][1], ctrlpoints[i][2]));
    }

    plot.setPointsOnCurve(ap);
    plot.resize(800,600);
    plot.show();
    plot.updateData();
    return a.exec();
}

