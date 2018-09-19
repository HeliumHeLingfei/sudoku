#include "sudoku.h"
#include "ui_sudoku.h"
#include <QPainter>
#include <QPen>

sudoku::sudoku(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sudoku)
{
    ui->setupUi(this);
    sele=new selectdif(this);                                           //显示难度选择界面
    sele->move((QApplication::desktop()->width() - sele->width())/2,
           (QApplication::desktop()->height() - sele->height())/2);
    connect(sele,SIGNAL(dif(int)),this,SLOT(closing(int)));
    sele->exec();
    disconnect(sele,SIGNAL(dif(int)),this,SLOT(closing(int)));

    ui->label->setText("Level "+QString::number(sele->getd()));         //显示难度
    f=new Frame(sele->getd(),ui->frame);
    pau=new pause(ui->frame);
    pau->hide();

    timer=new QTimer;                                                   //设置时间
    timer->setInterval(1000);
    timer->setSingleShot(false);
    temp="";
    ui->frame->setFrameStyle(QFrame::Panel|QFrame::Raised);             //设置画布位置
    ui->frame->setLineWidth(4);
    f->move(4,4);
    f->show();

    restarttime();

    ui->back->setEnabled(false);
    ui->forward->setEnabled(false);
    ui->clean->setEnabled(false);


    connectnumberbuttons();
    connect(sele,SIGNAL(dif(int)),f,SLOT(setdefault(int)));
    connect(this,SIGNAL(setframenum(int)),f,SLOT(setnum(int)));
    connect(f,SIGNAL(settemp(QString)),this,SLOT(settemp(QString)));
    connect(f,SIGNAL(setbuttons(int)),this,SLOT(setbuttonsstate(int)));
    connect(this,SIGNAL(back()),f,SLOT(back()));
    connect(this,SIGNAL(forward()),f,SLOT(forward()));
    connect(this,SIGNAL(del()),f,SLOT(dele()));
    connect(this,SIGNAL(note()),f,SLOT(noting()));
    connect(pau,SIGNAL(frameshow()),f,SLOT(show()));
    connect(pau,SIGNAL(frameshow()),timer,SLOT(start()));
    connect(timer,SIGNAL(timeout()),this,SLOT(times()));
    connect(this,SIGNAL(re()),f,SLOT(replay()));
    connect(f,SIGNAL(hint(QString,QString,QString)),this,SLOT(sethint(QString,QString,QString)));
    connect(f,SIGNAL(won()),this,SLOT(won()));
    connect(this,SIGNAL(load(int)),f,SLOT(setdefault(int)));
    connect(this,SIGNAL(save(QTime)),f,SLOT(save(QTime)));
    connect(f,SIGNAL(settime(QTime)),this,SLOT(settime(QTime)));

}

void sudoku::connectnumberbuttons(){                //signalmapper
    map = new QSignalMapper(this);
    map->setMapping(ui->pb_1,tr("1"));
    connect(ui->pb_1,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_2,tr("2"));
    connect(ui->pb_2,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_3,tr("3"));
    connect(ui->pb_3,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_4,tr("4"));
    connect(ui->pb_4,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_5,tr("5"));
    connect(ui->pb_5,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_6,tr("6"));
    connect(ui->pb_6,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_7,tr("7"));
    connect(ui->pb_7,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_8,tr("8"));
    connect(ui->pb_8,SIGNAL(clicked(bool)),map,SLOT(map()));
    map->setMapping(ui->pb_9,tr("9"));
    connect(ui->pb_9,SIGNAL(clicked(bool)),map,SLOT(map()));
    connect(map,SIGNAL(mapped(QString)), this, SLOT(keyPressed(QString)));
}

void sudoku::setbuttonsstate(int w){
    if(w==1){                               //all enable
        ui->pb_1->setEnabled(true);
        ui->pb_2->setEnabled(true);
        ui->pb_3->setEnabled(true);
        ui->pb_4->setEnabled(true);
        ui->pb_5->setEnabled(true);
        ui->pb_6->setEnabled(true);
        ui->pb_7->setEnabled(true);
        ui->pb_8->setEnabled(true);
        ui->pb_9->setEnabled(true);
    }
    if(w==2){                               //enable
        setbuttonsstate(3);
        int q=temp.toInt();
        for(int i=0;i<temp.size();i++){
            int t=q%10;
            switch (t) {
            case 1:
                ui->pb_1->setEnabled(true);
                break;
            case 2:
                ui->pb_2->setEnabled(true);
                break;
            case 3:
                ui->pb_3->setEnabled(true);
                break;
            case 4:
                ui->pb_4->setEnabled(true);
                break;
            case 5:
                ui->pb_5->setEnabled(true);
                break;
            case 6:
                ui->pb_6->setEnabled(true);
                break;
            case 7:
                ui->pb_7->setEnabled(true);
                break;
            case 8:
                ui->pb_8->setEnabled(true);
                break;
            case 9:
                ui->pb_9->setEnabled(true);
                break;
            default:
                break;
            }
            q=q/10;
        }
    }
    if(w==3){                               //all disable
        ui->pb_1->setEnabled(false);
        ui->pb_2->setEnabled(false);
        ui->pb_3->setEnabled(false);
        ui->pb_4->setEnabled(false);
        ui->pb_5->setEnabled(false);
        ui->pb_6->setEnabled(false);
        ui->pb_7->setEnabled(false);
        ui->pb_8->setEnabled(false);
        ui->pb_9->setEnabled(false);
    }
    if(w==4){                               //highlight
        setbuttonsstate(9);
        int q=temp.toInt();
        for(int i=0;i<temp.size();i++){
            int t=q%10;
            switch (t) {
            case 1:
                ui->pb_1->setChecked(true);
                break;
            case 2:
                ui->pb_2->setChecked(true);
                break;
            case 3:
                ui->pb_3->setChecked(true);
                break;
            case 4:
                ui->pb_4->setChecked(true);
                break;
            case 5:
                ui->pb_5->setChecked(true);
                break;
            case 6:
                ui->pb_6->setChecked(true);
                break;
            case 7:
                ui->pb_7->setChecked(true);
                break;
            case 8:
                ui->pb_8->setChecked(true);
                break;
            case 9:
                ui->pb_9->setChecked(true);
                break;
            default:
                break;
            }
            q=q/10;
        }
    }
    if(w==5){                               //back disable
        ui->back->setEnabled(false);
    }
    if(w==6){                               //back enable
        ui->back->setEnabled(true);
    }
    if(w==7){                               //forward disable
        ui->forward->setEnabled(false);
    }
    if(w==8){                               //forward enable
        ui->forward->setEnabled(true);
    }
    if(w==9){                               //dehighlight
        ui->pb_1->setChecked(false);
        ui->pb_2->setChecked(false);
        ui->pb_3->setChecked(false);
        ui->pb_4->setChecked(false);
        ui->pb_5->setChecked(false);
        ui->pb_6->setChecked(false);
        ui->pb_7->setChecked(false);
        ui->pb_8->setChecked(false);
        ui->pb_9->setChecked(false);
    }
    if(w==10){                              //clear disable
        ui->clean->setEnabled(false);
    }
    if(w==11){                              //clear ensable
        ui->clean->setEnabled(true);
    }
}

void sudoku::keyPressed(QString a){         //响应数字按下
    //qDebug()<<temp<<"_"<<a;
    if(temp.indexOf(a)>=0){                 //如果已有该数字，则从该格内删去该数字
        int ind=temp.indexOf(a);
        //qDebug()<<temp<<"-"<<ind;
        temp=temp.mid(0,ind)+temp.mid(ind+1);
        setbuttonsstate(1);
    }else{
        if(temp.length()>=4)                //如果已有4个数字，则不响应
            return;
        temp=temp+a;
        if(temp.length()==4){               //如果已有4个数字，则按钮关闭
            setbuttonsstate(2);
        }
    }
    if(temp=="")
        setbuttonsstate(10);
    else
        setbuttonsstate(11);
    emit setframenum(temp.toInt());         //发出信号传递数字给画布
    //qDebug()<<"******";
}
void sudoku::settemp(QString a){
    ui->hint->setEnabled(true);
        temp=a;
        int q=a.toInt();
        if(q>10000){
            setbuttonsstate(3);
            ui->hint->setEnabled(false);
            return;
        }
        if(temp.length()==4){
            setbuttonsstate(2);
        }else{
            setbuttonsstate(1);
        }
        setbuttonsstate(4);
        if(q==0)
            setbuttonsstate(10);
        else
            setbuttonsstate(11);
}

void sudoku::sethint(QString l, QString r, QString s){
    hints[0]=l;
    hints[1]=r;
    hints[2]=s;
}

void sudoku::times()
{
    time=time.addSecs(1);
    ui->time->display(time.toString());
}

void sudoku::restarttime(){
    time.setHMS(0,0,0);
    timer->start();
}

void sudoku::settime(QTime a){
    time=a;
    timer->start();
}

void sudoku::closing(int d){
    if(d==0)
        exit(0);
}

void sudoku::won(){
    int ret = QMessageBox::warning(0, tr("胜利"),"您完成了"+ui->label->text()+"！\n"+"用时："+time.toString(),QMessageBox::Ok);
}

sudoku::~sudoku()
{
    delete ui;
}

void sudoku::on_back_clicked()
{
    emit back();
}

void sudoku::on_forward_clicked()
{
    emit forward();
}

void sudoku::on_clean_clicked()
{
    setbuttonsstate(10);
    emit del();
}

void sudoku::on_notice_clicked()
{
    emit note();
}

void sudoku::on_pause_clicked()
{
    pau->show();
    f->hide();
    timer->stop();
}

void sudoku::on_replay_clicked()
{
    emit re();
    if(!f->ga->isloaded)
        restarttime();
    f->show();
    pau->hide();
}

void sudoku::on_select_triggered()
{
    sele->exec();
    if(sele->getd()!=0){
        on_replay_clicked();
        ui->label->setText("Level "+QString::number(sele->getd()));
    }
}

void sudoku::on_hint_clicked()
{
    QString lrs;
    QString hi;
    for(int i=0;i<3;i++){               //显示提示信息
        if(hints[i].size()!=1){
            if(hints[i].startsWith("l"))
                lrs="行";
            if(hints[i].startsWith("r"))
                lrs="列";
            if(hints[i].startsWith("s"))
                lrs="九宫格";
            if(hints[i].contains("m")){
                hi="有错误。";
                int ret = QMessageBox::warning(0, tr("提示"),"该"+lrs+hi,QMessageBox::Ok);
                return;
            }
        }
    }
    QString y="";
    for(int i=1;i<10;i++){
        QString t=QString::number(i);
        if(hints[0].contains(t)&&hints[1].contains(t)&&hints[2].contains(t)){
            y=y+t;
        }
    }
    if(y.size()==1){
        int ret = QMessageBox::warning(0, tr("提示"),"该空格应该填入"+y,QMessageBox::Ok);
        return;
    }else{
        int ret = QMessageBox::warning(0, tr("提示"),"请换一格试试",QMessageBox::Ok);
        return;
    }
           // QString hi=""
           // int ret = QMessageBox::warning(0, tr("提示"), tr("Do you really want to exit?"),QMessageBox::Ok);
}

void sudoku::on_load_triggered()
{
    emit load(20);
    if(f->ga->isloaded){
        on_replay_clicked();
        ui->label->setText("自选关卡");
    }
}

void sudoku::on_save_triggered()
{
    emit save(time);
}
