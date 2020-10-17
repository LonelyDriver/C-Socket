#include "socket.h"
#include <cstring>

infra::tcp::Socket::Socket() :
m_socket(),
m_logging(LogManager::GetLogger("SOCKET"))
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0){
        m_logging->Error(STREAM("Socket creation failed with code "<<m_socket));
    }
}

void infra::tcp::Socket::Connect(const std::string& ip_address, int port){
    struct sockaddr_in server;
    unsigned long addr;
    int ret;
    std::memset(&server, 0, sizeof(server));
    
    addr = inet_addr(ip_address.c_str());

    std::memcpy(&server.sin_addr, &addr, sizeof(addr));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if((ret=connect(m_socket, (struct sockaddr*)&server, sizeof(server))) < 0){
        m_logging->Error(STREAM("Connecting to server: "<<ip_address<<" on port: "<<port<<" failed with errorcode: "<<ret));
        throw SocketException("Connecting to server:"+ip_address+" on port:"+std::to_string(port)+" failed", "Connect",ret,"");
    }
    m_logging->Info(STREAM("Connected to "<<ip_address<<" on port "<<port));
}

void infra::tcp::Socket::Send(const std::string& message){
    Send(std::vector<unsigned char>(message.begin(), message.end()));
}

void infra::tcp::Socket::Send(const std::vector<unsigned char>& message){
    if(m_socket < 0){
        m_logging->Warn(STREAM("Invalid socket"));
        throw SocketException("Invalid socket", "Send", 0,"");
    }
    if(message.empty()){
        return;
    }
    m_logging->Debug(STREAM("Sending message:\n"<<message.data()));
    int message_size = message.size();
    int to_sent = message_size;
    while(to_sent > 0){
        int sent = send(m_socket,(const char*)message.data(), message.size(), 0);
        if(sent < 0){
            m_logging->Error(STREAM("Error sending message: "<<sent));
            throw SocketException("Could not sent message"+std::string((const char*)message.data()), "Send",sent,"");
        }
        to_sent -= sent;
    }
    m_logging->Debug("Message sent successfull");
}

std::vector<unsigned char> infra::tcp::Socket::Receive(int byte_count){
    if(m_socket < 0){
        m_logging->Warn(STREAM("Invalid socket"));
        throw SocketException("Invalid socket", "Receive", 0,"");
    }
    if(byte_count < 1 || 4096){
        throw SocketException("Invalid byte count", "Receive", 0,"Byte count have to be in range of [1:512]");
    }

    std::vector<unsigned char> buf(byte_count);

    int ret = recv(m_socket, buf.data(), buf.size(), 0);
    if(ret < 0){
        throw SocketException("Could not receive message", "Receive", errno, "");
    }
    m_logging->Debug(STREAM("Received "<<ret<<" bytes:\n"<<buf.data()));

    return buf;
}

void infra::tcp::Socket::Close(){
    if(m_socket < 0){
        m_logging->Warn(STREAM("Invalid socket"));
        throw SocketException("Invalid socket", "Close", 0,"");
    }
    int ret = close(m_socket);
    if(ret < 0){
        throw SocketException("Could close socket", "Close", errno, "");
    }
    m_logging->Debug("Closing socket successfull");
}