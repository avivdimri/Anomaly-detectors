/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include "CLI.h"
#include "commands.h"

using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO : public DefaultIO{
    int id_client;
public:
    explicit SocketIO(int id){
        id_client = id;
    }
    virtual string read() {
        string clientInput="";
        char c=0;
        recv(id_client,&c,sizeof(char),0);
        while(c!='\n'){
            clientInput+=c;
            recv(id_client,&c,sizeof(char),0);
        }
        return clientInput;
    }

    virtual void write(string text){
        send(id_client,text.c_str(),text.length(),0);

    }

    virtual void write(float f) {
        stringstream s;
        s << f;
        string input = s.str();
        const char *text = input.c_str();
        send(id_client,text,input.size(),0);
    }

    virtual void read(float *f) {
        string clientInput="";
        char c=0;
        recv(id_client,&c,sizeof(char),0);
        while(c!='\n'){
            clientInput+=c;
            recv(id_client,&c,sizeof(char),0);
        }
        *f = stof(clientInput);
    }

};

class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
       SocketIO socet(clientID);
       CLI cli(&socet);
       cli.start();
    }
};


class Server {
	thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    bool need_stop;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
