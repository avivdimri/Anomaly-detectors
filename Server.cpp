
#include "Server.h"
Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0) {
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (::bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0){
        throw "bind failed";
    }
    if (listen(fd,3)<0) {
        throw "listen failed";
    }
    need_stop = false;
}

void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch, this]() {
        while (!need_stop) {
            socklen_t client_size = sizeof(client);
            alarm(5);
            int a_client = accept(fd, (struct sockaddr *) &client, &client_size);
            if (a_client < 0) {
                throw "accept failed";
            }
            ch.handle(a_client);
            close(a_client);
        }
        close(fd);
    });

}

void Server::stop(){
    need_stop = true;
	t->join(); // do not delete this!
}

Server::~Server() {
}

