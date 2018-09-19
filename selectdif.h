#ifndef SELECTDIF_H
#define SELECTDIF_H

#include <QDialog>
#include <QApplication>

namespace Ui {
class selectdif;
}

class selectdif : public QDialog            //选择难度界面
{
    Q_OBJECT

public:
    explicit selectdif(QWidget *parent = 0);
    ~selectdif();
    int getd();

signals:
    dif(int);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::selectdif *ui;
    int d;
};

#endif // SELECTDIF_H
