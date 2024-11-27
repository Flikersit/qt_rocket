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

};

#endif
