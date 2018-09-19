#ifndef PAUSE_H
#define PAUSE_H

#include <QFrame>

namespace Ui {
class pause;
}

class pause : public QFrame         //暂停界面
{
    Q_OBJECT

public:
    explicit pause(QWidget *parent = 0);
    ~pause();

private:
    Ui::pause *ui;
    void mousePressEvent(QMouseEvent *ev);

signals:
    frameshow();
};

#endif // PAUSE_H
