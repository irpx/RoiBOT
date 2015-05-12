#include "ChatBot.h"
#include "socketclient/client.hpp"
#include "util/StringUtils.h"
#include "textgeneration/RoiGenerator.hpp"

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

	if (name.empty() || uid.empty() || url.empty())
	{
		std::cout << "Command line parameters:  name:BotName uid:000000000 url:http://url:3002" << std::endl;
		return 0;
	}
	
	/* For testing
	name = "WankBOT";
	uid = "135284529";
	url = "http://urtela.redlynx.com:3002";
	*/

	RoiGenerator roi;
	bool roiEnabled = false;
	bool running = true;
	Client client;
	ChatBot bot(client, name, uid, "roi");
	client.connect(url);

	bot.addLocalCommand("join",[&](std::string data)
	{
		bot.send("|/join " + data);
	});

	bot.addLocalCommand("part", [&](std::string data)
	{
		client.emit("part_channel", data);
	});
	
	bot.addLocalCommand("roi", [&](std::string data)
	{
		roiEnabled = !roiEnabled;
		std::cout << "Roi Enabled: " << (roiEnabled ? "True" : "False") << std::endl;
	});
	bot.addLocalCommand("exit", [&](std::string data)
	{
		running = false;
	});
	bot.addLocalCommand("reload", [&](std::string data)
	{
		std::cout << " Reloading..." << std::endl;
		bot.command("Odottakaas hetki...");
		roi.reload();
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

	bot.addChatCommand("roi", [&](std::string data)
	{
		std::vector<std::string> chatData;
		std::stringstream f(data);
		std::string s;
		while (std::getline(f, s, ' '))
		{
			if (s.find("roi") != std::string::npos)
			{
				chatData.emplace_back(s);
			}
			else
			{
				if (s.size() >= 4)
				{
					roi.learn(s);
				}
			}
		}

		if (roiEnabled)
		{
			for (auto&& word : chatData)
			{
				auto output = roi.getRoi(word);
				if (output.size() > 0)
				{

					size_t limit = output.size();
					size_t idx = rand() % limit;
					if (idx >= output.size())
					{
						idx = 0;
					}
					std::string word = output[idx];
					std::cout << word << std::endl;
					bot.sendChat(word);

					int reply = rand();
					if ((reply % 10) == 0)
					{
						bot.sendChat("hih ;)");
					}
					else if ((reply % 11) == 0)
					{
						bot.sendChat("Oliko ees paras?");
					}
					else if ((reply % 12) == 0)
					{
						bot.sendChat("Aika huono :(");
					}
					else if ((reply % 13) == 0)
					{
						bot.sendChat("Nyt suutun!");
					}
					else if ((reply % 14) == 0)
					{
						bot.sendChat(roi.getRandomWord());
					}
				}
			}
		}
	});
	
	while (running)
	{
		std::string mystring;
		std::getline(std::cin, mystring);
		bot.command(mystring);
	}

	return 0;
}

