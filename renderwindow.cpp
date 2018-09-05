#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <QMatrix4x4>

#include "shader.h"
#include "mainwindow.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    mContext = new QOpenGLContext(this);
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    mMVPmatrix = new QMatrix4x4{};
    mMVPmatrix->setToIdentity();
}

RenderWindow::~RenderWindow()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

static GLfloat vertices[] =
{
    // Positions         // Colors
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // Bottom Right
    0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
};

void RenderWindow::init()
{

    //********************** General OpenGL stuff **********************

    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    if (!mInitialized)
        mInitialized = true;

    initializeOpenGLFunctions();

    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);     //draws only front side of models - usually what you want -
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    mShaderProgram = new Shader("../MinimalQtOpenGL4/plainvertex.vert", "../MinimalQtOpenGL4/plainfragment.frag");


    //********************** Making the object to be drawn **********************

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                6 * sizeof(GLfloat),  // stride
                (GLvoid*)0            // array buffer offset
                );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6 * sizeof( GLfloat ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );

    glBindVertexArray( 0 );
}

void RenderWindow::render()
{
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(mShaderProgram->getProgram() );

    glBindVertexArray( mVAO );

    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMVPmatrix->constData());

    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkForGLerrors();

    mContext->swapBuffers(this);

    //               degree, x,   y,   z -axis
    mMVPmatrix->rotate(2.f, 0.f, 1.0, 0.f);
}

void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    if (isExposed())
    {
        //This timer runs the actual MainLoop
        mTimer.start(16, this); //this
        mTimeStart.start();
    }
}

void RenderWindow::timerEvent(QTimerEvent *)
{
    render();

    //The rest here is just to show the frame rate:
    int msSinceLastFrame = mTimeStart.restart();    //restart() returns ms since last restart.
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 60) //once pr 60 frames =  update the message once pr second
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time Pr FrameDraw: " + QString::number(msSinceLastFrame) + " ms");
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        qDebug() << "Can this system run QOpenGLDebugLogger? :" << format.testOption(QSurfaceFormat::DebugContext);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
    {
        mMainWindow->statusBar()->showMessage(" AAAA");
    }
    if(event->key() == Qt::Key_S)
    {
        mMainWindow->statusBar()->showMessage(" SSSS");
    }
}
