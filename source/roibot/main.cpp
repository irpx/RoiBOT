#include "ChatBot.h"
#include "socketclient/client.hpp"
#include "util/StringUtils.h"
#include "textgeneration/RoiGenerator.hpp"

#include <iostream>

/*
http://www.omdbapi.com/?t=crossroads&y=&plot=short&r=json

sio::socket::event_listener_aux m_onJoinChannel;
sio::socket::event_listener_aux m_onChat;

m_callbacks.insert(std::make_pair("exit", [&](std::string msg)
{
getSocket()->emit("chat message", m_channel + "|Pieni hetki, palaan pian...");
m_running = false;
}));

m_callbacks.insert(std::make_pair("reload", [&](std::string msg)
{
getSocket()->emit("chat message", m_channel + "|Pieni hetki, keksin uusia sanoja...");
m_roiGenerator.reload();
//getSocket()->emit("chat message", m_channel + "|Noniin, osaan nyt " + m_roiGenerator.getTotalWordCount() + " sanaa");
}));

m_callbacks.insert(std::make_pair("learn", [&](std::string msg)
{
getSocket()->emit("chat message", m_channel + "|Haa, nyt keksin! " + msg);
m_roiGenerator.learn(msg);
}));

m_callbacks.insert(std::make_pair("roi", [&](std::string msg)
{
m_roi = !m_roi;
std::cout << "Roi Enabled: " << m_roi << std::endl;
if (m_roi)
{
getSocket()->emit("chat message", m_channel + "|Noniin, Kertokaapas juttuja ;)");
}
else
{
getSocket()->emit("chat message", m_channel + "|Puuh! Nyt en jaksa. Pieni tauko kiitos...");
}
}));

m_callbacks.insert(std::make_pair("channel", [&](std::string msg){m_channel = msg; std::cout << "Channel set:" << m_channel << std::endl; }));
m_callbacks.insert(std::make_pair("part", [&](std::string msg)
{
getSocket()->emit("part_channel", m_channel);
}));
m_callbacks.insert(std::make_pair("join", [&](std::string msg)
{
m_channel = msg;
getSocket()->emit("chat message", "|/join " + msg);
getSocket()->emit("chat message", msg + "|moi ;)");
}));

m_callbacks.insert(std::make_pair("info", [&](std::string msg)
{
std::stringstream output;
output << "Osaan " << m_roiGenerator.getTotalWordCount() << "  sanaa joista " << m_roiGenerator.getLearnedWordCount() << " olen oppinut. Olen kertonut " << m_roiGenerator.getRoiCount() << " tarinaa.";
getSocket()->emit("chat message", m_channel+"|" + output.str());
}));

m_onJoinChannel = [&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::ptr &ack_resp)->void
{
m_channel = data->get_string();
getSocket()->emit("chat message", "|/join " + m_channel);
getSocket()->emit("chat message", "|/nick " + m_name);
getSocket()->emit("chat message", m_channel + "|moi taas");
};
m_onChat = [&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::ptr &ack_resp)->void
{
std::string chat = data->get_string();
std::cout << chat << std::endl;

if (chat.find(m_name) == std::string::npos)
{
checkRoi(chat.substr(chat.find_last_of('|') + 1));
}
};

//user_join
//user_disconnect
//user_part
//join_channel
//user_list
//nick_change

getSocket()->on("your_channel", m_onJoinChannel);
getSocket()->on("chat message", m_onChat);

getSocket()->emit("login", m_uid);

void checkRoi(std::string &chat)
{
std::vector<std::string> chatData;
std::stringstream f(chat);
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
m_roiGenerator.learn(s);
}
}
}

if (m_roi)
{
for (auto&& word : chatData)
{
auto output = m_roiGenerator.getRoi(word);
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
getSocket()->emit("chat message", m_channel + "|" + word);

int reply = rand();
if ((reply % 10) == 0)
{
getSocket()->emit("chat message", m_channel + "|hih ;)");
}
else if ((reply % 11) == 0)
{
getSocket()->emit("chat message", m_channel + "|Oliko ees paras?");
}
else if ((reply % 12) == 0)
{
getSocket()->emit("chat message", m_channel + "|Aika huono :(");
}
else if ((reply % 13) == 0)
{
getSocket()->emit("chat message", m_channel + "|Nyt suutun!");
}
}
return;
}
}
};

while (m_running)
{
std::string mystring;
std::getline(std::cin, mystring);
//std::cin >> mystring;

size_t pos = mystring.find('/');

if (pos == 0)
{
pos = mystring.find(' ');
std::string command = mystring.substr(1, pos - 1);
std::cout << "command: [" << command << "]" << std::endl;

auto it = m_callbacks.find(command);
if (it != m_callbacks.end())
{
mystring = mystring.substr(pos + 1);
auto func = it->second;
func(mystring);
}
else
{
getSocket()->emit("chat message", "|" + mystring);
}
}
else
{
getSocket()->emit("chat message", m_channel + "|" + mystring);
}
}
*/

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

	name = "WankBOT";
	uid = "135284529";
	url = "http://urtela.redlynx.com:3002";

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

