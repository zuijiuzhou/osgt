#pragma once

#include <memory>
#include <functional>

#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgGA/GuiEventHandler>

class TranslationDragger;

using TranslatedCallback = std::function<void(TranslationDragger *sender, const osg::Vec3 &offset)>;
using TranslationStartedCallback = std::function<void(TranslationDragger *sender)>;

class TranslationDragger : public osg::MatrixTransform
{
    friend class TranslationDraggerEventCallback;

public:
    TranslationDragger();

    void setDirection(const osg::Vec3 &dir);

    void setPosition(const osg::Vec3 &posi);

    void setTranslatedCallback(TranslatedCallback callback) { translated_callback_ = std::move(callback); }

    void setTranslationStartedCallback(TranslationStartedCallback callback) { translation_started_callback_ = std::move(callback); }

    void setIndex(int idx) { index_ = idx; }

    int index() const { return index_; }

    double initialValue() const { return initial_val_; }

    void setInitialValue(double val) { initial_val_ = val; }
private:
    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

    void createGeometry();

    void onDragStart(const osg::Vec2 &pt_start_screen);
    void onDrag(const osg::Vec2 &pt_screen, const osg::Camera &camera);
    void onDragEnd(bool is_cancelled);

private:
    osg::Vec3 direction_;
    osg::Vec3 position_;

    osg::MatrixTransform *dragger_ = nullptr;

    bool is_start_ = false;
    bool is_cancelled_ = false;

    osg::Vec2 pt_start_screen_;
    osg::Vec3 pt_start_;

    int index_;
    double initial_val_ = 0.;

    TranslatedCallback translated_callback_;
    TranslationStartedCallback translation_started_callback_;
};
