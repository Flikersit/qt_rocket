#include "Sibka.h"
#include <QPolygonF>
#include <QVBoxLayout>
#include <QTransform>
#include <QDebug>

Sibka::Sibka(QWidget *parent) : QWidget(parent) {
    // Создаем сцену
    scene = new QGraphicsScene(this);
    
    scene->setSceneRect(-150, -150, 300, 300); // Центр сцены в (0, 0)

    arrowLine = new QGraphicsLineItem(0, 0, 0, -50);
    arrowLine->setPen(QPen(Qt::red, 3));
    scene->addItem(arrowLine);

    // Создаем виджет для отображения сцены
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setFixedSize(300, 300); // Размер окна
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setBackgroundBrush(Qt::black);

    qDebug() << "Scene and arrow created, positioned at center.";
}

void Sibka::update_arrow(qreal angle, qreal speed) {
    // Поворачиваем стрелку и изменяем её длину
    qreal length = std::min(150.0, speed); // Ограничиваем максимальную длину 150
    qreal radian = qDegreesToRadians(angle);

    // Конечные координаты линии на основе угла и длины
    qreal x = length * sin(radian);
    qreal y = -length * cos(radian); // Минус, так как ось Y направлена вниз

    arrowLine->setLine(0, 0, -x, y);

    qDebug() << "Arrow updated: angle =" << angle << "degrees, speed =" << speed;
}
