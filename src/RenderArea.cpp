#include "RenderArea.h"

#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgViewer/GraphicsWindow>

#include <QApplication>

RenderArea::RenderArea()
{
    root_ = new osg::MatrixTransform();
};

void RenderArea::initializeGL()
{
    if (viewer_)
        return;

    auto viewer = new osgViewer::Viewer();
    auto traits = new osg::GraphicsContext::Traits();
    traits->width = this->width();
    traits->height = this->height();
    traits->samples = 4;
    traits->depth = 32;
    traits->glContextVersion = "4.6";
    traits->inheritedWindowData = new osgViewer::GraphicsWindowWin32::WindowData((HWND)winId());
    // auto gc = new osgViewer::GraphicsWindowWin32(traits);
    auto gc = new osgViewer::GraphicsWindowEmbedded(traits);

    auto camera = viewer->getCamera();
    camera->setViewport(0, 0, traits->width, traits->height);
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    camera->setClearColor(osg::Vec4(1, 1, 1, 1));
    camera->setGraphicsContext(gc);

    viewer->setCamera(camera);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator());
    viewer->realize();
    viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    viewer->setSceneData(root_);
    viewer->setLightingMode(osg::View::HEADLIGHT);
    viewer_ = viewer;
    gc_ = gc;
}
void RenderArea::resizeGL(int w, int h)
{
    auto ratio = this->devicePixelRatio();

    h *= ratio;
    w *= ratio;
    // auto screen_num = QApplication::screenAt(,);

    gc_->resized(0, 0, w, h);
    viewer_->getCamera()->setViewport(0, 0, w, h);
    viewer_->getEventQueue()->windowResize(0, 0, w, h);
}

void RenderArea::resizeEvent(QResizeEvent *e)
{
    QOpenGLWidget::resizeEvent(e);
}

void RenderArea::paintGL()
{
    viewer_->frame();
}

void RenderArea::addModel(osg::Node *node)
{
    root_->addChild(node);
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    unsigned int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MiddleButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    default:
        break;
    }
    gc_->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
    QOpenGLWidget::mousePressEvent(event);
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    unsigned int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MiddleButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    default:
        break;
    }
    gc_->getEventQueue()->mouseMotion(event->x(), event->y(), button);
    QOpenGLWidget::mouseMoveEvent(event);
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    unsigned int button = 0;
    switch (event->button())
    {
    case Qt::LeftButton:
        button = 1;
        break;
    case Qt::MiddleButton:
        button = 2;
        break;
    case Qt::RightButton:
        button = 3;
        break;
    default:
        break;
    }
    gc_->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
    QOpenGLWidget::mouseReleaseEvent(event);
}

void RenderArea::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
    gc_->getEventQueue()->mouseScroll(motion);
    QOpenGLWidget::wheelEvent(event);
}

bool RenderArea::event(QEvent *event)
{
    bool handled = QOpenGLWidget::event(event);
    update();
    return handled;
}