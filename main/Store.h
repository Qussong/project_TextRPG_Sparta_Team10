#pragma once

#include <vector>

enum EItem
{
	Heal = 0,
	Strange = 1,
};

class Store
{
private:
	static Store* instance;

	Store();
	~Store()
	{

	}

	Store(const Store&) = delete;
	Store& operator = (const Store&) = delete;

public:
	static Store* getInstance()
	{
		if (nullptr == instance)
		{
			instance = new Store();
		}
		return instance;
	}

	void destroyInstance()
	{
		if (nullptr != instance)
		{
			delete instance;
			instance = nullptr;
		}
	}
	
private:
	std::vector<class Item*> items;
	class Character* player;

public:
	void buyStuff(int idx);
	void sellStuff(int idx);
	void showList();
};