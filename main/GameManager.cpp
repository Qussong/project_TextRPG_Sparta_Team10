#include "GameManager.h"
#include "../Character/Character.h"
#include "FileReaderManager.h"
#include <vector>
#include "Monster.h"
#include "Wolf.h"
#include "Goblin.h"
#include "Orc.h"
#include "Troll.h"
#include "StrongMonster.h"
#include "Boss.h"
#include "Store.h"
#include "Enum.h"

GameManager* GameManager::instance = nullptr;
Store* Store::instance = nullptr;

#pragma region Game Logic

void GameManager::Init()
{
	SetPlayerCharacter();	// 초기 플레이어 이름 설정 및 생성

	std::cout << "\n\n\n";

	Intro();	// 초반 스토리 출력 및 플레이어 기본 세팅
}

void GameManager::Progress()
{
	// 게임 종료 조건 : 
	//		1. 캐릭터 사망
	//		2. 보스 처치 성공
	bool bPlayerAlive = true;
	// 플레이어가 살아있고, 보스가 죽지 않았으면 게임 진행
	// 둘 중 하나라도 죽으면 게임 끝
	while (bPlayerAlive && false == bBossDeath)
	{
		bPlayerAlive = Update();
	}

	// 게임종료 로직 수행
	if (false == bPlayerAlive)
	{
		std::cout << std::endl;
		FRM->OpenFile(L"../story/Dead.txt");
		FRM->PrintLineAll(Character::getInstance()->getName());
		FRM->CloseFile();
	}
}

bool GameManager::Update()
{
	Character* player = Character::getInstance();
	Store* store = Store::getInstance();
	Monster* genMonster = nullptr;

	// 플레이어 입력
	int Select = SelectOption();
	switch (Select)
	{
	case 1:	// 상점
		SelectStore();
		break;
	case 2:	// 상태 확인
		SelectStatus();
		break;
	case 3:	// 인벤토리 확인
		SelectInventory();
		break;
	case 4:	// 전투
		BattleReward(SelectBattle());
		break;
	case 9:	// 게임 종료
		return SelectEndGame();
	default:
		__noop;
	}

	return player->IsAlive();
}

void GameManager::Destroy()
{
	GameManager::DestroyInstance();
}

#pragma endregion


#pragma region Player Input

int GameManager::SelectOption()
{
	int Select = 0;
	std::cout << "\n\n▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽▽\n\n";
	std::cout << "------------------" << endl;
	std::cout << "1번 : 상점" << endl;
	std::cout << "2번 : 상태 확인" << endl;
	std::cout << "3번 : 인벤토리 확인" << endl;
	std::cout << "4번 : 전투 시작" << endl;
	std::cout << "9번 : 게임 종료" << endl;
	std::cout << "------------------" << endl;
	std::cout << std::endl;
	std::cout << "다음 행동을 선택하세요 : ";
	std::cin >> Select;

	return Select;
}

void GameManager::SelectStore()
{
	Store* store = Store::getInstance();
	Character* player = Character::getInstance();

	// 상점을 들리게 된다면 -> 템 사는거 내가 선택해서 구매 (템 : 물약, 수상한 물약)
	// 아이템 목록 출력
	GameManager::setConsoleColor(EColor::GREEN);
	store->showList();
	GameManager::setConsoleColor(EColor::WHITE);
	int storeSelect = 0;
	std::cout << "1번 : 아이템 구매" << std::endl;
	std::cout << "\n2번 : 아이템 판매" << std::endl;
	std::cout << "\n[ 다른 값을 입력하면 상점을 나갑니다. ]\n\n";
	std::cout << "다음 행동을 선택하세요 : ";
	std::cin >> storeSelect;
	std::cout << std::endl;

	// 아이템 선택 및 구매
	if (storeSelect == 1)
	{
		std::cout << "구매하고자 하는 아이템의 번호를 입력해주세요 : ";
		int buySelect = 0;
		std::cin >> buySelect;
		store->buyStuff(buySelect);
		std::cout << std::endl;
	}
	else if (storeSelect == 2)
	{
		GameManager::setConsoleColor(EColor::GREEN);
		player->showInventory();
		GameManager::setConsoleColor(EColor::WHITE);
		std::cout << "판매하고자 하는 아이템의 번호를 입력해주세요 : ";
		int sellSelect = 0;
		std::cin >> sellSelect;
		store->sellStuff(sellSelect);
		std::cout << std::endl;
	}
}

void GameManager::SelectStatus()
{
	Character* player = Character::getInstance();

	player->displayStatus();
	displayKillCounts();
}

void GameManager::SelectInventory()
{
	Character* player = Character::getInstance();

	GameManager::setConsoleColor(EColor::GREEN);
	player->showInventory();
	GameManager::setConsoleColor(EColor::WHITE);
}

Monster* GameManager::SelectBattle()
{
	Character* player = Character::getInstance();

	// 몬스터 등장 
	Monster* genMonster = GenMonster(Character::getInstance()->getLevel());
	GameManager::setConsoleColor(EColor::RED);
	std::cout << "\n야생의 " << genMonster->mGetName() << "이(가) 출몰했습니다.\n";
	GameManager::setConsoleColor(EColor::WHITE);

	if (genMonster->mGetName() == "오크 족장")
	{
		static_cast<Boss*>(genMonster)->SoundEffect(EBossCondition::INTRO);
	}

	// 몬스터 스텟 출력
	genMonster->mDisplayStatus();
	std::cin.get();

	// 전투
	while (genMonster->mGetHealth() > 0 && player->IsAlive())
	{
		// enter 치면 턴 넘기기
		std::cin.get();

		// 플레이어 물약 사용
		GameManager::setConsoleColor(EColor::AQUA);
		player->usePotion();
		GameManager::setConsoleColor(EColor::WHITE);

		//	플레이어가 몬스터 공격
		genMonster->mTakeDamage(player->getAttack());
		std::wcout << player->getName() << L"이(가) ";
		std:cout << genMonster->mGetName() << "을(를) 공격했습니다." << std::endl;
		std::cout << genMonster->mGetName() << "은(는) " << player->getAttack() << "만큼 대미지를 입었습니다." << std::endl;
		std::cout << genMonster->mGetName() << "의 현재 체력 : " << genMonster->mGetHealth() << " / " << genMonster->mGetMaxHealth() << std::endl;
		std::cin.get();

		if (genMonster->mGetName() == "오크 족장"
			&& genMonster->mGetHealth() == 0)
		{
			static_cast<Boss*>(genMonster)->SoundEffect(EBossCondition::DEATH);
			return genMonster;
		}

		// 몬스터가 플레이어 공격
		if (genMonster->mGetHealth() > 0)
		{
			player->TakeDamage(genMonster->mGetAttack());
			std::cout << genMonster->mGetName() << "이(가) ";
			std::wcout << player->getName() << L"을(를) 공격했습니다." << std::endl;
			std::wcout << player->getName() << L"은(는) ";
			std::cout << genMonster->mGetAttack() << "만큼 대미지를 입었습니다." << std::endl;
			std::cout << "현재 체력 : " << player->getHealth() << " / " << player->getMaxhealth() << std::endl;
		}

		if (genMonster->mGetName() == "오크 족장")
		{
			Boss* boss = static_cast<Boss*>(genMonster);
			float per = static_cast<float>(genMonster->mGetHealth()) / genMonster->mGetMaxHealth() * 100;

			if (boss->GetHalfAlarm() && per <= 50)
			{
				static_cast<Boss*>(genMonster)->SoundEffect(EBossCondition::HALF);
			}
			else if (boss->GetAlmostAlarm() && per <= 20)
			{
				static_cast<Boss*>(genMonster)->SoundEffect(EBossCondition::ALMOST);
			}
		}
	}

	return genMonster;
}

void GameManager::BattleReward(Monster* genMonster)
{
	Character* player = Character::getInstance();

	// 몬스터가 죽은 경우
	if (genMonster->mGetHealth() <= 0)
	{
		if (genMonster->mGetName() == "늑대")
		{
			wolfCount++;
		}
		else if (genMonster->mGetName() == "고블린")
		{
			goblinCount++;
		}
		else if (genMonster->mGetName() == "오크")
		{
			orcCount++;
		}
		else if (genMonster->mGetName() == "트롤")
		{
			trollCount++;
		}
		else if (genMonster->mGetName() == "오크 족장")
		{
			GM->SetGameEndCondition(true);
			return;
		}

		std::cout << genMonster->mGetName() << "이(가) 죽었습니다." << std::endl;
		std::cin.get();

		std::cout << "다음 보상을 획득했습니다." << std::endl;
		GameManager::setConsoleColor(EColor::GREEN);
		int rndGold = rand() % 11 + 10;
		player->setGold(player->getGold() + rndGold);
		std::cout << rndGold << " 골드 획득" << std::endl;
		if (player->getLevel() < 10)
		{
			player->setExperience(player->getExperience() + genMonster->mGetExp());
			std::cout << genMonster->mGetExp() << " 경험치 획득" << std::endl;
		}
		GameManager::setConsoleColor(EColor::WHITE);
		std::cin.get();
	}

	//레벨업 체크
	if (player->getExperience() >= player->getExperienceToNextLevel()
		&& player->getLevel() < 10)
	{
		GameManager::setConsoleColor(EColor::YELLOW);
		player->levelUp();
		GameManager::setConsoleColor(EColor::WHITE);
	}
}

bool GameManager::SelectEndGame()
{
	Character* player = Character::getInstance();

	player->setHealth(0);
	std::cout << "게임을 종료합니다." << endl;
	return false;
}

#pragma endregion


#pragma region Function

Monster* GameManager::GenMonster(int playerLevel)
{
	Monster* result = nullptr;

	std::vector<Monster*> Monsters;
	Monsters.push_back(new Wolf(playerLevel));
	Monsters.push_back(new Goblin(playerLevel));
	Monsters.push_back(new Orc(playerLevel));
	Monsters.push_back(new Troll(playerLevel));
	Monsters.push_back(new Boss(playerLevel));

	if (playerLevel < 4)
	{
		// 늑대, 고블린 (0, 1)
		int idx = rand() % 2; // 0 ~ 1
		result = Monsters[idx];
	}
	else if (playerLevel < 7)
	{
		// (늑대, 고블린) + 강한, 오크 (0,1,2)
		int idx = rand() % 3;	// 0 ~ 2
		result = Monsters[idx];

		if (idx < 2) {
			result = new StrongMonster(result);
		}
	}
	else if (playerLevel < 10)
	{
		// 오크, 트롤 (2,3)
		int idx = rand() % 2 + 2;	// 0 ~ 1
		result = Monsters[idx];
	}
	else if (playerLevel == 10)
	{
		int idx = 4;
		result = Monsters[idx];
	}

	return result;
}

void GameManager::displayKillCounts() const
{
	{
		std::cout << "\n------------------" << std::endl;
		std::cout << "[ 처치한 몬스터 ]" << std::endl;
		std::cout << "늑대 : " << wolfCount << std::endl;
		std::cout << "고블린 : " << goblinCount << std::endl;
		std::cout << "오크 : " << orcCount << std::endl;
		std::cout << "트롤 : " << trollCount << std::endl;
		std::cout << "------------------\n" << std::endl;
	}
}

void GameManager::SetPlayerCharacter()
{
	std::wstring playerName = L"";
	std::cout << "플레이어의 이름을 입력하세요 : ";
	std::wcin >> playerName;
	//std::wcout << playerName;
	Character::getInstance(playerName);
}

void GameManager::Intro()
{
	// 초반 스토리 출력
	FRM->OpenFile(L"../story/intro.txt");
	FRM->PrintLineAll(Character::getInstance()->getName(), false);
	FRM->CloseFile();

	// 초반 스토리 2
	FRM->OpenFile(L"../story/intro2.txt");
	FRM->PrintLineAll(Character::getInstance()->getName(), false);
	FRM->CloseFile();

	std::cout << "\n\n\n";

	// 플레이어 레벨 2로 설정하고 시작
	GameManager::setConsoleColor(EColor::YELLOW);
	Character::getInstance()->levelUp();
	GameManager::setConsoleColor(EColor::WHITE);

	//초반 스토리 3
	FRM->OpenFile(L"../story/intro3.txt");
	FRM->PrintLineAll(Character::getInstance()->getName(), false);
	FRM->CloseFile();
}

#pragma endregion