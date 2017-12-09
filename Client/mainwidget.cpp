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

    InitMap();
    TestGetCard();
    TestPlayCard();
    ui->pushButton->setEnabled(false);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(TestGetCard()));

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
//    QList<Card> list;
//    QList<Card*> deck;
//    foreach (auto var, list) {
//        Card * temp = &var;
//        deck.
//    };
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

void Mainwidget::InitMap()
{
    /*
     * 手牌位置的map
     * */
    this->handMap[0].x = 200;
    this->handMap[0].y = 650;
    this->handMap[0].hasCard = false;

    this->handMap[1].x = 400;
    this->handMap[1].y = 650;
    this->handMap[1].hasCard = false;

    this->handMap[2].x = 600;
    this->handMap[2].y = 650;
    this->handMap[2].hasCard = false;

    this->handMap[3].x = 800;
    this->handMap[3].y = 650;
    this->handMap[3].hasCard = false;

    this->handMap[4].x = 1000;
    this->handMap[4].y = 650;
    this->handMap[4].hasCard = false;

    /*
     * 己方场上怪物位置的map
     **/

    this->fieldMap[0].x = 450;
    this->fieldMap[0].y = 410;
    this->fieldMap[0].hasCard = false;

    this->fieldMap[1].x = 700;
    this->fieldMap[1].y = 410;
    this->fieldMap[1].hasCard = false;

    this->fieldMap[2].x = 950;
    this->fieldMap[2].y = 410;
    this->fieldMap[2].hasCard = false;
}


void Mainwidget::initialazation()
{
    Mainwidget::_card[0] = new QLabel(this);
    Mainwidget::_card[1] = new QLabel(this);
    Mainwidget::_card[2] = new QLabel(this);
    Mainwidget::_card[3] = new QLabel(this);
    Mainwidget::_card[4] = new QLabel(this);

    //设置可以捕捉鼠标移动事件
    _card[0]->setMouseTracking(true);
    _card[1]->setMouseTracking(true);
    _card[2]->setMouseTracking(true);
    _card[3]->setMouseTracking(true);
    _card[4]->setMouseTracking(true);

    //设置手牌的位置
    _card[0]->setGeometry(200,650,150,200);
    _card[1]->setGeometry(400,650,150,200);
    _card[2]->setGeometry(600,650,150,200);
    _card[3]->setGeometry(800,650,150,200);
    _card[4]->setGeometry(1000,650,150,200);

    //设置手牌背景颜色
    _card[0]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[1]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[2]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[3]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card[4]->setStyleSheet("background-color:rgb(255, 131, 197)");

    //设置界面右边展示手牌详细信息的label
    Mainwidget::_detailedCard[0] = new QLabel(this);
    Mainwidget::_detailedCard[1] = new QLabel(this);

    _detailedCard[0]->setGeometry(1280,0,300,400);
    _detailedCard[1]->setGeometry(1280,400,300,450);
    _detailedCard[0]->setStyleSheet("background-color:rgb(255,131,197)");
    _detailedCard[1]->setStyleSheet("background-color:rgb(85,255,0)");

    //自己出的卡，为了指示卡片位置和大小
    Mainwidget::_card1[0] = new QLabel(this);
    Mainwidget::_card1[1] = new QLabel(this);
    Mainwidget::_card1[2] = new QLabel(this);


    _card1[0]->setGeometry(450,410,150,200);
    _card1[1]->setGeometry(700,410,150,200);
    _card1[2]->setGeometry(950,410,150,200);


    _card1[0]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card1[1]->setStyleSheet("background-color:rgb(255, 131, 197)");
    _card1[2]->setStyleSheet("background-color:rgb(255, 131, 197)");


    setBackground();

}

void Mainwidget::setBackground()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int randNum;
    randNum = qrand() % 3;
    qDebug()<<randNum;
    switch (randNum) {
    case 0:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/resoure/img/bg1.jpg)}");
        break;
    case 1:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/resoure/img/bg2.jpg)}");
        break;
    case 2:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/resoure/img/bg3.jpg)}");

        break;
    default:
        break;
    }
}


void Mainwidget::TestGetCard()
{
    /**
      *获得新卡的操作，部分代码可重用
      *
      */
    if (this->_mPlayer.cardInHand.count()<5){
        CardWidget* newCardWidget = new CardWidget(this,1);
        Magic_FireBall* f= new Magic_FireBall();
        this->_mPlayer.cardInHand.insert(f,newCardWidget);
        //this->_mPlayer.idCard.insert(f->getID(),f);，
        for(int i = 0;i<5;i++){
            if (this->handMap[i].hasCard == false){
                newCardWidget->setGeometry(this->handMap[i].x,this->handMap[i].y,newCardWidget->width(),newCardWidget->height());
                this->handMap[i].hasCard = true;
                break;
            }
        }
        newCardWidget->show();
        connect(newCardWidget,SIGNAL(SMyPointer(CardWidget*)),this,SLOT(OnWidgetClicked(CardWidget*)));
        this->ui->pushButton->setEnabled(false);
    }
}

void Mainwidget::TestPlayCard()
{
    if (this->_mPlayer.battleField.count()<3){
        CardWidget* newCardWidget = new CardWidget(this,1);
        Magic_KingSpell* k = new Magic_KingSpell();
        this->_mPlayer.battleField.insert(k,newCardWidget);
        for(int i = 0;i<5;i++){
            if (this->fieldMap[i].hasCard == false){
                newCardWidget->setGeometry(this->fieldMap[i].x,this->fieldMap[i].y,newCardWidget->width(),newCardWidget->height());
                this->fieldMap[i].hasCard = true;
                break;
            }
        }
        newCardWidget->show();
        connect(newCardWidget,SIGNAL(SMyPointer(CardWidget*)),this,SLOT(OnWidgetClicked(CardWidget*)));
    }

}

void Mainwidget::TestDeleteCard()
{
    this->_mPlayer.cardInHand.remove(this->_mPlayer.cardInHand.key(this->selectCard));
}

void Mainwidget::OnWidgetClicked(CardWidget* widget)
{
    if (ChooseTarget){
        this->targetCard = widget;
    }
    else
    {
        this->selectCard = widget;
        if (this->_mPlayer.cardInHand.key(widget,nullptr)!=nullptr)//如果这场卡属于手牌
        {
        ui->pushButton->setEnabled(true);
        }
        else {
            qDebug()<<"not belong to cardInHand!";
        }
    }

}
