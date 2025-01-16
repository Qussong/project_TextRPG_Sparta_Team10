#include "Character.h"

#include "../main/Item.h"
#include "../main/HealthPotion.h"
#include "../main/StrangePotion.h"

Character* Character::instance = nullptr;

Character::Character(wstring name)
{
	this->name = name;
	this->level = 9;
	this->attack = 400;
	this->health = 200;
	this->maxHealth = 20000;
	this->experience = 0;
	this->experienceToNestLevel = 100;
	this->gold = 0;

	Stuff* healingPotion = new Stuff();
	healingPotion->Item = new HealthPotion();
	healingPotion->ItemName = healingPotion->Item->getName();
	healingPotion->ItemQuantity = 2;
	this->inventory.push_back(healingPotion);

	Stuff* strangePotion = new Stuff();
	strangePotion->Item = new StrangePotion();
	strangePotion->ItemName = strangePotion->Item->getName();
	strangePotion->ItemQuantity = 1;
	this->inventory.push_back(strangePotion);
}

Character* Character::getInstance(wstring name)
{
	if (instance == nullptr)
	{
		instance = new Character(name);
	}

	return instance;
}

void Character::displayStatus()
{
	cout << "\n---------------------" << endl;
	cout << "[ Status ]" << endl;
	wcout << "Name : " << name << endl;
	cout << "Level : " << level << endl;
	cout << "Exp : " << experience << " / " << experienceToNestLevel << endl;
	cout << "Health : " << health << " / " << maxHealth << endl;
	cout << "Attack : " << attack << endl;
	cout << "---------------------\n" << endl;
}
void Character::levelUp()
{
	if (level < 10)
	{
		this->level++;
		if (this->experience > this->experienceToNestLevel)
		{
			this->experience -= experienceToNestLevel;
		}
		this->maxHealth += level * 20;
		this->health = maxHealth;
		this->attack += level * 5;
		this->experienceToNestLevel += experienceToNestLevel * 0.2;

		cout << "---------------------" << endl;
		cout << "Level Up!" << endl;
		cout << "최대 체력 "<< level * 20 << " 상승!" << endl;
		cout << "공격력 " << level * 5 << " 상승!" << endl;
		cout << "---------------------" << endl;
		std::cout << endl;
	}
	else
	{
		cout << "------------------" << endl;
		cout << "Already Maxlevel!" << endl;
		cout << "------------------" << endl;
	}
}
//void Character::useItem(int index)

int Character::getMaxhealth() const
{
	return this->maxHealth;
}
int Character::getHealth() const
{
	return this->health;
}
void Character::setHealth(int health)
{
	this->health = health;
}
void Character::setMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
}
bool Character::IsAlive()
{
	if (this->health > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Character::getAttack() const
{
	return this->attack;
}
int Character::getGold() const
{
	return this->gold;
}
void Character::setGold(int gold)
{
	this->gold = gold;
}
void Character::TakeDamage(int damage)
{
	if (this->health - damage > 0)
	{
		this->health -= damage;
	}
	else
	{
		this->health = 0;
	}
}
double Character::getExperience() const
{
	return this->experience;
}
void Character::setAttack(int attack)
{
	this->attack = attack;
}
double Character::getExperienceToNextLevel() const
{
	return this->experienceToNestLevel;
}

wstring Character::getName() const
{
	return this->name;
}

Stuff* Character::getItem(int idx)
{
	return inventory[idx];
}

void Character::usePotion()
{
	if(this->health < this->health/50 && inventory[0]->ItemQuantity > 0)
	{
		inventory[0]->Item->use(instance);
		inventory[0]->ItemQuantity--;
	}
	else if(inventory[1]->ItemQuantity > 0)
	{
		int per = rand() % 3;
		if (per == 2)
		{
			inventory[1]->Item->use(instance);
			inventory[1]->ItemQuantity--;
		}
	}
}

void Character::showInventory()
{
	cout << "\n---------------------인벤토리---------------------" << endl;
	cout << "보유 중인 아이템 목록\n" << endl;
	cout << "골드 : " << this->gold << endl;
	cout << endl;

	for (int i = 0; i < inventory.size(); i++)
	{
		cout << i + 1 << " : " << inventory[i]->ItemName << " - " 
			
			<< inventory[i]->ItemQuantity << "개 -\n" << endl;
	}
	cout << "--------------------------------------------------\n" << endl;
}

void Character::setExperience(double experience)
{
	this->experience = experience;
}
int Character::getLevel() const
{
	return this->level;
}
