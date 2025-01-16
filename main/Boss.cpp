#include "Boss.h"
#include <string>
#include <iostream>
#include "FileReaderManager.h"
#include "../Character/Character.h"

Boss::Boss(int level)
	:Monster(level,
		"오크 족장",
		4000, 150, 0)
{
}

void Boss::mTakeDamage(int damage)
{
	health -= damage;
	if (health < 0) {
		health = 0;
	}
}

void Boss::SoundEffect(EBossCondition condition)
{
	switch (condition)
	{
		case EBossCondition::INTRO :
		{
			std::cout << "\n===================" << std::endl;
			FRM->OpenFile(L"../story/BossStart.txt");
			FRM->PrintLineAll(Character::getInstance()->getName());
			FRM->CloseFile();
			std::cout << "===================" << std::endl;
			break;
		}
		case EBossCondition::HALF:
		{
			std::cout << "\n===================" << std::endl;
			FRM->OpenFile(L"../story/Boss50.txt");
			FRM->PrintLineAll(Character::getInstance()->getName());
			FRM->CloseFile();
			std::cout << "===================" << std::endl;
			bHalfAlarm = false;
			break;
		}
		case EBossCondition::ALMOST:
		{
			std::cout << "\n===================" << std::endl;
			FRM->OpenFile(L"../story/Boss20.txt");
			FRM->PrintLineAll(Character::getInstance()->getName());
			FRM->CloseFile();
			std::cout << "===================" << std::endl;
			bAlmostAlarm = false;
			break;
		}
		case EBossCondition::DEATH:
		{
			std::cout << "\n===================" << std::endl;
			FRM->OpenFile(L"../story/BossDead.txt");
			FRM->PrintLineAll(Character::getInstance()->getName());
			FRM->CloseFile();
			std::cout << "===================" << std::endl;
			break;
		}
		default:
			__noop;
	}

	//if (bMeet)
	//{
	//	std::cout << "===================" << std::endl;
	//	FRM->OpenFile(L"../story/BossStart.txt");
	//	FRM->PrintLineAll(Character::getInstance()->getName());
	//	FRM->CloseFile();
	//	std::cout << "===================" << std::endl;

	//	bMeet = false;
	//}
	//else if (bHpHalf && per <= 50)
	//{
	//	std::cout << "===================" << std::endl;
	//	FRM->OpenFile(L"../story/Boss50.txt");
	//	FRM->PrintLineAll(Character::getInstance()->getName());
	//	FRM->CloseFile();
	//	std::cout << "===================" << std::endl;

	//	bHpHalf = false;
	//}
	//else if (bAlmostDeath && per <= 20)
	//{
	//	// comment
	//	std::cout << "===================" << std::endl;
	//	FRM->OpenFile(L"../story/Boss20.txt");
	//	FRM->PrintLineAll(Character::getInstance()->getName());
	//	FRM->CloseFile();
	//	std::cout << "===================" << std::endl;

	//	bAlmostDeath = false;
	//}
	//else if (bDeath && per <= 0)
	//{
	//	// comment
	//	std::cout << "===================" << std::endl;
	//	FRM->OpenFile(L"../story/BossDead.txt");
	//	FRM->PrintLineAll(Character::getInstance()->getName());
	//	FRM->CloseFile();
	//	std::cout << "===================" << std::endl;

	//	bDeath = false;
	//}
}
