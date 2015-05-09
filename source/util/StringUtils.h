#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <locale>

namespace StringUtils
{
	// trim from start
	static std::string& ltrim(std::string &s)
	{
		std::locale loc;
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](char c)-> bool{ return !std::isspace(c, loc); }));
		return s;
	};

	// trim from end
	static std::string& rtrim(std::string &s)
	{
		std::locale loc;
		s.erase(std::find_if(s.rbegin(), s.rend(), [&](char c)-> bool{ return !std::isspace(c, loc); }).base(), s.end());
		return s;
	};

	// trim from both ends
	static std::string& trim(std::string &s)
	{
		return ltrim(rtrim(s));
	};

	static std::vector<std::string> split(std::string& str, const char delim)
	{
		std::vector<std::string> elements;
		std::stringstream ss(trim(str));
		std::string item;
		while (std::getline(ss, item, delim))
		{
			elements.push_back(item);
		}
		return elements;
	};
	static std::string stripBetweenCharacters(std::string& input, const char startChar, const char endChar, bool nested = false)
	{
		std::string stripped;
		size_t startPos = input.find(startChar);
		if (startPos != std::string::npos && startPos < input.size())
		{
			size_t endPos = input.find(endChar);

			if (endPos != std::string::npos)
			{
				if (nested)
				{
					//Detect if nested.
					size_t nestStart = input.find(startChar, startPos + 1);
					if (nestStart < endPos)
					{
						//Get number of nested characters and skip until end reached.
						size_t numNestedBlocks = 0;
						while (nestStart != std::string::npos && nestStart < endPos)
						{
							numNestedBlocks++;
							nestStart = input.find(startChar, nestStart + 1);
						}

						while (numNestedBlocks > 0 && endPos < input.size())
						{
							endPos = input.find(endChar, endPos + 1);
							numNestedBlocks--;
						}
					}
				}

				if (endPos <= startPos) return stripped;

				//Offsets to skip the actual characters.
				size_t length = endPos - startPos;
				stripped = input.substr(startPos + 1, length - 1);
				input.erase(startPos, length + 1);
			}
		}
		return stripped;
	};
	static std::pair<std::string, std::string> splitToPair(std::string& input, const char splitChar)
	{
		size_t splitPos = input.find(splitChar);

		std::pair<std::string, std::string> pair;

		pair.first = trim(input.substr(0, splitPos));
		pair.second = trim(input.substr(splitPos + 1));
		return pair;
	};
};