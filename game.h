#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QtWidgets>
#include <QtGlobal>
#include <QFile>

class game : public QObject
{
    Q_OBJECT
public:
    explicit game(QObject *parent = 0);
    ~game();
    bool isloaded;
    QTime loadedtime;

    int getnumber(int a,int b);             //读取数字
    int getdefnumber(int a,int b);             //读取默认数字
    void setnumber(int a,int b,int c);      //输入数字
    void create(int);                       //生成数独
    QString estiwin(int type,int n);        //返回完成判断信息
    void load();
    void save(QTime);
private:
    int numbers[10][10];                    //后台数字
    int defnumbers[10][10];
    void reading();                         //读取基础数独
    void exchange(bool doline,int a,int b); //交换行列宫
    void swap(int &a,int &b);               //交换数字
signals:

public slots:
};

#endif // GAME_H
