#include "PointConverter.h"

static osg::Matrix getScreenToWorldMatrix(osg::View *view)
{
    return osg::Matrix::inverse(getWorldToScreenMatrix(view));
}

static osg::Matrix getWorldToScreenMatrix(osg::View *view)
{
    auto camera = view->getCamera();
    auto view_matrix = camera->getViewMatrix();
    auto proj_matrix = camera->getProjectionMatrix();
    auto vp = camera->getViewport();
    auto screen_matrix = vp->computeWindowMatrix();

    return view_matrix * proj_matrix * screen_matrix;
}

static osg::Vec3 ndcPointToWorld(osg::View *view, const osg::Vec2 &pt)
{
    auto camera = view->getCamera();
    auto view_matrix = camera->getViewMatrix();
    auto proj_matrix = camera->getProjectionMatrix();
    auto vp_matrix = view_matrix * proj_matrix;
    auto pv_matrix = osg::Matrix::inverse(vp_matrix);

    auto world_pt = osg::Vec3(pt, 0) * pv_matrix;
    return world_pt;
}

static osg::Vec3 screenPointToWorld(osg::View *view, const osg::Vec2 &pt)
{
    auto camera = view->getCamera();
    auto view_matrix = camera->getViewMatrix();
    auto proj_matrix = camera->getProjectionMatrix();
    auto vp = camera->getViewport();
    auto screen_matrix = vp->computeWindowMatrix();
    auto vps_matrix = view_matrix * proj_matrix * screen_matrix;
    auto spv_matrix = osg::Matrix::inverse(vps_matrix);

    // QT 及 Windows窗口坐标系统原点(0,0)在左上角，OpenGL在左下角，注意D3D在左上角
    auto world_pt = osg::Vec3(pt.x(), vp->y() - pt.y(), 0);
    world_pt = world_pt * spv_matrix;
    return world_pt;
}

static osg::Vec2 screenPointToNdc(osg::View* view, const osg::Vec2& pt){
    auto camera = view->getCamera();
    auto vp = camera->getViewport();
    auto ndc_pt = osg::Vec2(pt.x(), vp->y() - pt.y());
    auto x = ndc_pt.x() * 2. / vp->x() - 1.;
    auto y = 1. - (2. * ndc_pt.y() / vp->y());
    return osg::Vec2(x, y);
}