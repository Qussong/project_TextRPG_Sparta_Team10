#include "Store.h"

#include "Item.h"
#include "HealthPotion.h"
#include "StrangePotion.h"

#include "../Character/Character.h"

#include <iostream>

/// <summary>
/// 물약가격
/// 
/// 체력 물약 : 80 
/// 이상한 물약 : 100
/// 
/// 팔때는 원금의 60%
/// 
/// </summary>

Store::Store()
{
	Character* player = Character::getInstance();

	items.push_back(new HealthPotion());
	items.push_back(new StrangePotion());
}


void Store::buyStuff(int idx)
{
	// 1번 : 체력 물약 , 2번 : 이상한 물약
	Character* player = Character::getInstance();
	int gold = player->getGold();
	Stuff* Item;
	
	switch (idx-1)
	{
	case EItem::Heal:
		if (gold >= 80)
		{
			player->setGold(player->getGold() - 80);
			Item = player->getItem(EItem::Heal);
			Item->ItemQuantity += 1;
			std::cout << "체력 포션을 구매했습니다." << std::endl;
		}
		else
		{
			std::cout << "\n골드가 부족합니다!" << std::endl;
		}
		break;
	case EItem::Strange:
		if (gold >= 100)
		{
			player->setGold(player->getGold() - 100);
			Item = player->getItem(EItem::Strange);
			Item->ItemQuantity += 1;
			std::cout << "체력 포션을 구매했습니다." << std::endl;
		}
		else
		{
			std::cout << "골드가 부족합니다!" << std::endl;
		}
		break;
	default:
		__noop;
	}
	std::cout << std::endl;
}

void Store::sellStuff(int idx)
{
	// 1번 : 체력 물약 , 2번 : 이상한 물약
	Character* player = Character::getInstance();
	int gold = player->getGold();
	Stuff* Item;

	switch (idx-1)
	{
	case EItem::Heal:
		player->setGold(player->getGold() + 48);
		Item = player->getItem(EItem::Heal);
		Item->ItemQuantity -= 1;
		std::cout << "체력 포션을 판매했습니다." << std::endl;
		std::cout << "골드가 48 증가했습니다." << std::endl;
		break;
	case EItem::Strange:
		player->setGold(player->getGold() + 60);
		Item = player->getItem(EItem::Strange);
		Item->ItemQuantity -= 1;
		std::cout << "수상한 포션을 판매했습니다." << std::endl;
		std::cout << "골드가 60 증가했습니다." << std::endl;
		break;
	default:
		__noop;
	}
	std::cout << std::endl;
}

void Store::showList()
{
	std::cout << "---------------------상점---------------------" << std::endl;
	std::cout << "판매 목록" << std::endl;
	for (int i = 0; i < items.size(); i++)
	{
		std::cout << i + 1 << " : " << items[i]->getName();
		std::cout << "\n ///// 구매 가격: " << items[i]->getPrice() << " ///// 판매 가격: " << items[i]->getPrice()*0.6 << " /////\n" << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << std::endl;
}
