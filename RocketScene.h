#ifndef ROCKETSCENE_H
#define ROCKETSCENE_H

#include <QGraphicsScene>
#include <QtGUI>

class RocketScene: public QGraphicsScene{

    public:
        RocketScene();

        void paint_platforms();

        int launchPadOffSet;

        void reset();

    private:
        int height;
        int width;
}

#endif