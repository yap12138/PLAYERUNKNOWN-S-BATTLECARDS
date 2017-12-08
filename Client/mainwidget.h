#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include "connectwidget.h"
#include <card.h>
#include <QHash>
#include <QDataStream>
#include <cardwidget.h>
#include "magicdeck.h"

namespace Ui {
class Widget;
}

class Mainwidget : public QWidget
{
    Q_OBJECT

typedef int CardID;

//typedef int labelID;

public:
    explicit Mainwidget(QWidget *parent = 0);
    ~Mainwidget();

private:
    //敌人
    struct enemuy{
        char name[20];  //名字
        int cardNum = 0;//手牌数
        int consume = 0;//剩余费用
        int oddCards = 30;//牌库剩余
        QHash<int,CardWidget> battleField;//战场上的怪物
    };

    //己方
    struct me{
        char name[20];  //名字
        int cardNum = 0;//手牌数
        int consume = 0;//剩余费用
        int oddCards = 30;//牌库剩余
        QHash<Card*,CardWidget*> cardInHand;//手牌
        QHash<Card*,CardWidget*> battleField;//战场上的怪物
    };

private:
    Ui::Widget *ui;
    QLabel* _card[5];          //初始时自己手上的四张牌，加载界面时加载出来
    QLabel* _detailedCard[2]; //右侧显示详细的卡片信息，_detailedCard[0]是右上放大版的卡片，_detailedCard[1]是右下以文字形式显示卡片内容的标签
    QLabel* _card1[3];      //自己出的卡，不是加载界面时就生成，这里写只是为了指示卡片具体大小和位置
    ConnectWidget* _connectUi;
    enemuy _ePlayer;
    me _mPlayer;
    QTcpSocket *_client;
    int _lastHandX;
    int _lastHandY;
    int _space = 50;

signals:
    void STurnStart(int consume,int category,Card& newCard);//回合开始
    void SGetNewCard(Card& newCard);//获得牌的信号
    void STurnEnd();//回合结束的信号
    void SPlayCard(int cardID);
    void SMyConsumeChanged(int consume);
    void SGameStart(const QString& name);
    void SEnemyPlayerStatusChanged(int hp,int consume);
    void SEnemyPlayCard(int category,int cardID,int target);
    void SArmsDamaged(int attacker_id,int attack);
    void SMonsterDamaged(int attacked_id,int hp);
    void SCreateCard();
    void STauntSkill(int monsterID);
    void SDisconnected();

private slots:
//    void TurnStart(int consume,int category,Card& newCard);
//    void GetNewCard(Card& newCard);//向手牌组里添加新卡
//    void TurnEnd();//发送回合结束的报文
//    void PlayCard(int cardID);//出牌的相关操作
//    void MyStatusChanged(int hp,int consume);
//    void GameStart(const QString& name);//游戏开始
//    void EnemyPlayerStatusChanged(int hp,int consume);//对方玩家状态改变
//    void EnemyPlayCard(int category,int cardID,int target);//对方出牌
//    void ArmsDamaged(int attacked_id,int attack);//武器受损
//    void MonsterDamaged(int attacked_id,int hp);//怪物受损
//    void CreateCard();//初始发牌
//    void TauntSkill(int monsterID);//嘲讽发动了
//    void Disconnected();//敌方断开连接
      void Test();

private:
    void initialazation();  //初始化界面

    //信息处理
void onReadyRead();//准备读取
void sendMessage();//发送信息

};

#endif // WIDGET_H
