#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {
class Widget;
}

class Mainwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Mainwidget(QWidget *parent = 0);
    ~Mainwidget();

private:
    Ui::Widget *ui;
    QLabel *_card[5];          //初始时自己手上的四张牌，加载界面时加载出来
    QLabel *_detailedCard[2]; //右侧显示详细的卡片信息，_detailedCard[0]是右上放大版的卡片，_detailedCard[1]是右下以文字形式显示卡片内容的标签
    QLabel *_card1[3];      //自己出的卡，不是加载界面时就生成，这里写只是为了指示卡片具体大小和位置

private:
    void initialazation();  //初始化界面
};

#endif // WIDGET_H
