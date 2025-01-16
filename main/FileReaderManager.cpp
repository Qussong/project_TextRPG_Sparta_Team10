#include "FileReaderManager.h"

#include <thread> // for std::this_thread::sleep_for
#include <chrono> // for std::chrono::milliseconds

#include <windows.h>

FileReaderManager* FileReaderManager::instance = nullptr;

/// <summary>
/// 
/// std::getline은 기본적으로 std::string과 함께 사용된다.
/// std::string은 UTF-8 문자열을 다룰 때 개별 문자의 의미를 올바르게 처리하지 못한다.
/// 
/// std::wifstream
/// std::wstring
/// 
/// </summary>
void FileReaderManager::ReadFile()
{
	std::wstring line;
	bool bEnd = false;
	while (false == bEnd)
	{
		if (std::getline(instance->readFile, line))
		{
			lineContainer.push_back(line);
			
			if (readFile.eof())
			{
				bEnd = true;
				//std::cout << "Reached the end of the file." << std::endl;
			}

			if (readFile.fail()) {
				std::cerr << "An error occurred while reading the file!" << std::endl;
				bEnd = true;
			}
			//else if (readFile.good()) 
			//{
			//	std::cout << "The file stream is in good condition." << std::endl;
			//}
		}
	}
}

bool FileReaderManager::OpenFile(std::wstring filePath)
{
	readFile.open(filePath);
	if (false == readFile.is_open())
	{
		std::cout << "Failed to open file for reading!" << std::endl;
		return false;
	}

	ReadFile();

	return true;
}

void FileReaderManager::PrintLine(int idx, std::wstring playerName, bool bTypeEffect)
{
	if (playerName != L"")
	{
		insertPlayerName(playerName);
	}

	if (bTypeEffect)
	{
		typingEffect(lineContainer[idx]);
	}
	else
	{
		std::wcout << lineContainer[idx];
	}

}

void FileReaderManager::PrintLineAll(std::wstring playerName, bool bTypeEffect)
{
	if (playerName != L"")
	{
		insertPlayerName(playerName);
	}

	for (int i = 0; i < lineContainer.size(); ++i)
	{
		if (bTypeEffect)
		{
			typingEffect(lineContainer[i]);
		}
		else
		{
			std::wcout << lineContainer[i];
		}
		std::cout << std::endl;
	}
}

void FileReaderManager::CloseFile()
{
	lineContainer.clear();
	readFile.close();
}

void FileReaderManager::typingEffect(const std::wstring& text, int delay)
{
	for (wchar_t ch : text) {
		std::wcout << ch << std::flush; // flush를 사용하여 즉각 출력
		std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // 딜레이
	}
}

void FileReaderManager::insertPlayerName(std::wstring playerName)
{
	for (int i = 0; i < lineContainer.size(); i++)
	{
		wchar_t target = L'#';
		size_t pos = lineContainer[i].find(target);

		while (pos != std::string::npos) {
			if (pos != std::string::npos)
			{
				lineContainer[i].replace(pos, 1, playerName);
			}
			pos = lineContainer[i].find(target, pos + 1);
		}
	}
}