#include "RocketScene.h"
#include <QGraphicsRectItem>
#include <QTransform>

RocketScene::RocketScene(){
    height = 600;
    width = 800;
    launchPadOffSet = 50;

    setBackgroundBrush(Qt::black);
    setSceneRect(-width/2, -height, width, height);//it works dont touch
}

void RocketScene::paint_platforms(){

    QGraphicsRectItem *rect_1 = new QGraphicsRectItem(0, 0, 20, 40);
    QGraphicsRectItem *rect_2 = new QGraphicsRectItem(0, 0, 20, 40);

    rect_1->setBrush(QColor(165, 42, 42));
    rect_2->setBrush(QColor(165, 42, 42));
    rect_1->setBrush(Qt::white);
    rect_2->setBrush(Qt::white);

    rect_1->setPos(launchPadOffSet - width/2, -height);
    rect_2->setPos(width/2 - launchPadOffSet, 0);

    QGraphicsRectItem *rect_3 = new QGraphicsRectItem(0, 0, 50, 10);
    QGraphicsRectItem *rect_4 = new QGraphicsRectItem(0, 0, 50, 10);

    rect_3->setBrush(Qt::white);
    rect_4->setBrush(Qt::white);

    rect_3->setPos(launchPadOffSet - width/2, 40);
    rect_4->setPos(width/2 - launchPadOffSet, 40);


    addItem(rect_1);
    addItem(rect_2);
    addItem(rect_3);
    addItem(rect_4);

    qDebug() << "Item's in scene" << items().size();

}

void RocketScene::reset(){
    clear();
    paint_platforms();
}
