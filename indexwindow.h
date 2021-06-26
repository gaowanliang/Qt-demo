#ifndef INDEXWINDOW_H
#define INDEXWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

#include "livingroom.h"
namespace Ui {
class indexWindow;
}

class indexWindow : public QWidget
{
    Q_OBJECT

public:
    explicit indexWindow(QWidget *parent = nullptr);
    ~indexWindow();


private slots:
    void onGobackPushButtonSlots();
    void onLivingRoomPushButtonSlots();
    void receiveData1();

signals:
    void sendData(); //传递数据的信号


private:
    Ui::indexWindow *ui;
    QVBoxLayout * vLayout;
    QPushButton * livingRoomPushButton, *bedroom1PushButton, * garagePushButton, * basementPushButton, *bedroom2PushButton, \
    *kitchenPushButton, *LocationPushButton,*bedroom3PushButton, *anteportPushButton, *gobackPushButton;
    QHBoxLayout * h1Layout,* h2Layout, * h3Layout, *h4Layout;

    livingroom * liv;

    void setButtonImage(QPushButton *button, QString image);


};

#endif // INDEXWINDOW_H
