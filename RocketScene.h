#ifndef ROCKETSCENE_H
#define ROCKETSCENE_H

#include <QGraphicsScene>
#include <QtGUI>

class RocketScene: public QGraphicsScene{

    public:
        RocketScene();

        void paint_platforms(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

        int launchPadOffSet;

    private:
        int height;
        int width;
}

#endif
