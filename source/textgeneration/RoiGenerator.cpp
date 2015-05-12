#include "RoiGenerator.hpp"


#include <fstream>
#include <iostream>
#include <locale>
#include <algorithm>
#include <xlocale>

RoiGenerator::RoiGenerator()
	: m_roiCount(0)
	, m_learnCount(0)
	, m_learnings("learn.txt",std::ofstream::out)
{
	loadWords();
}

RoiGenerator::~RoiGenerator()
{
	m_learnings.close();
}

std::vector<std::string> RoiGenerator::getRoi(std::string& word)
{
	std::locale loc;
	word.erase(std::remove_if(word.begin(), word.end(), [&](char chr){return !std::isalpha(chr, loc); }), word.end());

	if (word.length() < 4)
		return std::vector<std::string>();

	std::vector<std::string> resultVector;
	for (const auto& other : words)
	{
		if (other.length() < 4)
			continue;

		std::string copyWord1 = other.substr(0, 2);
		std::string copyWord2 = word.substr(0, 2);

		if (copyWord1 == copyWord2)
			continue;

		copyWord1 += word.substr(2);
		copyWord2 += other.substr(2);

		std::string lastword = copyWord1.substr(0, copyWord1.length() - 3);
		if (allWords.find(lastword) != allWords.end())
		{
			if (lastword.length() > 2)
			{
				if (allWords.find(copyWord2) != allWords.end())
				{
					std::string output = std::string("harva ") + other + " " + word + ", kuin " + copyWord2 + " " + lastword + "-roi\n";
					resultVector.emplace_back(output);
				}
			}
		}
	}

	if (resultVector.size() > 0)
	{
		m_roiCount++;
	}
	return resultVector;
}

void RoiGenerator::reload()
{
	std::cout << "Reloading Word lists..." << std::endl;
	m_learnCount = 0;
	allWords.clear();
	words.clear();

	loadWords();
}

void RoiGenerator::loadWords()
{
	// manually accepted parts
	allWords.insert("koka");
	allWords.insert("kulli");
	allWords.insert("kepu");
	allWords.insert("eukko");
	allWords.insert("kotka");
	allWords.insert("ripuli");
	allWords.insert("kipu");
	allWords.insert("pers");
	allWords.insert("anus");

	std::string fileName = "sanalista.txt";
	std::ifstream fileStream(fileName.c_str());

	std::string line;

	if (fileStream.is_open())
	{
		std::cout << "Processing...";
		int run = 0;
		while (fileStream.good())
		{
			getline(fileStream, line);

			if (allWords.find(line) == allWords.end())
			{
				allWords.insert(line);
				words.emplace_back(line);
			}
			if (++run % 500)
			{
				std::cout << ".";
			}
		}
		fileStream.close();
	}
	else
	{
		std::cout << "Unable to open file " << fileName.c_str() << std::endl;
	}

	std::cout << "Words: " << allWords.size() << std::endl;

	
}

void RoiGenerator::learn(std::string word)
{
	std::locale loc;
	word.erase(std::remove_if(word.begin(), word.end(), [&](char chr){return !std::isalpha(chr, loc); }), word.end());

	m_learnCount++;
	std::cout << "learned: " << word << std::endl;
	allWords.insert(word.c_str());
	words.emplace_back(word.c_str());

	m_learnings << word << std::endl;
}

std::string RoiGenerator::getRandomWord()
{
	int r = int((static_cast<float>(rand()) / 32767.f)*allWords.size());
	auto random_it = std::next(std::begin(allWords), r);
	return *random_it;
}
