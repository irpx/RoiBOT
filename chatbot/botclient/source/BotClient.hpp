#pragma once

#include "RoiGenerator.hpp"
#include "socket.io/sio_client.h"
#include <iostream>
#include <boost/system/config.hpp>
#include <functional>
#include <sstream>
#include <chrono>

class Client
{
public:
	Client(std::string name,std::string uid)
		: m_running(true)
		, m_roi(false)
		, m_name(name)
		, m_uid(uid)
	{
		auto current_socket = getSocket();
		m_client.set_open_listener(std::bind(&Client::onConnected, this));
		m_client.set_close_listener(std::bind(&Client::onClose, this, std::placeholders::_1));
		m_client.set_fail_listener(std::bind(&Client::onFail, this));
	};
	~Client()
	{

	};
	void connect(std::string url)
	{
		m_client.connect(url);
	}

	void onEvent(sio::event& event)
	{
		std::cout << "event";
	}

	void onChatMessage(const std::string& name, sio::message::ptr const& message, bool need_ack, sio::message::ptr& ack_message)
	{
		std::cout << "Chat Message: " << name << std::endl;
	}
	
	void onConnected()
	{
		std::cout << "Connected..." << std::endl;

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
		
		/*
		user_join
		user_disconnect
		user_part
		join_channel
		user_list
		nick_change
		*/
		getSocket()->on("your_channel", m_onJoinChannel);
		getSocket()->on("chat message", m_onChat);

		getSocket()->emit("login", m_uid);
	};
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
	void onClose(sio::client::close_reason const& reason)
	{
		std::cout << "Closed reason:" << reason << std::endl;
	};
	void onFail()
	{
		std::cout << "sio failed " << std::endl;
	}
	sio::socket::ptr getSocket()
	{
		return m_client.socket();
	};
	void exit()
	{
		m_running = false;
	};
	void run()
	{
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
	};
private:
	bool							m_roi;
	RoiGenerator					m_roiGenerator;
	std::string						m_channel;
	std::string						m_name;
	std::string						m_uid;

	std::map<std::string, std::function<void(std::string)>>	m_callbacks;
	
	sio::socket::event_listener_aux m_onJoinChannel;
	sio::socket::event_listener_aux m_onChat;
	
	bool m_running;
	sio::client	m_client;
};
