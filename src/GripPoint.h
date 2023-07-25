#include <osg/Vec3>

class GripPoint
{
public:
    GripPoint(int idx, const osg::Vec3 &posi, const osg::Vec3 &dir)
        : index_(idx), posi_(posi), dir_(dir)
    {
    }

    const osg::Vec3 &position() const { return posi_; }
    const osg::Vec3 &direction() const { return dir_; }
    const int &index() const { return index_; }

private:
    int index_;
    osg::Vec3 posi_;
    osg::Vec3 dir_;
};