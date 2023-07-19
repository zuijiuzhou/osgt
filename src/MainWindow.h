#include <QMainWindow>
#include "RenderArea.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow();

    void init();

    RenderArea *renderArea() const { return render_area_; }

private:
    RenderArea *render_area_ = nullptr;
};