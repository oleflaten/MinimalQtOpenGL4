#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>

#include "renderwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    QSurfaceFormat format;

    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);

    format.setOption(QSurfaceFormat::DebugContext);

    format.setDepthBufferSize(16);

    qDebug() << "Requesting surface format" << format;

    mRenderWindow = new RenderWindow(format, this);

    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    mRenderWindowContainer->setFocus();
}
