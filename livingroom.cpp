#include "livingroom.h"
#include "ui_livingroom.h"
#include <QDebug>
livingroom::livingroom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::livingroom)
{
    ui->setupUi(this);
    connect(ui->gobackPushButton, SIGNAL(clicked(bool)), this, SLOT(onGobackPushButtonSlots()));
    QPalette plt;
    QBrush bs(QPixmap(":/myimgs/images/bg.jpg").scaled(this->width(), this->height()));
    plt.setBrush(QPalette::Background, bs);
    this->setPalette(plt);

    player = new QMediaPlayer;
    videoWidget = new QVideoWidget;
    ui->verticalLayout_5->addWidget(videoWidget);
    player->setMedia(QUrl::fromLocalFile("D:\\program_data\\qt\\netClient\\test.mp4"));
    player->setVideoOutput(videoWidget);

    videoWidget->show();
    player->play();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &livingroom::showTime);
    timer->start(1000);
    ui->lcdNumber_3->setDigitCount(8);
    showTime();
    showColon = true;


}

livingroom::~livingroom()
{
    delete ui;
}

void livingroom::onGobackPushButtonSlots()
{
    //logi=new LoginWidget();

    emit sendData1();
    this->destroy();
}

void livingroom::on_pushButton_5_clicked()
{
    double g = ui->lcdNumber->value()+0.5;
    QString str = QString::number(g, 'f', 1);
    ui->lcdNumber->display(str);
}

void livingroom::on_pushButton_4_clicked()
{
    double g = ui->lcdNumber->value()-0.5;
    QString str = QString::number(g, 'f', 1);
    ui->lcdNumber->display(str);
}

void livingroom::on_pushButton_6_clicked()
{
    double g = ui->lcdNumber_2->value()+0.5;
    QString str = QString::number(g, 'f', 1);
    ui->lcdNumber_2->display(str);
}

void livingroom::on_pushButton_8_clicked()
{
    double g = ui->lcdNumber_2->value()-0.5;
    QString str = QString::number(g, 'f', 1);
    ui->lcdNumber_2->display(str);
}

void livingroom::showTime()
{
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm:ss");
    if(showColon)
    {
        text[2] = ':';
        text[5] = ':';
        showColon = false;
    }
    else
    {
        text[2] = ' ';
        text[5] = ' ';
        showColon = true;
    }
    ui->lcdNumber_3->display(text);

}

void livingroom::on_pushButton_22_clicked()
{
    QString b=tr("%1").arg(player->position());
    qDebug()<< b;

    player->setPosition(player->position()+5000);

}

void livingroom::on_pushButton_21_clicked()
{
    QString b=tr("%1").arg(player->position());
    qDebug()<< b;

    player->setPosition(player->position()-5000);
}
