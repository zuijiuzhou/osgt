#pragma once
#include <osg/Vec3>
#include <osg/Matrix>

class Line
{
public:
    Line(const osg::Vec3 &origin, const osg::Vec3 &dir)
        : origin_(origin), dir_(dir)
    {
    }

    const osg::Vec3 &origin() const { return origin_; }
    const osg::Vec3 &dir() const { return dir_; }

private:
    osg::Vec3 origin_;
    osg::Vec3 dir_;
};

class Plane
{
public:
    Plane(const osg::Vec3 &origin, const osg::Vec3 &norm)
        : origin_(origin), norm_(norm)
    {
    }

    bool intersectWith(const Line &line, osg::Vec3 &intersection_pt)
    {
        auto &lori = line.origin();
        auto &ldir = line.dir();

        auto ptol = origin_ - lori;
        auto dot = ptol * norm_;
        if (abs(dot) < 0.0000001)
        {
            return false;
        }
        else
        {
            auto t = ptol * norm_ / dot;
            intersection_pt = lori + ldir * t;
            return true;
        }
    }

private:
    osg::Vec3 origin_;
    osg::Vec3 norm_;
};
