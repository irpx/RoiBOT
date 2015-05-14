#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>

#define BUFFER_SIZE 8192

class FlashServer;
class ClientConnection : public std::enable_shared_from_this < ClientConnection >
{
public:
	ClientConnection(FlashServer& server);
	~ClientConnection();
	void acceptConnection(const boost::system::error_code& error);
	void onWriteCompleted(const boost::system::error_code& error, size_t bytesWritten);
	void onReceiveCompleted(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket& getSocket() { return m_socket; };
	void send(std::string message);
	void receive();
	void close();
	void update();
	int getClientID() { return m_clientID; };
private:
	void onNetworkOperationCompleted();
	bool													m_accepted;
	int														m_clientID;
	FlashServer&											m_server;
	boost::asio::ip::tcp::socket							m_socket;
	char*													m_receiveBuffer;
	char*													m_writeBuffer;
	bool													m_receiving;
	bool													m_sending;
	std::string												m_sendQueue;
};
