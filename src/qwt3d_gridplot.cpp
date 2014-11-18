#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning ( disable : 4305 )
#pragma warning ( disable : 4786 )
#endif

//#include <QtCore/QElapsedTimer>
//#include <QDebug>

#include "qwt3d_gridplot.h"
#include "qwt3d_enrichment_std.h"

using namespace std;
using namespace Qwt3D;

GridPlot::GridData::GridData()
{
    datatype_p = Qwt3D::GRID;
    setSize(0,0);
    setPeriodic(false,false);
}

GridPlot::GridData::GridData(unsigned int columns, unsigned int rows)
{
    datatype_p = Qwt3D::GRID;
    setSize(columns,rows);
    setPeriodic(false,false);
}

int GridPlot::GridData::columns() const 
{ 
    return (int)vertices.size();
}

int GridPlot::GridData::rows() const 
{ 
    return (empty()) ? 0 : (int)vertices[0].size();
}

void GridPlot::GridData::setSize(unsigned int columns, unsigned int rows)
{
    setHull(ParallelEpiped());
    vertices = std::vector<DataColumn>(columns);
    for (unsigned int i=0; i!=vertices.size(); ++i)
    {
        vertices[i] = DataColumn(rows);
    }
    normals = std::vector<DataColumn>(columns);
    for (unsigned int i=0; i!=normals.size(); ++i)
    {
        normals[i] = DataColumn(rows);
    }
}

/**
Initializes with dataNormals()==false, NOFLOOR, resolution() == 1
*/
GridPlot::GridPlot( QWidget * parent, const QGLWidget * shareWidget)
    : SurfacePlot( parent, shareWidget)
{
    resolution_p = 1;
    plotlets_p[0].data = ValuePtr<Data>(new GridData);
}

void GridPlot::setColorFromVertex(const Plotlet& pl, const Triple& vertex, bool skip)
{
    if (skip)
        return;

    RGBA col = pl.appearance->dataColor()->rgba(vertex);
    
    glColor4d(col.r, col.g, col.b, col.a);
}

void GridPlot::createNormals(const Plotlet& pl)
{
    if (!normals())
        return;

    Arrow arrow;
    arrow.setQuality(normalQuality());

    Triple /*basev, topv, */ norm;

    int step = resolution();

    const GridData& data = dynamic_cast<const GridData&>(*pl.data);
    const Color& color = *pl.appearance->dataColor();
    double diag = (data.hull().maxVertex-data.hull().minVertex).length() * normalLength();

    arrow.assign(*this);
    arrow.drawBegin();
    for (int i = 0; i <= data.columns() - step; i += step)
    {
        for (int j = 0; j <= data.rows() - step; j += step)
        {
            norm = data.normals[i][j];//topv-basev;
            norm.normalize();
            norm	*= diag;

            const Triple &vert = data.vertices[i][j];
            arrow.setTop(vert+norm);
            arrow.setColor(color.rgba(vert.x, vert.y, vert.z));
            arrow.draw(vert);
        }
    }
    arrow.drawEnd();
}

void GridPlot::readIn(GridData& gdata, Triple** data, unsigned int columns, unsigned int rows)
{
    gdata.setSize(columns,rows);

    ParallelEpiped range(Triple(DBL_MAX,DBL_MAX,DBL_MAX),Triple(-DBL_MAX,-DBL_MAX,-DBL_MAX));

    /* fill out the vertex array for the mesh. */
    for (unsigned i = 0; i != columns; ++i)
    {
        for (unsigned j = 0; j != rows; ++j)
        {
            Triple& val = data[i][j];

            gdata.vertices[i][j] = val;

            if (val.x > range.maxVertex.x)
                range.maxVertex.x = val.x;
            if (val.y > range.maxVertex.y)
                range.maxVertex.y = val.y;
            if (val.z > range.maxVertex.z)
                range.maxVertex.z = val.z;
            if (val.x < range.minVertex.x)
                range.minVertex.x = val.x;
            if (val.y < range.minVertex.y)
                range.minVertex.y = val.y;
            if (val.z < range.minVertex.z)
                range.minVertex.z = val.z;
        }
    }
    gdata.setHull(range);
}


void GridPlot::readIn(GridData& gdata, double** data, unsigned int columns, unsigned int rows
                      , double minx, double maxx, double miny, double maxy)
{
    gdata.setPeriodic(false,false);
    gdata.setSize(columns,rows);

    double dx = (maxx - minx) / (gdata.columns() - 1);
    double dy = (maxy - miny) / (gdata.rows() - 1);

    double tmin = DBL_MAX;
    double tmax = -DBL_MAX;

    /* fill out the vertex array for the mesh. */
    for (unsigned i = 0; i != columns; ++i)
    {
        for (unsigned j = 0; j != rows; ++j)
        {
            Triple& gdata_ij = gdata.vertices[i][j];
            double& val = data[i][j];

            gdata_ij.x = minx + i*dx;
            gdata_ij.y = miny + j*dy;

			gdata_ij.z = val;

			if (!_isnan(val))
			{
				if (val > tmax)
					tmax = val;
				if (val < tmin)
					tmin = val;
			}
        }
    }
    ParallelEpiped hull =
            ParallelEpiped(
                Triple(
                    gdata.vertices[0][0].x,
            gdata.vertices[0][0].y,
            tmin
            ),
            Triple(
                gdata.vertices[gdata.columns()-1][gdata.rows()-1].x,
            gdata.vertices[gdata.columns()-1][gdata.rows()-1].y,
            tmax
            )
            );

    gdata.setHull(hull);
}


void GridPlot::calcNormals(GridData& gdata)
{
	//QElapsedTimer timer;
	//timer.start();

    unsigned int rows = gdata.rows();
    unsigned int columns = gdata.columns();

    // normals   
    Triple u, v, n;  // for cross product

    for (unsigned i = 0; i != columns; ++i)
    {
        for (unsigned j = 0; j != rows; ++j)
        {
            n = Triple(0,0,0);

            const Triple& vert_ij = gdata.vertices[i][j];

			if (_isnan(vert_ij.z))
			{
				//gdata.normals[i][j] = n;
				continue;
			}

            if (i<columns-1 && j<rows-1)
            {
                /*	get two vectors to cross */
                u = gdata.vertices[i+1][j] - vert_ij;
                v = gdata.vertices[i][j+1] - vert_ij;
                /* get the normalized cross product */
                n += normalizedcross(u,v); // right hand system here !
            }

            if (i>0 && j<rows-1)
            {
                u = gdata.vertices[i][j+1] - vert_ij;
                v = gdata.vertices[i-1][j] - vert_ij;
                n += normalizedcross(u,v);
            }

            if (i>0 && j>0)
            {
                u = gdata.vertices[i-1][j] - vert_ij;
                v = gdata.vertices[i][j-1] - vert_ij;
                n += normalizedcross(u,v);
            }

            if (i<columns-1 && j>0)
            {
                u = gdata.vertices[i][j-1] - vert_ij;
                v = gdata.vertices[i+1][j] - vert_ij;
                n += normalizedcross(u,v);
            }
            n.normalize();

            gdata.normals[i][j] = n;
        }
    }

	//qDebug() << "GridPlot::calcNormals(): " << timer.elapsed();
}


void GridPlot::sewPeriodic(GridData& gdata)
{
    // sewing
    Triple n;

    unsigned int columns = gdata.columns();
    unsigned int rows = gdata.rows();

    if (gdata.uperiodic())
    {
        for (unsigned i = 0; i != columns; ++i)
        {
            n = gdata.normals[i][0] + gdata.normals[i][rows-1];
            n.normalize();
            gdata.normals[i][0] = gdata.normals[i][rows-1] = n;
        }
    }
    if (gdata.vperiodic())
    {
        for (unsigned j = 0; j != rows; ++j)
        {
            n = gdata.normals[0][j] + gdata.normals[columns-1][j];
            n.normalize();
            gdata.normals[0][j] = gdata.normals[columns-1][j] = n;
        }
    }
}

/*!
  Convert user grid data to internal vertex structure.
  See also NativeReader::operator() and Function::create()

  \param append For append==true the new dataset will be appended. If false (default), all data  will
  be replaced by the new data. This includes destruction of possible additional datasets/Plotlets.
  \return Index of new entry in dataset array (append == true), 0 (append == false) or -1 for errors
  */
int GridPlot::createDataset(Triple** data, unsigned int columns, unsigned int rows, 
                            bool uperiodic /*=false*/, bool vperiodic /*=false*/, bool append /*= false*/)
{
    int ret = prepareDatasetCreation<GridData>(append);
    if (ret < 0)
        return -1;

    GridData& plotdata = dynamic_cast<GridData&>(*plotlets_p[ret].data);
    readIn(plotdata, data, columns, rows);
    calcNormals(plotdata);
    plotdata.setPeriodic(uperiodic,vperiodic);
    sewPeriodic(plotdata);

    updateData();
    updateNormals();
    createCoordinateSystem();

    return ret;
}	

/*! 
  Convert user grid data to internal vertex structure.
  See also NativeReader::operator() and Function::create()

  \param append For append==true the new dataset will be appended. If false (default), all data  will
  be replaced by the new data. This includes destruction of possible additional datasets/Plotlets.
  \return Index of new entry in dataset array (append == true), 0 (append == false) or -1 for errors
  */
int GridPlot::createDataset(double** data, unsigned int columns, unsigned int rows,
                            double minx, double maxx, double miny, double maxy, bool append /*= false*/)
{	
    int ret = prepareDatasetCreation<GridData>(append);
    if (ret < 0)
        return -1;

    GridData& plotdata = dynamic_cast<GridData&>(*plotlets_p[ret].data);
    plotdata.setPeriodic(false,false);
    plotdata.setSize(columns,rows);
    readIn(plotdata,data,columns,rows,minx,maxx,miny,maxy);
    calcNormals(plotdata);

    updateData();
    updateNormals();
    createCoordinateSystem();

    return ret;
}	

void GridPlot::data2Floor(const Plotlet& pl)
{
    int step = resolution();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);

    const GridData& data = dynamic_cast<const GridData&>(*pl.data);
    double zshift = data.hull().minVertex.z;
    for (int i = 0; i < data.columns() - step; i += step)
    {
        glBegin(GL_TRIANGLE_STRIP);

        const Triple& vert1 = data.vertices[i][0];
        setColorFromVertex(pl, vert1);
        glVertex3d(vert1.x, vert1.y, zshift);

        const Triple& vert2 = data.vertices[i+step][0];
        setColorFromVertex(pl, vert2);
        glVertex3d(vert2.x, vert2.y, zshift);

        for (int j = 0; j < data.rows() - step; j += step)
        {
            const Triple& vert1 = data.vertices[i][j+step];
            setColorFromVertex(pl, vert1);
            glVertex3d(vert1.x, vert1.y, zshift);

            const Triple& vert2 = data.vertices[i+step][j+step];
            setColorFromVertex(pl, vert2);
            glVertex3d(vert2.x, vert2.y, zshift);
        }

        glEnd();
    }
}

void GridPlot::isolines2Floor(const Plotlet& pl)
{
    if (!isolines())
        return;

    const GridData& data = dynamic_cast<const GridData&>(*pl.data);
    const Color& color = *pl.appearance->dataColor();

    double zshift = data.hull().minVertex.z;
    if (delayisolinecalculation_p)
    {
        double step = (data.hull().maxVertex.z - data.hull().minVertex.z) / isolines();
        for (unsigned k = 0; k != isolines(); ++k)
        {
            isolinesZ_p[k] = zshift + k * step;
        }
    }

    int step = resolution();
    int cols = data.columns();
    int rows = data.rows();

    Triple t[4];
    vector<Triple> intersection;

    double lambda = 0;

    GLStateBewarer sb2(GL_LINE_SMOOTH, false);

    for (unsigned k = 0; k != isolines(); ++k)
    {
        double val = isolinesZ_p[k];
        if (val > data.hull().maxVertex.z || val < data.hull().minVertex.z)
            continue;
        
        for (int i = 0; i < cols-step; i += step)
        {
            for (int j = 0; j < rows-step; j += step)
            {
                t[0] =  data.vertices[i][j];
                t[1] =  data.vertices[i+step][j];
                t[2] =  data.vertices[i+step][j+step];
                t[3] =  data.vertices[i][j+step];

                double diff = 0;
                for (int m = 0; m!=4; ++m)
                {
                    int mm = (m+1)%4;
                    if ((val>=t[m].z && val<=t[mm].z) || (val>=t[mm].z && val<=t[m].z))
                    {
                        diff = t[mm].z - t[m].z;

                        if (isPracticallyZero(diff)) // degenerated
                        {
                            intersection.push_back(t[m]);
                            intersection.push_back(t[mm]);
                            continue;
                        }

                        lambda =  (val - t[m].z) / diff;
                        intersection.push_back(Triple(t[m].x + lambda * (t[mm].x-t[m].x), t[m].y + lambda * (t[mm].y-t[m].y), zshift));
                    }
                }

                drawIntersection(intersection, color.rgba(t[0].x,t[0].y,t[0].z));
                intersection.clear();
            }
        }
    }
}

/*!
  Sets data resolution (res == 1 original resolution) and updates widget
  If res < 1, the function does nothing
*/
void GridPlot::setResolution( int res )
{
    if (resolution_p == res || res < 1)
        return;

    resolution_p = res;
    updateData();
    updateNormals();
    if (initializedGL())
        updateGL();

    emit resolutionChanged(res);
}

void GridPlot::createOpenGlData(const Plotlet& pl)
{
    createFloorOpenGlData();

    const GridData& data = dynamic_cast<const GridData&>(*pl.data);
    const Appearance& app = *pl.appearance;

    if (app.plotStyle() == NOPLOT)
        return;

    for (Appearance::ELCIT it = app.elist.begin(); it!=app.elist.end(); ++it)
    {
        drawEnrichment(pl, **it);
    }

    int i, j;
    RGBA col;
    int step = resolution();

    if (app.plotStyle() == Qwt3D::POINTS)
    {
        Dot dot;
        drawEnrichment(pl, dot);
        return;
    }
    else if (app.plotStyle() == Qwt3D::USER)
    {
        if (app.userStyle())
            drawEnrichment(pl, *app.userStyle());
        return;
    }

    setLineWidth(app.meshLineWidth());

    GLStateBewarer sb(GL_POLYGON_OFFSET_FILL,true);
    glPolygonOffset(app.polygonOffset(),1.0);

    GLStateBewarer sb2(GL_LINE_SMOOTH, app.smoothDataMesh());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int lastcol = data.columns();
    int lastrow = data.rows();

    if (app.plotStyle() != WIREFRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);

        bool hl = (app.plotStyle() == HIDDENLINE);
        if (hl)
        {
            col = backgroundRGBAColor();
            glColor4d(col.r, col.g, col.b, col.a);
        }

        for (i = 0; i < lastcol - step; i += step)
        {
			bool stripStarted = false;

            const Triple& norm1 = data.normals[i][0];
            const Triple& vert1 = data.vertices[i][0];
			processVertex(vert1, norm1, pl, hl, stripStarted);

            const Triple& norm2 = data.normals[i+step][0];
            const Triple& vert2 = data.vertices[i+step][0];
			processVertex(vert2, norm2, pl, hl, stripStarted);

            for (j = 0; j < lastrow - step; j += step)
            {
                const Triple& norm1 = data.normals[i][j+step];
                const Triple& vert1 = data.vertices[i][j+step];
				processVertex(vert1, norm1, pl, hl, stripStarted);

                const Triple& norm2 = data.normals[i+step][j+step];
                const Triple& vert2 = data.vertices[i+step][j+step];
				processVertex(vert2, norm2, pl, hl, stripStarted);
            }

			if (stripStarted)
				glEnd();
        }
    }

    if (app.plotStyle() == FILLEDMESH || app.plotStyle() == WIREFRAME || app.plotStyle() == HIDDENLINE)
    {
        glColor4d(app.meshColor().r, app.meshColor().g, app.meshColor().b, app.meshColor().a);

        if (step < data.columns() && step < data.rows())
        {
            glBegin(GL_LINE_LOOP);

            for (i = 0; i < data.columns() - step; i += step)
            {
                const Triple& vert1 = data.vertices[i][0];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }

            for (j = 0; j < data.rows() - step; j += step)
            {
                const Triple& vert1 = data.vertices[i][j];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }

            for (; i >= 0; i -= step)
            {
                const Triple& vert1 = data.vertices[i][j];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }

            for (; j >= 0; j -= step)
            {
                const Triple& vert1 = data.vertices[0][j];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }

            glEnd();
        }

        // weaving
        for (i = step; i < data.columns() - step; i += step)
        {
            glBegin(GL_LINE_STRIP);
            for (j = 0; j < data.rows(); j += step)
            {
                const Triple& vert1 = data.vertices[i][j];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }
            glEnd();
        }

        for (j = step; j < data.rows() - step; j += step)
        {
            glBegin(GL_LINE_STRIP);
            for (i = 0; i < data.columns(); i += step)
            {
                const Triple& vert1 = data.vertices[i][j];
				if (!_isnan(vert1.z))
					glVertex3d(vert1.x, vert1.y, vert1.z);
            }
            glEnd();
        }
    }
}

void GridPlot::processVertex(const Triple& vert1, const Triple& norm1, const Plotlet& pl, bool hl, bool& stripStarted)
{
	if (_isnan(vert1.z))
	{
		if (stripStarted){
			stripStarted = false;
			glEnd();
		}
	}
	else{
		if (!stripStarted){
			stripStarted = true;
			glBegin(GL_TRIANGLE_STRIP);
		}

		setColorFromVertex(pl, vert1, hl);
		glNormal3d(norm1.x, norm1.y, norm1.z);
		glVertex3d(vert1.x, vert1.y, vert1.z);
	}
}

void GridPlot::drawEnrichment(const Plotlet& pl, Enrichment& p)
{
    switch(p.type())
    {
        case Enrichment::USERENRICHMENT:
        {
            p.assign(*this);
            UserEnrichment* ue = (UserEnrichment*)&p;
            ue->drawBegin();
            ue->draw();
            ue->drawEnd();
        }
        break;
    case Enrichment::VERTEXENRICHMENT:
    {
        p.assign(*this);
        VertexEnrichment* ve = (VertexEnrichment*)&p;
        ve->drawBegin();
        const GridData& data = dynamic_cast<const GridData&>(*pl.data);
        int step = resolution();
        for (int i = 0; i <= data.columns() - step; i += step)
            for (int j = 0; j <= data.rows() - step; j += step)
                ve->draw(data.vertices[i][j]);
        ve->drawEnd();
    }
        break;
    case Enrichment::EDGEENRICHMENT:
        break;
    default:
        break; //todo
    }
}
