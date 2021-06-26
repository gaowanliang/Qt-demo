#ifndef RGSWIDGET_H
#define RGSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QUdpSocket>
namespace Ui {
class rgsWidget;
}

class rgsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit rgsWidget(QWidget *parent = nullptr);
    ~rgsWidget();

private slots:
    void onRgsPushButtonSlots();
    //套接字的槽函数
    void onSocketReadyRead();

private:
    Ui::rgsWidget *ui;
    QVBoxLayout *rgsLayout;
    QPushButton *rgsPushButton;
    QLineEdit *rgsCntLineEdit;
    QLineEdit *rgsPwdLineEdit;

    QUdpSocket *sd;


};

#endif // RGSWIDGET_H
