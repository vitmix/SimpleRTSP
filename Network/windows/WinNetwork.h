#ifndef WIN_NETWORK_H
#define WIN_NETWORK_H

#include "../Network.h"
#include <winsock2.h>
#include <vector>

class WinSocket : public Socket
{
public:
    WinSocket(const std::string& ipAddr, const unsigned& ipPort);
    virtual ~WinSocket();

    bool init() override;
    bool listenForConnections() override;
    void close() override;

    unsigned getNumOfConnection() const;

    std::string read();
    bool write(const std::string& data);

private:
    WSADATA m_wsaData; // maybe move to the separate class
    SOCKET m_socket = INVALID_SOCKET;
    std::string m_ipAddr;
    unsigned m_ipPort;
    sockaddr_in m_addr;
    std::vector<SOCKET> m_connections;
};

#endif // !WIN_NETWORK_H
