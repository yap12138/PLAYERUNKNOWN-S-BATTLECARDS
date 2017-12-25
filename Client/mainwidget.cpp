#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QMessageBox>

Mainwidget::Mainwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initDetailArea();

    _client = new QTcpSocket(this);
    _connectUi = new ConnectWidget(_client,this);
    //@yap
    initConnect();
    initMap();
    setBackground();
}

Mainwidget::~Mainwidget()
{
    delete ui;
}

//@yap
bool Mainwidget::eventFilter(QObject *watched, QEvent *event)
{
    CardWidget * cw = dynamic_cast<CardWidget*> (watched);
    if (event->type() == QEvent::Enter)
    {
        cw->setGeometry(cw->x(), cw->y()-40, cw->width(), cw->height());
        Card * card = nullptr;
        if (this->_me.cardInHand.values().contains(cw))
            card = this->_me.cardInHand.key(cw, nullptr);
        else if (this->_me.battleField.values().contains(cw))
            card = this->_me.battleField.key(cw, nullptr);
        else
            card = cw->_realCard;
        if (card == nullptr)
            return true;
        ui->_detailName->setText(card->getName());
        ui->_detailDescription->setText(card->getDescription());
        this->_detailCard->_consume->setText(QString::number(card->getConsume()));
        this->_detailCard->setImage(*(cw->getImage()));
        this->_detailCard->_weapon_bg->hide();  //先隐藏武器栏
        this->_detailCard->_weapon->setText(QStringLiteral(""));
        if (dynamic_cast<MagicCard *>(card) == nullptr)
        {
            MonsterCard * t1;
            this->_detailCard->_attack_bg->show();
            if ((t1=dynamic_cast<MonsterCard *>(card)) != nullptr)
            {
                this->_detailCard->_attack->setText(QString::number(t1->getAttack()));
                if (t1->getArms() != nullptr)
                {
                    this->_detailCard->_weapon_bg->show();  //有武器则将武器栏显示出来
                    this->_detailCard->_weapon->setText(QString::number(t1->getArms()->getAttackBuf()));
                    ui->_detailName->setText(ui->_detailName->text() + QStringLiteral(" (装备：") + t1->getArms()->getName() + QStringLiteral(" )"));
                }
            }
            else
            {
                ArmsCard * t2 = dynamic_cast<ArmsCard *>(card);
                this->_detailCard->_attack->setText(QString::number(t2->getAttackBuf()));
            }
        }
        else
        {
            this->_detailCard->_attack_bg->hide();
            this->_detailCard->_attack->setText(QStringLiteral(""));
        }
    }
    else if(event->type() == QEvent::Leave)
    {
        cw->setGeometry(cw->x(), cw->y()+40, cw->width(), cw->height());
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        if (lastCard != nullptr)
            //lastCard->setAutoFillBackground(false);
            lastCard->setOpacity(1);
        //cw->setAutoFillBackground(true);
        cw->setOpacity(0);
        lastCard = cw;
    }
    return QWidget::eventFilter(watched, event);
}

void Mainwidget::initConnect()
{
    connect(_client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_client, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(ui->_playCard,SIGNAL(clicked(bool)),this,SLOT(PlayCard()));
    connect(ui->_attackMonster,SIGNAL(clicked(bool)),this,SLOT(AttackMonster()));
    connect(ui->_attackPlayer,SIGNAL(clicked(bool)),this,SLOT(AttackPlayer()));
    connect(ui->_turnEnd,SIGNAL(clicked(bool)),this,SLOT(TurnEnd()));
    connect(ui->_cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

void Mainwidget::initDetailArea()
{
    _detailCard = new CardWidget(this,2);
    _detailCard->setGeometry(1280,0,_detailCard->width(),_detailCard->height());
    // 设置卡详情背景半透明
    QPalette myPalette;
    QColor myColor(255,255,255);
    myColor.setAlphaF(0.3);
    myPalette.setBrush(backgroundRole(),myColor);
    _detailCard->setPalette(myPalette);
    _detailCard->setAutoFillBackground(true);
    _detailCard->setCursor(Qt::ArrowCursor);
    this->ui->_detailName->setPalette(myPalette);
    this->ui->_detailName->setAutoFillBackground(true);
    this->ui->_detailDescription->setPalette(myPalette);
    this->ui->_detailDescription->setAutoFillBackground(true);
}

void Mainwidget::initMap()
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

    this->enemyfieldMap[0].x = 450;
    this->enemyfieldMap[0].y = 150;
    this->enemyfieldMap[0].hasCard = false;

    this->enemyfieldMap[1].x = 700;
    this->enemyfieldMap[1].y = 150;
    this->enemyfieldMap[1].hasCard = false;

    this->enemyfieldMap[2].x = 950;
    this->enemyfieldMap[2].y = 150;
    this->enemyfieldMap[2].hasCard = false;
}

void Mainwidget::setBackground()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int randNum;
    randNum = qrand() % 3;
    switch (randNum) {
    case 0:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/background/resoure/img/bg1.jpg)}");
        break;
    case 1:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/background/resoure/img/bg2.jpg)}");
        break;
    case 2:
        this->ui->_backgroud->setStyleSheet("QLabel{background-image: url(:/background/resoure/img/bg3.jpg)}");

        break;
    default:
        break;
    }
}

//回合开始              得费，发牌
void Mainwidget::TurnStart(QDataStream &stream)
{
    //TODO int consume, int category ,Card &newCard
    int consume;
    stream>>consume;
    this->_me.consume += consume;
    if (this->_me.oddCards > 0)
    {
        int category;
        stream>>category;
        GetNewCard(stream,category);
    }

    //更新UI
    this->ui->label_my_consume->setText(QString::number(this->_me.consume));
    EnableAllBtn();
    EnableAllWidget();

    foreach (auto var, this->_me.battleField.keys()){
        dynamic_cast<MonsterCard*>(var)->setHasAttack(false);
    }

    //@yap 日志
    this->ui->_battleLog->append(QStringLiteral("己方：回合开始"));
}

//向手牌组里添加新卡
void Mainwidget::GetNewCard(QDataStream &stream,int category)
{
    Card *m;
    QString filename;//根据category为filename指定相应的图片路径
    filename = getCardImg(category);
    switch (category) {
    case 10:
        m = new Monster_DragonBorn();
        break;
    case 11:
        m = new Monster_DogInScau();
        break;
    case 12:
        m = new Monster_CrazyCow();
        break;
    case 13:
        m = new Monster_Devil();
        break;
    case 14:
        m = new Monster_WhiteFriend();
        break;
    case 15:
        m = new Monster_Peashooter();
        break;
    case 20:
        m = new Magic_FireBall();
        break;
    case 21:
        m = new Magic_WakeUp();
        break;
    case 22:
        m = new Magic_KingSpell();
        break;
    case 23:
        m = new Magic_WindStrom();
        break;
    case 24:
        m = new Magic_AGiftFromTeacher();
        break;
    case 31:
        m = new Arms_98K();
        break;
    case 32:
        m = new Arms_InfinityEdge();
        break;
    case 33:
        m = new Arms_ArchangelSword();
        break;
    case 34:
        m = new Arms_DeathfireStaff();
        break;
    case 35:
        m = new Arms_Nokia();
        break;
    default:
        break;
    }
    stream>>*m;
    if(this->_me.cardInHand.size()<5)
    {
        //更新UI
        qDebug()<<m->getName();
        //UI更新，插入到手牌组
        SetHandCardUI(m,filename);
    }
    this->_me.oddCards--;
    this->ui->_numOfCards_m->setText(QString::number(this->_me.oddCards));
}

//发送回合结束的报文
void Mainwidget::TurnEnd()
{
    QDataStream stream(_client);
    stream.setVersion(QDataStream::Qt_5_9);
    stream<<2;
    _client->flush();
    Cancel();
    DisableAllBtn();

    this->_enemy.oddCards--;
    this->ui->_numOfCards_e->setText(QString::number(_enemy.oddCards));

    this->ui->_battleLog->append(QStringLiteral("敌方: 回合开始"));
}

//出牌
void Mainwidget::PlayCard()
{
    Card* usedCard = const_cast<Card*>(this->_me.cardInHand.key(this->selectCard,nullptr));//选中的手牌
    //@yap
    CardWidget* usedCardWidget = this->selectCard;//选中的手牌的UI
    int target = 0;
    int myID = 0;
    if(usedCard != nullptr) {
        int newConsume = this->_me.consume-usedCard->getConsume();
        if (ChooseTarget){//选择目标阶段
            if (this->targetCard != nullptr)//如果已经选择了目标
            {
                QString log_str = QStringLiteral("己方：");
                //选择的目标就是targetCard,根据具体是哪一张卡，再进行相关的操作
                //根据具体是哪一张卡，再进行相关的操作
                if (dynamic_cast<MagicCard*>(usedCard)!=nullptr){
                    qDebug()<<"magic";
                    switch (usedCard->getCategory()) {
                    case 20://火球术
                        target = this->_enemy.battleField.key(this->targetCard);
                        myID = usedCard->getId();
                        //日志
                        log_str = log_str + QStringLiteral("使用 ") + usedCard->getName() + QStringLiteral(" 对敌方 ")
                                + this->_enemy.battleField[target]->_realCard->getName() + QStringLiteral(" 造成6点伤害");
                        break;
                    case 22://王者咏唱
                        qDebug()<<".";
                        target = this->_me.battlerID.key(this->_me.battleField.key(this->targetCard));
                        myID = usedCard->getId();
                        qDebug()<<"..";
                        //日志
                        log_str = log_str + QStringLiteral("使用 ") + usedCard->getName() + QStringLiteral(" 给 ")
                                + this->_me.battlerID[target]->getName() + QStringLiteral(" 增加4点攻击力");
                    }
                }
                else if(dynamic_cast<ArmsCard*>(usedCard)!= nullptr){
                    qDebug()<<"arms";
                    MonsterCard* targetMonster = const_cast<MonsterCard*>(dynamic_cast<MonsterCard*>(this->_me.battleField.key(this->targetCard)));
                    *targetMonster + *dynamic_cast<ArmsCard*>(usedCard);
                    //根据武器卡的类别 给怪兽设置上去
                    //更新怪兽的UI
                    targetCard->_weapon_bg->show();
//                    this->selectCard->_weapon_bg->show();
//                    this->selectCard->_attack->setText(QString::number(targetMonster->getAttack()));
                    //@yap
                    myID = usedCard->getId();
                    target = targetMonster->getId();

                    log_str = log_str + QStringLiteral("给 ") + targetMonster->getName()
                            + QStringLiteral(" 装备上了 ") + usedCard->getName();
                }
                ChooseTarget = false; //终止选择目标阶段
                //恢复出牌按钮的文字为“出牌”
                ui->_playCard->setText(QStringLiteral("出牌"));
                //把手牌从UI以及结构里删除
                DeleteHandCard(usedCardWidget,usedCard);
                qDebug()<<"delete";
                //发送出牌报文
                sendMessage(myID,target);
                MyConsumeChangedUI(this->_me.hp,newConsume);
                //发出日志
                this->ui->_battleLog->append(log_str);
                EnableAllWidget();//把所有的点击重新有效化
                this->selectCard = nullptr;
                this->targetCard = nullptr;
            }
        }
        else //非选择目标阶段
        {
            qDebug()<<"2 jieduan";
            if ( newConsume>=0){ //如果够费打出
                //非选择目标阶段
                //打出怪兽牌放置在场上，打出魔法或者装备卡，详细信息输出在右边的详细列表
                //如果是怪兽牌，放置在场上
                if ( (dynamic_cast<MonsterCard*>(usedCard)!= nullptr)){

                    //@yap <3
                    if (this->_me.battleField.count()<3 ){
                        //构建新的卡牌部件，并且加入到场上的组
                        MonsterCard* newMonster = dynamic_cast<MonsterCard*>(usedCard);
                        CardWidget* newCardWidget = new CardWidget(this,1);
                        //@yap 注册过滤器
                        newCardWidget->installEventFilter(this);
                        this->_me.battleField.insert(usedCard,newCardWidget);
                        for(int i = 0;i<5;i++){
                            if (this->fieldMap[i].hasCard == false){
                                newCardWidget->setGeometry(this->fieldMap[i].x,this->fieldMap[i].y,newCardWidget->width(),newCardWidget->height());
                                this->fieldMap[i].hasCard = true;
                                break;
                            }
                        }
                        newCardWidget->setImage(QPixmap(getCardImg(newMonster->getCategory())));
                        //设置怪兽的HP
                        newCardWidget->_attack->setText(QString::number(newMonster->getAttack()));
                        newCardWidget->show(1);
                        connect(newCardWidget,SIGNAL(SMyPointer(CardWidget*)),this,SLOT(OnWidgetClicked(CardWidget*)));
                        //删除手牌卡片UI
                        //从手牌地图中删除
                        DeleteHandCard(usedCardWidget,usedCard);
                        // 怪兽打到场上
                        myID = usedCard->getId();
                        target = -2;
                        sendMessage(myID,target);
                        MyConsumeChangedUI(this->_me.hp,newConsume);
                        //日志
                        this->ui->_battleLog->append(QStringLiteral("己方：") + QStringLiteral("使用 ") + usedCard->getName());
                        this->selectCard = nullptr;
                    }
                }
                else if (dynamic_cast<MagicCard*>(usedCard)!= nullptr){
                    //如果要打出的是魔法卡添加战斗日志
                    QString log_str = QStringLiteral("己方：");
                    switch (usedCard->getCategory()) {
                        case 20:
                        {
                            //火球术
                            // 要选取目标，无效化己方怪兽点击及手牌点击
                            DisableMyMonster();
                            DisableCard();
                            this->ChooseTarget = true;//进入选牌阶段
                            ui->_playCard->setText(QStringLiteral("确认"));
                            break;
                        }
                        case 21:
                        {
                            //苏醒
                            //删除卡片UI
                            //从手牌地图中删除
                            myID = usedCard->getId();
                            target = usedCard->getId();
                            sendMessage(myID,target);
                            MyConsumeChangedUI(this->_me.hp,newConsume);
                            log_str = log_str + QStringLiteral("使用 ") + usedCard->getName() + QStringLiteral(" 回复两点法力");
                            DeleteHandCard(usedCardWidget,usedCard);
                            this->selectCard = nullptr;
                            this->ui->_battleLog->append(log_str);
                            break;
                        }
                        case 22:
                        {
                            //王者咏唱
                            // 要选取目标，无效化敌方怪兽点击及手牌点击
                            DisableEnemyMonster();
                            DisableCard();
                            //出牌变为确认
                            this->ChooseTarget = true;//进入选牌阶段
                            ui->_playCard->setText(QStringLiteral("确认"));
                            break;
                        }
                        case 23:
                        {
                            //风暴
                            // 发送出牌报文
                            myID = usedCard->getId();
                            QList<int> list = this->_enemy.battleField.keys();
                            if (!list.isEmpty()){//如果列表不为空
                                for (int i = 0;i<list.count();i++){
                                    target = list.value(i);
                                    sendMessage(myID,target);

                                    log_str = QStringLiteral("己方：") + QStringLiteral("使用 ") + usedCard->getName() + QStringLiteral(" 对敌方 ")
                                            + this->_enemy.battleField[target]->_realCard->getName() + QStringLiteral(" 造成4点伤害");
                                    this->ui->_battleLog->append(log_str);
                                }
                                MyConsumeChangedUI(this->_me.hp,newConsume);
                                //删除手牌卡片UI
                                //从手牌地图中删除
                                DeleteHandCard(usedCardWidget,usedCard);
                                this->selectCard = nullptr;
                            }
                            break;
                        }
                        case 24: //老师的馈赠
                            // 发送出牌报文
                        {
                            qDebug()<<"k.";
                            myID = usedCard->getId();
                            target = usedCard->getId();
                            qDebug()<<"k..";
                            sendMessage(myID,target);
                            MyConsumeChangedUI(this->_me.hp,newConsume);
                            //删除卡片UI
                            //从手牌地图中删除
                            log_str = log_str + QStringLiteral("使用 ") + usedCard->getName() + QStringLiteral(" 本回合获得10法力");
                            DeleteHandCard(usedCardWidget,usedCard);
                            this->selectCard = nullptr;
                            qDebug()<<"k...";
                            this->ui->_battleLog->append(log_str);
                            break;
                        }
                    }
                }
                else if (dynamic_cast<ArmsCard*>(usedCard)!=nullptr){
                    //TODO 如果要打出的是装备卡，要选取目标，无效化敌方怪兽点击及手牌点击
                    DisableEnemyMonster();
                    DisableCard();
                    //出牌变为确认
                    this->ChooseTarget = true;//进入选牌阶段
                    ui->_playCard->setText(QStringLiteral("确认"));
                }

            }
        }
        //@yap
        //qDebug()<<usedCard->getName();
    }
    qDebug()<<"p c size:"<<_me.cardInHand.size();
}

//获取对手名字
void Mainwidget:: GameStart(QDataStream &stream)
{
    stream>>_enemy.name;
    //@yap 增加谁先出，牌组大小
    int whoToFirst, sizeofCardDeck;
    stream>>whoToFirst;
    stream>>sizeofCardDeck;
    this->_me.oddCards = sizeofCardDeck;
    this->_enemy.oddCards = sizeofCardDeck;
    this->_me.name = _connectUi->getName();
    //@yap
    _connectUi->resetWidget();
    this->ui->p1_name->setText(this->_me.name);
    this->ui->p2_name->setText(this->_enemy.name);
    //更新UI
    DisableAllBtn();
    DisableEnemyMonster();
    DisableMyMonster();
    DisableCard();
    this->_enemy.oddCards -= 4;
    this->ui->_numOfCards_e->setText(QString::number(_enemy.oddCards));
    //日志
    this->ui->_battleLog->append(QStringLiteral("游戏开始"));
    if (whoToFirst == 1)
    {
        this->ui->_battleLog->append(QStringLiteral("敌方：回合开始"));
        this->_enemy.oddCards --;
        this->ui->_numOfCards_e->setText(QString::number(_enemy.oddCards));
    }
}

//人物状态更新       血量，费用消耗
void Mainwidget::PlayerStatusChanged(QDataStream &stream)
{
    //int hp, int consume
    int sign;
    stream>>sign;
    if(sign == 0)
    {
        stream>>this->_me.hp;
        stream>>this->_me.consume;
        this->ui->label_my_hp->setText(QString::number(this->_me.hp));
        this->ui->label_my_consume->setText(QString::number(this->_me.consume));
    }
    else if(sign == 1)
    {
        stream>>this->_enemy.hp;
        stream>>this->_enemy.consume;
        qDebug()<<_enemy.hp;
        qDebug()<<_enemy.consume;
        this->ui->label_enemy_hp->setText(QString::number(this->_enemy.hp));
        this->ui->label_enemy_consume->setText(QString::number(this->_enemy.consume));
    }
}

//对方出牌              Category,出的牌id，己/敌方id
void Mainwidget::EnemyPlayCard(QDataStream &stream)
{
    //@yap log
    QString log_str = QStringLiteral("敌方：");

    int category;
    int cardID;
    int target;
    stream>>category;
    stream>>cardID;
    stream>>target;
    Card *m = nullptr;

    if(target==-2)
    {

        QString filename = getCardImg(category);
        switch (category) {
        case 10:
            m = new Monster_DragonBorn();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 11:
            m = new Monster_DogInScau();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 12:
            m = new Monster_CrazyCow();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 13:
            m = new Monster_Devil();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 14:
            m = new Monster_WhiteFriend();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 15:
            m = new Monster_Peashooter();
            SetEnemyMonsterUI(cardID,m,filename);
            break;
        case 20:
            m = new Magic_FireBall();

            //@yap 以下都注释了
//            //SetDetailedCard(m,filename);
//            break;
//        case 21:
//            m = new Magic_WakeUp();
//            //SetDetailedCard(m,filename);
//            break;
//        case 22:
//            m = new Magic_KingSpell();
//            //SetDetailedCard(m,filename);
//            break;
//        case 23:
//            m = new Magic_WindStrom();
//            //SetDetailedCard(m,filename);
//            break;
//        case 24:
//            m = new Magic_AGiftFromTeacher();
//            //SetDetailedCard(m,filename);
//            break;
//        case 31:
//            m = new Arms_98K();
//            //SetDetailedCard(m,filename);
//            break;
//        case 32:
//            m = new Arms_InfinityEdge();
//            //SetDetailedCard(m,filename);
//            break;
//        case 33:
//            m = new Arms_ArchangelSword();
//            //SetDetailedCard(m,filename);
//            break;
//        case 34:
//            m = new Arms_DeathfireStaff();
//            //SetDetailedCard(m,filename);
//            break;
//        case 35:
//            m = new Arms_Nokia();
//            //SetDetailedCard(m,filename);
//            break;
        default:
            break;
        }
        log_str = log_str + QStringLiteral("使用 ") + m->getName();
    }
    else if(target == -1)
    {
        //日志 攻击人
        log_str = log_str + this->_enemy.battleField[cardID]->_realCard->getName() + QStringLiteral(" 攻击 ") + QStringLiteral("你");
    }
    else
    {
        //日志
        switch (category)
        {
        //魔法卡
        case 20:
            m = new Magic_FireBall();
            log_str = log_str + QStringLiteral("使用 ") + m->getName() + QStringLiteral(" 对己方 ")
                    + this->_me.battlerID[target]->getName() + QStringLiteral(" 造成6点伤害");
            break;
        case 21:
            m = new Magic_WakeUp();
            log_str = log_str + QStringLiteral("使用 ") + m->getName() + QStringLiteral(" 回复两点法力");
            break;
        case 22:
            m = new Magic_KingSpell();
            log_str = log_str + QStringLiteral("使用 ") + m->getName() + QStringLiteral(" 给 ")
                    + this->_enemy.battleField[target]->_realCard->getName() + QStringLiteral(" 增加4点攻击力");
            break;
        case 23:
            m = new Magic_WindStrom();
            log_str = log_str + QStringLiteral("使用 ") + m->getName() + QStringLiteral(" 对己方 ")
                    + this->_me.battlerID[target]->getName() + QStringLiteral(" 造成4点伤害");
            break;
        case 24:
            m = new Magic_AGiftFromTeacher();
            log_str = log_str + QStringLiteral("使用 ") + m->getName() + QStringLiteral(" 本回合获得10法力");
            break;

            //装备
        case 31:
            m = new Arms_98K();
            log_str = log_str + QStringLiteral("给 ") + this->_enemy.battleField[target]->_realCard->getName()
                    + QStringLiteral(" 装备上了 ") + m->getName();
            break;
        case 32:
            m = new Arms_InfinityEdge();
            log_str = log_str + QStringLiteral("给 ") + this->_enemy.battleField[target]->_realCard->getName()
                    + QStringLiteral(" 装备上了 ") + m->getName();
            break;
        case 33:
            m = new Arms_ArchangelSword();
            log_str = log_str + QStringLiteral("给 ") + this->_enemy.battleField[target]->_realCard->getName()
                    + QStringLiteral(" 装备上了 ") + m->getName();
            break;
        case 34:
            m = new Arms_DeathfireStaff();
            log_str = log_str + QStringLiteral("给 ") + this->_enemy.battleField[target]->_realCard->getName()
                    + QStringLiteral(" 装备上了 ") + m->getName();
            break;
        case 35:
            m = new Arms_Nokia();
            log_str = log_str + QStringLiteral("给 ") + this->_enemy.battleField[target]->_realCard->getName()
                    + QStringLiteral(" 装备上了 ") + m->getName();
            break;
        default:
            //怪攻击
            log_str = log_str + this->_enemy.battleField[cardID]->_realCard->getName() + QStringLiteral(" 攻击 ")
                    + this->_me.battlerID[target]->getName();
            break;
        }

        if ( category >= 30)
        {
            this->_enemy.battleField[target]->_weapon_bg->show();
            MonsterCard* mc = dynamic_cast<MonsterCard *>(this->_enemy.battleField[target]->_realCard);
            ArmsCard* ac = dynamic_cast<ArmsCard *>(m);
            *mc + *ac;
        }
        else if (m != nullptr)  //如果不是装备牌那就delete掉
            delete m;
    }
    this->ui->_battleLog->append(log_str);
}

//武器受损              怪id，武器攻击值
void Mainwidget::ArmsDamaged(QDataStream &stream)
{
    int attacker_id;//攻击者id
    int attack;     //攻击后的攻击力
    stream>>attacker_id;
    stream>>attack;
    if(this->_me.battlerID.contains(attacker_id))
    {
        //@yap
        MonsterCard * card = dynamic_cast<MonsterCard*>(this->_me.battlerID[attacker_id]);
        if(attack>0)
        {
            const_cast<ArmsCard *>(card->getArms())->setAttackBuf(attack);
            this->_me.battleField[card]->_weapon->setText(QString::number(attack));
        }
        else
        {
            this->_me.battleField[card]->_weapon->setText("");
            this->_me.battleField[card]->_weapon_bg->hide();
            //@yap 卸载武器
            ArmsCard * ac = const_cast<ArmsCard *>(card->getArms());
            card->removeArms();
            delete ac;
        }
    }
    else
    {
        //@yap
        if(attack>0)
        {
            this->_enemy.battleField[attacker_id]->_weapon->setText(QString::number(attack));
        }
        else
        {
            this->_enemy.battleField[attacker_id]->_weapon->setText("");
            this->_enemy.battleField[attacker_id]->_weapon_bg->hide();

            MonsterCard * card = dynamic_cast<MonsterCard*>(this->_enemy.battleField[attacker_id]->_realCard);
            ArmsCard * ac = const_cast<ArmsCard *>(card->getArms());
            card->removeArms();
            delete ac;
        }
    }

}

//怪物受损              怪id，怪攻击值
void Mainwidget::MonsterDamaged(QDataStream &stream)
{
    int attacked_id;
    int hp;
    stream>>attacked_id;
    stream>>hp;
    QString log_str;
    if(this->_me.battlerID.contains(attacked_id))
    {
        log_str = QStringLiteral("己方: ");
        if(hp>0)
        {
            MonsterCard* m =  dynamic_cast<MonsterCard*>(this->_me.battlerID[attacked_id]);
            int damage = m->getAttack()-hp;
            qDebug()<<"受伤"<<damage;
            if (damage>0){
                log_str = log_str + m->getName() + QStringLiteral(" 受到 ")+QString::number(damage) + QStringLiteral("点伤害 ");
            }
            else {
                log_str = log_str + m->getName() + QStringLiteral(" 增加")+QString::number(-damage) + QStringLiteral("点血量 ");
            }

            m->setAttack(hp);
            this->_me.battleField[m]->_attack->setText(QString::number(hp));
        }

        else
        {

            Card *c = this->_me.battlerID[attacked_id];
            log_str = log_str + c->getName() + QStringLiteral(" 死亡");
            qDebug()<<c->getName();
            CardWidget *w = this->_me.battleField[c];
            this->_me.battleField.remove(c);
            for(int i = 0; i < 3; i++)
            {
                if (w->x()==fieldMap[i].x){
                    fieldMap[i].hasCard=false;
                    break;
                }
            }
            if ( lastCard == w )
                lastCard=nullptr;
            this->_me.battlerID.remove(attacked_id);
            w->removeEventFilter(this);
            w->deleteLater();
            delete c;
        }
        this->ui->_battleLog->append(log_str);
    }

    else
    {
        CardWidget *w = this->_enemy.battleField.value(attacked_id, nullptr);
        log_str = QStringLiteral("敌方: ");
        if(hp>0)
        {
            int damage =  w->_attack->text().toInt()-hp;
            if (damage>0){
                log_str = log_str + w->_realCard->getName() + QStringLiteral("受到")+QString::number(damage) + QStringLiteral("点伤害 ");
            }
            else {
                log_str = log_str + w->_realCard->getName() + QStringLiteral(" 增加")+QString::number(-damage) + QStringLiteral("点血量 ");
            }
            //@yap 给对方怪兽卡设置新攻击
            dynamic_cast<MonsterCard *>(w->_realCard)->setAttack(hp);
            w->_attack->setText(QString::number(hp));
        }
        else
        {

            log_str = log_str + w->_realCard->getName() + QStringLiteral(" 死亡");
            this->_enemy.battleField.remove(attacked_id);
            for(int i = 0; i < 3; i++)
            {
                if (w->x()==enemyfieldMap[i].x){
                    enemyfieldMap[i].hasCard=false;
                    break;
                }
            }
            if ( lastCard == w )
                lastCard=nullptr;
            w->removeEventFilter(this);
            w->deleteLater();
        }
       this->ui->_battleLog->append(log_str);
    }
}

//初始发牌              出牌顺序，QList
void Mainwidget::CreateCard(QDataStream &stream)
{
    for(int i = 0; i < 4; i++)
    {
        int category;
        stream>>category;
        GetNewCard(stream,category);
    }
}

//敌方断开连接 场地清空
void Mainwidget::Disconnected()
{
    initMap();
    setBackground();

    foreach (auto var, this->_me.battleField) {
        Card *c = _me.battleField.key(var);
        _me.battleField.remove(_me.battleField.key(var));
        delete c;
        var->deleteLater();
    }

    foreach (auto var, this->_me.cardInHand) {
        Card *c = _me.battleField.key(var);
        _me.battleField.remove(_me.battleField.key(var));
        delete c;
        var->deleteLater();
    }

    _me.battlerID.clear();

    foreach (auto var, _enemy.battleField) {
        var->deleteLater();
    }

    _enemy.battleField.clear();

    //@yap
    selectCard = nullptr;//指向被选中的部件
    targetCard = nullptr;//指向目标部件
    ChooseTarget = false;

    for(int i = 0; i < 5; i++)
    {
        handMap[i].hasCard = false;
    }

    for(int i = 0; i < 3; i++)
    {
        fieldMap[i].hasCard = false;
    }
    _connectUi->show();
    //游戏结束
}

void Mainwidget::DisableAllBtn()
{
    ui->_attackMonster->setEnabled(false);
    ui->_attackPlayer->setEnabled(false);
    ui->_playCard->setEnabled(false);
    ui->_cancel->setEnabled(false);
    ui->_turnEnd->setEnabled(false);
}

void Mainwidget::EnableAllBtn()
{
    ui->_attackMonster->setEnabled(true);
    ui->_attackPlayer->setEnabled(true);
    ui->_playCard->setEnabled(true);
    ui->_cancel->setEnabled(true);
    ui->_turnEnd->setEnabled(true);
}

void Mainwidget::Cancel()
{
    EnableAllWidget();
    this->selectCard = nullptr;
    this->targetCard = nullptr;
    ChooseTarget = false;
    EnableAllBtn();
    this->ui->_playCard->setText(QStringLiteral("出牌"));
    this->ui->_attackMonster->setText(QStringLiteral("攻击怪兽"));
}


void Mainwidget::AttackMonster()
{
    Card* attackCard = this->_me.battleField.key(this->selectCard,nullptr);//选中的是场上的怪兽牌
    int target = 0;
    int myID = 0;
    if (attackCard!=nullptr && dynamic_cast<MonsterCard*>(attackCard)->hasAttack()==false){//如果选中了场上的怪兽牌
        if (ChooseTarget)//选择目标阶段
        {
            //            int targetChoose = this->_enemy.battleField.key(this->targetCard,-1);
            if (this->targetCard != nullptr){//如果已经选择了目标
                target = this->_enemy.battleField.key(this->targetCard);
                myID = attackCard->getId();
                sendMessage(myID,target);//发送报文
                dynamic_cast<MonsterCard*>(attackCard)->setHasAttack(true);
                ChooseTarget = false;//退出目标选择阶段
                this->ui->_attackMonster->setText(QStringLiteral("攻击怪兽"));
                this->ui->_attackPlayer->setEnabled(true);
                EnableAllWidget();//恢复所有点击
                this->selectCard = nullptr;
                this->targetCard = nullptr;
                QString log_str = QStringLiteral("己方：");
                log_str = log_str + attackCard->getName() + QStringLiteral(" 对敌方 ")
                        + this->_enemy.battleField[target]->_realCard->getName() + QStringLiteral(" 发动攻击");
                this->ui->_battleLog->append(log_str);
            }
        }
        else {
            //无效化己方的手牌和己方怪兽的点击
            DisableCard();
            DisableMyMonster();
            this->ui->_attackPlayer->setEnabled(false);
            this->ui->_attackMonster->setText(QStringLiteral("确认"));
            ChooseTarget = true;//进入目标选择阶段
        }
    }
    qDebug()<<"at m size:"<<_me.cardInHand.size();
}

void Mainwidget::AttackPlayer()
{
    Card* attackCard = this->_me.battleField.key(this->selectCard,nullptr);//选中的是场上的怪兽牌
    int target = 0;
    int myID = 0;
    if (attackCard!=nullptr && dynamic_cast<MonsterCard*>(attackCard)->hasAttack()==false){
        myID = attackCard->getId();
        target = -1;
        sendMessage(myID,target);
        this->ui->_battleLog->append(QStringLiteral("己方：") +attackCard->getName()+ QStringLiteral(" 攻击敌方玩家"));
        dynamic_cast<MonsterCard*>(attackCard)->setHasAttack(true);
        this->selectCard = nullptr;
    }
    qDebug()<<"at p size:"<<_me.cardInHand.size();
}

void Mainwidget::OnWidgetClicked(CardWidget *widget)
{
    if (ChooseTarget){
        this->targetCard = widget;
    }
    else
    {
        this->selectCard = widget;

        //        //TODO 你知道的
        //        if (this->_me.cardInHand.key(widget,nullptr)!=nullptr)//如果这场卡属于手牌
        //        {
        //            SetDetailedCard(this->_me.cardInHand.key(widget),"");
        //            ui->_playCard->setEnabled(true);//激活出牌按钮
        //            ui->_attackMonster->setEnabled(false);//无效化攻击按钮
        //            ui->_attackPlayer->setEnabled(false);
        //        }
        //        else if (this->_me.battleField.key(widget,nullptr)!=nullptr){//如果这张卡是场上的怪兽卡
        //            ui->_attackMonster->setEnabled(true);//激活攻击按钮
        //            ui->_attackPlayer->setEnabled(true);
        //            ui->_playCard->setEnabled(false);//无效化出牌按钮
        //        }
    }
}

//@yap 直接调用私有方法
void Mainwidget::onReadyRead()//准备读取
{
    QDataStream in(_client);
    in.setVersion(QDataStream::Qt_5_9);
    int msgCategory;
    while (_client->bytesAvailable() >= sizeof(int))
    {
        in>>msgCategory;
        switch (msgCategory) {
        case 0: //获取对手名字
            GameStart(in);
            break;
        case 1: //回合开始              得费，发牌
            TurnStart(in);
            break;
        case 2: //人物状态更新          血量，费用消耗
            PlayerStatusChanged(in);
            break;
        case 3: //对方出牌              Category,出的牌id，己/敌方id
            EnemyPlayCard(in);
            break;
        case 4: //武器受损              怪id，武器攻击值
            ArmsDamaged(in);
            break;
        case 5: //怪物受损              怪id，怪攻击值
            MonsterDamaged(in);
            break;
        case 6: //初始发牌              出牌顺序，QList
            CreateCard(in);
            break;
        case 8: //敌方断开
            Disconnected();
            break;
        case 9: //获得牌               Card
            int category;
            in>>category;
            GetNewCard(in,category);
            break;
        case 10://游戏结束
            Disconnected();
            break;
        default:
            break;
        }
    }

}

void Mainwidget::onDisconnect()
{
    QMessageBox::information(this, QStringLiteral("断开连接"), QStringLiteral("与服务器断开连接, 游戏即将关闭"));
    qApp->exit();
}

void Mainwidget::sendMessage(int MyCardId, int EnemyCardId)//发送信息
{
    QDataStream out(_client);
    out<<1;
    out<<MyCardId;
    out<<EnemyCardId;
    this->_client->flush();
}

QString Mainwidget::getCardImg(int category)
{
    QString filename;
    switch (category) {
    case 10:
        filename = ":/cardWidget/resoure/img/attack.png";
        break;
    case 11:
        filename = ":/cardWidget/resoure/img/attack.png";
        break;
    case 12:
        filename = ":/cardWidget/resoure/img/attack.png";
        break;
    case 13:
        filename = ":/cardWidget/resoure/img/attack.png";
        break;
    case 14:
        filename = ":/cardWidget/resoure/img/attack.png";
        break;
    case 15:
        filename = ":/card/resoure/cardres/peashooter.jpg";
        break;
    case 20:
        break;
    case 21:
        break;
    case 22:
        break;
    case 23:
        break;
    case 24:
        break;
    case 31:
        break;
    case 32:
        break;
    case 33:
        break;
    case 34:
        break;
    case 35:
        break;
    default:
        break;
    }
    return filename;
}

void Mainwidget::DeleteHandCard(CardWidget *deleteCard, Card *usedCard)
{
    if (deleteCard != nullptr){
        //找到相应的地图数组
        for (int i = 0 ; i< 5 ;i++){
            if (deleteCard->x() == handMap[i].x){
                handMap[i].hasCard = false;
                break;
            }
        }
        //从手牌中删除
        this->_me.cardInHand.remove(usedCard);
        //        qDebug()<<(usedCard == nullptr);
        if (dynamic_cast<MagicCard*>(usedCard)!=nullptr){
            delete usedCard;
            qDebug()<<"delete MagicCard ed";
        }
        //@yap 注销事件过滤器
        if ( this->lastCard == deleteCard )
            this->lastCard = nullptr;
        deleteCard->removeEventFilter(this);
        deleteCard->deleteLater();
    }
}

void Mainwidget::SetHandCardUI(Card *newCard, QString filename)
{
    //创建部件，设置图片
    if (this->_me.cardInHand.count()<5){
        CardWidget* newCardWidget = new CardWidget(this,1);
        //@yap
        newCardWidget->installEventFilter(this);

        this->_me.cardInHand.insert(newCard,newCardWidget);
        for(int i = 0;i<5;i++){
            if (this->handMap[i].hasCard == false){
                newCardWidget->setGeometry(this->handMap[i].x,this->handMap[i].y,newCardWidget->width(),newCardWidget->height());
                //使该位置表示为有卡
                this->handMap[i].hasCard = true;
                break;
            }
        }
        newCardWidget->setImage(QPixmap(filename));//设置背景
        newCardWidget->_consume->setText(QString::number(newCard->getConsume()));//设置费用
        if  (dynamic_cast<MonsterCard*>(newCard) != nullptr){//如果是怪兽卡设置攻击力
            newCardWidget->_attack->setText(QString::number(dynamic_cast<MonsterCard*>(newCard)->getAttack()));
        }
        else if  (dynamic_cast<ArmsCard*>(newCard) != nullptr){//如果是装备卡设置攻击力
            newCardWidget->_attack->setText(QString::number(dynamic_cast<ArmsCard*>(newCard)->getAttackBuf()));
        }
        else {//魔法卡则不显示攻击图标
            newCardWidget->_attack_bg->hide();
        }
        newCardWidget->show(0);//显示部件
        this->_me.battlerID.insert(newCard->getId(),newCard);//插入ID+卡的组
        this->_me.cardInHand.insert(newCard,newCardWidget);//插入卡+部件的组
        connect(newCardWidget,SIGNAL(SMyPointer(CardWidget*)),this,SLOT(OnWidgetClicked(CardWidget*)));
    }
}

void Mainwidget::MyConsumeChangedUI(int hp, int newConsume)
{
    //改变数据结构的费用
    this->ui->label_my_consume->setText(QString::number(newConsume));
    this->ui->label_enemy_hp->setText(QString::number(hp));
    this->_me.consume = newConsume;
    this->_me.hp = hp;
}

void Mainwidget::DisableEnemyMonster()
{
    foreach (CardWidget* temp,this->_enemy.battleField){
        temp->setEnabled(false);
    }
}

void Mainwidget::DisableMyMonster()
{
    foreach (CardWidget* temp,this->_me.battleField){
        temp->setEnabled(false);
    }
}

void Mainwidget::EnableAllWidget()
{
    foreach (CardWidget* temp,this->_enemy.battleField){
        temp->setEnabled(true);
    }
    foreach (CardWidget* temp2,this->_me.battleField){
        temp2->setEnabled(true);
    }
    qDebug()<<"size:"<<_me.cardInHand.size();
    foreach (CardWidget* temp3, this->_me.cardInHand) {
        temp3->setEnabled(true);
        qDebug()<<"kk";
    }
    qDebug()<<"out";
}

void Mainwidget::DisableCard()
{
    foreach (CardWidget* temp, this->_me.cardInHand) {
        temp->setEnabled(false);
    }
}

void Mainwidget::SetEnemyMonsterUI(int id, Card *newCard, QString filename)
{
    //创建部件，设置图片
    if (this->_enemy.battleField.count()<3){
        CardWidget* newCardWidget = new CardWidget(this,1);
        newCardWidget->installEventFilter(this);
        newCardWidget->_realCard = newCard;
        this->_enemy.battleField.insert(id,newCardWidget);
        for(int i = 0;i<3;i++){
            if (this->enemyfieldMap[i].hasCard == false){
                newCardWidget->setGeometry(this->enemyfieldMap[i].x,this->enemyfieldMap[i].y,newCardWidget->width(),newCardWidget->height());
                //使该位置表示为有卡
                this->enemyfieldMap[i].hasCard = true;
                break;
            }
        }
        newCardWidget->setImage(QPixmap(filename));//设置背景
        if  (dynamic_cast<MonsterCard*>(newCard) != nullptr){//如果是怪兽卡设置攻击力
            newCardWidget->_attack->setText(QString::number(dynamic_cast<MonsterCard*>(newCard)->getAttack()));
        }
        newCardWidget->show(1);//显示部件
        connect(newCardWidget,SIGNAL(SMyPointer(CardWidget*)),this,SLOT(OnWidgetClicked(CardWidget*)));
    }
}
