#include "rgswidget.h"
#include "ui_rgswidget.h"
#include "proto.h"

#include <QString>
#include <QDebug>
#include <QBrush>
#include <QCryptographicHash>
#include <QMessageBox>
rgsWidget::rgsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rgsWidget)
{
    ui->setupUi(this);
    rgsLayout = new QVBoxLayout;
    rgsCntLineEdit = new QLineEdit(this);
    rgsCntLineEdit->setPlaceholderText(QString("注册账号"));
    rgsPwdLineEdit = new QLineEdit(this);
    rgsPwdLineEdit->setPlaceholderText(QString("账号密码"));
    rgsPwdLineEdit->setEchoMode(QLineEdit::Password);
    rgsPushButton = new QPushButton(QString("立即注册"));

    rgsLayout->addWidget(rgsCntLineEdit);
    rgsLayout->addWidget(rgsPwdLineEdit);
    rgsLayout->addWidget(rgsPushButton);

    this->setLayout(rgsLayout);

    connect(rgsPushButton,SIGNAL(clicked(bool)),this, SLOT(onRgsPushButtonSlots()));

    sd = new QUdpSocket(this);
    connect(sd,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));


}

rgsWidget::~rgsWidget()
{
    delete ui;
}

void rgsWidget::onRgsPushButtonSlots()
{
    struct rgs_st sndbuf;
    QString cntStr = rgsCntLineEdit->text();
    QString pwdStr = rgsPwdLineEdit->text();
    //  Qstring -> c string
    QByteArray b = cntStr.toLatin1();
    strcpy(sndbuf.cnt, b.data());
    b = pwdStr.toLatin1();

    b = QCryptographicHash::hash(b,QCryptographicHash::Sha512);
    strcpy(sndbuf.pwd, b.toHex().data());

    qDebug("cnt:%s, pwd:%s",sndbuf.cnt,sndbuf.pwd);

    sd->writeDatagram((char *)&sndbuf, sizeof (sndbuf),QHostAddress(SERVERIP),SERVERPORT);

}

void rgsWidget::onSocketReadyRead()
{
    //接收服务端发送的数据包
    struct rgs_st rcvbuf;
    sd->readDatagram((char *)&rcvbuf, sizeof(rcvbuf));
    switch (rcvbuf.rgsState) {
    case RGS_OK:
        QMessageBox::information(this, QString("注册"), QString("恭喜你，注册成功"),QMessageBox::Yes);
        this->close();
        break;
    case RGS_EXISTS:
        if(QMessageBox::question(this, QString("注册"), QString("所注册的账号已经存在，是否重新注册？"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes){
            rgsCntLineEdit->clear();
            rgsPwdLineEdit->clear();
        }else{
            this->close();
        }
    default:
        break;
    }
}
