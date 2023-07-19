#include "MainWindow.h"

MainWindow::MainWindow()
{
    
}

void MainWindow::init()
{
    resize(600, 400);
    auto ra = new RenderArea();
    setCentralWidget(ra);
    render_area_ = ra;
}