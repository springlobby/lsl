#include "socket.h"

#include <utils/net.h>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <sstream>

namespace BA = boost::asio;
namespace BS = boost::system;
namespace IP = boost::asio::ip;

namespace LSL {

Socket::Socket()
    : m_sock(m_netservice)
    , m_rate(-1)
{
}

void Socket::Connect(const std::string &server, int port)
{
    boost::system::error_code err;
    IP::address tempAddr = IP::address::from_string(server, err);
    if (err)
    {
        // error, maybe a hostname?
        IP::tcp::resolver resolver(m_netservice);
        std::ostringstream portbuf;
        portbuf << port;
        IP::tcp::resolver::query query(server, portbuf.str());
        IP::tcp::resolver::iterator iter = resolver.resolve(query, err);
        if (err)
        {
            doneConnecting(false, err.message());
            return;
        }
        tempAddr = iter->endpoint().address();
    }
    IP::tcp::endpoint serverep(tempAddr, port);
    m_sock.async_connect(serverep, boost::bind(&Socket::ConnectCallback, this, BA::placeholders::error));
}

void Socket::ConnectCallback(const boost::system::error_code &error)
{
    if (!error)
    {
        doneConnecting(true, "");
        BA::async_read_until(m_sock, m_incoming_buffer, "\n", boost::bind(&Socket::ReceiveCallback, this, BA::placeholders::error, BA::placeholders::bytes_transferred));
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
        std::istream buf(&m_incoming_buffer);
        buf >> command;
        std::getline(buf, msg);
        if (!msg.empty())
            msg = msg.substr(1);
        //emits the signal
        dataReceived(command, msg);
    }
    else
    {
        if (error.value() == BS::errc::connection_reset || error.value() == BA::error::eof)
        {
            m_sock.close();
            socketDisconnected();
        }
        else if (m_sock.is_open()) //! ignore error messages after connect was closed
        {
            networkError(error.message());
        }
    }
    if (m_sock.is_open())
    {
        BA::async_read_until(m_sock, m_incoming_buffer, "\n", boost::bind(&Socket::ReceiveCallback, this, BA::placeholders::error, BA::placeholders::bytes_transferred));
    }
}

std::string Socket::GetHandle() const
{
    std::string handle;
    #ifdef WIN32

    IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information for 16 cards
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

    DWORD dwStatus = GetAdaptersInfo ( AdapterInfo, &dwBufLen); // Get info
        if (dwStatus != NO_ERROR) return _T(""); // Check status
    for (unsigned int i=0; i<std::min( (unsigned int)6, (unsigned int)AdapterInfo[0].AddressLength); i++)
    {
        handle += TowxString(((unsigned int)AdapterInfo[0].Address[i])&255);
        if (i != 5) handle += _T(':');
    }
    #elif defined(linux)
    int sock = socket (AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return _T(""); //not a valid socket
    }
    struct ifreq dev; //container for the hw data
    struct if_nameindex *NameList = if_nameindex(); //container for the interfaces list
    if (NameList == NULL)
    {
        close(sock);
        return _T(""); //cannot list the interfaces
    }

    int pos = 0;
    std::string InterfaceName;
    do
    {
        if (NameList[pos].if_index == 0)
        {
            close(sock);
            if_freenameindex(NameList);
            return _T(""); // no valid interfaces found
        }
        InterfaceName = NameList[pos].if_name;
        pos++;
    } while (InterfaceName.substr(0,2) == "lo" || InterfaceName.substr(0,3) == "sit");

    if_freenameindex(NameList); //free the memory

    strcpy (dev.ifr_name, InterfaceName.c_str()); //select from the name
    if (ioctl(sock, SIOCGIFHWADDR, &dev) < 0) //get the interface data
    {
        close(sock);
        return _T(""); //cannot list the interfaces
    }

    for (int i=0; i<6; i++)
    {
        handle += TowxString(((unsigned int)dev.ifr_hwaddr.sa_data[i])&255);
        if (i != 5) handle += _T(':');
    }
    close(sock);
#endif
    return handle;

}

void Socket::SetSendRateLimit(int Bps)
{
}

} // namespace LSL
