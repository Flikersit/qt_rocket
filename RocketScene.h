#ifndef ROCKETSCENE_H
#define ROCKETSCENE_H

#include <QGraphicsScene>
#include <QtGUI>
#include "./RocketObject.h"

class RocketScene: public QGraphicsScene{

    Q_OBJECT

public:
    RocketScene(RocketObject *rocket = nullptr);

    void paint_begin_position();

    void reset();

    int launchPadOffSet;

    RocketObject *rocket;

private:
    int height;
    int width;
};

#endif
