#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QEvent>
#include <QtWidgets>
#include <QDebug>
#include "game.h"
namespace Ui {
class Frame;
}

class posi{                                                 //测定类
public:
    int x;
    int y;
    posi(QPoint);
    posi(int,int);
};

class backup{                                               //储存备用动作
public:
    QList<int> l;
    QList<int> r;
    QList<int> number;
};

class Frame : public QFrame                                 //主画布
{
    Q_OBJECT

public:
    explicit Frame(int d,QWidget *parent = 0);
    ~Frame();
    game *ga;           //数独生产与判断

private:
    Ui::Frame *ui;
    QPen *sty;
    posi *po;           //当前位置信息
    backup *fo;         //储存恢复信息

    bool st;        //判断是否有选中
    bool sn;        //判断是否有数字填入
    bool didback;   //判断当前动作是否为撤销动作
    bool didfor;    //判断当前动作是否为恢复动作
    QList<int> _l;  //储存位置
    QList<int> _r;  //储存位置
    int _num;       //判断撤销次数
    QList<int> num[10][10]; //储存数字
    int defaultnum[10][10]; //储存默认数字
    bool noted[10][10];     //储存标记信息
    QString ewin[3][10];    //判断是否完成
    QString defaultewin[3][10];//储存默认判断完成信息

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void paintframe(QPainter *p);   //画格子
    void paintdefaultnumber(QPainter *p);   //画默认数字
    void paintnumber(QPainter *p);  //画数字
    void paintnote(QPainter *p);    //画标记
    void paintback(QPainter *p);    //画背景
    void win(); //判断胜利


signals:
    void settemp(QString);  //设置界面的判断数
    void setbuttons(int);   //设置界面的按钮状况
    void won();             //胜利信号
    void hint(QString l,QString r,QString s);   //设置提示信息
    void settime(QTime);

private slots:
    void setdefault(int);   //输入默认数字
    void setnum(int);       //输入数字
    void back();
    void forward();
    void dele();
    void noting();
    void replay();          //重新开始
    void save(QTime);
};



#endif // FRAME_H
