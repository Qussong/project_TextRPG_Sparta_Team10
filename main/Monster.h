#pragma once
#include <string>

//몬스터 클래스
class Monster {
protected:
    std::string name;
    int health;
    int attack;

public:
    Monster(std::string name, int health, int attack);
    virtual ~Monster() {}

    virtual std::string GetName() const = 0;
    virtual int GetHealth() const = 0;
    virtual int GetAttack() const = 0;
    virtual void TakeDamage(int damage) = 0;
};