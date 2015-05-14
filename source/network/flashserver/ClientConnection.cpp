#include "ClientConnection.h"
#include "FlashServer.h"
#include "util/debug.h"

ClientConnection::ClientConnection(FlashServer& server)
	: m_server(server)
	, m_socket(m_server.getIOService())
	, m_receiving(false)
	, m_sending(false)
	, m_clientID(server.getClientID())
	, m_accepted(false)
{
	m_receiveBuffer = new char[BUFFER_SIZE];
	memset(m_receiveBuffer, 0, sizeof(m_receiveBuffer));
	m_writeBuffer = new char[BUFFER_SIZE];
	memset(m_writeBuffer, 0, sizeof(m_writeBuffer));
}

ClientConnection::~ClientConnection()
{
}

void ClientConnection::acceptConnection(const boost::system::error_code& error)
{
	if (error)
	{
		LOG("acceptConnection Error:%s", error.message().c_str());
		return;
	}
	LOG("Accepted connection...");
	m_accepted = true;
	m_server.acceptConnection(shared_from_this());
	//Wait for received data.
	receive();
}

void ClientConnection::send(std::string message)
{
	if (!m_accepted) return;

	LOG("Sending %d\n%s\n",message.size(), message.c_str());
	if (m_sending)
	{
		LOG("Queueing data to be sent later...");
		m_sendQueue.append(message.c_str());
		LOG("SendQueue %d\n%s\n", m_sendQueue.size(), m_sendQueue.c_str());
		return;
	}
	if (message.size() >= BUFFER_SIZE)
	{
		LOG("SENDING BIGGER THAN BUFFER! %d %d", BUFFER_SIZE, message.size());
		//m_sendQueue.append(message.substr(BUFFER_SIZE, message.end()));
		//message.erase(BUFFER_SIZE, message.end());
	}

	m_sending = true;
	memset(m_writeBuffer, 0, BUFFER_SIZE);
	memcpy(m_writeBuffer, message.data(), message.size());

	LOG("WriteBuffer\n%s\n", m_writeBuffer);

	boost::asio::async_write(
		m_socket,
		boost::asio::buffer(m_writeBuffer, message.size()),
		std::bind(&ClientConnection::onWriteCompleted,this,std::placeholders::_1,std::placeholders::_2)
	);
}

void ClientConnection::onWriteCompleted(const boost::system::error_code& error, size_t bytesWritten)
{
	if (error)
	{
		LOG("onWriteCompleted Error:%s", error.message().c_str());
		close();
		return;
	}
	m_sending = false;
	LOG("onWriteCompleted Sent:%u", bytesWritten);
	onNetworkOperationCompleted();
}

void ClientConnection::receive()
{
	if (m_receiving)
	{
		LOG("Already waiting for data to be received...");
		return;
	}
	LOG("Waiting Received data...");
	m_receiving = true;
	memset(m_receiveBuffer, 0, sizeof(m_receiveBuffer));

	m_socket.async_receive(
		boost::asio::buffer(m_receiveBuffer, BUFFER_SIZE),
		0,
		std::bind(&ClientConnection::onReceiveCompleted,this,std::placeholders::_1,std::placeholders::_2)
	);
}
void ClientConnection::onReceiveCompleted(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		LOG("onReceiveCompleted Error:%s", error.message().c_str());
		close();
		return;
	}
	LOG("onReceiveCompleted DataSize:%u", bytes_transferred);
	m_receiving = false;
	std::string input(m_receiveBuffer, bytes_transferred);
	LOG("%s", input.c_str());
	m_server.onDataReceived(shared_from_this(), input);
}

void ClientConnection::update()
{
	return;
	//TODO if closed disconnect
	if (!m_socket.is_open())
	{
		close();
		return;
	}
	if (!m_sending && !m_receiving)
	{
		onNetworkOperationCompleted();
	}
}

void ClientConnection::close()
{
	if (m_socket.is_open())
	{
		m_socket.shutdown(boost::asio::socket_base::shutdown_both);
		m_socket.close();
	}
	m_server.closeConnection(shared_from_this());
}

void ClientConnection::onNetworkOperationCompleted()
{
	if (m_sendQueue.size() > 0)
	{
		LOG("Sending Queued data...");
		std::string output = m_sendQueue.c_str();
		m_sendQueue.clear();
		send(output);
	}
	else
	{
		receive();
	}
}