#include <QtGui>
#include <QWidget>
#include <QTimer>
#include <QNetworkReply>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include "./RocketObject.h"
#include "./RocketScene.h"


class ControlPanel: public QWidget{
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr, RocketScene *scene = nullptr);
    void resetSimulation();

    RocketScene *scene;

    QNetworkAccessManager* networkManager;

    QSlider *thrustSlider;    //

    QPushButton *resetButton; // Restart

    QCheckBox *checkbox_left; 
    QCheckBox *checkbox_right;

    QLabel *check_label;

    QLabel *statusLabel;      // Status display
    QLabel *connection;      // connection label
    QLabel *xposition;
    QLabel *yposition;
    QLabel *rotation;

    QBarSet *vxSet;
    QBarSet *vySet;
    QBarSeries *series;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QBarCategoryAxis *axisY;

    QTimer timer;

    bool reset;

public slots:
        void requestData();
        void onDataReceived();
        void post_data();
        void onError(QNetworkReply::NetworkError code);
        void left_engine_change();
        void right_engine_change();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

};
