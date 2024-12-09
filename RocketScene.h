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

    void reset();

    double launchPadOffSet;

    RocketObject *rocket;

    double height;
    double width;

};

#endif
