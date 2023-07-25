#pragma once
#include <vector>

#include <osg/MatrixTransform>
#include <osg/Geometry>

#include "GripPoint.h"

class Box : public osg::MatrixTransform
{
public:
    Box(double len, double w, double h);

    double len() const { return len_; }
    double width() const { return width_; }
    double height() const { return height_; }

    void setLen(double len);
    void setWidth(double width);
    void setHeight(double height);

    const std::vector<GripPoint> &getGripPoints() const { return grippoints_; }

private:
    void updateGeometry();

private:
    double len_ = 0, width_ = 0, height_ = 0;
    osg::Geometry *box_ = nullptr;
    std::vector<GripPoint> grippoints_;
};