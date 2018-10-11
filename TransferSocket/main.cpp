#include <iostream>
#include <algorithm>
#include <thread>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include "client.h"
#pragma comment(lib, "ws2_32.lib")

#define M 1010
#define N 128

Client list[110];
int top_list;

void deal(int pos) {
	char recvstr[N], sendstr[N];
	Client now = list[pos];
	now.sendHello();
	while (1) {
		recv(now.soc, recvstr, N, 0);
		if (now.judge(recvstr,list,top_list) == -1)return;
		//char ip[16];
		//inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
		/*
		time
		name
		list
		send
		exit
		*/

	}
}

int main() {
	WORD wVersionRequest = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(wVersionRequest, &wsadata))return 0;

	std::thread thd[M];
	int top_thd = 0;

	SOCKET ser = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3783);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int len = sizeof(sockaddr);
	bind(ser, (sockaddr*)&addr, len);

	int lis = listen(ser, 10);
	if (lis == SOCKET_ERROR) {
		std::cout << "Failed to listen" << std::endl;
		return 0;
	}
	SOCKADDR_IN addrClient;
	while (1) {
		SOCKET now = accept(ser, (sockaddr*)&addrClient, &len);
		if (now == INVALID_SOCKET) {
			continue;
		}
		char ip[16];
		inet_ntop(AF_INET, &addrClient.sin_addr, ip, sizeof(ip));
		Client nowClient(top_list, ip, now, addrClient, 0);
		list[top_list++]=nowClient;
		thd[top_thd++] = std::thread(deal, top_list-1);
		//top_thd++;

		if (top_thd == 5)break; 
	}
	for (int i = 0; i < top_thd; i++) {
		thd[i].join();
	}
	closesocket(ser);
	WSACleanup();
	return 0;
}