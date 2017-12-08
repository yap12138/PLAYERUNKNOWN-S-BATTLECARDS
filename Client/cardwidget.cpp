#include "cardwidget.h"
#include "ui_cardwidget.h"

CardWidget::CardWidget(QWidget *parent, int mode) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
}

CardWidget::~CardWidget()
{
    delete ui;
}
