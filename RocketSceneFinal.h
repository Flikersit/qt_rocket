#pragma once
#include <QtGui>
#include <QWidget>
#include <QTimer>

class RocketSceneFinal: public QWidget
{
    Q_OBJECT

public:
    RocketSceneFinal(QWidget *parent = 0);
    void paintRocket(QPainter *painter, double scaleX, double scaleY);
    void setPositionUpdate(int x, int y);
    //void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const;

    int launchPadOffSet;
    int defaultHeight;
    int defaultWidth;

    double x;
    double y;
    double rotation;
    double main_engine;

    double real_width;
    double real_height;
    double serverWidth;
    double serverHeight;
    double lastWidth;
    double lastHeight;

    int sceneWidth;
    int sceneHeight;

    double Wscale;
    double Hscale;

    double aspectratio;

    bool in_air;


    bool isconnected;

    bool rightEngine;
    bool leftEngine;
    
    bool in_platform = true;


protected:
    void paintEvent(QPaintEvent *event) override;


};
