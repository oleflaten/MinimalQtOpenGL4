#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QBasicTimer>
#include <QTime>

class QOpenGLContext;
class Shader;
class MainWindow;

class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }
    void exposeEvent(QExposeEvent *) override;

 void error(const QString &msg);

private slots:
    void render();

private:
    void init();

    void startOpenGLDebugger();
    void checkForGLerrors();

    QOpenGLContext *mContext;
    bool mInitialized;

    Shader *mShaderProgram;
    GLint  mMatrixUniform;

    GLuint mVAO;
    GLuint mVBO;

    QMatrix4x4 *mMVPmatrix;

    QBasicTimer mTimer;     //timer that drives the gameloop
    QTime mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

protected:
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    //    void keyReleaseEvent(QKeyEvent *event) override{}
    //    void wheelEvent(QWheelEvent *event) override{}

    void timerEvent(QTimerEvent *) override;
};

#endif // RENDERWINDOW_H
