#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QGraphicsView>
#include <QTransform>
#include <QVariant>
#include "ControlPanel.h"



ControlPanel::ControlPanel(QWidget *parent, RocketScene *scene){

    this->scene = scene;
    reset = false;

    
    resetButton = new QPushButton("Reset");
    

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
    connection = new QLabel("Connection state: " + QVariant(scene->rocket->isconnected).toString() );
    connection->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    statusLabel->setStyleSheet("color: black; background-color: #f5f5f5; padding: 5px;");
    statusbox->addWidget(connection);
    statusbox->addWidget(statusLabel);
    status_widget->setLayout(statusbox);


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
    scene->rocket->refresh_position();
    QWidget *coordinate_widget = new QWidget(); 
    QVBoxLayout *coordinate_layout = new QVBoxLayout(this);
    xposition = new QLabel("X position: " +  QString::number(scene->rocket->x));
    yposition = new QLabel("Y position: " +  QString::number(scene->rocket->y));
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
    layout->addWidget(status_widget);
    layout->addWidget(coordinate_widget);
    layout->addWidget(check_widget);
    layout->addWidget(widget_for_slider);
    layout->addWidget(chartView);

    QWidget *hwidget = new QWidget();
    hwidget->setLayout(layout);
    vlayout->addWidget(hwidget);

    QGraphicsView *view = new QGraphicsView(this->scene);

    QTransform transform;//IT WORKS DONT TOUCH
    transform.scale(1, -1);//!!
    view->setTransform(transform);//!!
    view->setRenderHint(QPainter::Antialiasing);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    vlayout->addWidget(view);

    setLayout(vlayout);

    networkManager = new QNetworkAccessManager(this);
    

    connect(resetButton, &QPushButton::released, this, &ControlPanel::resetSimulation);
    connect(&timer, &QTimer::timeout, this, &ControlPanel::requestData);

    connect(checkbox_left, &QCheckBox::stateChanged, this, &ControlPanel::left_engine_change);
    connect(checkbox_right, &QCheckBox::stateChanged, this, &ControlPanel::right_engine_change);
    connect(thrustSlider, &QSlider::valueChanged, this, &ControlPanel::post_data);


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

    bool left_engine = jResponse["subitems"].toArray()[1].toObject()["LeftThruster"].toObject()["v"].toBool();
    bool right_engine = jResponse["subitems"].toArray()[2].toObject()["RightThruster"].toObject()["v"].toBool();

    double x_position = jResponse["subitems"].toArray()[6].toObject()["X"].toObject()["v"].toDouble();
    double y_position = jResponse["subitems"].toArray()[7].toObject()["Y"].toObject()["v"].toDouble();

    double lounch_pad = jResponse["subitems"].toArray()[13].toObject()["LaunchpadOffset"].toObject()["v"].toDouble();

    double vx = jResponse["subitems"].toArray()[8].toObject()["VX"].toObject()["v"].toDouble();
    double vy = jResponse["subitems"].toArray()[8].toObject()["VY"].toObject()["v"].toDouble();

    bool touchdown = jResponse["subitems"].toArray()[11].toObject()["Touchdown"].toObject()["v"].toBool();
    bool crashed = jResponse["subitems"].toArray()[12].toObject()["Crashed"].toObject()["v"].toBool();

    bool reset = jResponse["subitems"].toArray()[3].toObject()["Reset"].toObject()["v"].toBool();

    
    vxSet->replace(0, vx);
    vySet->replace(0, vy);

    scene->rocket->isconnected = true;
    scene->rocket->x = x_position;
    scene->rocket->y = y_position;
    scene->rocket->rotation = rotation;

    xposition->setText("X position: " + QString::number(scene->rocket->x));
    yposition->setText("Y position: " + QString::number(scene->rocket->y));
    this->rotation->setText("Rotation: " + QString::number(scene->rocket->rotation));

    if(touchdown){
        statusLabel->setText("Rocket state: Touchdown");
    }else if(crashed){
        statusLabel->setText("Rocket state: Crashed");
    }else{
        statusLabel->setText("Rocket state: In air");
    }
    connection->setText("Connection state: " + QVariant(scene->rocket->isconnected).toString() );

    scene->width = width/2;
    scene->height = height;
    scene->launchPadOffSet = lounch_pad;
    scene->paint_position(x_position, y_position);

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
    engineThrottle["EngineThrottle"] = QJsonObject{{"v", double(thrustSlider->value()/10)}};
    subitems.append(engineThrottle);

    QJsonObject leftThruster;
    leftThruster["LeftThruster"] = QJsonObject{{"v", checkbox_left->isChecked()}};
    subitems.append(leftThruster);

    QJsonObject rightThruster;
    rightThruster["RightThruster"] = QJsonObject{{"v", checkbox_right->isChecked()}};
    subitems.append(rightThruster);

    QJsonObject reset;
    reset["BSTATE"] = QJsonObject{{"v", this->reset}};
    subitems.append(reset);

    QJsonObject width;
    width["Width"] = QJsonObject{{"v", scene->width}};
    subitems.append(width);

    QJsonObject height;
    height["Height"] = QJsonObject{{"v", scene->height}};

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
    } else if (event->key()==Qt::Key_W){
        int value = thrustSlider->value();
        thrustSlider->setValue(value+1);
    } else if (event->key()==Qt::Key_S){
        int value = thrustSlider->value();
        thrustSlider->setValue(value-1);
    }

}

void ControlPanel::left_engine_change(){

    scene->rocket->leftEngine = checkbox_left->isChecked();
    this->post_data();

}

void ControlPanel::right_engine_change(){

    scene->rocket->rightEngine = checkbox_right->isChecked();
    this->post_data();

}

void ControlPanel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (scene) {
        QGraphicsView *view = findChild<QGraphicsView *>();
        if (view) {
            view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        }
    }
}
