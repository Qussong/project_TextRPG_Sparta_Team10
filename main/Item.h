#pragma once
#include <string>

class Character;

class Item
{
protected:
	int quantity = 0;
	int price = 0;
	std::string name = "";

public:
	virtual std::string getName() = 0;
	virtual int getPrice() = 0;
	virtual void use(Character* character);

public:
	int getQuantity();
};

