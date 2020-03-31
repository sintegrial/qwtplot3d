#if defined(_MSC_VER) /* MSVC Compiler */
/* 'identifier' : truncation from 'type1' to 'type2' */
#pragma warning(disable : 4305)
#endif

#include <qwt3d_curveplot.h>

using namespace std;
using namespace Qwt3D;

CurvePlot::CurveData::CurveData() {
    datatype_p = Qwt3D::GRAPH;
    setHull(ParallelEpiped());
}

CurvePlot::CurveData::~CurveData() { setHull(ParallelEpiped()); }

bool CurvePlot::CurveData::empty() const { return true; }

CurvePlot::CurvePlot(QWidget *parent, const QGLWidget *shareWidget)
    : Plot3D(parent, shareWidget) {
    plotlets_p[0].data = ValuePtr<Data>(new CurveData);
    plotlets_p[0].data->setHull(
                ParallelEpiped(Triple(-50, 20, 40), Triple(-40, 30, 50)));

    setRotation(30, 0, 15);
    setScale(1, 1, 1);
    setShift(0.15, 0, 0);
    setZoom(0.9);

    setCoordinateStyle(BOX);
}

/*!
 * \brief CurvePlot::createDataset
 * \return
 */
int CurvePlot::createDataset() {
    updateData();
    createCoordinateSystem();

    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    return 0;
}

void CurvePlot::createOpenGlData(const Plot3D::Plotlet &pl) {
    if (pl.appearance->plotStyle() == NOPLOT)
        return;

    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    glColor3f(1.0, 0.0, 0.0);

    gluBeginCurve(theNurb);

    gluNurbsCurve(theNurb, 13, knots, 3, &ctrlpoints[0][0], 4, GL_MAP1_VERTEX_3);

    gluEndCurve(theNurb);

    glPopMatrix();
}