// Phase 6 compatibility shim: qgl.h → QOpenGLWidget (Qt6)
// Qt6 removed QGLWidget; the replacement is QOpenGLWidget.
// This header provides a minimal bridge for old code.
#ifndef __TEXMOL_QGL_COMPAT_H__
#define __TEXMOL_QGL_COMPAT_H__

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>

// Backward-compatible alias
using QGLWidget = QOpenGLWidget;
using QGLFormat = QSurfaceFormat;

#endif
