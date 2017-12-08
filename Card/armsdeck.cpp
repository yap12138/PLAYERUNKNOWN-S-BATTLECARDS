#include "armsdeck.h"

Arms_98K::Arms_98K()
{
    *_name = "Kar 98K";
    *_description = "Kar.98k是二战期间产量最多的轻武器之一，被认为是第二次世界大战中最好的旋转后拉式枪机步枪之一。";
    this->_consume = 5;
    this->_attackBuff = 6;
}

int Arms_98K::getCategory() const
{
    return Arms_98K::_category;
}
