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

void CurvePlot::nurbsError(GLenum errorCode)
{
    const GLubyte *estring = gluErrorString(errorCode);
    qWarning("Nurbs Error: %s \n", estring);
}

CurvePlot::CurvePlot(QWidget *parent)
    : Plot3D(parent)
    , mGLU_Nurb(gluNewNurbsRenderer())
    , mKnotCount(0)
    , mKnots(Q_NULLPTR)
    , mStride(0)
    , mCtrlPtns(Q_NULLPTR)
    , mOrder(0)
    , mLinePoints()
{
    gluNurbsCallback(mGLU_Nurb, GLU_ERROR, (GLvoid(*)())nurbsError);
    plotlets_p[0].data = ValuePtr<Data>(new CurveData);
//    plotlets_p[0].data->setHull(ParallelEpiped(Triple(-1, -1, -1), Triple(1, 1, 1)));
    createCoordinateSystem();

    setRotation(30, 0, 15);
    setScale(1, 1, 1);
    setShift(0.15, 0, 0);
    setZoom(0.9);

    setCoordinateStyle(BOX);
    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
}

CurvePlot::~CurvePlot()
{
    delete [] mKnots;
    delete [] mCtrlPtns;
}

void CurvePlot::setPointsOnCurve(const TripleVector &points)
{
    mLinePoints = points;

    Qwt3D::ParallelEpiped ep = Qwt3D::hull(mLinePoints);
    Qwt3D::ParallelEpiped ori = plotlets_p[0].data->hull();
    if (ori.minVertex != ori.maxVertex) {
        Qwt3D::ParallelEpiped hull = Qwt3D::sum(ep, ori);
        plotlets_p[0].data->setHull(hull);
    } else {
        plotlets_p[0].data->setHull(ep);
    }

    createCoordinateSystem();
}

/*!
 * \brief CurvePlot::setNurbsData
 *  user's duty to make sure input nurbs data correct
 * \param knots
 * \param ctrlpnts
 * \param order
 */
void CurvePlot::setNurbsData(int knotCount, double *knots, int stride, double *ctrlpnts, int order)
{
    mKnotCount = knotCount;
    delete [] mKnots;
    mKnots = new float [mKnotCount];
//    free(mKnots);
//    mKnots = (float *)malloc(sizeof(float)*mKnotCount);
    for (int i = 0; i < mKnotCount; ++i) {
        mKnots[i] = knots[i];
    }

    mStride = stride;
    mOrder = order;
    int total = mKnotCount - mOrder;
    delete [] mCtrlPtns;
    mCtrlPtns = new float [total*mStride];
//    free(mCtrlPtns);
//    mCtrlPtns = (float *)malloc(sizeof(float)*total*mStride);

    for (int i = 0; i < total*mStride; ++i) {
        mCtrlPtns[i] = static_cast<float>(ctrlpnts[i]);
    }

    Triple min;
    Triple max;
    for (int i = 0; i < total; ++i) {
        double x = mCtrlPtns[i*mStride + 0];
        double y = mCtrlPtns[i*mStride + 1];
        double z = mCtrlPtns[i*mStride + 2];
        if (i == 0) {
            min.x = x; min.y = y; min.z = z;
            max.x = x; max.y = y; max.z = z;
        }

        if (x < min.x)
            min.x = x;
        if (y < min.y)
            min.y = y;
        if (z < min.z)
            min.z = z;

        if (x > max.x)
            max.x = x;
        if (y > max.y)
            max.y = y;
        if (z > max.z)
            max.z = z;
    }
    Qwt3D::ParallelEpiped ep(min, max);
    Qwt3D::ParallelEpiped ori(plotlets_p[0].data->hull());
    if (ori.minVertex != ori.maxVertex) {
        Qwt3D::ParallelEpiped hull = Qwt3D::sum(ep, ori);
        plotlets_p[0].data->setHull(hull);
    } else {
        plotlets_p[0].data->setHull(ep);
    }

    createCoordinateSystem();
}

void CurvePlot::createOpenGlData(const Plot3D::Plotlet &pl)
{
    if (pl.appearance->plotStyle() == NOPLOT)
        return;

    // draw nurbs
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glPushMatrix();

    gluBeginCurve(mGLU_Nurb);
    glColor3f(0.0, 0.0, 0.0);
    gluNurbsCurve(mGLU_Nurb, mKnotCount, mKnots, mStride, mCtrlPtns, mOrder, GL_MAP1_VERTEX_3);
    gluEndCurve(mGLU_Nurb);

    // draw control points
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = 0; i < mKnotCount - mOrder; ++i) {
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