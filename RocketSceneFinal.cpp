#include <QPainter>
#include <QSizePolicy>
#include "RocketSceneFinal.h"

RocketSceneFinal::RocketSceneFinal(QWidget *parent){
    aspectratio = 0.6;
    launchPadOffSet = 30;
    rotation = 0.0;
    defaultWidth = 400;
    defaultHeight = 300;
    x = -width()/2 + launchPadOffSet;
    y = 5;
    isconnected = false;
    serverHeight = 0.0;
    serverWidth = 0.0;
    real_width = 0.0;
    real_height = 0.0;
    in_air = false;
    this->setMinimumSize(100, 100);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    lastHeight = 0.0;
    lastWidth = 0.0;
    update();

}

void RocketSceneFinal::paintEvent(QPaintEvent *evnt){
    if (serverWidth > 0 && serverHeight > 0) {
    aspectratio = static_cast<double>(serverHeight) / serverWidth;
    double widgetAspectRatio = static_cast<double>(height()) / width();

    if (widgetAspectRatio > aspectratio) {
        
        real_width = width();
        real_height = real_width * aspectratio;
    } else {
        
        real_height = height();
        real_width = real_height / aspectratio;
    }
}else {
    real_width = width();
    real_height = height();
}

    double scaleX = real_width/300;
    double scaleY = real_height/300;


    int sceneWidth = (serverWidth > 0) ? serverWidth : defaultWidth;
    int sceneHeight = (serverHeight > 0) ? serverHeight : defaultHeight;
    if (serverWidth != lastWidth || serverHeight != lastHeight) {
    lastWidth = serverWidth;
    lastHeight = serverHeight;
    }

    if (serverHeight !=0.0 & serverWidth !=0.0){
        Hscale = real_height/serverHeight;
        Wscale = real_width/serverWidth;
    }else{
        Wscale = 1.0;
        Hscale = 1.0;
    }
    QPainter painter(this);
    painter.translate(width()/2, height());
    painter.scale(1, -1);
    painter.setBrush(Qt::green);
    if (width() > real_width) {
        int sideMargin = (width() - real_width) / 2;
        painter.drawRect(-width()/2, 0, sideMargin, height());  // слева
        painter.drawRect(real_width/2, 0, sideMargin, height()); // справа
    }

// Верхняя и нижняя горизонтальные полосы
    if (height() > real_height) {
        int topBottomMargin = (height() - real_height);
        painter.drawRect(-width()/2, real_height, width(), topBottomMargin );
    }
    painter.setBrush(Qt::black);
    painter.drawRect(-real_width/2, 0, real_width, real_height);

    painter.setBrush(Qt::blue);
    painter.drawRect(-real_width/2 + launchPadOffSet*Wscale, 10, 50*scaleX, 10*scaleY);
    painter.drawRect(real_width/2 - launchPadOffSet*Wscale - 50*scaleX, 10, 50*scaleX, 10*scaleY);

    painter.save();

    if(in_platform & serverHeight == 0.0 & serverWidth == 0.0){
        x = -real_width/2 + launchPadOffSet;
        qDebug()<< "disconnect";

    }else{
        if (serverHeight == 0.0 & serverWidth == 0.0){
            qDebug()<<"Wscale=1";
            Wscale = 1.0;
            Hscale = 1.0;
        }
    }
    qDebug()<<"Wscale: "<< Wscale;
    painter.translate(x*Wscale + 25*scaleX, y*Hscale + 5*Hscale);
    painter.translate(0, 42*scaleY);
    painter.rotate(rotation);
    painter.translate(0, -22*scaleY);
    paintRocket(&painter, scaleX, scaleY);

    painter.restore();



}

void RocketSceneFinal::paintRocket(QPainter *painter, double scaleX, double scaleY) {

    // Рисуем основное тело
    painter->scale(1, -1);
    painter->setBrush(Qt::red);
    double bodyW = 12 * scaleX;
    double bodyH = 48 * scaleY;
    painter->drawRect(-bodyW, -bodyH, 2 * bodyW, bodyH);
    //painter->drawRect(-12, -48, 24, 48);

    // Рисуем нос ракеты
    QPolygonF nose;
    nose << QPointF(-bodyW, -bodyH) << QPointF(bodyW, -bodyH) << QPointF(0, -bodyH - 20 * scaleY);
    painter->setBrush(Qt::gray);
    painter->drawPolygon(nose);

    // Рисуем двигатели



    double engineW = 6 * scaleX;
    double engineH = 6 * scaleY;
    painter->setBrush(Qt::yellow);
    painter->drawRect(-10 * scaleX, 0, engineW, engineH);
    painter->drawRect(4 * scaleX, 0, engineW, engineH);

    //painter->setBrush(Qt::yellow);
    //painter->drawRect(-10, 0, 6, 6);
    //painter->drawRect(4, 0, 6, 6);

    // Дополнительные элементы

    //painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(QPointF(0, 5), 4, 6);

    //if (in_air) {
        // Цвет огня
    //    painter->setBrush(QColor(255, 165, 0));
    //    QPolygonF flame;
    //    flame << QPointF(-10, 5) 
    //          << QPointF(10, 5)   
    //          << QPointF(0, 20); 
    //    painter->drawPolygon(flame);

    //    painter->setBrush(Qt::yellow);
    //    QPolygonF innerFlame;
    //    innerFlame << QPointF(-6, 6)
    //               << QPointF(6, 6)   
    //               << QPointF(0, 15); 
    //    painter->drawPolygon(innerFlame);
    //}


    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(QPointF(0, 5 * scaleX), 4 * scaleX, 6 * scaleY);

    // Огонь если в воздухе
    if (in_air) {
        painter->setBrush(QColor(255, 165, 0)); // оранжевый
        QPolygonF flame;
        flame << QPointF(-10 * scaleX, 5 * scaleY)
              << QPointF(10 * scaleX, 5 * scaleY)
              << QPointF(0, 20 * scaleY);
        painter->drawPolygon(flame);

        painter->setBrush(Qt::yellow);
        QPolygonF innerFlame;
        innerFlame << QPointF(-6 * scaleX, 6 * scaleY)
                   << QPointF(6 * scaleX, 6 * scaleY)
                   << QPointF(0, 15 * scaleY);
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
