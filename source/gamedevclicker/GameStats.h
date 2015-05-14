#pragma once
#include <string>
#include <vector>
struct Stat
{
	std::string name;
	std::string sValue;
	int			iValue;
	float		fValue;
	bool		bValue;
};

class GameStats
{
public:
	GameStats();
	~GameStats();

	std::vector<Stat*>	stats;
};

