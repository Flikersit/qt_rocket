#ifndef ROCKETSCENE_H
#define ROCKETSCENE_H

#include <QGraphicsScene>
#include <QtGUI>
#include "./RocketObject.h"

class RocketScene: public QGraphicsScene{

    Q_OBJECT

public:
    RocketScene(RocketObject *rocket = nullptr);

    void paint_position(double x, double y);
    void paint_begin();


    double launchPadOffSet;

    RocketObject *rocket;

    double height;
    double width;
    double aspectRatio;
    bool resetSim = false;

    QGraphicsRectItem *rect_3;
    QGraphicsRectItem *rect_4;
    QGraphicsRectItem *rect_5;
    QGraphicsPolygonItem *arrowItem;

};

#endif
