#include "RocketScene.h"
#include <QGraphicsRectItem>
#include <QTransform>
#include <QDebug>

RocketScene::RocketScene(RocketObject *rocket){
    this->rocket = rocket;

    height = 600.0;
    width = 1000.0;
    launchPadOffSet = 100.0;
    aspectRatio = static_cast<double>(width)/height;

    rect_3 = new QGraphicsRectItem(0, 0, 50, 10);
    rect_4 = new QGraphicsRectItem(0, 0, 50, 10);
    rect_5 = new QGraphicsRectItem(0, 0, 40, 40);

    QPolygonF arrow;
    arrow << QPointF(0, 20)
          << QPointF(-10, 0)
          << QPointF(-5, 0)
          << QPointF(-5, -20)
          << QPointF(5, -20)
          << QPointF(5, 0)
          << QPointF(10, 0);

    arrowItem = new QGraphicsPolygonItem(arrow);


    setBackgroundBrush(Qt::black);
    setSceneRect(-(width/2), 0, width, height);//it works dont touch
    
    paint_begin();

    


}

void RocketScene::paint_begin(){
    setSceneRect(-(width/2), 0, width, height);


    rect_3->setBrush(Qt::blue);
    rect_4->setBrush(Qt::blue);

    rect_3->setPos(launchPadOffSet - width/2, 10);
    rect_4->setPos(width/2 - launchPadOffSet -50, 10);

    //rect_5->setBrush(Qt::white);
    //rect_5->setPos(-20, -20);
    arrowItem->setBrush(Qt::white);
    arrowItem->setPos(-10, 20);

    rocket->setPos(launchPadOffSet - width/2 + 25 , 46);

    addItem(arrowItem);
    addItem(rect_3);
    addItem(rect_4);
    //addItem(rect_5);
    addItem(rocket);

    qDebug() << "Item's in scene" << items().size();

}


void RocketScene::paint_position(double x, double y){
    qDebug() << "Rocket disappear(";
    rocket->setPos(x-43, y+46);
    rocket->setRotation(round(((rocket->rotation)*(180.0/M_PI))*100)/100);
    this->update();
}
