#pragma once

#if __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define sock int
#elif WINVER
#include <winsock2.h>
#endif

