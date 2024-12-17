#include "Sibka.h"
#include <QGraphicsRectItem>
#include <QTransform>
#include <QDebug>

Sibka::Sibka(){
    QPolygonF arrow;
    arrow << QPointF(0, 20)
          << QPointF(-10, 0)
          << QPointF(-5, 0)
          << QPointF(-5, -20)
          << QPointF(5, -20)
          << QPointF(5, 0)
          << QPointF(10, 0);
        
    arrowItem = new QGraphicsPolygonItem(arrow);

}

void Sibka::begin_draw(){

}
