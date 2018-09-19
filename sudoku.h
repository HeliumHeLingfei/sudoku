#ifndef SUDOKU_H
#define SUDOKU_H

#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>
#include "frame.h"
#include "pause.h"
#include "selectdif.h"
namespace Ui {
class sudoku;
}

class sudoku : public QMainWindow           //主界面
{
    Q_OBJECT

public:
    explicit sudoku(QWidget *parent = 0);
    ~sudoku();

protected:

signals:
    setframenum(int);           //传递数字信息到主画布中
    back();
    forward();
    del();
    note();
    re();               //replay
    displaytime(QTime);
    load(int);
    save(QTime);
private slots:
    void keyPressed(QString);
    void settemp(QString);
    void sethint(QString,QString,QString);
    void setbuttonsstate(int);
    void times();
    void restarttime();
    void closing(int);
    void won();
    void settime(QTime);

    void on_back_clicked();
    void on_forward_clicked();
    void on_clean_clicked();
    void on_notice_clicked();
    void on_pause_clicked();
    void on_replay_clicked();
    void on_select_triggered();
    void on_hint_clicked();

    void on_load_triggered();

    void on_save_triggered();

private:
    Ui::sudoku *ui;
    QSignalMapper *map;
    QString temp;
    Frame *f;
    pause *pau;
    selectdif *sele;
    QTime time;
    QTimer *timer;
    QString hints[3];

    void connectnumberbuttons();
};

#endif // SUDOKU_H
