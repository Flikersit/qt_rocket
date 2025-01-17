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


    RocketSceneFinal *rScene = new RocketSceneFinal();
    ControlPanel *rPanel = new ControlPanel(nullptr, rScene);

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWindow);
    mainLayout->addWidget(rPanel);

    mainWindow->setLayout(mainLayout);

    mainWindow->show();

    return app.exec();
}
