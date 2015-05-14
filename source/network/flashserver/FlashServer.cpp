#include "FlashServer.h"
#include "util/debug.h"
#include <functional>

FlashServer::FlashServer(unsigned short port)
	: m_port(port)
	, m_io_service()
	, m_acceptor(m_io_service)
	, m_onUpdate(nullptr)
	, m_onConnect(nullptr)
	, m_onDisconnect(nullptr)
	, m_onData(nullptr)
	, m_nextClientID(0)
	, m_activeConnection(nullptr)
{
	auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_port);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();
	addConnection();
}

FlashServer::~FlashServer()
{
}

void FlashServer::sendToAll(std::string message)
{
	m_io_service.stop();//TODO Thread safeks
	for (auto connection : m_connections)
	{
		connection->send(message);
	}
}

void FlashServer::addConnection()
{
	LOG("Adding connection...");
	std::shared_ptr<ClientConnection> connection = std::make_shared<ClientConnection>(*this);
	m_connections.push_back(connection);
	m_acceptor.async_accept(connection->getSocket(), std::bind(&ClientConnection::acceptConnection, connection, std::placeholders::_1));
}

void FlashServer::acceptConnection(std::shared_ptr<ClientConnection> connection)
{
	LOG("Client Accepted, total clients:%u", m_connections.size());
	if (m_onConnect)
	{
		m_onConnect(connection);
	}
	addConnection();
}

void FlashServer::update()
{
	m_io_service.run();
	
	if (m_onUpdate)
	{
		m_onUpdate();
	}
}

void FlashServer::onDataReceived(std::shared_ptr<ClientConnection> connection, std::string& data)
{
	if (data.find("policy-file-request") != std::string::npos)
	{
		std::string flashHandshake =
"<?xml version =\"1.0\"?>\n\
<!DOCTYPE cross-domain-policy SYSTEM \"http://www.adobe.com/xml/dtds/cross-domain-policy.dtd\">\n\
<cross-domain-policy>\n\
<allow-access-from domain=\"localhost\" to-ports=\"" + std::to_string(m_port) + "\" />\n\
</cross-domain-policy>";
		connection->send(flashHandshake);
		return;
	}

	if (m_onData)
	{
		//Set the active connection for the handler
		m_activeConnection = connection;
		std::string response = m_onData(data);
		m_activeConnection = nullptr;
		if (response.empty())
		{
			connection->receive();
		}
		else
		{
			connection->send(response);
		}
	}
	else
	{
		connection->receive();
	}
}

void FlashServer::closeConnection(std::shared_ptr<ClientConnection> connection)
{
	if (m_onDisconnect)
	{
		m_onDisconnect(connection->getClientID());
	}

	m_connections.erase(
		std::remove_if(m_connections.begin(), m_connections.end(),
		[&connection](std::shared_ptr<ClientConnection> activeConnection)
		{
			return connection == activeConnection;
		}
	));
	LOG("Connection closed, total clients:%u", m_connections.size()-1);//-1 since one is always listening.
}