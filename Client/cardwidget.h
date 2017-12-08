#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(QWidget *parent = 0, int mode = 1);
    ~CardWidget();

private:
    Ui::CardWidget *ui;
};

#endif // CARDWIDGET_H
