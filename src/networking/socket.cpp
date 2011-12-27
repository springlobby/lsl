#include "socket.h"

namespace LSL {

Socket::Socket()
    : m_rate(-1)
{
}

void Socket::Connect(const std::string &server, int port)
{
    using namespace boost::asio;
    boost::system::error_code err;
    ip::address tempAddr = netcode::WrapIP(server, &err);
    if (err)
    {
        // error, maybe a hostname?
        ip::tcp::resolver resolver(netservice);
        std::ostringstream portbuf;
        portbuf << port;
        ip::tcp::resolver::query query(server, portbuf.str());
        ip::tcp::resolver::iterator iter = netcode::WrapResolve(resolver, query, &err);
        if (err)
        {
            doneConnecting(false, err.message());
            return;
        }
        tempAddr = iter->endpoint().address();
    }
    ip::tcp::endpoint serverep(tempAddr, port);
    sock.async_connect(serverep, boost::bind(&Socket::ConnectCallback, this, placeholders::error));
}

void Socket::ConnectCallback(const boost::system::error_code &error)
{
    if (!error)
    {
        doneConnecting(true, "");
        boost::asio::async_read_until(sock, incomeBuffer, "\n", boost::bind(&Socket::ReceiveCallback, this, placeholders::error, placeholders::bytes_transferred));
    }
    else
    {
        doneConnecting(false, error.message());
    }
}

void Socket::ReceiveCallback(const boost::system::error_code &error, size_t bytes)
{
    if (!error)
    {
        std::string msg;
        std::string command;
        std::istream buf(&incomeBuffer);
        buf >> command;
        std::getline(buf, msg);
        if (!msg.empty())
            msg = msg.substr(1);
        //emits the signal
        dataReceived(command, msg);
    }
    else
    {
        if (error.value() == connection_reset || error.value() == boost::asio::error::eof)
        {
            sock.close();
            Disconnected();
        }
        else if (sock.is_open()) //! ignore error messages after connect was closed
        {
            NetworkError(error.message());
        }
    }
    if (sock.is_open())
    {
        boost::asio::async_read_until(sock, incomeBuffer, "\n", boost::bind(&Socket::ReceiveCallback, this, placeholders::error, placeholders::bytes_transferred));
    }
}

// namespace LSL
