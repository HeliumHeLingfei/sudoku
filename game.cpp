#include "game.h"

game::game(QObject *parent) : QObject(parent)
{
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            numbers[i][j]==0;
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            defnumbers[i][j]==0;
    isloaded=false;
}

game::~game(){

}

int game::getnumber(int a,int b){
    int k=numbers[a][b];
    return k;
}
int game::getdefnumber(int a,int b){
    int k=defnumbers[a][b];
    return k;
}

void game::setnumber(int a,int b,int c){
    numbers[a][b]=c;
    return;
}

void game::save(QTime a){
    QString fileName = QFileDialog::getSaveFileName(0,tr("储存为"),".",tr("save Files (*.txt)"));
    qDebug()<<fileName;
     if (!fileName.isNull())
     {
         QFile f(fileName);
         if (!f.open(QFile::WriteOnly | QIODevice::Truncate))
             return;
         QTextStream saving(&f);
         for(int i=1;i<10;i++){
             for(int j=1;j<10;j++){
                saving<<QString::number(numbers[i][j])+" ";
                qDebug()<<QString::number(numbers[i][j]);
             }
         }
         saving<<"d ";
         for(int i=1;i<10;i++){
             for(int j=1;j<10;j++){
                saving<<QString::number(defnumbers[i][j])+" ";
             }
         }
         saving<<"t"<<QString::number(a.hour())<<" "<<QString::number(a.minute())<<" "<<QString::number(a.second())<<"\n";
         saving.flush();
         f.close();
     }else{
         return;
     }
}

void game::load(){
    QString fileName = QFileDialog::getOpenFileName(0,tr("读取"),".",tr("save Files (*.txt)"));
    qDebug()<<fileName;
        if (!fileName.isNull())
        {
            qDebug()<<fileName;
            QFile f(fileName);
            if(!f.open(QIODevice::ReadOnly|QIODevice::Text))
                return;
            QTextStream reading(&f);
            int n=0;
            QString _n="";
            while(!reading.atEnd())
            {
                QString text=reading.read(1);
                if(text=='d'){
                    text=reading.read(1);
                    break;
                }
                if(text!=' '&&text!="\n"){
                    _n=_n+text;
                    continue;
                }
                n++;
                numbers[(n-1)/9+1][(n-1)%9+1]=_n.toInt();
                _n="";
            }
            n=0;
            while(!reading.atEnd())
            {
                QString text=reading.read(1);
                if(text=='t'){
                    break;
                }
                if(text!=' '&&text!="\n"){
                    _n=_n+text;
                    continue;
                }
                n++;
                qDebug()<<_n<<n;
                defnumbers[(n-1)/9+1][(n-1)%9+1]=_n.toInt();
                _n="";
            }
            QString _t="";
            int tim[3];
            int i=0;
            while(!reading.atEnd())
            {
                QString text=reading.read(1);
                if(text!=' '&&text!="\n"){
                    _t=_t+text;
                    continue;
                }
                tim[i]=_t.toInt();
                i++;
                _t="";
            }
            loadedtime.setHMS(tim[0],tim[1],tim[2]);
            f.close();
            qDebug()<<"ga.numbers";
            for(int i=1;i<10;i++){
                QString a="";
                for(int j=1;j<10;j++)
                    a=a+QString::number(numbers[i][j])+" ";
                qDebug()<<a;
            }
            qDebug()<<"ga.defnumbers";
            for(int i=1;i<10;i++){
                QString a="";
                for(int j=1;j<10;j++)
                    a=a+QString::number(defnumbers[i][j])+" ";
                qDebug()<<a;
            }
            isloaded=true;
        }
        else{
            isloaded=false;
            return;
        }
}

void game::reading(){
    QFile f(":/sudokusamples/sample.txt");
    if(!f.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream reading(&f);
    int n=0;
    while(!reading.atEnd())
    {
        QString text=reading.read(1);
        if(text!=' '&&text!="\n"){
            n++;
            if(text.toInt()){
                numbers[(n-1)/9+1][(n-1)%9+1]=text.toInt();
            }
        }
    }
    f.close();
}

void game::swap(int &a,int &b){
    int k=a;
    a=b;
    b=k;
}

void game::exchange(bool doline,int a,int b){
    if(doline){     //1为交换行，0为交换列
        for(int i=1;i<10;i++){
            swap(numbers[a][i],numbers[b][i]);
        }
    }else{
        for(int i=1;i<10;i++){
            swap(numbers[i][a],numbers[i][b]);
        }
    }
}

void game::create(int d){
    reading();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设定种子
    for(int i=0;i<10;i++){          //进行十组操作
        for(int j=0;j<2;j++){
            switch (qrand()%4) {    //交换行
            case 0:
                exchange(1,1,2);
                break;
            case 1:
                exchange(1,1,3);
                break;
            case 2:
                exchange(1,3,2);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){
            switch (qrand()%4) {
            case 0:
                exchange(1,5,4);
                break;
            case 1:
                exchange(1,6,5);
                break;
            case 2:
                exchange(1,6,4);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){
            switch (qrand()%4) {
            case 0:
                exchange(1,7,8);
                break;
            case 1:
                exchange(1,7,9);
                break;
            case 2:
                exchange(1,8,9);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){       //交换列
            switch (qrand()%4) {
            case 0:
                exchange(0,1,2);
                break;
            case 1:
                exchange(0,1,3);
                break;
            case 2:
                exchange(0,3,2);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){
            switch (qrand()%4) {
            case 0:
                exchange(0,5,4);
                break;
            case 1:
                exchange(0,6,5);
                break;
            case 2:
                exchange(0,6,4);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){
            switch (qrand()%4) {
            case 0:
                exchange(0,7,8);
                break;
            case 1:
                exchange(0,7,9);
                break;
            case 2:
                exchange(0,8,9);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){       //交换九宫格
            switch (qrand()%4) {
            case 0:
                exchange(1,1,4);
                exchange(1,2,5);
                exchange(1,3,6);
                break;
            case 1:
                exchange(1,1,7);
                exchange(1,2,8);
                exchange(1,3,9);
                break;
            case 2:
                exchange(1,4,7);
                exchange(1,5,8);
                exchange(1,6,9);
                break;
            default:
                break;
            }
        }
        for(int j=0;j<2;j++){
            switch (qrand()%4) {
            case 0:
                exchange(0,1,4);
                exchange(0,2,5);
                exchange(0,3,6);
                break;
            case 1:
                exchange(0,1,7);
                exchange(0,2,8);
                exchange(0,3,9);
                break;
            case 2:
                exchange(0,4,7);
                exchange(0,5,8);
                exchange(0,6,9);
                break;
            default:
                break;
            }
        }
    }
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=1;i<10;i++)                               //挖去数字
        for(int j=1;j<10;j++)
        {
            int k=qrand()%100;
            if((70-4*d)<k)
                numbers[i][j]=0;
        }
    for(int i=1;i<10;i++)
        for(int j=1;j<10;j++){
            defnumbers[i][j]=numbers[i][j];
        }

}

QString game::estiwin(int type,int n){
    QString e="";
    int p[10];
    for(int i=0;i<10;i++)
        p[i]=0;
    if(type==0){                            //判断行
        e="l";
        for(int i=1;i<10;i++)
            p[numbers[n][i]]++;
        for(int i=1;i<10;i++){
            if(p[i]>1){
                e=e+"m"+QString::number(i);
                qDebug()<<"estwin"<<type<<n<<e;
                return e;
            }
            if(p[i]==0)
                e=e+QString::number(i);
        }
    }
    if(type==1){                            //判断列
        e="r";
        for(int i=1;i<10;i++)
            p[numbers[i][n]]++;
        for(int i=1;i<10;i++){
            if(p[i]>1){
                e=e+"m"+QString::number(i);
                qDebug()<<"estwin"<<type<<n<<e;
                return e;
            }
            if(p[i]==0)
                e=e+QString::number(i);
        }
    }
    if(type==2){                            //判断块
        e="s";
        int l=((n-1)/3)*3+1;
        int r=((n-1)%3)*3+1;
        for(int i=l;i<l+3;i++)
            for(int j=r;j<r+3;j++)
                p[numbers[i][j]]++;
        for(int i=1;i<10;i++){
            if(p[i]>1){
                e=e+"m"+QString::number(i);
                qDebug()<<"estwin"<<type<<n<<e;
                return e;
            }
            if(p[i]==0)
                e=e+QString::number(i);
        }
    }
    //qDebug()<<"estwin"<<type<<n<<e;
    return e;
}
