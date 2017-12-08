#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(QWidget *parent = 0, int mode = 1);
    ~CardWidget();

    void setImage(const QPixmap &img);

public:
    QLabel* _consume;
    QLabel* _attack;
private:
    Ui::CardWidget *ui;
    QLabel* _card_bg;
    QLabel* _consume_bg;
    QLabel* _attack_bg;

    QLabel* _card;

    void setUpUi(int mode);
};

#endif // CARDWIDGET_H
