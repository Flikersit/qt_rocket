#include "ControlPanel.h"



ControlPanel::ControlPanel(QWidget *parent = nullptr, RocketObject *rocket, RocketScene *scene){

    this.rocket = rocket;
    this.scene = scene;

    resetButton = new QPushButton("Reset");
    statusLabel = new QLabel("Tattatatta");
    

    connect(resetButton, &QPushButton::released, this, &ControlPanel::resetSimulation);
}




ControlPanel::resetSimulation(){
    scene->reset;
}


ControlPanel::keyPressEvent(QKeyEvent *event){

    if (event->key()==Qt::Key_R){
		resetSimulation();
	}

}