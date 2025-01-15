#pragma once
#include <iostream>
#include <fstream>
#include <windows.h> // Windows API 사용

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
		std::cout << "[ Create Game Manager ]" << std::endl;
	}

	~GameManager() 
	{
	}

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
			std::cout << "[ Delete Game Manager ]" << std::endl;
		}
	}

	void displayKillCounts() const;	// 몬스터 처치 기록

	void Init();		// 초기화
	void Progress();	// 게임 로직
	bool Update();		// 게임 데이터 업데이트
	void Destroy();		// 마무리 정리

private:
	Monster* GenMonster(int playerLevel);

public:
	/// <summary>
	/// 
	/// 0 = Black
	/// 1 = Blue
	/// 2 = Green
	/// 3 = Aqua
	/// 4 = Red
	/// 5 = Purple
	/// 6 = Yellow
	/// 7 = White
	/// 
	/// </summary>
	static void setConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들 가져오기
		SetConsoleTextAttribute(hConsole, color);         // 색상 설정
	}
};
