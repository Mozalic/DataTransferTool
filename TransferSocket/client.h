#pragma once
#include <iostream>
#include <algorithm>
#include <thread>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <string.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

#define M 1010
#define N 128

class Client {
private:
	char sendstr[128];
	void sendtime() {
		_itoa_s(time(NULL), sendstr, 2, 10);
		send(soc, sendstr, strlen(sendstr), 0);
	}
	void sendname() {
		send(soc, "Mozalic", strlen("Mozalic"), 0);
	}
	void sendlist (Client* list,int top) {
		for (int i = 0; i < top; i++) {
			Client now = list[i];
			if (now.relased)continue;
			char tmp[30];
			_itoa_s(now.id, tmp, 2, 10);
			for (int j = 0; j <= 1; j++)sendstr[i * 23 + j] = tmp[j];
			for (int j = 0; j < 16; j++)sendstr[i * 23 + 2 + j] = ip[j];
			sendstr[i * 23 + 18] = ':';
			_itoa_s(now.addr.sin_port, tmp, 2, 10);
			for (int j = 0; j < 4; j++)sendstr[i * 23 + 19 + j] = tmp[j];
		}
		send(soc, sendstr, strlen(sendstr), 0);
	}
	void sendmessage(char* a, Client* list, int top) {
		char tmp[128];
		int flag = 0;
		for (int i = 8; i < 128; i++) {
			sendstr[i - 8] = a[i];
		}
		int tag = (a[5] - '0') * 10 + a[6] - '0';
		for (int i=0; i < top; i++) {
			if (list[i].relased)continue;
			if (list[i].id == tag) {
				flag = i;
				break;
			}
		}
		if (!flag) {
			send(soc, "User not found!", strlen("User not found!"), 0);
		}
		send(list[flag].soc, sendstr, strlen(sendstr), 0);
	}
public:
	int id;
	char ip[16];
	SOCKET soc;
	SOCKADDR_IN addr;
	bool relased;
	Client() {

	}
	Client(int id_,char* ip_ ,SOCKET soc_, SOCKADDR_IN addr_,bool relased_){
		id = id_;
		strcpy_s(ip, ip_);
		soc = soc_;
		addr = addr_;
		relased = relased_;
	}
	void sendHello() {
		send(soc, "Hello!", 7, 0);
	}
	int judge(char* a, Client* list,int top) {
		/*
		time
		name
		list
		send id message
		exit
		*/
		switch (a[0]) {
		case 't':
			sendtime();
			break;
		case 'n':
			sendname();
			break;
		case 'l':
			sendlist(list,top);
		case 's':
			sendmessage(a,list,top);
		case 'e':
			return -1;
		}
		return 0;
	}
};