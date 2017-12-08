#include "mainwidget.h"
#include "ui_mainwidget.h"


Mainwidget::Mainwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initialazation();
//    _connectUi = new ConnectWidget(this);
//    _connectUi->show();
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(Test()));
}

Mainwidget::~Mainwidget()
{
    delete ui;
//  delete _mylabelMap;
//    delete _connectUi;
}

//void Mainwidget::TurnStart(int consume, int category, Card &newCard)
//{

//}

//void Mainwidget::GetNewCard(Card& newCard)
//{
//    //TODO 获得新卡片时执行的槽函数，category来源于解析到的数据
//}

//void Mainwidget::TurnEnd()
//{
//    //TODO 发送回合结束的报文
//}



//void Mainwidget::GameStart(const QString &name)
//{

//}

//void Mainwidget::EnemyPlayerStatusChanged(int hp, int consume)
//{

//}

//void Mainwidget::EnemyPlayCard(int category, int cardID, int target)
//{

//}

//void Mainwidget::ArmsDamaged(int attacked_id, int attack)
//{

//}

//void Mainwidget::MonsterDamaged(int attacked_id, int hp)
//{

//}

//void Mainwidget::CreateCard()
//{
////    QList<Card> list;
////    QList<Card*> deck;
////    foreach (auto var, list) {
////        Card * temp = &var;
////        deck.
////    };
//}


//void Mainwidget::TauntSkill(int monsterID)
//{

//}

//void Mainwidget::PlayCard(int cardID)
//{
//    //TODO 本地更新UI，向服务器发送报文
//}

//void Mainwidget::MyStatusChanged(int hp,int consume)
//{

//}

//void Mainwidget::Disconnected()
//{

//}

void Mainwidget::onReadyRead()//准备读取
{
    QDataStream in(_client);
    in.setVersion(QDataStream::Qt_5_9);
    int msgCategory;
    in>>msgCategory;
    QString s;
    switch (msgCategory) {
    case 0: //获取对手名字
        in>>s;
        qDebug()<<s;
        qDebug()<<"_enemyName";
        break;
    case 1: //回合开始              得费，发牌

        break;
    case 2: //对方任务状态更新       血量，费用消耗
        break;
    case 3: //对方出牌              Category,出的牌id，己/敌方id
        break;
    case 4: //武器受损              怪id，武器攻击值
        break;
    case 5: //怪物受损              怪id，怪攻击值
        break;
    case 6: //初始发牌              出牌顺序，QList
        break;
    case 7: //技能                 发动技能的怪id
        break;
    case 8: //敌方断开
        break;
    case 9: //获得牌               Card
        break;
    default:
        break;
    }

}

void Mainwidget::sendMessage()//发送信息
{

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

void Mainwidget::Test()
{
    if (this->_mPlayer.cardInHand.count()<5){
        if (this->_mPlayer.cardInHand.isEmpty()){
            CardWidget* newCard = new CardWidget(this,1);
            newCard->setGeometry(200,650,newCard->width(),newCard->height());
            newCard->show();
            Magic_FireBall* f= new Magic_FireBall();
            this->_mPlayer.cardInHand.insert(f,newCard);
            this->_lastHandX = newCard->x()+newCard->width()+this->_space;
            this->_lastHandY = newCard->y();
        }
        else {
            CardWidget* newCard = new CardWidget(this,1);
            newCard->setGeometry(this->_lastHandX,this->_lastHandY,newCard->width(),newCard->height());
            newCard->show();
            Magic_FireBall* f= new Magic_FireBall();
            this->_mPlayer.cardInHand.insert(f,newCard);
            this->_lastHandX = newCard->x()+newCard->width()+this->_space;
            this->_lastHandY = newCard->y();
        }
    }
}
