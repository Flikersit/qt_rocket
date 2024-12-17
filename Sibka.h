#ifndef SIBKA_H
#define SIBKA_H

#include <QGraphicsScene>
#include <QtGUI>

class Sibka: public QGraphicsScene{
    public:
        Sibka();
        void begin_draw();
        QGraphicsPolygonItem *arrowItem;
};


#endif
