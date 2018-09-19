#include "pause.h"
#include "ui_pause.h"

pause::pause(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::pause)
{
    ui->setupUi(this);
}

pause::~pause()
{
    delete ui;
}

void pause::mousePressEvent(QMouseEvent *ev){
    this->hide();
    emit frameshow();
}
