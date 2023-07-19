#include <osg/MatrixTransform>
#include <osg/Geometry>

class Box : public osg::MatrixTransform
{
public:
    Box(double len, double w, double h);

    double len() const { return len_; }
    double width() const { return width_; }
    double height() const { return height_; }

private:
    void UpdateGeometry();

private:
    double len_ = 0, width_ = 0, height_ = 0;
    osg::Geometry *box_ = nullptr;
};