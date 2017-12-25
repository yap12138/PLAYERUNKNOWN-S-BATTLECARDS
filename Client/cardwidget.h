#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <carddeck.h>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
//#include "mainwidget.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(QWidget *parent = 0, int mode = 1);
    ~CardWidget();

    inline const QPixmap & getImage() const { return this->_s_card; }

    void setImage(const QPixmap &var_img);
    void show(int mode);
    void setOpacity(int level);

signals:
    void SMyPointer(CardWidget*);

public:
    QLabel* _consume;
    QLabel* _attack;
    QLabel* _weapon;

    QLabel* _weapon_bg;
    QLabel* _attack_bg;

    Card* _realCard = nullptr;
private:
    Ui::CardWidget *ui;

    QLabel* _card_bg;
    QLabel* _consume_bg; 

    QLabel* _card;

    QPixmap _s_card_bg = QPixmap(":/cardWidget/resoure/img/card_bg.png");
    QPixmap _s_consume_bg = QPixmap(":/cardWidget/resoure/img/consume.png");
    QPixmap _s_attack_bg = QPixmap(":/cardWidget/resoure/img/attack.png");
    QPixmap _s_weapon_bg = QPixmap(":/cardWidget/resoure/img/attack.png");
    QPixmap _s_card;

    QPixmap _a_card_bg = QPixmap(_s_card_bg.size());
    QPixmap _a_consume_bg = QPixmap(_s_consume_bg.size());
    QPixmap _a_attack_bg = QPixmap(_s_attack_bg.size());
    QPixmap _a_weapon_bg = QPixmap(_s_weapon_bg.size());
    QPixmap _a_card;

    void initAlpha();
    void setUpUi(int mode);
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // CARDWIDGET_H
