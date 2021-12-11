#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <string>
#include <thread>

#include "defer.h"


#pragma comment (lib, "ws2_32.lib")


int main() {
	int res = 0;

	WSAData wsa_data = {};
	res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (res != 0) { return 0; }
	defer([]() { WSACleanup(); });

	addrinfo hints = {}, * result = nullptr;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	res = getaddrinfo(NULL, "27015", &hints, &result);
	if (res != 0) { return 0; }
	defer([=]() { freeaddrinfo(result); });

	SOCKET listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET) { return 0; }
	defer([=]() { closesocket(listen_socket); });

	res = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) { return 0; }

	res = listen(listen_socket, SOMAXCONN);
	if (res == SOCKET_ERROR) { return 0; }

	while (true) {
		std::cout << "listening..." << std::endl;

		SOCKET client_socket = accept(listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET) { return 0; }

		std::thread([client_socket]() {
			std::cout << "accepted socket " << client_socket << " in thread " << std::this_thread::get_id() << std::endl;
			defer([=]() { closesocket(client_socket); });

			constexpr size_t buffer_len = 1024;
			std::string receive_buffer(buffer_len, '\0');

			while (true) {
				int res = recv(client_socket, receive_buffer.data(), buffer_len, 0);
				if (res == 0) {
					res = shutdown(client_socket, SD_SEND);
					if (res == SOCKET_ERROR) { return; } else { break; }
				}
				if (res < 0) { break; }

				std::cout << std::string_view(receive_buffer).substr(0, res) << std::endl;

				res = send(client_socket, receive_buffer.data(), res, 0);
				if (res == SOCKET_ERROR) { break; }
			}

			std::cout << "thread " << std::this_thread::get_id() << " exited" << std::endl;
		}).detach();
	}
}