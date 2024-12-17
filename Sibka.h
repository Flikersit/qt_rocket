#ifndef SIBKA_H
#define SIBKA_H

#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class Sibka : public QWidget {
    Q_OBJECT

public:

    Sibka(QWidget *parent = nullptr);
    void update_arrow(qreal angle, qreal factor);

    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPolygonItem *arrowItem;
    QGraphicsLineItem * arrowLine; 

};

#endif // SIBKA_H
