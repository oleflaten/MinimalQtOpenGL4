#include "glwidget.h"
#include <QSurfaceFormat>


GLWidget::GLWidget(QSurfaceFormat format, QWidget *parent)
{
    setFormat(format);
}
