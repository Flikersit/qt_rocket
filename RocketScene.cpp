#include "RocketScene.h"
#include <QGraphicsRectItem>

RocketScene::RocketScene(){
    height = 600;
    width = 800;
    launchPadOffSet = 50;

    setBackgroundBrush(Qt::black);
    setSceneRect(-width/2, height, width, height);
}

void RocketScene::paint_platforms(){

    QGraphicsRectItem *rect_1 = new QGraphicsRectItem(0, 0, 5, 10);
    QGraphicsRectItem *rect_2 = new QGraphicsRectItem(0, 0, 5, 10);

    rect_1->setBrush(QColor(165, 42, 42));
    rect_2->setBrush(QColor(165, 42, 42));

    rect_1->setPos(launchPadOffSet - width/2, 0);
    rect_2->setPos(width/2 - launchPadOffSet, 0);

    QGraphicsRectItem *rect_3 = new QGraphicsRectItem(0, 0, 7, 4);
    QGraphicsRectItem *rect_4 = new QGraphicsRectItem(0, 0, 7, 4);

    rect_3->setBrush(Qt::gray);
    rect_4->setBrush(Qt::gray);

    rect_3->setPos(launchPadOffSet - width/2, 10);
    rect_4->setPos(width/2 - launchPadOffSet, 10);

    addItem(rect_1);
    addItem(rect_2);
    addItem(rect_3);
    addItem(rect_4);
    
}

void RocketScene::reset(){
    paint_platforms();
}
