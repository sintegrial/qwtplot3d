#pragma once

#include "qwt3d_plot3d.h"

namespace Qwt3D {

class CurvePlot : public Plot3D {
public:
    CurvePlot(QWidget *parent = nullptr, const QGLWidget *shareWidget = nullptr);

    int createDataset();

protected:
    void createOpenGlData(const Plotlet &pl);

private:
    class CurveData : public Data {
    public:
        CurveData();
        virtual ~CurveData();
        CurveData *clone() const { return new CurveData(*this); }
        bool empty() const;
    };

    GLUnurbsObj *theNurb = gluNewNurbsRenderer();
    GLfloat ctrlpoints[9][3] = {{-47.343, 23.057, 41}, {-46.804, 23.052, 41},
                                {-45.655, 23.146, 41}, {-44.004, 23.840, 42},
                                {-42.756, 24.882, 42}, {-41.930, 25.988, 42},
                                {-41.357, 27.256, 45}, {-41.178, 28.209, 45},
                                {-41.149, 28.720, 45}}; //控制点

    GLfloat knots[13] = {0,        0,        0, 0, 0.174058, 0.386051, 0.551328,
                         0.693068, 0.834781, 1, 1, 1,        1};
};

} // namespace Qwt3D
