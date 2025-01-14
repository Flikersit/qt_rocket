#include "Sibka.h"
#include <QPolygonF>
#include <QVBoxLayout>
#include <QTransform>
#include <QDebug>

Sibka::Sibka(QWidget *parent) : QWidget(parent) {
    // 
    scene = new QGraphicsScene(this);
    
    scene->setSceneRect(-150, -150, 300, 300); // 

    arrowLine = new QGraphicsLineItem(0, 0, 0, -50);
    arrowLine->setPen(QPen(Qt::red, 3));
    scene->addItem(arrowLine);

    // 
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFixedSize(300, 300); // 
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setBackgroundBrush(Qt::black);

    qDebug() << "Scene and arrow created, positioned at center.";
}

void Sibka::update_arrow(qreal angle, qreal speed) {
    // 
    qreal length = std::min(150.0, speed); // 
    qreal radian = qDegreesToRadians(angle);

    // 
    qreal x = length * sin(radian);
    qreal y = -length * cos(radian); // 

    arrowLine->setLine(0, 0, -x, y);

    qDebug() << "Arrow updated: angle =" << angle << "degrees, speed =" << speed;
}
