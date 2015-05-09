#include "client.hpp"
#include <boost/system/config.hpp>
#include <boost/random/random_device.hpp>

Client::Client()
	: m_onConnect(nullptr)
{
	auto current_socket = getSocket();
	m_client.set_open_listener(std::bind(&Client::onConnected, this));
	m_client.set_close_listener(std::bind(&Client::onClose, this, std::placeholders::_1));
	m_client.set_fail_listener(std::bind(&Client::onFail, this));
};

Client::~Client()
{

}