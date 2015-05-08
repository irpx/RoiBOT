#pragma once

#include <unordered_set>
#include <string>
#include <vector>
#include <fstream>


class RoiGenerator
{
public:
	RoiGenerator();
	~RoiGenerator();

	std::vector<std::string> getRoi(std::string& word);
	void reload();
	void learn(std::string word);
	size_t getTotalWordCount(){ return allWords.size(); };
	int getLearnedWordCount(){ return m_learnCount; };
	int getRoiCount(){ return m_roiCount; };
private:
	std::ofstream				m_learnings;
	int m_learnCount;
	int m_roiCount;
	void loadWords();
	std::unordered_set<std::string> allWords;
	std::vector<std::string> words;
};

