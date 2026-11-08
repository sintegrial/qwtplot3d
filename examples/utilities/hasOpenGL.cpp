#include "hasOpenGL.h"

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    #include <QGLFormat>
#else
    #include <QOpenGLContext>
#endif


namespace Qwt3D
{

bool hasOpenGL()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    return QGLFormat::hasOpenGL();
#else
    QOpenGLContext context;
    return context.create() && context.isValid();
#endif
}

} // namespace Qwt3D
