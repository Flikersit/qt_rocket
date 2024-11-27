#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "RocketObject.h"
#include "RocketScene.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.setSceneRect(-200, -200, 400, 400);

    RocketScene *rscene = new RocketScene();
    scene.addItem(rscene);

    QGraphicsView view(&rscene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle("Rocket");
    view.resize(800, 600);
    view.show();

    return app.exec();
}

