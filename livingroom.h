#ifndef LIVINGROOM_H
#define LIVINGROOM_H

#include <QWidget>
#include <QUrl>
//要包含下面的两个文件，必须在.pro文件中添加  QT += multimedia  multimediawidgets
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QTime>
#include <QTimer>
#include <QMouseEvent>

namespace Ui {
class livingroom;
}

class livingroom : public QWidget
{
    Q_OBJECT

public:
    explicit livingroom(QWidget *parent = nullptr);
    ~livingroom();

private:
    Ui::livingroom *ui;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    QPoint dragPosition;    //保存鼠标点相对电子时钟窗体左上角的偏移值
    bool showColon;         //用于显示冒号

private slots:
    void onGobackPushButtonSlots();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void  showTime();       //显示当前的时间

    void on_pushButton_22_clicked();

    void on_pushButton_21_clicked();

signals:
    void sendData1(); //传递数据的信号
};

#endif // LIVINGROOM_H
