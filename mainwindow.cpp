#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
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
    delete mRenderWindowWidget;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 -
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The example rendering will need a depth buffer - not requiered to set in glfw-tutorials!
    format.setDepthBufferSize(16);

    //Just prints out what OpenGL format we will get
    // - can be deleted
    qDebug() << "Requesting surface format" << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindowWidget = new RenderWindow(format, this);

    //Checks if renderwindow did initialize, else prints error and quits
    if (!mRenderWindowWidget->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindowWidget;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindowWidget);
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted
    mRenderWindowContainer->setFocus();
}
