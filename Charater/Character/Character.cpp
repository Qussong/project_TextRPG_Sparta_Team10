#include "Character.h"

Character::Character(string name)
{
	this->name = name;
	this->level = 1;
	this->attack = 30;
	this->health = 200;
	this->maxhealth = 200;
	this->experience = 0;
	this->gold = 0;
}

static Character::Character* getInstance(string name = "")
{
	if (instance == nullptr)
	{
		instance = new Character(name);
	}
	return instance;
}
void Character::displayStatus()
{
	cout << "Health: " << health << " / " << maxhealth << endl;
	cout << "Level: " << level << endl;
	cout << "Exp: " << experience << " / 100" << endl;

}
void Character::levelUp()
{
	this->level++;
	this->maxhealth += level * 20;
	this->health = maxhealth;
	this->attack += level * 5;
}
void Character::useItem(int index)
{

}