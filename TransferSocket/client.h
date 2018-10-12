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
		//printf("%d\n", time(NULL));
		_itoa(time(NULL), sendstr, 10);
		//printf("sendtime= %s\n", sendstr);
		send(soc, sendstr, 128, 0);
	}
	void sendname() {
		send(soc, "Mozalic", 128, 0);
	}
	void sendlist (Client* list,int top) {
		int cnt = 0;
		for (int i = 0; i < top; i++) {
			Client now = list[i];
			if (now.relased)continue;
			char tmp[30];
			_itoa(now.id, tmp, 10);
			//printf("id= %s  %d\n", tmp, now.id);
			for (int j = 0; j <= 1; j++)sendstr[cnt * 23 + j] = tmp[j];
			if (now.id < 10)sendstr[cnt * 23 + 1] = ':';
			for (int j = 0; j < 15; j++)sendstr[cnt * 23 + 2 + j] = ip[j];
			sendstr[cnt * 23 + 17] = ':';
			_itoa(now.addr.sin_port, tmp, 10);
			//printf("id= %s  %d\n", tmp, now.addr.sin_port);
			for (int j = 0; j < 4; j++)sendstr[cnt * 23 + 18 + j] = tmp[j];
			sendstr[cnt * 23 + 22] = ';';
			cnt++;
		}sendstr[cnt * 23] = '\0';
		//printf("sendlist= %s\n", sendstr);
		send(soc, sendstr, 128, 0);
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
				flag = i+1;
				break;
			}
		}
		if (!flag) {
			send(soc, "User not found!\0", 128, 0);
			return;
		}
		//printf("sendmessage= %s\n", sendstr);
		send(list[flag-1].soc, sendstr, 128, 0);
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
		strcpy(ip, ip_);
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
		printf("Receive request=%s. From Client:%d\n", a, id);
		switch (a[0]) {
		case 't':
			sendtime();
			break;
		case 'n':
			sendname();
			break;
		case 'l':
			sendlist(list,top);
			break;
		case 's':
			sendmessage(a,list,top);
			break;
		case 'e':
			return -1;
		}
		return 0;
	}
};