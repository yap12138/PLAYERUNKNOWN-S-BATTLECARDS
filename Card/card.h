#ifndef CARD_H
#define CARD_H

#include "card_global.h"
#include <QString>
#include <QObject>
#include <QDataStream>

namespace PUBC {
    class Card;
}

class CARDSHARED_EXPORT Card
{

public:
    Card();
    explicit Card(QString &name, QString &description, const int consume = 0);
    Card(const Card &);
    virtual ~Card();

    virtual int skill();//卡牌效果
    virtual int getCategory() const;

    inline int getId() const { return this->_id; }
    inline const QString& getName() const { return *this->_name; }
    inline const QString& getDescription() const { return *this->_description; }
    inline int getConsume() const { return this->_consume; }
    inline void setConsume(int newConsume) { this->_consume = newConsume; }
    inline QObject const * getTAG() const { return this->_Tag; }
    inline void setTAG(QObject& tag) { this->_Tag = &tag; }

    Card &operator = (const Card &);   //重载赋值运算符
    CARDSHARED_EXPORT friend QDataStream &operator<<(QDataStream &, const Card &);
    CARDSHARED_EXPORT friend QDataStream &operator>>(QDataStream &, Card &);
private:
    static int _count;
    int _id;    //唯一id
protected:
    static const int _category = 0;  //卡牌种类

    QString* _name;  //卡牌名字
    QString* _description;   //卡牌描述
    int _consume;   //使用费用消耗

    QObject* _Tag;  //标签
};


#endif // CARD_H
