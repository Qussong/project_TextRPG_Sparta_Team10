#include "Monster.h"
#include <iostream>
Monster::Monster(int level, std::string name, int maxHealth, int attack, int exp)
	: level(level), name(name), maxHealth(maxHealth), attack(attack), exp(exp) {
	health = maxHealth;
}

int Monster::mGetAttack() const
{
	return attack;
}

int Monster::mGetExp() const 
{
	return exp;
}

void Monster::mDisplayStatus()
{
	std::cout << std::endl;
	std::cout << "------------------" << std::endl;
	std::cout << " [ " << name << " ] " << std::endl;
	std::cout << "Level: " << level << std::endl;
	std::cout << "Health: " << health << " / " << maxHealth << std::endl;
	std::cout << "Attack: " << attack << std::endl;
	std::cout << "------------------" << std::endl;
}