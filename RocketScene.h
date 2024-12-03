#ifndef ROCKETSCENE_H
#define ROCKETSCENE_H

#include <QGraphicsScene>
#include <QtGUI>

class RocketScene: public QGraphicsScene{

    Q_OBJECT

public:
    RocketScene();

    void paint_platforms();

    void reset();

    int launchPadOffSet;

private:
    int height;
    int width;
};

#endif
