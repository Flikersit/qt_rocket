#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QGraphicsView>
#include <QTransform>
#include <QVariant>
#include "ControlPanel.h"



ControlPanel::ControlPanel(QWidget *parent, RocketScene *scene){

    this->scene = scene;

    resetButton = new QPushButton("Reset");
    leftButton = new QPushButton("Left");
    rightButton = new QPushButton("Right");

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



    statusLabel = new QLabel("Rocket state: ");
    connection = new QLabel("Connection state: " + QVariant(scene->rocket->isconnected).toString() );

    thrustSlider = new QSlider();

    scene->rocket->refresh_position();
    QWidget *coordinate_widget = new QWidget(); 
    QVBoxLayout *coordinate_layout = new QVBoxLayout(this);
    xposition = new QLabel("X position: " +  QString::number(scene->rocket->x));
    yposition = new QLabel("Y position: " +  QString::number(scene->rocket->y));
    rortation = new QLabel("Rotation: ");

    coordinate_layout->addWidget(xposition);
    coordinate_layout->addWidget(yposition);
    coordinate_layout->addWidget(rortation);
    coordinate_widget->setLayout(coordinate_layout);

    QHBoxLayout *layout = new QHBoxLayout(this);
    QVBoxLayout *vlayout = new QVBoxLayout(this);

    //Taying to create Bar: Begin
    vxSet = new QBarSet("VX");
    vySet = new QBarSet("VY");
    series = new QBarSeries();
    series->append(vxSet);
    series->append(vySet);
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Speed Graph");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    axisX = new QBarCategoryAxis();
    axisX->append("VX");
    axisX->append("VY");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisY = new QBarCategoryAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //i'VE END IT AND I WANNA CRY




    layout->addWidget(resetButton);
    layout->addWidget(leftButton);
    layout->addWidget(rightButton);
    layout->addWidget(statusLabel);
    layout->addWidget(connection);
    layout->addWidget(coordinate_widget);
    layout->addWidget(chartView);
    layout->addWidget(check_widget);
    layout->addWidget(thrustSlider);

    QWidget *hwidget = new QWidget();
    hwidget->setLayout(layout);
    vlayout->addWidget(hwidget);

    QGraphicsView *view = new QGraphicsView(this->scene);

    QTransform transform;//IT WORKS DONT TOUCH
    transform.scale(1, -1);//!!
    view->setTransform(transform);//!!

    vlayout->addWidget(view);

    setLayout(vlayout);

    networkManager = new QNetworkAccessManager(this);
    

    connect(resetButton, &QPushButton::released, this, &ControlPanel::resetSimulation);
    connect(&timer, &QTimer::timeout, this, &ControlPanel::requestData);

    connect(checkbox_left, &QCheckBox::stateChanged, this, &ControlPanel::left_engine_change);
    connect(checkbox_right, &QCheckBox::stateChanged, this, &ControlPanel::right_engine_change);


    timer.start(250);
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
	qDebug() << str;

	QJsonDocument jDoc = QJsonDocument::fromJson(data);
	QJsonObject jResponse = jDoc.object();

    double height = jResponse["subitems"].toArray()[5].toObject()["Height"].toObject()["v"].toDouble();
    double width = jResponse["subitems"].toArray()[4].toObject()["Width"].toObject()["v"].toDouble();
    double rotation = jResponse["subitems"].toArray()[10].toObject()["Rotation"].toObject()["v"].toDouble();

    bool right_engine = jResponse["subitems"].toArray()[1].toObject()["LeftThruster"].toObject()["v"].toBool();
    bool right_engine = jResponse["subitems"].toArray()[2].toObject()["RightThruster"].toObject()["v"].toBool();


    scene->rocket->isconnected = true;
    connection->setText("Connection state: " + QVariant(scene->rocket->isconnected).toString() ); 

}


void ControlPanel::post_data(){
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

    QJsonArray subitems;

    QJsonObject engineThrottle;
    engineThrottle["EngineThrottle"] = QJsonObject{{"v", 0.0}};
    subitems.append(engineThrottle);

    QJsonObject leftThruster;
    leftThruster["LeftThruster"] = QJsonObject{{"v", checkbox_left->isChecked()}};
    subitems.append(leftThruster);

    QJsonObject rightThruster;
    rightThruster["RightThruster"] = QJsonObject{{"v", checkbox_right->isChecked()}};
    subitems.append(rightThruster);

    QJsonObject reset;
    reset["Reset"] = QJsonObject{{"v", false}};
    subitems.append(reset);

    QJsonObject width;
    width["Width"] = QJsonObject{{"v", 800.0}};
    subitems.append(width);

    QJsonObject height;
    height["height"] = QJsonObject{{"v", 600.0}};

    QJsonObject rootObject;
    rootObject["subitems"] = subitems;

    QJsonDocument jsonDoc(rootObject);
    QByteArray payload = jsonDoc.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = networkManager->post(request, payload);

    connect(reply, &QNetworkReply::readyRead, this, &ControlPanel::onDataReceived);
    connect(reply, &QNetworkReply::errorOccurred, this, &ControlPanel::onError); 

}


void ControlPanel::onError(QNetworkReply::NetworkError code){
    scene->rocket->isconnected = false;
    connection->setText("Connection state: " + QVariant(scene->rocket->isconnected).toString());

}


void ControlPanel::resetSimulation(){
    scene->reset();
    xposition->setText("X position: " +  QString::number(scene->rocket->x));
    yposition->setText("Y position: " +  QString::number(scene->rocket->y));
}


void ControlPanel::keyPressEvent(QKeyEvent *event){

    if (event->key()==Qt::Key_R){
		resetSimulation();
	} else if (event->key()==Qt::Key_A){
        checkbox_left->setChecked(!(checkbox_left->isChecked()));
    } else if (event->key()==Qt::Key_D){
        checkbox_right->setChecked(!(checkbox_right->isChecked()));
    }

}

void ControlPanel::left_engine_change(){

    scene->rocket->leftEngine = checkbox_left->isChecked();

}

void ControlPanel::right_engine_change(){

    scene->rocket->rightEngine = checkbox_right->isChecked();

}
