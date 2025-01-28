#include <QtGui>
#include <QWidget>
#include <QTimer>
#include <QNetworkReply>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>
#include "./RocketSceneFinal.h"
#include "./Sibka.h"


class ControlPanel: public QWidget{
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr, RocketSceneFinal *scene = nullptr);
    void resetSim();
    void resizeEvent(QResizeEvent *event) override;

    RocketSceneFinal *scene;
    Sibka *sibka;

    QNetworkAccessManager* networkManager;

    QSlider *thrustSlider;    //

    QPushButton *resetButton; // Restart

    QCheckBox *checkbox_left; 
    QCheckBox *checkbox_right;

    QLabel *check_label;

    int last_h;
    int last_w;

    QLabel *statusLabel;      // Status display
    QLabel *connection;      // connection label
    QLabel *xposition;
    QLabel *yposition;
    QLabel *rotation;

    QLabel *labelWidth;
    QLabel *labelHeight;
    
    QLineEdit *inputWidth;
    QLineEdit *inputHeight;

    QBarSet *vSet;

    QBarSeries *series;
    QChart *chart;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;

    QTimer timer;

    bool reset;
    bool vykreslit;

public slots:
        void ontimeout();
        void requestData();
        void onDataReceived();
        void post_main_engine();
        void post_left_thruster();
        void post_right_thruster();
        void post_reset();
        void post_width();
        void post_height();
        void onError(QNetworkReply::NetworkError code);
        void left_engine_change();
        void right_engine_change();
        void main_changed();
        void update_sizeW();
        void update_sizeH();


protected:
    void keyPressEvent(QKeyEvent *event) override;

};
