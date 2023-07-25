#pragma once
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <osg/Switch>
#include <osgViewer/Viewer>

class RenderArea : public QOpenGLWidget
{
public:
    RenderArea();

    void addModel(osg::Node *node);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void mouseReleaseEvent(QMouseEvent *event)override;

    virtual void wheelEvent(QWheelEvent *event)override;

    virtual bool event(QEvent* event) override;

    void resizeEvent(QResizeEvent *e) override;
private:
    osgViewer::Viewer *viewer_ = nullptr;
    osgViewer::GraphicsWindow *gc_ = nullptr;
    osg::Group *root_ = nullptr;
};