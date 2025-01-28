#include <QPainter>
#include <QSizePolicy>
#include "RocketSceneFinal.h"

RocketSceneFinal::RocketSceneFinal(QWidget *parent){
    aspectratio = 0.6;
    launchPadOffSet = 30;
    rotation = 0.0;
    x = -width()/2 + launchPadOffSet;
    y = 5;
    isconnected = false;
    serverHeight = 0.0;
    serverWidth = 0.0;
    in_air = false;
    this->setMinimumSize(100, 100);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    update();

}

void RocketSceneFinal::paintEvent(QPaintEvent *evnt){
    if (serverHeight !=0.0 & serverWidth !=0.0){
        Hscale = static_cast<double>(height())/serverHeight;
        Wscale = static_cast<double>(width())/serverWidth;
    }else{
        Wscale = 1.0;
        Hscale = 1.0;
    }
    QPainter painter(this);
    painter.translate(width()/2, height());
    painter.scale(1, -1);

    painter.setBrush(Qt::black);
    painter.drawRect(-width()/2, 0, width(), height());

    painter.setBrush(Qt::blue);
    painter.drawRect(-width()/2 + launchPadOffSet*Wscale, 10, 50, 10);
    painter.drawRect(width()/2 - launchPadOffSet*Wscale - 50, 10, 50, 10);

    painter.save();

    if(in_platform & serverHeight == 0.0 & serverWidth == 0.0){
        x = -width()/2 + launchPadOffSet;
        qDebug()<< "disconnect";

    }else{
        if (serverHeight == 0.0 & serverWidth == 0.0){
            qDebug()<<"Wscale=1";
            Wscale = 1.0;
            Hscale = 1.0;
        }
    }
    qDebug()<<"X"<<x;
    painter.translate(x*Wscale + 25, y*Hscale);
    painter.translate(0, 42);
    painter.rotate(rotation);
    painter.translate(0, -22);
    paintRocket(&painter);

    painter.restore();



}

void RocketSceneFinal::paintRocket(QPainter *painter) {

    // Рисуем основное тело
    painter->scale(1, -1);
    painter->setBrush(Qt::red);
    painter->drawRect(-12, -48, 24, 48);

    // Рисуем нос ракеты
    QPolygonF nose;
    nose << QPointF(-12, -48) << QPointF(12, -48) << QPointF(0, -68);
    painter->setBrush(Qt::gray);
    painter->drawPolygon(nose);

    // Рисуем двигатели
    painter->setBrush(Qt::yellow);
    painter->drawRect(-10, 0, 6, 6);
    painter->drawRect(4, 0, 6, 6);

    // Дополнительные элементы
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(QPointF(0, 5), 4, 6);

    if (in_air) {
        // Цвет огня
        painter->setBrush(QColor(255, 165, 0));
        QPolygonF flame;
        flame << QPointF(-10, 5) 
              << QPointF(10, 5)   
              << QPointF(0, 20); 
        painter->drawPolygon(flame);

        painter->setBrush(Qt::yellow);
        QPolygonF innerFlame;
        innerFlame << QPointF(-6, 6)
                   << QPointF(6, 6)   
                   << QPointF(0, 15); 
        painter->drawPolygon(innerFlame);
    }
}

void RocketSceneFinal::setPositionUpdate(int x, int y){
    this->x = x;
    this->y = y;
    updateGeometry();
    update();

}


QSize RocketSceneFinal::sizeHint() const {
    return QSize(serverWidth, serverHeight); 
}
