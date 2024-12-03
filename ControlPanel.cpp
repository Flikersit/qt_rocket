#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ControlPanel.h"



ControlPanel::ControlPanel(QWidget *parent, RocketObject *rocket, RocketScene *scene){

    this->rocket = rocket;
    this->scene = scene;

    resetButton = new QPushButton("Reset");
    leftButton = new QPushButton("Left");
    rightButton = new QPushButton("Right");


    statusLabel = new QLabel("Rocket state");
    connection = new QLabel("Disconnected");

    thrustSlider = new QSlider();


    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(resetButton);
    layout->addWidget(leftButton);
    layout->addWidget(rightButton);
    layout->addWidget(statusLabel);
    layout->addWidget(connection);
    layout->addWidget(thrustSlider);
    
    setLayout(layout);

    networkManager = new QNetworkAccessManager(this);
    

    connect(resetButton, &QPushButton::released, this, &ControlPanel::resetSimulation);
    connect(&timer, &QTimer::timeout, this, &ControlPanel::requestData);


    timer.start(250);
}


void ControlPanel::requestData(){
    QUrl url = QUrl("http://localhost:8008/api/data/pidcontrol_control/PIDU:pv?data&mime=application/json");

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

    connection->setText("Connected"); 

}


void ControlPanel::onError(QNetworkReply::NetworkError code){

    connection->setText("Disconnected");

}


void ControlPanel::resetSimulation(){
    scene->reset();
}


void ControlPanel::keyPressEvent(QKeyEvent *event){

    if (event->key()==Qt::Key_R){
		resetSimulation();
	}

}
