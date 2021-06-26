#include "loginwidget.h"
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include <QMessageBox>
#include <QCryptographicHash>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug("login widget created");
    this->setFixedSize(400,300);
    this->setWindowTitle(QString("登陆界面"));
    this->setWindowIcon(QIcon(QString(":/myimgs/d8.ico")));

    cntLineEdit=new QLineEdit(this);
    cntLineEdit->setPlaceholderText("请输入账号");
    pwdLineEdit=new QLineEdit(this);
    cntLineEdit->setAlignment(Qt::AlignCenter);
    pwdLineEdit->setEchoMode(QLineEdit::Password);
    pwdLineEdit->setPlaceholderText(QString("请输入密码"));
    pwdLineEdit->setAlignment(Qt::AlignCenter);

    loginPushButton = new QPushButton(QString("登录"));
    loginPushButton->setFlat(true);
    rgsPushButton = new QPushButton(QString("注册"));
    hLayout = new QHBoxLayout;
    hLayout->addWidget(rgsPushButton);
    hLayout->addStretch();
    hLayout->addWidget(loginPushButton);


    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(cntLineEdit);
    vLayout->addWidget(pwdLineEdit);
    vLayout->addLayout(hLayout);

    vLayout->setContentsMargins(50, 60, 50, 60);

    this->setLayout(vLayout);

    // 注册界面new
    rgs = new rgsWidget();


    // 连接信号(注册按钮)与槽(当前对象)
    connect(rgsPushButton, SIGNAL(clicked(bool)), this, SLOT(onRgsPushButtonSlots()));
    connect(loginPushButton, SIGNAL(clicked(bool)), this, SLOT(onLoginPushButtonSlots()));
    sd = new QUdpSocket(this);
    connect(sd,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));


}

LoginWidget::~LoginWidget()
{
    qDebug("login widget destroyed");
}

void LoginWidget::onRgsPushButtonSlots()
{
    rgs->show();
    //this->hide();
}

void LoginWidget::onLoginPushButtonSlots()
{
    struct login_st sndbuf;
        QString cntStr = cntLineEdit->text();
        QString pwdStr = pwdLineEdit->text();
        //  Qstring -> c string
        QByteArray b = cntStr.toLatin1();
        strcpy(sndbuf.cnt, b.data());
        b = pwdStr.toLatin1();

        b = QCryptographicHash::hash(b,QCryptographicHash::Sha512);
        strcpy(sndbuf.pwd, b.toHex().data());

        qDebug("cnt:%s, pwd:%s",sndbuf.cnt,sndbuf.pwd);

        sd->writeDatagram((char *)&sndbuf, sizeof (sndbuf),QHostAddress(SERVERIP),SERVERPORT);
}

void LoginWidget::onSocketReadyRead()
{
    struct login_st rcvbuf;
    sd->readDatagram((char *)&rcvbuf, sizeof(rcvbuf));
    switch (rcvbuf.login_state) {
    case LOGIN_OK:
        QMessageBox::information(this, QString("登录"), QString("恭喜你，登录成功"),QMessageBox::Yes);
        ind=new indexWindow();
        connect(ind, SIGNAL(sendData()), this, SLOT(receiveData()));
        ind->show();
        this->hide();
        cntLineEdit->clear();
        pwdLineEdit->clear();
        break;
    case LOGIN_CNT_ERROR:
        QMessageBox::question(this, QString("登录"), QString("用户不存在，请重新输入"),QMessageBox::Yes);
            cntLineEdit->clear();
            pwdLineEdit->clear();
        break;
    case LOGIN_PWD_ERROR:
        QMessageBox::question(this, QString("登录"), QString("密码错误，请重新输入"),QMessageBox::Yes);
            cntLineEdit->clear();
            pwdLineEdit->clear();
        break;
    default:
        break;
    }
}

void LoginWidget::receiveData()
{
    //qDebug("123");
    this->show();
}
