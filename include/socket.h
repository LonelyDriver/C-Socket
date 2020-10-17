#pragma once
#include <string>
#include <vector>
#include <exception>
#include "types.h"
#include "logging_manager.h"

namespace infra{
    namespace tcp{
        class SocketException : public std::exception{
        public:
            const char* m_msg;
            const char* m_func;
            int m_error_code;
            const char* m_info;
            SocketException(const std::string& msg, const std::string& func, int error_code, const std::string& info) :
            m_msg(msg.c_str()),
            m_func(func.c_str()),
            m_error_code(error_code),
            m_info(info.c_str()){}

            const char * GetMsg(){return m_msg;}
            const char * GetFunc(){return m_func;}
            const char * GetInfo(){return m_info;}
            int GetErrorCode(){return m_error_code;}

        };

        class Socket{
        public:
            Socket();
            void Connect(const std::string& ip_address, int port);
            void Send(const std::string& message);
            void Send(const std::vector<unsigned char>& message);
            std::vector<unsigned char> Receive(int byte_count=512);
            void Close();

        private:
            sock m_socket;
            Logger m_logging;
        };
    }
}