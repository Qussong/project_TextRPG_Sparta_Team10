#include <iostream>
#include <string>
#include <vector>

class Item;

using namespace std;

struct Stuff
{
    Item* Item;
    string ItemName;
    int ItemQuantity;
};

class Character 
{
private:
    //string name;
    wstring name;
    static Character* instance;
    int level;
    int health;
    int maxHealth;
    int attack;
    double experience;
    double experienceToNestLevel;
    int gold;
    vector<Stuff*> inventory;

    Character(wstring name);

    Character(const Character&) = delete;
    Character& operator = (const Character&) = delete;

public:
    static Character* getInstance(wstring name = L"");
    void displayStatus();
    void levelUp();
    int getMaxhealth() const;
    int getHealth() const;
    int getLevel() const;
    void setHealth(int health);
    void setMaxHealth(int maxHealth);
    bool IsAlive();
    int getAttack() const;
    int getGold() const;
    void setGold(int gold);
    void TakeDamage(int damage);
    void setExperience(double experience);
    void setAttack(int attack);
    double getExperience() const;
    double getExperienceToNextLevel() const;
    //string getName() const;
    wstring getName() const;
    Stuff* getItem(int idx);

    // 물약사용
    void usePotion();
    // 인벤토리 확인
    void showInventory();
};


