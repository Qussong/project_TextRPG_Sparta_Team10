#pragma once
#include <iostream>
#include <fstream>
#include <windows.h> // Windows API 사용

#define GM GameManager::GetInstance()

class Monster;

class GameManager
{
private:
	int wolfCount = 0;
	int goblinCount = 0;
	int orcCount = 0;
	int trollCount = 0;
	static GameManager* instance;

	GameManager()
	{
		//std::cout << "[ Create Game Manager ]" << std::endl;
	}
	~GameManager() {}

	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

public:
	static GameManager* GetInstance() 
	{
 		if (nullptr == instance)
		{
			instance = new GameManager();
		}
		return instance;
	}
	static void DestroyInstance()
	{
		if (nullptr != instance)
		{
			delete instance;
			instance = nullptr;
			//std::cout << "[ Delete Game Manager ]" << std::endl;
		}
	}

// Game Logic
public:
	void Init();		// 초기화
	void Progress();	// 게임 로직
	bool Update();		// 게임 데이터 업데이트
	void Destroy();		// 마무리 정리

// Function
public:
	static void setConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들 가져오기
		SetConsoleTextAttribute(hConsole, color);         // 색상 설정
	}
	void SetGameEndCondition(bool flag) { bBossDeath = flag; }

private:
	bool bBossDeath = false;

	Monster* GenMonster(int playerLevel);
	void displayKillCounts() const;	// 몬스터 처치 기록

	void SetPlayerCharacter();	// 초기 플레이어 이름 설정 및 생성
	void Intro();

// Player Input
private:
	int SelectOption();
	void SelectStore();
	void SelectStatus();
	void SelectInventory();
	Monster* SelectBattle();
	void BattleReward(Monster* genMonster);
	bool SelectEndGame();

};
