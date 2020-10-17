#include "socket.h"

int main(){

    infra::tcp::Socket s;
    s.Connect("127.0.0.1", 1337);
    s.Send("Hello, World");
    s.Close();
    

    return 0;
}