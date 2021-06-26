#include "indexwindow.h"
#include "ui_indexwindow.h"
#include <QBrush>
indexWindow::indexWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::indexWindow)
{
    ui->setupUi(this);
    QPalette plt;
    QBrush bs(QPixmap(":/myimgs/images/bg.png").scaled(960, 500));
    plt.setBrush(QPalette::Background, bs);
    this->setPalette(plt);
    this->setMinimumSize(960,500);


    livingRoomPushButton = new QPushButton("客厅");
    setButtonImage(livingRoomPushButton, ":/myimgs/images/keting.png");

    bedroom1PushButton = new QPushButton(QString("卧室1"));
    setButtonImage(bedroom1PushButton, ":/myimgs/images/woshiyi.png");

    garagePushButton = new QPushButton(QString("车库"));
    setButtonImage(garagePushButton, ":/myimgs/images/cheku.png");

    basementPushButton = new QPushButton(QString("地下室"));
    setButtonImage(basementPushButton, ":/myimgs/images/dixiashi.png");

    bedroom2PushButton = new QPushButton(QString("卧室2"));
    setButtonImage(bedroom2PushButton, ":/myimgs/images/woshier.png");

    kitchenPushButton = new QPushButton(QString("厨房"));
    setButtonImage(kitchenPushButton, ":/myimgs/images/chufang.png");

    LocationPushButton = new QPushButton(QString("外景"));
    setButtonImage(LocationPushButton, ":/myimgs/images/waijing.png");

    bedroom3PushButton = new QPushButton(QString("卧室3"));
    setButtonImage(bedroom3PushButton, ":/myimgs/images/woshisan.png");

    anteportPushButton = new QPushButton(QString("外大门"));
    setButtonImage(anteportPushButton, ":/myimgs/images/waidamen.png");

    gobackPushButton = new QPushButton(QString("注销"));
    setButtonImage(gobackPushButton, ":/myimgs/images/goback.png");
    gobackPushButton->setIconSize(QSize(30, 30));

    //setButtonImage(garagePushButton, ":/myimgs/images/cheku.png");




    h1Layout = new QHBoxLayout;
    h2Layout = new QHBoxLayout;
    h3Layout = new QHBoxLayout;
    h4Layout = new QHBoxLayout;


    vLayout = new QVBoxLayout;

    h1Layout->addWidget(livingRoomPushButton);
    h1Layout->addWidget(bedroom1PushButton);
    h1Layout->addWidget(garagePushButton);

    h2Layout->addWidget(basementPushButton);
    h2Layout->addWidget(bedroom2PushButton);
    h2Layout->addWidget(kitchenPushButton);

    h3Layout->addWidget(LocationPushButton);
    h3Layout->addWidget(bedroom3PushButton);
    h3Layout->addWidget(anteportPushButton);


    h4Layout->addStretch(1);
    h4Layout->addWidget(gobackPushButton);


    vLayout->addLayout(h1Layout);
    vLayout->addLayout(h2Layout);
    vLayout->addLayout(h3Layout);
    vLayout->addLayout(h4Layout);

    this->setLayout(vLayout);
    connect(gobackPushButton, SIGNAL(clicked(bool)), this, SLOT(onGobackPushButtonSlots()));
    connect(livingRoomPushButton, SIGNAL(clicked(bool)), this, SLOT(onLivingRoomPushButtonSlots()));

}

indexWindow::~indexWindow()
{
    delete ui;
}

void indexWindow::onGobackPushButtonSlots()
{
    //logi=new LoginWidget();

    emit sendData();
    this->destroy();
}

void indexWindow::onLivingRoomPushButtonSlots()
{
    liv = new livingroom();
    connect(liv, SIGNAL(sendData1()), this, SLOT(receiveData1()));
    liv->show();
    this->hide();
}

void indexWindow::receiveData1()
{
    this->show();
}

void indexWindow::setButtonImage(QPushButton *button, QString image)
{

        button->setText("");
        button->setIcon(QIcon(QString(image)));
        button->setIconSize(QSize(249, 133));
        button->setFlat(true);
        button->setStyleSheet("border: 0px"); //消除边框

}
