#include <iostream>
#include <QApplication>

#include <osg/ShapeDrawable>

#include "MainWindow.h"
#include "Box.h"

osg::Node* CreateBox(){
    return new Box(20,10,10);
}

osg::Node* CreateCone(){
    auto cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(-20, 0, 0), 20, 30));
    auto geo = new osg::Geode();
    geo->addDrawable(cone);
    return geo;
}

int main(int argc, char **argv)
{

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    MainWindow wnd;
    wnd.init();
    wnd.renderArea()->addModel(CreateBox());
    // wnd.renderArea()->addModel(CreateCone());
    wnd.show();
    return app.exec();
}