#pragma once

#include "qwt3d_plot3d.h"

namespace Qwt3D {

class CurvePlot : public Plot3D {
public:
    explicit CurvePlot(QWidget *parent = nullptr);
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

    GLUnurbsObj *mGLU_Nurb;
    int mKnotCount;
    float *mKnots;
    int mStride;
    float *mCtrlPtns;
    int mOrder;
    TripleVector mLinePoints;
    static void nurbsError(GLenum errorCode);
};

} // namespace Qwt3D
