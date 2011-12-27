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
