#include "WinNetwork.h"
#include <iostream>
#include <array>

namespace
{
    static constexpr const auto MAX_NUM_OF_CONN = 1u;
    static constexpr const auto BUFF_SIZE = 50u;
    static std::array<char, BUFF_SIZE> buffer;

    static const std::string DELIM("\r\n\r\n");
}

WinSocket::WinSocket(const std::string& ipAddr, const unsigned& ipPort)
    : m_ipAddr{ ipAddr }, m_ipPort{ ipPort }
{
    m_connections.reserve(MAX_NUM_OF_CONN);
}

WinSocket::~WinSocket()
{
    if (m_socket != INVALID_SOCKET)
    {
        const auto result = closesocket(m_socket);
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Failed to close socket with error " << WSAGetLastError() << std::endl;
        }
    }

    for (const auto& sock : m_connections)
    {
        if (m_socket != INVALID_SOCKET)
        {
            closesocket(sock);
        }
    }

    WSACleanup();
}

bool WinSocket::init()
{
    WORD mVersionRequested = MAKEWORD(2, 2);
    
    if (WSAStartup(mVersionRequested, &m_wsaData) != NO_ERROR)
    {
        std::cerr << "Failed to find a usable WinSock DLL." << std::endl;
        return false;
    }

    if (LOBYTE(m_wsaData.wVersion) != 2 || HIBYTE(m_wsaData.wVersion) != 2)
    {
        std::cerr << "Failed to find WinSock DLL of version 2.2" << std::endl;
        return false;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket
    if (m_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket. Error code is " << WSAGetLastError() << std::endl;
        // WSACleanup(); // moved to the destructor
        return false;
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(m_ipAddr.c_str()); // INADDR_ANY
    m_addr.sin_port = htons(m_ipPort);

    if (bind(m_socket, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind socket. Error code is " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool WinSocket::listenForConnections()
{
    if (m_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to run listen command, socket is invalid. Error: " << WSAGetLastError() << std::endl;
        return false;
    }

    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen, socket is invalid. Error: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Server is listening on port " << m_ipPort << std::endl;

    auto clientSocket = accept(m_socket, nullptr, nullptr); // fetch info about client's ip and port
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    
    m_connections.push_back(std::move(clientSocket));

    std::cout << "Client connected..." << std::endl; // add client's ip and port

    return true;
}

void WinSocket::close()
{
}

unsigned WinSocket::getNumOfConnection() const
{
    return static_cast<unsigned>(m_connections.size());
}

std::string WinSocket::read()
{
    if (m_connections.empty())
        return {};

    auto& connSock = m_connections.back();

    std::string message;
    message.reserve(BUFF_SIZE);

    while (true)
    {
        auto bytesReceived = recv(connSock, buffer.data(), buffer.size(), 0);
        if (bytesReceived > 0)
        {
            message.insert(message.end(), buffer.begin(), buffer.begin() + bytesReceived);
            
            //std::cout << "Read chunk: " << message << std::endl;

            if (message.rfind(DELIM) != message.npos)
                break;
        }
        else if (bytesReceived == 0)
        {
            std::cerr << "Connection closed" << std::endl;
            closesocket(connSock);
            m_connections.pop_back();
            break;
        }
        else
        {
            std::cerr << "Error occurred during recv: " << WSAGetLastError() << std::endl;
        }
    }

    return message;
}

bool WinSocket::write(const std::string& data)
{
    if (m_connections.empty())
    {
        std::cerr << "No connection is available" << std::endl;
        return false;
    }

    if (data.empty())
    {
        std::cerr << "Data to write is empty" << std::endl;
        return false;
    }

    auto& connSock = m_connections.back();
    const auto dataSize = static_cast<int>(data.size());
    int bytesSent{};
    while (bytesSent < dataSize)
    {
        auto result = send(connSock, data.data() + bytesSent, dataSize - bytesSent, 0);
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Failed to write data to a socket. Error: " << WSAGetLastError() << std::endl;
            return false;
        }
        bytesSent += result;
    }
    
    return true;
}
