#include <QPainter>
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

    }else{
        if (serverHeight == 0.0 & serverWidth == 0.0){
            Wscale = 1.0;
            Hscale = 1.0;
        }
    }

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
        flame << QPointF(-10, 5)  // Левый край огня
              << QPointF(10, 5)   // Правый край огня
              << QPointF(0, 20); // Нижняя точка огня (самый низ пламени)
        painter->drawPolygon(flame);

        painter->setBrush(Qt::yellow);
        QPolygonF innerFlame;
        innerFlame << QPointF(-6, 6)  // Левый край внутреннего огня
                   << QPointF(6, 6)   // Правый край внутреннего огня
                   << QPointF(0, 15); // Нижняя точка внутреннего огня
        painter->drawPolygon(innerFlame);
    }
}

void RocketSceneFinal::setPositionUpdate(int x, int y){
    this->x = x;
    this->y = y;
    update();

}

void RocketSceneFinal::setRotationUpdate(double uhel){
    this->rotation = uhel;
    update();

}

//void RocketSceneFinal::setSceneDimensions(int width, int height){
//    this->sceneWidth = width;
//    this->sceneHeight = height;
//    setFixedSize();

//}

QSize RocketSceneFinal::sizeHint() const {
    return QSize(1000, 600); // Укажите подходящие размеры
}

void RocketSceneFinal::resizeEvent(QResizeEvent *event){
    double new_aspect = static_cast<double>(height())/static_cast<double>(width());
    if (new_aspect != 0.6){
        if (new_aspect>0.6)
        {
            resize(width(), static_cast<double>(width())*aspectratio);

        }else{
            resize(static_cast<double>(height())* (1/aspectratio), height());
        }
        

    }



    update();

}