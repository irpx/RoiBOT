#include "client.hpp"
#include <boost/system/config.hpp>
#include <boost/random/random_device.hpp>

Client::Client()
	: m_onConnect(nullptr)
{
	m_onChat = [this](std::string const& eveny, sio::message::ptr const& data, bool isAck, sio::message::ptr &ack_resp)->void
	{
		auto it = m_callbacks.find(eveny);
		if (it != m_callbacks.end())
		{
			std::string message = data->get_string();
			auto func = it->second;
			func(message);
		}
	};
	auto current_socket = getSocket();
	m_client.set_open_listener(std::bind(&Client::onConnected, this));
	m_client.set_close_listener(std::bind(&Client::onClose, this, std::placeholders::_1));
	m_client.set_fail_listener(std::bind(&Client::onFail, this));
};

Client::~Client()
{

}