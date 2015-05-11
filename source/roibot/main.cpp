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

	name = "BobTheBot";
	uid = "135284522";
	url = "http://urtela.redlynx.com:3002";

	Client client;
	ChatBot bot(client, name, uid);
	client.connect(url);

	bot.addLocalCommand("join",[&](std::string data)
	{
		bot.send("|/join " + data);
	});

	bot.addLocalCommand("nick", [&](std::string data)
	{
		bot.setName(data);
		bot.send("|/nick " + data);
	});

	bot.addLocalCommand("channel", [&](std::string data)
	{
		bot.setChannel(data);
	});

	bot.addLocalCommand("chat", [&](std::string data)
	{
		bot.send(data);
	});

	bot.addLocalCommand("info", [&](std::string data)
	{
		std::cout << "I am bot: " << bot.getName() << " uid: " << bot.getUID() << " channel: " << bot.getChannel() << std::endl;
	});

	while (true)
	{
		std::string mystring;
		std::getline(std::cin, mystring);

		bot.command(mystring);
	}

	return 0;
}

