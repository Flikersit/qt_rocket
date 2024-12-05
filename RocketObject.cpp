#include "RocketObject.h"


RocketObject::RocketObject(){
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF RocketObject::boundingRect() const{
    return QRectF(-12, 0, 24, 60);
}

void RocketObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->scale(1, -1);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->setBrush(Qt::red);
    painter->drawRect(-12, -24, 24, 48);

    QPolygonF nose;
    nose << QPointF(-12, -24) << QPointF(12, -24) << QPointF(0, -36);
    painter->setBrush(Qt::gray);
    painter->drawPolygon(nose);

    painter->setBrush(Qt::yellow);
    painter->drawRect(-10, 24, 6, 10);
    painter->drawRect(4, 24, 6, 10);

    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(QPointF(0, 30), 4, 6);
}

void RocketObject::refresh_position(){
    x = this->scenePos().x();
    y = this->scenePos().y();

}

