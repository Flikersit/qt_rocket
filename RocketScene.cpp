#include "RocketScene.h"
#include <QGraphicsRectItem>

RocketScene::RocketScene(){
    height = 1000;
    width = 1200;
    launchPadOffSet = 50;

    setBackgroundBrush(Qt::black);
    setSceneRect(0, 0, width, height);
}

RocketScene::paint_platforms(){

    QGraphicsRectItem *rect_1 = new QGraphicsRectItem(0, 0, 5, 10);
    QGraphicsRectItem *rect_2 = new QGraphicsRectItem(0, 0, 5, 10);

    rect_1->setBrush(Qt::brown);
    rect_2->setBrush(Qt::brown);


    rect_1->setPos(launchPadOffSet, 0);
    rect_2->setPos(width-launchPadOffSet, 0);

    QGraphicsRectItem *rect_3 = new QGraphicsRectItem(0, 0, 7, 4);
    QGraphicsRectItem *rect_4 = new QGraphicsRectItem(0, 0, 7, 4);

    rect_3->setBrush(Qt::gray);
    rect_4->setBrush(Qt::gray);

    rect_3->setPos(width-launchPadOffSet, 10);
    rect_4->setPos(width-launchPadOffSet, 10);

    addItem(rect_1);
    addItem(rect_2);
    addItem(rect_3);
    addItem(rect_4);

}

RocketScene::reset(){
    paint_platforms()
    
}

