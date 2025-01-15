#include "Item.h"
#include "../Character/Character.h"

int Item::getPrice()
{
    return price;
}

void Item::use(Character* character)
{
    if (quantity == 0)
    {
        return;
    }
    --quantity;
}

int Item::getQuantity()
{
    return quantity;
}
