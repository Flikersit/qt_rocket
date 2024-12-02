
#include <QtGui>
#include <QWidget>
#include <QTimer>
#include "./RocketObject.h"
#include "./RocketScene.h"


class ControlPanel : public QWidget {
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr, RocketObject *rocket, RocketScene *scene);
    RocketObject *rocket;
    RocketScene *scene;

signals:
    void thrustChanged(double thrust);
    void leftThrusterActivated(bool active);
    void rightThrusterActivated(bool active);
    void resetSimulation();

private:
    QSlider *thrustSlider;    // 
    QPushButton *leftButton; // Left engine
    QPushButton *rightButton; // Right engine
    QPushButton *resetButton; // Restart 
    QLabel *statusLabel;      // Status display
    

protected:
    void keyPressEvent(QKeyEvent *event) override;
};
