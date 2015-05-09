#pragma once

#include "sio_client.h"

#include <iostream>
#include <functional>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>
#include <memory>

class Client
{
public:
	Client();
	~Client();

	void connect(std::string url)
	{
		m_client.connect(url);
	}
	
	void onConnected()
	{
		std::cout << "Connected..." << std::endl;
	};
	
	void onClose(sio::client::close_reason const& reason)
	{
		std::cout << "Closed reason:" << reason << std::endl;
	};
	void onFail()
	{
		std::cout << "sio failed " << std::endl;
	};
	void emit(std::string command, std::string message)
	{
		getSocket()->emit(command, message);
	};
	void addCallback(std::string name, std::function<void(std::string)> callback)
	{
		sio::socket::event_listener_aux func = [&](std::string const& eveny, sio::message::ptr const& data, bool isAck, sio::message::ptr &ack_resp)->void
		{
			std::string message = data->get_string();
			callback(message);
		};
		m_callbacks.insert(std::make_pair(name, callback));
		getSocket()->on(name, func);
	};
	void setOnConnect(std::function<void(void)> oncon)
	{
		m_onConnect;
	};
private:
	std::map<std::string, std::function<void(std::string)>>	m_callbacks;
	sio::socket::ptr getSocket() { return m_client.socket(); };

	std::function<void(void)>		m_onConnect;
	
	sio::client	m_client;
};
