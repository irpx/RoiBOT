#include "ChatBot.h"
#include "socketclient/client.hpp"
#include "util/StringUtils.h"

#include <iostream>

int main(int argc, char* argv[])
{
	std::string name, uid, url;

	std::pair<std::string, std::string*> parameters[] = {
		std::make_pair("name", &name),
		std::make_pair("uid", &uid),
		std::make_pair("url", &url)
	};

	for (int i = 0; i < argc; i++)
	{
		std::string arg = argv[i];

		for (auto&& param : parameters)
		{
			if (arg.find(param.first) == 0)
			{
				std::cout << param.first << ": " << param.second << std::endl;
				param.second->append(StringUtils::splitToPair(arg, ':').second.c_str());
			}
		}
	}

	//std::cout << "Name: " << name << " Uid: " << uid << " Url: " << url << std::endl;
	Client client;
	ChatBot bot(client, name, uid);
	client.connect(url);
	//client.run();

	return 0;
}

