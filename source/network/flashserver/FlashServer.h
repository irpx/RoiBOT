#pragma once
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include "ClientConnection.h"
#include <boost/asio.hpp>

class FlashServer
{
public:
	FlashServer(unsigned short port);
	~FlashServer();
	void addConnection();
	void sendToAll(std::string message);
	boost::asio::io_service& getIOService() { return m_io_service; };
	void acceptConnection(std::shared_ptr<ClientConnection> connection);
	void closeConnection(std::shared_ptr<ClientConnection> connection);
	void update();
	unsigned short getPort() { return m_port; };
	int getClientID() { return m_nextClientID++; };
	void onDataReceived(std::shared_ptr<ClientConnection> connection, std::string& data);
	void setOnUpdateCallback(std::function<void(void)> onUpdate) { m_onUpdate = onUpdate; };
	void setOnData(std::function<std::string(std::string&)> onData) { m_onData = onData; };
	void setOnConnectCallback(std::function<void(std::shared_ptr<ClientConnection>)> onConnect) { m_onConnect = onConnect; };
	void setOnDisconnectCallback(std::function<void(int)> onDisconnect) { m_onDisconnect = onDisconnect; };
	std::shared_ptr<ClientConnection> getActiveConnection() { return m_activeConnection; };
private:
	int														m_nextClientID;
	unsigned short											m_port;

	boost::asio::io_service									m_io_service;
	boost::asio::ip::tcp::acceptor							m_acceptor;
	std::vector<std::shared_ptr<ClientConnection>>			m_connections;

	std::shared_ptr<ClientConnection>						m_activeConnection;

	std::function<void(void)>								m_onUpdate;
	std::function<std::string(std::string&)>				m_onData;
	std::function<void(std::shared_ptr<ClientConnection>)>	m_onConnect;
	std::function<void(int)>								m_onDisconnect;
};
