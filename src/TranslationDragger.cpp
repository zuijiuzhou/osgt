#include "TranslationDragger.h"

#include <iostream>

#include <osg/ShapeDrawable>
#include <osg/AutoTransform>
#include <osg/PolygonMode>
#include <osgGA/GuiEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>

#include "Ge.h"

class TranslationDraggerEventCallback : public osgGA::GUIEventHandler
{
public:
    TranslationDraggerEventCallback(TranslationDragger *dragger)
        : dragger_(dragger)
    {
    }
    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa, osg::Object *, osg::NodeVisitor *) override
    {
        return dragger_->handle(ea, aa);
    }

private:
    TranslationDragger *dragger_;
};

TranslationDragger::TranslationDragger()
{
    createGeometry();
}

void TranslationDragger::createGeometry()
{
    auto cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0, 0, 30), 8, 20));
    auto cyli = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0, 0, 15), 5, 30));
    cone->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    cyli->setColor(osg::Vec4(1.f, 0.f, 0.f, 1.f));

    auto geo = new osg::Geode();

    geo->addDrawable(cone);
    geo->addDrawable(cyli);
    geo->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

    auto at = new osg::AutoTransform();
    at->addChild(geo);

    at->setAutoScaleToScreen(true);

    dragger_ = new osg::MatrixTransform();
    dragger_->addChild(at);
    dragger_->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::Face::FRONT_AND_BACK, osg::PolygonMode::Mode::FILL), osg::StateAttribute::ON);
    
    addEventCallback(new TranslationDraggerEventCallback(this));
    addChild(dragger_);
}

void TranslationDragger::setDirection(const osg::Vec3 &dir)
{
    direction_ = dir;
    dragger_->setMatrix(osg::Matrix::rotate(osg::Vec3(0.f, 0.f, 1.f), dir));
}

void TranslationDragger::setPosition(const osg::Vec3 &posi)
{
    setMatrix(osg::Matrix::translate(posi));
    position_ = posi;
}

void TranslationDragger::onDragStart(const osg::Vec2 &pt_start_screen)
{
    pt_start_screen_ = pt_start_screen;
    is_start_ = true;
    is_cancelled_ = false;
    if(translation_started_callback_){
        translation_started_callback_(this);
    }
}

void TranslationDragger::onDrag(const osg::Vec2 &pt_screen, const osg::Camera &camera)
{
    osg::Vec3 view_posi, view_target, view_up, view_dir;
    camera.getViewMatrixAsLookAt(view_posi, view_target, view_up);
    view_dir = view_target - view_posi;
    view_dir.normalize();

    auto is_ortho_projection = camera.getProjectionMatrix()(2, 3) == 0.;
    auto screen_to_world_matrix = osg::Matrix::inverse(camera.getViewMatrix() * camera.getProjectionMatrix());

    auto pt_start = screen_to_world_matrix.preMult(osg::Vec3(pt_start_screen_, 0.f));
    auto pt_end = screen_to_world_matrix.preMult(osg::Vec3(pt_screen, 0.f));

    osg::Vec3 eye_to_model_dir, line_start_dir, line_end_dir, move_plane_norm;
    if (is_ortho_projection)
    {
        eye_to_model_dir = view_dir;
        line_start_dir = view_dir;
        line_end_dir = view_dir;
    }
    else
    {
        eye_to_model_dir = position_ - view_posi;
        line_start_dir = pt_start - view_posi;
        line_end_dir = pt_end - view_posi;
    }
    move_plane_norm = (direction_ ^ eye_to_model_dir) ^ direction_;
    eye_to_model_dir.normalize();
    line_start_dir.normalize();
    line_end_dir.normalize();
    move_plane_norm.normalize();

    if((direction_ ^ eye_to_model_dir).length() < 0.0000001)
        return;

    Plane move_plane(position_, move_plane_norm);
    Line line_start(pt_start, line_start_dir);
    Line line_end(pt_end, line_end_dir);
    Line line_move(position_, direction_);

    osg::Vec3 ispt_start, ispt_end;
    move_plane.intersectWith(line_start, ispt_start);
    move_plane.intersectWith(line_end, ispt_end);

    osg::Vec3 offset = direction_ * (direction_ * (pt_end - pt_start));

    printf("%f,%f,%f\n", offset.x(), offset.y(), offset.z());
    if(translated_callback_){
        translated_callback_(this, offset);
    }
}

void TranslationDragger::onDragEnd(bool is_cancelled)
{
    pt_start_screen_ = osg::Vec2();
    is_start_ = false;
    is_cancelled_ = is_cancelled;
}

bool TranslationDragger::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
    auto et = ea.getEventType();
    auto btn = ea.getButton();
    if (et == osgGA::GUIEventAdapter::FRAME)
        return false;
    auto &view = dynamic_cast<osgViewer::Viewer::View &>(aa);
    auto pt_screen = osg::Vec2(ea.getXnormalized(), ea.getYnormalized());
    if (et == osgGA::GUIEventAdapter::PUSH && btn == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
    {
        if (is_start_)
            return false;
        osgUtil::LineSegmentIntersector::Intersections iss;
        if (view.computeIntersections(ea, iss))
        {
            for (auto &is : iss)
            {
                auto &np = is.nodePath;
                if (np.empty())
                    continue;
                for (auto n = np.rbegin(); n != np.rend(); n++)
                {
                    if (*n == this)
                    {
                        onDragStart(osg::Vec2(ea.getXnormalized(), ea.getYnormalized()));
                        return true;
                    }
                }
            }
        }
    }
    if (!is_start_)
        return false;
    if (et == osgGA::GUIEventAdapter::DRAG)
    {
        onDrag(osg::Vec2(ea.getXnormalized(), ea.getYnormalized()), *view.getCamera());
        return true;
    }
    else if (et == osgGA::GUIEventAdapter::RELEASE && btn == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
    {
        onDragEnd(false);
    }
    return false;
}