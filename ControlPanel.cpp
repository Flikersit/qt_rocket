#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QGraphicsView>
#include <QTransform>
#include <QVariant>
#include <QMainWindow>
#include <cmath>
#include <math.h>
#include "ControlPanel.h"



ControlPanel::ControlPanel(QWidget *parent, RocketSceneFinal *scene){

    this->scene = scene;
    reset = false;

    //reset button + scene sizw
    resetButton = new QPushButton("Reset");
    labelWidth = new QLabel("Scene width");
    labelHeight = new QLabel("Scene height");
    inputWidth = new QLineEdit;
    inputHeight = new QLineEdit;
    QIntValidator *validator = new QIntValidator(100, 1500);
    inputHeight->setValidator(validator);
    inputWidth->setValidator(validator);
    inputHeight->setText(QString::number(scene->height()));
    inputWidth->setText(QString::number(scene->width()));
    inputHeight->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    inputWidth->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    labelHeight->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    labelWidth->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");

    QVBoxLayout *scenelayout = new QVBoxLayout;
    scenelayout->addWidget(resetButton);
    scenelayout->addWidget(labelWidth);
    scenelayout->addWidget(inputWidth);
    scenelayout->addWidget(labelHeight);
    scenelayout->addWidget(inputHeight);

    QWidget *scenerozmer = new QWidget();
    scenerozmer->setLayout(scenelayout);
    

    //switch left/right
    QWidget *check_widget = new QWidget();
    QVBoxLayout *check_box_layout = new QVBoxLayout(this);
    check_label = new QLabel("Switch enginees");
    checkbox_left = new QCheckBox("Left engine");
    checkbox_right = new QCheckBox("Right engine");

    check_label->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    checkbox_left->setStyleSheet("padding: 3px;");
    checkbox_right->setStyleSheet("padding: 3px;");


    check_box_layout->addWidget(check_label);
    check_box_layout->addWidget(checkbox_left);
    check_box_layout->addWidget(checkbox_right);
    check_widget->setLayout(check_box_layout);

    //status
    QWidget *status_widget = new QWidget();
    QVBoxLayout *statusbox = new QVBoxLayout(this);
    statusLabel = new QLabel("Rocket state: ");
    connection = new QLabel("Connection state: " + QVariant(scene->isconnected).toString() );
    connection->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    statusLabel->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    statusbox->addWidget(connection);
    statusbox->addWidget(statusLabel);
    status_widget->setLayout(statusbox);
    statusLabel->setText("Rocket state: Touchdown");


    //Slider for the main enginee
    QWidget *widget_for_slider = new QWidget();            
    QVBoxLayout *sliderbox = new QVBoxLayout(this);
    QLabel *label_for_slider = new QLabel("Thrust Slider");
    label_for_slider->setAlignment(Qt::AlignCenter);
    label_for_slider->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    thrustSlider = new QSlider(Qt::Horizontal, this);
    thrustSlider->setMinimum(0);
    thrustSlider->setMaximum(20);
    thrustSlider->setValue(0);
    thrustSlider->setSingleStep(1);
    sliderbox->addWidget(label_for_slider);
    sliderbox->addWidget(thrustSlider);
    widget_for_slider->setLayout(sliderbox);


    //position
    QWidget *coordinate_widget = new QWidget(); 
    QVBoxLayout *coordinate_layout = new QVBoxLayout(this);
    xposition = new QLabel("X position: " +  QString::number(scene->x));
    yposition = new QLabel("Y position: " +  QString::number(scene->y));
    rotation = new QLabel("Rotation: 0");
    xposition->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    yposition->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    rotation->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");

    coordinate_layout->addWidget(xposition);
    coordinate_layout->addWidget(yposition);
    coordinate_layout->addWidget(rotation);
    coordinate_widget->setLayout(coordinate_layout);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *vlayout = new QVBoxLayout(this);

    //Taying to create Bar: Begin
    vSet = new QBarSet("V");
    *vSet << 0 << 0;
    series = new QBarSeries();
    series->append(vSet);
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Speed Graph");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    axisX = new QBarCategoryAxis();
    axisX->append("VX");
    axisX->append("VY");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisY = new QValueAxis();
    axisY->setRange(0, 20);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //


    //for arrow
    sibka = new Sibka();
    sibka->update_arrow(0, 0);



    layout->addWidget(scenerozmer);
    layout->addWidget(status_widget);
    layout->addWidget(coordinate_widget);
    layout->addWidget(check_widget);
    layout->addWidget(widget_for_slider);
    layout->addWidget(chartView);
    layout->addWidget(sibka->view);

    QWidget *hwidget = new QWidget();
    hwidget->setLayout(layout);
    vlayout->addWidget(hwidget);

    vlayout->addWidget(scene);

    setLayout(vlayout);

    networkManager = new QNetworkAccessManager(this);
    

    connect(resetButton, &QPushButton::released, this, &ControlPanel::resetSim);
    connect(&timer, &QTimer::timeout, this, &ControlPanel::requestData);

    connect(checkbox_left, &QCheckBox::stateChanged, this, &ControlPanel::left_engine_change);
    connect(checkbox_right, &QCheckBox::stateChanged, this, &ControlPanel::right_engine_change);
    connect(thrustSlider, &QSlider::valueChanged, this, &ControlPanel::main_changed);
    connect(inputWidth, &QLineEdit::textChanged, this, &ControlPanel::update_size);
    connect(inputHeight, &QLineEdit::textChanged, this, &ControlPanel::update_size);


    timer.start(1000);
}


void ControlPanel::requestData(){
    QUrl url = QUrl("http://localhost:8008/api/tasks/rocket/Rocket?data&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);

	QNetworkReply* reply = networkManager->get(request);

	connect(reply, &QNetworkReply::errorOccurred, this, &ControlPanel::onError);
	connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::onDataReceived);
}



void ControlPanel::onDataReceived(){
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	QByteArray data = reply->readAll();
	QString str(data);
	qDebug() << "Geted data from server " <<str;

	QJsonDocument jDoc = QJsonDocument::fromJson(data);
	QJsonObject jResponse = jDoc.object();
    if (!jResponse.contains("subitems")){
        qDebug()<<"Do not consist subitems";
    }

    double height = jResponse["subitems"].toArray()[5].toObject()["Height"].toObject()["v"].toDouble();
    double width = jResponse["subitems"].toArray()[4].toObject()["Width"].toObject()["v"].toDouble();
    double rotation = jResponse["subitems"].toArray()[10].toObject()["Rotation"].toObject()["v"].toDouble();

    bool left_engine = jResponse["subitems"].toArray()[1].toObject()["LeftThruster"].toObject()["v"].toBool();
    bool right_engine = jResponse["subitems"].toArray()[2].toObject()["RightThruster"].toObject()["v"].toBool();
    double main_engine = jResponse["subitems"].toArray()[0].toObject()["EngineThrottle"].toObject()["v"].toDouble();

    double x_position = jResponse["subitems"].toArray()[6].toObject()["X"].toObject()["v"].toDouble();
    double y_position = jResponse["subitems"].toArray()[7].toObject()["Y"].toObject()["v"].toDouble();

    double lounch_pad = jResponse["subitems"].toArray()[13].toObject()["LaunchpadOffset"].toObject()["v"].toDouble();

    double vx = jResponse["subitems"].toArray()[8].toObject()["VX"].toObject()["v"].toDouble();
    double vy = jResponse["subitems"].toArray()[9].toObject()["VY"].toObject()["v"].toDouble();

    bool touchdown = jResponse["subitems"].toArray()[11].toObject()["Touchdown"].toObject()["v"].toBool();
    bool crashed = jResponse["subitems"].toArray()[12].toObject()["Crashed"].toObject()["v"].toBool();

    bool reset = jResponse["subitems"].toArray()[3].toObject()["Reset"].toObject()["v"].toBool();

    qDebug()<<"Ryclost po ose x GRAF"<< vx;
    vSet->replace(0, abs(vx));
    vSet->replace(1, abs(vy));
    chart->update();

    scene->main_engine = main_engine;
    scene->isconnected = true;
    scene->x = x_position;
    scene->y = y_position;
    scene->rotation = rotation*(((180.0/M_PI))*100)/100;

    xposition->setText("X position: " + QString::number(round((scene->x)*100)/100));
    yposition->setText("Y position: " + QString::number(round((scene->y)*100)/100));
    this->rotation->setText("Rotation: " + QString::number(round(((scene->rotation))*100)/100));

    if(touchdown){
        statusLabel->setText("Rocket state: Touchdown");
        scene->in_air = false;
    }else if(crashed){
        statusLabel->setText("Rocket state: Crashed");
        scene->in_platform = false;
        scene->in_air = false;
    }else{
        statusLabel->setText("Rocket state: In air");
        scene->in_platform = false;
        scene->in_air = true;
    }
    connection->setText("Connection state: " + QVariant(scene->isconnected).toString() );

    scene->serverWidth = width;
    scene->serverHeight = height;
    scene->launchPadOffSet = lounch_pad;


    scene->setPositionUpdate(x_position, y_position);


    double factor = sqrt(pow(vx, 2) + pow(vy, 2));
    sibka->update_arrow(round(((scene->rotation))*100)/100, factor*5);

}



void ControlPanel::post_left_thruster(){
    QUrl url = QUrl("http://localhost:8008/api/data/rocket/CNB_left_thruster:YCN?data&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);


    QString payload;
	payload += "{";
	payload += "\"v\":";
	payload += QVariant(this->scene->leftEngine).toString();
	payload += "}";

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);

}


void ControlPanel::post_right_thruster(){
     QUrl url = QUrl("http://localhost:8008/api/data/rocket/CNB_right_thruster:YCN?&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);


    QString payload;
	payload += "{";
	payload += "\"v\":";
	payload += QVariant(this->scene->rightEngine).toString();
	payload += "}";

    qDebug()<<"Payload for right thruster" << payload;

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);
}


void ControlPanel::post_main_engine(){
     QUrl url = QUrl("http://localhost:8008/api/data/rocket/CNR_EngineThrottle:ycn?&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);

    QString payload;
	payload += "{";
	payload += "\"v\":";
	payload += QString::number(this->scene->main_engine);
	payload += "}";

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());


    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);
}


void ControlPanel::post_height(){
     QUrl url = QUrl("http://localhost:8008/api/data/rocket/Rocket:Height?data&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);


    QString payload;
	payload += "{";
	payload += "\"v\": ";
	payload += QString::number(this->scene->height());
	payload += "}";

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);
}

void ControlPanel::post_width(){
     QUrl url = QUrl("http://localhost:8008/api/data/rocket/Rocket:Width?data&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);


    QString payload;
	payload += "{";
	payload += "\"v\":";
	payload += QString::number(this->scene->width());
	payload += "}";

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);
}


void ControlPanel::post_reset(){
     QUrl url = QUrl("http://localhost:8008/api/data/rocket/MP_reset:BSTATE?data&mime=application/json");

	QNetworkRequest request = QNetworkRequest(url);

	// HTTP Basic authentication header value: base64(username:password)
	QString username = "admin";
	QString password = "";
	QString concatenated = username + ":" + password;
	QByteArray data = concatenated.toLocal8Bit().toBase64();
	QString headerData = "Basic " + data;
	request.setRawHeader("Authorization", headerData.toLocal8Bit());
	request.setRawHeader("Content-Type", "application/json");

	QSslConfiguration conf = request.sslConfiguration();
	conf.setPeerVerifyMode(QSslSocket::VerifyNone);
	request.setSslConfiguration(conf);


    QString payload;
	payload += "{";
	payload += "\"v\":";
	payload += QVariant(true).toString();
	payload += "}";

    QNetworkReply *reply = networkManager->post(request, payload.toUtf8());
    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::requestData);
	connect(reply, &QNetworkReply::errorOccurred,this, &ControlPanel::onError);
}


void ControlPanel::onError(QNetworkReply::NetworkError code){
    scene->isconnected = false;
    connection->setText("Connection state: " + QVariant(scene->isconnected).toString());

}


void ControlPanel::keyPressEvent(QKeyEvent *event){

    if (event->key()==Qt::Key_R){
		this->resetSim();
	} else if (event->key()==Qt::Key_A){
        checkbox_left->setChecked(!(checkbox_left->isChecked()));
    } else if (event->key()==Qt::Key_D){
        checkbox_right->setChecked(!(checkbox_right->isChecked()));
    } else if (event->key()==Qt::Key_W){
        int value = thrustSlider->value();
        thrustSlider->setValue(value+1);
    } else if (event->key()==Qt::Key_S){
        int value = thrustSlider->value();
        thrustSlider->setValue(value-1);
    }

}

void ControlPanel::left_engine_change(){

    scene->leftEngine = checkbox_left->isChecked();
    this->post_left_thruster();

}

void ControlPanel::right_engine_change(){

    scene->rightEngine = checkbox_right->isChecked();
    this->post_right_thruster();

}

void ControlPanel::main_changed(){
    double newValue = double(thrustSlider->value()) / 10;
    scene->main_engine = newValue;  
    qDebug() << "Main engine updated locally:" << newValue;
    post_main_engine();
}


void ControlPanel::resetSim(){
    thrustSlider->setValue(0);
    main_changed();
    checkbox_left->setChecked(false);
    checkbox_right->setChecked(false);
    scene->in_platform = true;
    scene->rotation = 0;
    scene->rightEngine = false;
    scene->leftEngine = false;
    scene->y = 24;
    this->post_reset();
    
}

void ControlPanel::update_size(){
    int newWidth = inputWidth->text().toInt();
    int newHeight = inputHeight->text().toInt();
    scene->resize(newWidth, newHeight);
}

void ControlPanel::resizeEvent(QResizeEvent *event) {
    inputHeight->setText(QString::number(scene->height()));
    inputWidth->setText(QString::number(scene->width()));
}
