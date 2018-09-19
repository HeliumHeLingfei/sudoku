#include "selectdif.h"
#include "ui_selectdif.h"

selectdif::selectdif(QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint),
    ui(new Ui::selectdif)
{
    ui->setupUi(this);
    d=0;
}

selectdif::~selectdif()
{
    delete ui;
}

void selectdif::on_buttonBox_accepted()
{
    d=ui->comboBox->currentIndex()+1;
    emit dif(d);
    close();
}

int selectdif::getd(){
    return d;
}

void selectdif::on_buttonBox_rejected()
{
    d=0;
    emit dif(d);
    close();
}
