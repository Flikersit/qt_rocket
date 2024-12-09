#include "RocketScene.h"
#include <QGraphicsRectItem>
#include <QTransform>

RocketScene::RocketScene(RocketObject *rocket){
    this->rocket = rocket;
    height = 400.0;
    width = 300.0;
    launchPadOffSet = 50.0;

    setBackgroundBrush(Qt::black);
    setSceneRect(-(width/2), 0, width, height);//it works dont touch

}

void RocketScene::paint_position(double x, double y){
    
    setSceneRect(-(width/2), 0, width, height);

    QGraphicsRectItem *rect_1 = new QGraphicsRectItem(0, 0, 20, 40);
    QGraphicsRectItem *rect_2 = new QGraphicsRectItem(0, 0, 20, 40);

    rect_1->setBrush(QColor(165, 42, 42));
    rect_2->setBrush(QColor(165, 42, 42));
    rect_1->setBrush(Qt::white);
    rect_2->setBrush(Qt::white);

    rect_1->setPos(launchPadOffSet - width, 0);
    rect_2->setPos(width - launchPadOffSet, 0);

    QGraphicsRectItem *rect_3 = new QGraphicsRectItem(0, 0, 50, 10);
    QGraphicsRectItem *rect_4 = new QGraphicsRectItem(0, 0, 50, 10);

    rect_3->setBrush(Qt::blue);
    rect_4->setBrush(Qt::blue);

    rect_3->setPos(launchPadOffSet - width -15, 40);
    rect_4->setPos(width - launchPadOffSet - 15, 40);

    rocket->setPos(x, y + 85.0);


    addItem(rect_1);
    addItem(rect_2);
    addItem(rect_3);
    addItem(rect_4);
    addItem(rocket);

    qDebug() << "Item's in scene" << items().size();

}

void RocketScene::reset(){
    clear();
    paint_position(0.0, 0.0);
    rocket->refresh_position();
}
