#include "frame.h"
#include "ui_frame.h"

Frame::Frame(int d,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    setdefault(d);
    replay();
}

Frame::~Frame()
{
    delete ui;
}

void Frame::mousePressEvent(QMouseEvent *ev){
    QPoint pnt=ev->pos();
    po= new posi(pnt);
    if(!st){                    //如果从未点选位置，即游戏初始状态，则将首个点选的位置信息储存
        _l.append(po->y);
        _r.append(po->x);
    }
    st=true;
    if(num[po->y][po->x].last()==0)
        emit settemp("");
    else
        emit settemp(QString::number(num[po->y][po->x].last()));
    win();
    update();
    ga->isloaded=false;
}

void Frame::setnum(int a){
    if(st){
        if(!didback){
            emit setbuttons(6);
            if(!didfor){                                //如果是在恢复动作后再输入数字，则清空所有储存的恢复动作
                fo=new backup();
                emit setbuttons(7);
            }
            didfor=false;

            _l.append(po->y);   //储存动作
            _r.append(po->x);
            num[_l.last()][_r.last()].append(a);
            sn=true;
            if(_num<10)         //设置允许的撤销步数
                _num++;
            if(a<10){           //输入game类数据，以备判断
                ga->setnumber(po->y,po->x,a);
            }else{
                ga->setnumber(po->y,po->x,0);
            }
            win();              //判断
            update();
        }else{                                          //如果是撤销动作，则不储存动作
            _l.append(po->y);
            _r.append(po->x);
            num[_l.last()][_r.last()].append(a);
            sn=true;
            didback=false;
            update();
        }
    }
}

void Frame::setdefault(int d){
    if(d==0)
        return;
    if(d==20){
        ga->load();
        if(!ga->isloaded)
            return;
        for(int i=1;i<10;i++)
            for(int j=1;j<10;j++)
                defaultnum[i][j]=ga->getdefnumber(i,j);
        for(int i=1;i<10;i++)
            for(int j=1;j<10;j++){
                num[i][j].clear();
                num[i][j].append(0);
                if(defaultnum[i][j]!=0){
                    num[i][j].append(defaultnum[i][j]+10000);
                }else{
                    num[i][j].append(ga->getnumber(i,j));
                }
            }
        for(int i=0;i<3;i++)
            for(int j=1;j<10;j++)
                defaultewin[i][j]=ga->estiwin(i,j);
        emit settime(ga->loadedtime);
        qDebug()<<ga->loadedtime.toString();
        qDebug()<<"f.numbers";
        for(int i=1;i<10;i++){
            QString a="";
            for(int j=1;j<10;j++)
                a=a+QString::number(num[i][j].last())+" ";
            qDebug()<<a;
        }
        qDebug()<<"f.defnumbers";
        for(int i=1;i<10;i++){
            QString a="";
            for(int j=1;j<10;j++)
                a=a+QString::number(defaultnum[i][j])+" ";
            qDebug()<<a;
        }
    }else{
        for(int i=0;i<10;i++)
            for(int j=0;j<10;j++)
                defaultnum[i][j]=0;
        ga=new game();
        ga->create(d);                  //生成数独
        for(int i=1;i<10;i++)
            for(int j=1;j<10;j++)
                defaultnum[i][j]=ga->getnumber(i,j);
        for(int i=0;i<3;i++)
            for(int j=1;j<10;j++)
                defaultewin[i][j]=ga->estiwin(i,j);
        //qDebug()<<"&&&&&&&&&&&&";
    }

}

void Frame::paintEvent(QPaintEvent *ev){
    QPainter *p=new QPainter(this);
    p->save();
    paintback(p);
    paintnote(p);
    paintframe(p);
    paintdefaultnumber(p);
    p->restore();
    if(sn)
        paintnumber(p);
}

void Frame::paintdefaultnumber(QPainter *p){
    for(int l=1;l<10;l++)
        for(int r=1;r<10;r++){
            if(num[l][r].last()<10000)
                continue;
            QPoint a;
            if(r<=3){
                a.setX(3+54*r-41);
            }else if(r>=7){
                a.setX(331+54*(r-6)-41);
            }else{
                a.setX(167+54*(r-3)-41);
            }
            if(l<=3){
                a.setY(3+54*l-12);
            }else if(l>=7){
                a.setY(331+54*(l-6)-12);
            }else{
                a.setY(167+54*(l-3)-12);
            }
            QFont f("Arial Rounded MT Bold");
            f.setPixelSize(45);
            f.setUnderline(true);
            f.setBold(true);
            if(st&&(num[l][r].last()-10000==num[po->y][po->x].last()||num[l][r].last()-10000==num[po->y][po->x].last()-10000))  //高亮数字
                p->setPen(Qt::green);
            else
                p->setPen(Qt::blue);
            p->setFont(f);
                p->drawText(a,QString::number(num[l][r].last()%10));
            }
}

void Frame::paintnote(QPainter *p){
    for(int l=0;l<10;l++)
        for(int r=0;r<10;r++){
            if(!noted[l][r])
                continue;
            QPoint a;
            if(r<=3){
                a.setX(3+54*r-8);
            }else if(r>=7){
                a.setX(331+54*(r-6)-8);
            }else{
                a.setX(167+54*(r-3)-8);
            }
            if(l<=3){
                a.setY(3+54*l-30);
            }else if(l>=7){
                a.setY(331+54*(l-6)-30);
            }else{
                a.setY(167+54*(l-3)-30);
            }
            p->setPen(Qt::red);
            p->setBrush(Qt::red);
            QPointF points[3] = {
                QPointF(a.x(), a.y()),
                QPointF(a.x()+4,a.y()+5),
                QPointF(a.x()-4,a.y()+5)
            };
            p->drawPolygon(points, 3);
       }
}

void Frame::paintframe(QPainter *p){
    int k=0;
    sty=new QPen();
    QColor *red=new QColor(250,0,0);
    QColor *black=new QColor(0,0,0);

    for(int i=2;i<496;i=i+54){
        if(k==0||k==3||k==6||k==9){
            sty->setWidth(4);
        }else{
            sty->setWidth(2);
        }

            if(st&&(k==po->x-1||k==po->x)){     //高亮当前位置
                sty->setColor(*red);
                sty->setWidth(sty->width()+1);
                p->setPen(*sty);
                p->drawLine(i,0,i,496);
            }else{
                sty->setColor(*black);
                p->setPen(*sty);
                p->drawLine(i,0,i,496);
            }
            if(st&&(k==po->y-1||k==po->y)){
                sty->setColor(*red);
                sty->setWidth(sty->width()+1);
                p->setPen(*sty);
                p->drawLine(0,i,496,i);
            }else{
                sty->setColor(*black);
                p->setPen(*sty);
                p->drawLine(0,i,496,i);
            }
        k++;
        if(!(k==2||k==5||k==8))
            i++;
    }

}

void Frame::paintnumber(QPainter *p){
    for(int l=1;l<10;l++)
        for(int r=1;r<10;r++){
            if(num[l][r].last()==0||num[l][r].last()>=10000)
                continue;
            QPoint a;
            if(r<=3){
                a.setX(3+54*r-41);
            }else if(r>=7){
                a.setX(331+54*(r-6)-41);
            }else{
                a.setX(167+54*(r-3)-41);
            }
            if(l<=3){
                a.setY(3+54*l-12);
            }else if(l>=7){
                a.setY(331+54*(l-6)-12);
            }else{
                a.setY(167+54*(l-3)-12);
            }
            QFont f("Segoe Script");
            f.setPixelSize(45);
            f.setBold(true);
            p->setFont(f);
            if(num[l][r].last()<10){                                                                                    //画单个数字
                if(st&&(num[l][r].last()==num[po->y][po->x].last()||num[l][r].last()==num[po->y][po->x].last()-10000))  //高亮数字
                    p->setPen(Qt::green);
                else
                    p->setPen(Qt::black);
                p->drawText(a,QString::number(num[l][r].last()));
            }
            else{                                                                                                       //画多个数字
                a.setX(a.x()-9);
                a.setY(a.y()-19);
                f.setPixelSize(25);
                p->setFont(f);
                p->drawText(a,QString::number(num[l][r].last()%10));
                int tem=num[l][r].last()/10;
                if(tem!=0){
                    a.setX(a.x()+29);
                    p->drawText(a,QString::number(tem%10));
                }
                tem=tem/10;
                if(tem!=0){
                    a.setX(a.x()-29);
                    a.setY(a.y()+26);
                    p->drawText(a,QString::number(tem%10));
                }
                tem=tem/10;
                if(tem!=0){
                    a.setX(a.x()+29);
                    p->drawText(a,QString::number(tem%10));
                }

            }
        }

}

void Frame::paintback(QPainter *p){
    for(int i=0;i<3;i++)
        for(int j=1;j<10;j++){
            if(ewin[i][j].size()==1){       //判断是否为完成的格子
                switch (i) {
                case 0:
                {
                    int a=2;
                    for(int k=0;k<j;k++){
                        a=a+54;
                        if(!(k==2||k==5||k==8))
                            a++;
                    }
                    p->setPen(Qt::NoPen);
                    p->setBrush(Qt::lightGray);
                    QPointF apoints[4] = {
                        QPointF(2,a),
                        QPointF(2,a-55),
                        QPointF(494,a-55),
                        QPointF(494,a)
                    };
                    p->drawPolygon(apoints,4);
                }
                    break;
                case 1:
                {
                    int b=2;
                    for(int k=0;k<j;k++){
                        b=b+54;
                        if(!(k==2||k==5||k==8))
                            b++;
                    }
                    p->setPen(Qt::NoPen);
                    p->setBrush(Qt::lightGray);
                    QPointF bpoints[4] = {
                        QPointF(b,2),
                        QPointF(b-55,2),
                        QPointF(b-55,494),
                        QPointF(b,494)
                    };
                    p->drawPolygon(bpoints,4);
                }
                    break;
                case 2:
                {
                    int d=2+((j-1)/3)*164;
                    int c=2+((j-1)%3)*164;
                    p->setPen(Qt::NoPen);
                    p->setBrush(Qt::lightGray);
                    QPointF cpoints[4] = {
                        QPointF(c,d),
                        QPointF(c,d+164),
                        QPointF(c+164,d+164),
                        QPointF(c+164,d)
                    };
                    p->drawPolygon(cpoints,4);
                }
                    break;
                default:
                    break;
                }
            }
        }
}

void Frame::back(){
        if(fo->l.size()==10){           //将动作储存到恢复信息中
            fo->l.removeFirst();
            fo->r.removeFirst();
            fo->number.removeFirst();
        }
        fo->number.append(num[_l.last()][_r.last()].takeLast());
        fo->l.append(_l.takeLast());
        fo->r.append(_r.takeLast());;

    emit setbuttons(8);

    po= new posi(_l.last(),_r.last());      //改变后台信息以改变显示
    st=true;
    if(num[po->y][po->x].last()==0)
        emit settemp("");
    else
        emit settemp(QString::number(num[po->y][po->x].last()));

    _num--;                                 //撤销步数-1
    update();
    //qDebug()<<"@@@@@@@@@@@@@@";
    if(_num==0){
        emit setbuttons(5);
    }
}

void Frame::forward(){
     _l.append(fo->l.takeLast());
     _r.append(fo->r.takeLast());
     num[_l.last()][_r.last()].append(fo->number.takeLast());

    didfor=true;

    emit setbuttons(6);
    po= new posi(_l.last(),_r.last());
    st=true;
    if(num[po->y][po->x].last()==0)
        emit settemp("");
    else
        emit settemp(QString::number(num[po->y][po->x].last()));
    //qDebug()<<"###############";

    update();
    if(fo->l.isEmpty()){
        emit setbuttons(7);
    }
}

void Frame::dele(){
    setnum(0);
    emit settemp("");
}

void Frame::noting(){
    if(noted[po->y][po->x])
        noted[po->y][po->x]=false;
    else
        noted[po->y][po->x]=true;
    update();
}

void Frame::replay(){
    st=false;           //重设初始参数
    sn=false;
    if(ga->isloaded)
        sn=true;
    didback=false;
    didfor=false;
    _num=0;
    _l.clear();
    _r.clear();
    for(int i=1;i<10;i++)
        for(int j=1;j<10;j++){
            if(!ga->isloaded){
                num[i][j].clear();
                num[i][j].append(0);
                if(defaultnum[i][j]!=0)
                    num[i][j].append(defaultnum[i][j]+10000);
            }
            noted[i][j]=false;
        }
    /*debug
    for(int i=1;i<10;i++){
        QString a="";
        for(int j=1;j<10;j++)
            a=a+QString::number(num[i][j].last())+" ";
        qDebug()<<a;
    }
    */
    if(!ga->isloaded){
        for(int i=1;i<10;i++)
            for(int j=1;j<10;j++)
                ga->setnumber(i,j,defaultnum[i][j]);
        for(int i=0;i<3;i++)
            for(int j=1;j<10;j++)
                ewin[i][j]=defaultewin[i][j];
    }
    if(ga->isloaded){
        for(int i=1;i<10;i++)
            for(int j=1;j<10;j++){
                po= new posi(i,j);
                if(!st){                    //如果从未点选位置，即游戏初始状态，则将首个点选的位置信息储存
                    _l.append(po->y);
                    _r.append(po->x);
                }
                st=true;
                if(num[po->y][po->x].last()==0)
                    emit settemp("");
                else
                    emit settemp(QString::number(num[po->y][po->x].last()));
                win();
                update();
            }
    }
    fo=new backup();
    //qDebug()<<"*******";
    update();
    emit setbuttons(1);
    emit setbuttons(5);
    emit setbuttons(7);
    emit setbuttons(9);
    emit setbuttons(10);
}

void Frame::save(QTime a){
    ga->save(a);
}

void Frame::win(){
    int s=((po->y-1)/3)*3+((po->x-1)/3)+1;      //更新当前格子的完成判断
    ewin[0][po->y]=ga->estiwin(0,po->y);
    ewin[1][po->x]=ga->estiwin(1,po->x);
    ewin[2][s]=ga->estiwin(2,s);

    emit hint(ewin[0][po->y],ewin[1][po->x],ewin[2][s]);

    for(int i=0;i<3;i++)
        for(int j=1;j<10;j++){
            if(ewin[i][j].size()!=1)
                return;
        }
    emit won();
}

posi::posi(int a,int b){
    this->y=a;          //直接设置位置
    this->x=b;
}

posi::posi(QPoint a){
    if(a.x()>0&&a.x()<=56){                         //位置映射
        x=1;
    }else if(a.x()>56&&a.x()<=110){
        x=2;
    }else if(a.x()>110&&a.x()<=164){
        x=3;
    }else if(a.x()>164&&a.x()<=220){
        x=4;
    }else if(a.x()>220&&a.x()<=274){
        x=5;
    }else if(a.x()>274&&a.x()<=328){
        x=6;
    }else if(a.x()>328&&a.x()<=384){
        x=7;
    }else if(a.x()>384&&a.x()<=438){
        x=8;
    }else if(a.x()>438&&a.x()<=496){
        x=9;
    }
    if(a.y()>0&&a.y()<=56){
        y=1;
    }else if(a.y()>56&&a.y()<=110){
        y=2;
    }else if(a.y()>110&&a.y()<=164){
        y=3;
    }else if(a.y()>164&&a.y()<=220){
        y=4;
    }else if(a.y()>220&&a.y()<=274){
        y=5;
    }else if(a.y()>274&&a.y()<=328){
        y=6;
    }else if(a.y()>328&&a.y()<=384){
        y=7;
    }else if(a.y()>384&&a.y()<=438){
        y=8;
    }else if(a.y()>438&&a.y()<=496){
        y=9;
    }
}
