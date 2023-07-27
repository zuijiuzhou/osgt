#include "TestDragger.h"

namespace
{
    Box *createBox()
    {
        auto box = new Box(20, 10, 10);
        box->setMatrix(osg::Matrix::rotate(0.5, osg::Vec3d(0, 1, 2)) * osg::Matrix::translate(100, 0, 50));
        return box;
    }

    TranslationDragger *createDragger(const GripPoint &gp)
    {
        auto dragger = new TranslationDragger();
        dragger->setDirection(gp.direction());
        dragger->setMatrix(osg::Matrix::translate(gp.position()));
        return dragger;
    }

    std::map<int, TranslationDragger *> draggers;
}

void TestTranslationDragger(MainWindow *wnd)
{
    auto box = createBox();
    wnd->renderArea()->addModel(box);

    auto &grippoints = box->getGripPoints();
    for (int i = 0; i < grippoints.size(); i++)
    {
        auto &gp = grippoints[i];
        auto dragger = createDragger(gp);
        dragger->setIndex(gp.index());
        dragger->setTranslatedCallback(
            [box](TranslationDragger *dragger, const osg::Vec3 &offset)
            {
                auto offlen = box->getGripPoints()[dragger->index()].direction() * offset > 0 ? offset.length() : -offset.length();
                if (dragger->index() == 0 || dragger->index() == 1)
                {
                    box->setLen(abs(dragger->initialValue() + offlen * 2));
                }
                else if (dragger->index() == 2 || dragger->index() == 3)
                {
                    box->setWidth(abs(dragger->initialValue() + offlen * 2));
                }
                else if (dragger->index() == 4 || dragger->index() == 5)
                {
                    box->setHeight(abs(dragger->initialValue() + offlen * 2));
                }
                for (int j = 0; j < draggers.size(); j++)
                {
                    draggers[j]->setMatrix(osg::Matrix::translate(box->getGripPoints()[j].position()));
                }
            });
        dragger->setTranslationStartedCallback(
            [box](TranslationDragger *dragger)
            {
                if (dragger->index() == 0 || dragger->index() == 1)
                {
                    dragger->setInitialValue(box->len());
                }
                else if (dragger->index() == 2 || dragger->index() == 3)
                {
                    dragger->setInitialValue(box->width());
                }
                else if (dragger->index() == 4 || dragger->index() == 5)
                {
                    dragger->setInitialValue(box->height());
                }
            });
        draggers.insert({i, dragger});
        box->addChild(dragger);
    }
}