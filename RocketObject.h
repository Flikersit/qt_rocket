#ifndef ROCKETOBJECT_H
#define ROCKETOBJECT_H

#include <QtGui>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class RocketObject : public QGraphicsItem
{

    public:
        RocketObject();

        QRectF boundingRect() const override;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

        void refresh_position();

        qreal x;
        qreal y;

        double rotation;
        double main_engine;

        bool leftEngine;
        bool rightEngine;


        bool isconnected;
        bool in_platform = true;



};

#endif
