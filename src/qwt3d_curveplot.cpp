#if defined(_MSC_VER) /* MSVC Compiler */
/* 'identifier' : truncation from 'type1' to 'type2' */
#pragma warning(disable : 4305)
#endif

#include <qwt3d_curveplot.h>

using namespace std;
using namespace Qwt3D;

CurvePlot::CurveData::CurveData()
{
    datatype_p = Qwt3D::GRAPH;
    setHull(ParallelEpiped());
}

CurvePlot::CurveData::~CurveData()
{
    setHull(ParallelEpiped());
}

bool CurvePlot::CurveData::empty() const
{
    return true;
}

CurvePlot::CurvePlot(QWidget *parent, const QGLWidget *shareWidget)
    : Plot3D(parent, shareWidget)
{
    theNurb = gluNewNurbsRenderer();
    plotlets_p[0].data = ValuePtr<Data>(new CurveData);
    plotlets_p[0].data->setHull(ParallelEpiped(Triple(-1, -1, -1), Triple(1, 1, 1)));
    createCoordinateSystem();

    setRotation(30, 0, 15);
    setScale(1, 1, 1);
    setShift(0.15, 0, 0);
    setZoom(0.9);

    setCoordinateStyle(BOX);
    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
}

void CurvePlot::setPointsOnCurve(const TripleVector &points)
{
    mLinePoints = points;
    Qwt3D::ParallelEpiped ep = Qwt3D::hull(mLinePoints);
    plotlets_p[0].data->setHull(ep);
    createCoordinateSystem();
}

/*!
 * \brief CurvePlot::setNurbsData
 *  user's duty to make sure input nurbs data correct
 * \param knots
 * \param ctrlpnts
 * \param order
 */
void CurvePlot::setNurbsData(int knotCount, float *knots, int stride, float *ctrlpnts, int order)
{
    mKnotCount = knotCount;
    mStride = stride;
    mOrder = order;
    mKnots = knots;
    mCtrlPtns = ctrlpnts;
}

void CurvePlot::createOpenGlData(const Plot3D::Plotlet &pl)
{
    if (pl.appearance->plotStyle() == NOPLOT)
        return;

    // draw nurbs
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    gluBeginCurve(theNurb);
    glColor3f(0.0, 0.0, 0.0);
    gluNurbsCurve(theNurb, mKnotCount, mKnots, mStride, mCtrlPtns, mOrder, GL_MAP1_VERTEX_3);
    gluEndCurve(theNurb);

    // draw control points
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = 0; i < mKnotCount - mOrder - 1; ++i) {
        int baseid = i*mStride;
        glVertex3f(mCtrlPtns[baseid], mCtrlPtns[baseid+1], mCtrlPtns[baseid+2]);
    }
    glEnd();

    // draw shot lines
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    int total = mLinePoints.size();
    for (int i = 0; i < total; ++i) {
        Triple tmp(mLinePoints[i]);
        glVertex3f(tmp.x, tmp.y, tmp.z);
    }
    glEnd();

    glPopMatrix();
}