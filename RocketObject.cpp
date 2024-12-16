#include "RocketObject.h"


RocketObject::RocketObject(){
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    this->main_engine=0.0;
    this->isconnected = false;
    this->leftEngine = false;
    this->rightEngine = false;
    this->rotation = 0.0;

    setTransformOriginPoint(0, 15);
}

QRectF RocketObject::boundingRect() const{
    return QRectF(-12, -15, 24, 48);
}

void RocketObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->scale(1,-1);

    // Основное тело ракеты
    painter->setBrush(Qt::red);
    painter->drawRect(-12, -33, 24, 48); // Тело ракеты (верх на -33, низ на 15)

    // Нос ракеты
    QPolygonF nose;
    nose << QPointF(-12, -33) << QPointF(12, -33) << QPointF(0, -45);
    painter->setBrush(Qt::gray);
    painter->drawPolygon(nose);

    painter->setBrush(Qt::yellow);
    painter->drawRect(-10, 15, 6, 10);
    painter->drawRect(4, 15, 6, 10);

    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(QPointF(0, 20), 4, 6);

}

void RocketObject::refresh_position(){
    x = this->scenePos().x();
    y = this->scenePos().y();

}

