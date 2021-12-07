#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <thread>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	int check_result = 0;

	//инициализация
	WSADATA wsa_data;
	check_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

	if (check_result != 0)
	{
		std::cout << "error starting\n" << "error: " << check_result << "\n";
		return 1;
	}

	//получение адреса
	addrinfo* result = nullptr;
	addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_addrlen = 0;
	hints.ai_canonname = 0;
	hints.ai_addr = 0;
	hints.ai_next = 0;
	hints.ai_flags = AI_PASSIVE;

	check_result = getaddrinfo("ru.wikipedia.org", "80", &hints, &result);

	if (check_result != 0)
	{
		std::cout << "error getting address\n";
		return 1;
	}

	//создание сокета
	SOCKET sckt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (sckt == INVALID_SOCKET)
	{
		std::cout << "error creating socket\n";
		return 1;
	}

	//привязка сокета
	/*check_result = bind(sckt, result->ai_addr, result->ai_addrlen);
	freeaddrinfo(result);

	if (check_result != 0)
	{
		std::cout << "error binding\n" << WSAGetLastError() << "\n";
		return 1;
	}*/

	check_result = connect(sckt, result->ai_addr, result->ai_addrlen);
	if (check_result == SOCKET_ERROR)
	{
		std::cout << "ass";

		closesocket(check_result);
		sckt = INVALID_SOCKET;
		return 1;
	}

	//прослушивание сокета

	/*check_result = listen(sckt, SOMAXCONN);

	if (check_result == SOCKET_ERROR)
	{
		std::cout << "error listening\n" << WSAGetLastError() << "\n";
		return -1;
	}*/


	//std::string request("GET / HTTP/1.1 Host: Localhost:8000");
	std::string request(
		"GET /wiki/HTTP HTTP/1.1\n"
		"Host: ru.wikipedia.org\n"
		"Accept: text/html\n"
		"accept-language: ru-RU\n"
		"user-agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.93 Safari/537.36\n"
		"\n"
	);

	std::thread thr([sckt, request]()
	{
		//while (true)
		{
			send(sckt, request.c_str(), request.size(), 0);
			std::cout << "request sent\n\n";
			//std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	});
	

	char* answer = new char[20480];
	for (unsigned int i = 0; i < 20480; ++i)
		answer[i] = 0;

	//while (true)
	{
		std::cout << "trying to recv...\n\n";
		int msg_size = recv(sckt, answer, 20480, 0);
		std::cout << msg_size << "\n" << answer << "\n";

		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	thr.join();

/*
	181
	HTTP/1.1 400 Bad Request
	Server: QRATOR
	Date: Mon, 06 Dec 2021 19:28:52 GMT
	Content-Type: text/html
	Content-Length: 36
	Connection: close
*/







	WSACleanup();

	return 0;
}