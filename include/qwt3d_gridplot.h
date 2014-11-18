#pragma once
#ifndef qwt3d_gridplot_h__2005_7_10_begin_guarded_code
#define qwt3d_gridplot_h__2005_7_10_begin_guarded_code

#include "qwt3d_surfaceplot.h"

namespace Qwt3D
{
//! A class representing  grid-generated surfaces
/**
  A GridPlot ...
  
*/
class QWT3D_EXPORT GridPlot : public SurfacePlot
{
    Q_OBJECT

public:
    GridPlot( QWidget * parent = 0, const QGLWidget * shareWidget = 0 );
    virtual ~GridPlot() {}
    int	resolution() const {return resolution_p;} //!< Returns data resolution (1 means all data)

    int createDataset(Qwt3D::Triple** data, unsigned int columns, unsigned int rows,
                      bool uperiodic = false, bool vperiodic = false, bool append = false);
    int createDataset(double** data, unsigned int columns, unsigned int rows,
                      double minx, double maxx, double miny, double maxy, bool append = false);

signals:
    void resolutionChanged(int);

public slots:
    void setResolution( int );

protected:  
    void createOpenGlData(const Plotlet& pl);
	void processVertex(const Triple& vert1, const Triple& norm1, const Plotlet& pl, bool hl, bool& stripStarted);

	void drawEnrichment(const Plotlet& pl, Enrichment& p);

    int resolution_p;

private:
    class GridData : public Data
    {
    public:
        GridData();
        GridData(unsigned int columns, unsigned int rows);//!< see setSize()

        GridData* clone() const {return new GridData(*this);}

        int columns() const;
        int rows() const;

        bool empty() const { return vertices.empty();}
        void setSize(unsigned int columns, unsigned int rows); //!< destroys content and set new size, elements are uninitialized

        DataMatrix vertices;		//!< mesh vertices
        DataMatrix normals;		//!< mesh normals
        void setPeriodic(bool u, bool v) {uperiodic_ = u; vperiodic_ = v;}
        bool uperiodic() const {return uperiodic_;}
        bool vperiodic() const {return vperiodic_;}

    private:
        bool uperiodic_, vperiodic_;
        void clear();
    };

    void createNormals(const Plotlet& pl);
    void data2Floor(const Plotlet& pl);
    void isolines2Floor(const Plotlet& pl);
    void setColorFromVertex(const Plotlet& pl, const Triple& vertex, bool skip = false);
    void calcNormals(GridData& gdata);
    void sewPeriodic(GridData& gdata);
    void readIn(GridData& gdata, Triple** data, unsigned int columns, unsigned int rows);
    void readIn(GridData& gdata, double** data, unsigned int columns, unsigned int rows,
                double minx, double maxx, double miny, double maxy);
};

} // ns


#endif /* include guard */
