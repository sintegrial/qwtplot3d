//-----------------------------------------------------------------
//              simpleplot.cpp
//
//      A simple example which shows how to use GridPlot
//-----------------------------------------------------------------

#include <math.h>
#include <qapplication.h>
#include <qwt3d_gridplot.h>
#include <qwt3d_function.h>


using namespace Qwt3D;

class CurvePlot : public Plot3D
{
public:
    CurvePlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );

    int createDataset();
protected:
    void createOpenGlData(const Plotlet& pl);

private:
    class CurveData : public Data
    {
    public:
        CurveData();
        virtual ~CurveData();
        CurveData* clone() const {return new CurveData(*this);}
        bool empty() const;
    };
};

CurvePlot::CurveData::CurveData()
{
  datatype_p=Qwt3D::GRAPH;
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

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    CurvePlot plot;
//    Plot plot;
    plot.createDataset();
    plot.resize(800,600);
    plot.show();
    return a.exec();
}

CurvePlot::CurvePlot(QWidget *parent, const QGLWidget *shareWidget)
    : Plot3D( parent, shareWidget)
{
    plotlets_p[0].data = ValuePtr<Data>(new CurveData);
    plotlets_p[0].data->setHull(ParallelEpiped(Triple(-50, 20, 40), Triple(-40, 30, 50)));

    setRotation(30,0,15);
    setScale(1,1,1);
    setShift(0.15,0,0);
    setZoom(0.9);

    setCoordinateStyle(BOX);
}


int CurvePlot::createDataset()
{
    updateData();
    createCoordinateSystem();

    coordinates()->axes[X1].setLabelString("x-axis");
    coordinates()->axes[Y1].setLabelString("y-axis");
    return 0;
}

void CurvePlot::createOpenGlData(const Plot3D::Plotlet &pl)
{
  if (pl.appearance->plotStyle() == NOPLOT)
    return;

  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_LINE_SMOOTH);

  // zjk
  glPushMatrix();

  glColor3f(1.0, 0.0, 0.0);
  GLUnurbsObj *theNurb = gluNewNurbsRenderer();
  GLfloat ctrlpoints[9][3] = {{-47.343,23.057,41},{-46.804, 23.052,41},{-45.655,23.146,41},
                              {-44.004,23.840,42}, {-42.756,24.882,42},{-41.930,25.988,42},
                              {-41.357,27.256,45},{-41.178, 28.209,45},{-41.149,28.720,45} };//控制点

  GLfloat knots[13] = {0,0,0,0,   0.174058,   0.386051, 0.551328 ,  0.693068,   0.834781,   1,1,1,1};


  gluBeginCurve(theNurb);

  gluNurbsCurve(theNurb,13,knots,3,&ctrlpoints[0][0],4,GL_MAP1_VERTEX_3);

  gluEndCurve(theNurb);

  glPopMatrix();
  // zjk end

}
