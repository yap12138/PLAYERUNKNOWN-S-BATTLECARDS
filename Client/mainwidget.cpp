#include "mainwidget.h"
#include "ui_mainwidget.h"

Mainwidget::Mainwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    /*
    Widget::_firstCard = new QHBoxLayout(this);
    Widget::_secondCard = new QHBoxLayout(this);
    Widget::_thirdCard = new QHBoxLayout(this);
    */
    initialazation();

}

Mainwidget::~Mainwidget()
{
    delete ui;
}

void Mainwidget::initialazation()
{
    Mainwidget::_card[0] = new QLabel(this);
    Mainwidget::_card[1] = new QLabel(this);
    Mainwidget::_card[2] = new QLabel(this);
    Mainwidget::_card[3] = new QLabel(this);
    Mainwidget::_card[4] = new QLabel(this);

    _card[0]->setGeometry(200,650,150,200);
    _card[1]->setGeometry(400,650,150,200);
    _card[2]->setGeometry(600,650,150,200);
    _card[3]->setGeometry(800,650,150,200);
    _card[4]->setGeometry(1000,650,150,200);

    //_card[0]->setText("card");

    _card[0]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[1]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[2]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[3]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[4]->setStyleSheet("background-color:rgb(255, 131, 197)");

    Mainwidget::_detailedCard[0] = new QLabel(this);
    Mainwidget::_detailedCard[1] = new QLabel(this);

    _detailedCard[0]->setGeometry(1300,50,300,400);
    _detailedCard[1]->setGeometry(1300,450,300,400);
    _detailedCard[0]->setStyleSheet("background-color:rgb(255,131,197)");
    _detailedCard[1]->setStyleSheet("background-color:rgb(85,255,0)");

                                          //自己出的卡，为了指示卡片位置和大小
    Mainwidget::_card1[0] = new QLabel(this);
    Mainwidget::_card1[1] = new QLabel(this);
    Mainwidget::_card1[2] = new QLabel(this);


    _card1[0]->setGeometry(450,350,150,200);
    _card1[1]->setGeometry(700,350,150,200);
    _card1[2]->setGeometry(950,350,150,200);


    //_card[0]->setText("card");

    _card1[0]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card1[1]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card1[2]->setStyleSheet("background-color:rgb(255, 131, 197)");


}
