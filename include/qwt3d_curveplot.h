#pragma once

#include "qwt3d_plot3d.h"

namespace Qwt3D {

class CurvePlot : public Plot3D {
public:
    explicit CurvePlot(QWidget *parent = nullptr, const QGLWidget *shareWidget = nullptr);
    ~CurvePlot();
    void setPointsOnCurve(const TripleVector &points);
    void setNurbsData(int knotCount, double *knots, int stride, double *ctrlpnts, int order);

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

    GLUnurbsObj *theNurb;
    int mKnotCount;
    float *mKnots;
    int mStride;
    float *mCtrlPtns;
    int mOrder;
    TripleVector mLinePoints;
};

} // namespace Qwt3D
