#include "Box.h"

#include <osg/Geode>
#include <osgUtil/SmoothingVisitor>

Box::Box(double len, double w, double h)
    : len_(len)
    , width_(w)
    , height_(h)
{


    auto colors = new osg::Vec4Array();
    colors->push_back(osg::Vec4(0.5f,0.5f, 0.f, 1.f));

    box_ = new osg::Geometry();
    box_->setVertexArray(new osg::Vec3Array(8));
    // box_->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP, 0, 24));
    box_->setColorArray(colors);
    box_->setColorBinding(osg::Geometry::BIND_OVERALL);
    box_->setUseDisplayList(false);

    auto prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(0);
    prim->push_back(1);
    prim->push_back(3);
    prim->push_back(2);
    box_->addPrimitiveSet(prim);

    prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(4);
    prim->push_back(0);
    prim->push_back(2);
    prim->push_back(6);
    box_->addPrimitiveSet(prim);

    prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(5);
    prim->push_back(4);
    prim->push_back(6);
    prim->push_back(7);
    box_->addPrimitiveSet(prim);

    prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(1);
    prim->push_back(5);
    prim->push_back(7);
    prim->push_back(3);
    box_->addPrimitiveSet(prim);

    prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(1);
    prim->push_back(0);
    prim->push_back(4);
    prim->push_back(5);
    box_->addPrimitiveSet(prim);

    prim = new osg::DrawElementsUShort(GL_QUADS);
    prim->push_back(6);
    prim->push_back(2);
    prim->push_back(3);
    prim->push_back(7);
    box_->addPrimitiveSet(prim);


    auto norms = new osg::Vec3Array();
    norms->push_back(osg::Vec3(-1.f, 0.f, 0.f));
    norms->push_back(osg::Vec3(0.f, -1.f, 0.f));
    norms->push_back(osg::Vec3(1.f, 0.f, 0.f));
    norms->push_back(osg::Vec3(0.f, 1.f, 0.f));
    norms->push_back(osg::Vec3(0.f, 0.f, 1.f));
    norms->push_back(osg::Vec3(0.f, 0.f, -1.f));
    box_->setNormalArray(norms);
    box_->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    auto geo = new osg::Geode();
    geo->addDrawable(box_);

    addChild(geo);

    UpdateGeometry();
}

void Box::UpdateGeometry()
{
    auto &vts = *static_cast<osg::Vec3Array *>(box_->getVertexArray());
    auto h_len = len_ / 2.;
    auto h_w = width_ / 2.;
    auto h_h = height_ / 2.;

    /*0*/ osg::Vec3 left_front_top(-h_len, -h_w, h_h);
    /*1*/ osg::Vec3 left_back_top(-h_len, h_w, h_h);
    /*2*/ osg::Vec3 left_front_bottom(-h_len, -h_w, -h_h);
    /*3*/ osg::Vec3 left_back_bottom(-h_len, h_w, -h_h);
    /*4*/ osg::Vec3 right_front_top(h_len, -h_w, h_h);
    /*5*/ osg::Vec3 right_back_top(h_len, h_w, h_h);
    /*6*/ osg::Vec3 right_front_bottom(h_len, -h_w, -h_h);
    /*7*/ osg::Vec3 right_back_bottom(h_len, h_w, -h_h);

    vts[0] = left_front_top;
    vts[1] = left_back_top;
    vts[2] = left_front_bottom;
    vts[3] = left_back_bottom;
    vts[4] = right_front_top;
    vts[5] = right_back_top;
    vts[6] = right_front_bottom;
    vts[7] = right_back_bottom;

    vts.dirty();

    box_->dirtyBound();
    box_->dirtyGLObjects();
}