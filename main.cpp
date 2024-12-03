#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>
#include <QHBoxLayout>
#include "ControlPanel.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("Rocket");
    mainWindow->resize(800, 600);

    RocketScene *rScene = new RocketScene();
    RocketObject *rObject = new RocketObject();
    ControlPanel *rPanel = new ControlPanel(nullptr, rObject, rScene);

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWindow);

    mainWindow->setLayout(mainLayout);

    mainWindow->show();

    return app.exec();
}

