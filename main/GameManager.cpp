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

GameManager* GameManager::instance = nullptr;
Store* Store::instance = nullptr;

void GameManager::displayKillCounts() const
{
	{
		std::cout << "\n------------------" << std::endl;
		std::cout << "현재까지 잡은 몬스터 수" << std::endl;
		std::cout << "늑대 : " << wolfCount << std::endl;
		std::cout << "고블린 : " << goblinCount << std::endl;
		std::cout << "오크 : " << orcCount << std::endl;
		std::cout << "트롤 : " << trollCount << std::endl;
		std::cout << "------------------\n" << std::endl;
	}
}

void GameManager::Init()
{
	Store::getInstance();
	// 플레이어 이름 입력
	std::string playerName = "";
	std::cout << "플레이어의 이름을 입력하세요 : ";
	std::cin >> playerName;
	Character::getInstance(playerName);
	std::cout << "\n\n\n";

	// 초반 스토리 출력
	FRM->OpenFile(L"../story/intro.txt");
	FRM->PrintLineAll(false);
	FRM->CloseFile();

	// 초반 스토리 2
	FRM->OpenFile(L"../story/intro2.txt");
	FRM->PrintLineAll(false);
	FRM->CloseFile();

	std::cout << "\n\n\n";

	// 플레이어 레벨 2로 설정하고 시작
	Character::getInstance()->levelUp();

	//초반 스토리 3
	FRM->OpenFile(L"../story/intro3.txt");
	FRM->PrintLineAll(false);
	FRM->CloseFile();
}

/// <summary>
/// 
/// 몬스터 : 늑대, 고블린, 오크, 트롤
///	 
/// 플레이어 레벨 1 ~ 4  : 늑대, 고블린
/// 플레이어 레벨 5 ~ 8 : (늑대, 고블린) + 강한, 오크
/// 플레이어 레벨 9 ~ 10 : 오크, 트롤
/// 
/// </summary>
bool GameManager::Update()
{
	Character* player = Character::getInstance();
	Store* store = Store::getInstance();
	
	//몬스터 소환
	Monster* genMonster = GenMonster(Character::getInstance()->getLevel());
	
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

	// 상점을 들리게 된다면 -> 템 사는거 내가 선택해서 구매 (템 : 물약, 수상한 물약)
	if (1 == Select)
	{
		// 아이템 목록 출력
		store->showList();
		int storeSelect = 0;
		std::cout << "1번 : 아이템 구매" << std::endl;
		std::cout << "2번 : 아이템 판매" << std::endl;
		std::cout << "3번 : 상점 나가기" << std::endl;
		std::cout << std::endl;
		std::cout << "다음 행동을 선택하세요 : ";
		std::cin >> storeSelect;
		std::cout << std::endl;

		// 아이템 선택 및 구매
		if (storeSelect == 1)
		{
			store->showList();
			std::cout << "구매하고자 하는 아이템의 번호를 입력해주세요 : ";
			int buySelect = 0;
			std::cin >> buySelect;
			store->buyStuff(buySelect);
			std::cout << std::endl;
		}
		else if (storeSelect == 2)
		{
			store->showList();
			player->showInventory();
			std::cout << "판매하고자 하는 아이템의 번호를 입력해주세요 : ";
			int sellSelect = 0;
			std::cin >> sellSelect;
			store->sellStuff(sellSelect);
			std::cout << std::endl;
		}
		// 구매 완료 후 상점 탈출
		else if (storeSelect == 3)
		{
			Select = 3;
		}
		
	}

	if (2 == Select)
	{
		player->displayStatus();
		displayKillCounts();
	}

	if (3 == Select)
	{
		player->showInventory();
	}

	if (4 == Select)
	{
		GameManager::setConsoleColor(4);
		std::cout << "\n야생의 " << genMonster->mGetName() << "이(가) 출몰했습니다.\n";
		GameManager::setConsoleColor(7);

		//몬스터 스텟 출력
		genMonster->mDisplayStatus();
		std::cin.get();

		// 전투
		while (genMonster->mGetHealth() > 0 && player->IsAlive())
		{
			std::cin.get();  //  enter 치면 턴 넘기기

			// 플레이어 물약 사용
			player->usePotion();
			
			//	플레이어가 몬스터 공격
			genMonster->mTakeDamage(player->getAttack());
			std::cout << player->getName() << "이(가) " << genMonster->mGetName() << "을(를) 공격했습니다." << std::endl;
			std::cout << genMonster->mGetName() << "은(는) " << player->getAttack() << "만큼 대미지를 입었습니다." << std::endl;
			std::cout << genMonster->mGetName() << "의 현재 체력 : " << genMonster->mGetHealth() << " / " << genMonster->mGetMaxHealth() << std::endl;

			// 보스
			if (genMonster->mGetName() == "오크 족장")
			{
				float per = static_cast<float>(genMonster->mGetHealth()) / genMonster->mGetMaxHealth() * 100;
				std::cout << "( Boss HP " << per << "% )\n\n";
				static_cast<Boss*>(genMonster)->SoundEffect(per);
			}
			std::cin.get();

			// 몬스터가 플레이어 공격
			if (genMonster->mGetHealth() > 0)
			{
				player->TakeDamage(genMonster->mGetAttack());
				std::cout << genMonster->mGetName() << "이(가) " << player->getName() << "을(를) 공격했습니다." << std::endl;
				std::cout << player->getName() << "은(는) " << genMonster->mGetAttack() << "만큼 대미지를 입었습니다." << std::endl;
				std::cout << "현재 체력 : " << player->getHealth() << " / " << player->getMaxhealth() << std::endl;
			}
		}
	}
	if (9 == Select)
	{
		player->setHealth(0);
		std::cout << "게임을 종료합니다." << endl;
		return false;
	}

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
		std::cout << genMonster->mGetName() << "이(가) 죽었습니다." << std::endl;
		std::cin.get();
		std::cout << "다음 보상을 획득했습니다." << std::endl;
		// 몬스터가 죽으면? -> 경험치/골드 획득
		int rndGold = rand() % 11 + 10;
		player->setGold(player->getGold() + rndGold);
		std::cout << rndGold << " 골드 획득" << std::endl;
		if (player->getLevel() < 10)
		{
			player->setExperience(player->getExperience() + genMonster->mGetExp());
			std::cout << genMonster->mGetExp() << " 경험치 획득" << std::endl;
		}
		std::cin.get();
	}

	//레벨업 체크
	if (player->getExperience() >= player->getExperienceToNextLevel() && player->getLevel() < 10) {
		player->levelUp();
	}
	if (player->getLevel() == 10)
	{
		player->setExperience(0);
	}

	return player->IsAlive();
}

void GameManager::Destroy()
{
	GameManager::DestroyInstance();
}

/// <summary>
/// 
/// container = [ 늑대, 고블린, 오크, 트롤 ]
///                0      1      2     3
/// 
/// 플레이어 레벨 1 ~ 4  : 늑대, 고블린
/// 플레이어 레벨 5 ~ 8 : (늑대, 고블린) + 강한, 오크
/// 플레이어 레벨 9 ~ 10 : 오크, 트롤
/// 
/// </summary>
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
	else if (playerLevel < 9)
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

#pragma region Progress
// 게임 로직 돌려주는 곳
void GameManager::Progress()
{
	// 게임 종료 조건 : Player 사망 또는 플레이어의 레벨이 10 미만일때
	bool bPlayerAlive = true;
	while (bPlayerAlive)
	{
		bPlayerAlive = Update();
	}
	// 게임종료 로직 수행
	if (!bPlayerAlive)
	{
		FRM->OpenFile(L"../story/Dead.txt");
		FRM->PrintLineAll();
		FRM->CloseFile();
	}

}
#pragma endregion