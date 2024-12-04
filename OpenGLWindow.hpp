#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <QWindow>
#include <QOpenGLFunctions>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
    Q_OBJECT

protected:
    bool event(QEvent* event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;

    QOpenGLContext *m_context;

public slots:
        void renderLater();
        void renderNow();

public:
    explicit OpenGLWindow(QWindow *parent = 0);

    virtual void initialize() = 0;
    virtual void render() = 0;
};


#endif