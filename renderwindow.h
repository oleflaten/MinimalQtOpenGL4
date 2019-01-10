#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QTime>

class QOpenGLContext;
class Shader;
class MainWindow;

///This inherits from QWindow so we can put it inside a QWidget
/// It has to inherit from QWindow, not a QWidget because we want to have
/// access to the full OpenGL, and then we have to set the format for this window.
/// Not possible for a Widget.
///
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL stuff
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }
    void resizeGL(int w, int h) override;
//    void exposeEvent(QExposeEvent *) override;

 void error(const QString &msg);

private slots:
    void paintGL() override;

private:
    void initializeGL() override;

    void startOpenGLDebugger();

    QOpenGLContext *mContext;
    bool mInitialized;

    Shader *mShaderProgram;
    GLint  mMatrixUniform;

    GLuint mVAO;
    GLuint mVBO;

    QMatrix4x4 *mMVPmatrix; //The matrix with the transform for the object we draw

    QTimer *mTimer;     //timer that drives the gameloop

    QTime mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

protected:
    //The QWindow that we inherit from has these functions to capture
    // mouse and keyboard. Uncomment to use
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}

    void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
