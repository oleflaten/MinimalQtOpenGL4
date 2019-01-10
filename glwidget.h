#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLWidget(QSurfaceFormat format, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // GLWIDGET_H
