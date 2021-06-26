#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "rgswidget.h"
#include <QUdpSocket>
#include "loginProto.h"
#include "indexwindow.h"

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private slots:
    void onRgsPushButtonSlots();
    void onLoginPushButtonSlots();
    void onSocketReadyRead();
    void receiveData();
private:
    QLineEdit * cntLineEdit, * pwdLineEdit;
    QVBoxLayout * vLayout;
    QPushButton * loginPushButton,*rgsPushButton;
    QHBoxLayout * hLayout;

    rgsWidget *rgs;
    indexWindow *ind;
    QUdpSocket *sd;

};

#endif // LOGINWIDGET_H
