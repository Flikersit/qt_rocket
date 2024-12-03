#include <QtGui>
#include <QWidget>
#include <QTimer>
#include <QNetworkReply>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include "./RocketObject.h"
#include "./RocketScene.h"


class ControlPanel: public QWidget{
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr, RocketObject *rocket = nullptr, RocketScene *scene = nullptr);
    void resetSimulation();


    RocketObject *rocket;
    RocketScene *scene;

    QNetworkAccessManager* networkManager;

    QSlider *thrustSlider;    //

    QPushButton *leftButton; // Left engine
    QPushButton *rightButton; // Right engine
    QPushButton *resetButton; // Restart 

    QLabel *statusLabel;      // Status display
    QLabel *connection;      // connection label
    
    QTimer timer;

public slots:
        void requestData();
        void onDataReceived();
        void onError(QNetworkReply::NetworkError code);


protected:
    void keyPressEvent(QKeyEvent *event) override;
};
