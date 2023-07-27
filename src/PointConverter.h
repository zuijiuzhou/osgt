#pragma once
#include <osg/View>

class PointConverter
{
public:
    static osg::Matrix getScreenToWorldMatrix(osg::View *view);
    static osg::Matrix getWorldToScreenMatrix(osg::View *view);

    static osg::Vec3 ndcPointToWorld(osg::View* view, const osg::Vec2& pt);
    static osg::Vec3 screenPointToWorld(osg::View* view, const osg::Vec2& pt);
    static osg::Vec2 screenPointToNdc(osg::View* view, const osg::Vec2& pt);
};