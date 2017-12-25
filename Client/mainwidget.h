#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QtNetwork>
#include <QHash>
#include "cardwidget.h"
#include "connectwidget.h"
#include "carddeck.h"

namespace Ui {
class Widget;
}

class Mainwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Mainwidget(QWidget *parent = 0);
    ~Mainwidget();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    //敌人
    struct enemuy{
        QString name;//名字
        int hp = 30; //生命
        int cardNum = 0;//手牌数
        int consume = 0;//剩余费用
        int oddCards = 32;//牌库剩余
        QHash<int,CardWidget*> battleField;//战场上的怪物
    };

    //己方
    struct me{
        QString name;  //名字
        int hp = 30;   //生命
        int cardNum = 0;//手牌数
        int consume = 0;//剩余费用
        int oddCards = 32;//牌库剩余
        QHash<Card*,CardWidget*> cardInHand;//手牌
        QHash<Card*,CardWidget*> battleField;//战场上的怪物
        QHash<int,Card*> battlerID;
    };

    //位置和窗口部件的结构体，包含着部件应该在的位置和该是否有部件
    struct WidgetMap{
        int x;
        int y;
        bool hasCard = false;
    };

private:
    Ui::Widget *ui;
    QLabel *_card[5];          //初始时自己手上的四张牌，加载界面时加载出来
    //QLabel *_detailedCard[2]; //右侧显示详细的卡片信息，_detailedCard[0]是右上放大版的卡片，_detailedCard[1]是右下以文字形式显示卡片内容的标签
    QLabel *_card1[3];      //自己出的卡，不是加载界面时就生成，这里写只是为了指示卡片具体大小和位置
    QLabel *_card2[3];
    ConnectWidget *_connectUi;

    QTcpSocket *_client;

    enemuy _enemy;
    me _me;

    CardWidget* lastCard = nullptr; //指向上次被选中的卡
    CardWidget* selectCard = nullptr;//指向被选中的部件
    CardWidget* targetCard = nullptr;//指向目标部件
    bool ChooseTarget = false;
    WidgetMap handMap[5];
    WidgetMap fieldMap[3];
    WidgetMap enemyfieldMap[3];

    QMap<int, QPixmap> _cardImg;

    //@yap
    CardWidget* _detailCard;    //右上详情

private:
    void initConnect();  //初始化界面
    void initDetailArea();
    void initMap();
    void initCardImg();
    void setBackground();
    void sendMessage(int MyCardId,int EnemyCardId);//发送信息
    QPixmap& getCardImg(int category);

    void DeleteHandCard(CardWidget* deleteCard, Card *usedCard);
    void SetHandCardUI(Card* newCard,QPixmap & img);
    void MyConsumeChangedUI(int hp,int newConsume);
    void DisableEnemyMonster();//无效化敌方怪物
    void DisableMyMonster();//无效化我方怪物
    void EnableAllWidget();//使所有的部件有效化
    void DisableCard();//使卡片无效化
    void SetEnemyMonsterUI(int id,Card* newCard, QPixmap& img);
    void SetDetailedCard(Card* card,QPixmap & img);

    void TurnStart(QDataStream &stream);//回合开始
    void GetNewCard(QDataStream &stream,int category);//向手牌组里添加新卡
    void GameStart(QDataStream &stream);//游戏开始
    void PlayerStatusChanged(QDataStream &stream);//对方玩家状态改变
    void EnemyPlayCard(QDataStream &stream);//对方出牌
    void ArmsDamaged(QDataStream &stream);//武器受损
    void MonsterDamaged(QDataStream &stream);//怪物受损
    void CreateCard(QDataStream &stream);//初始发牌
    void Disconnected();//敌方断开连接 场地清空

    void DisableAllBtn();//无效化除回合结束外的所有按钮
    void EnableAllBtn();//有效化所有按钮


private slots:

    void Cancel();
    void AttackMonster();
    void AttackPlayer();
    void OnWidgetClicked(CardWidget*widget);
    void PlayCard();//出牌的相关操作
    void TurnEnd();//发送回合结束的报文
    //信息处理
    void onReadyRead();//准备读取
    //@yap 与服务器断开连接
    void onDisconnect();
};

#endif // WIDGET_H
