
//Client.cpp
#include "Client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <stdlib.h>

Client::Client(const string& host, const string& port, const string& tprotocol)
{
    mSocket = sock(host,port,tprotocol); //создаем сокет по заданным параметрам
    if(type == SOCK_STREAM)              //если используется протокол TCP, подключаемся к серверу
        mSocket = connectSocket();
}

Client::~Client()
{
    shutdown(mSocket,SHUT_RDWR); //закрываем сокет
}

bool Client::bad()
{
    return (mSocket < 0); //проверка наличия ошибок
}

int Client::sock(const string& host, const string& port,const string& tprotocol)
{
    struct protoent *ppe; //указатель на запись с информацией о протоколе
    struct hostent *phe; //указатель на запись с информацией о хосте
	//обнуляем структуру адреса
	memset(&sin, 0, sizeof(sin));
	//указываем тип адреса (IPv4)
	sin.sin_family = AF_INET;
	//задаем порт
	sin.sin_port = htons((unsigned short)atoi(port.c_str()));
	//преобразовываем имя хоста в IP-адрес, предусмотрев возможность представить его в точечном десятичном формате
	if(phe = gethostbyname(host.c_str()))
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    //преобразовываем имя транспортного протокола в номер протокола
	if((ppe = getprotobyname(tprotocol.c_str())) == 0)
		{
			cout << "Ошибка преобразования имени транспортного протокола: " << strerror(errno) << endl; //сообщение об ошибке
			return -1;
		}

    //используем имя протокола для определения типа сокета
	if(tprotocol == "udp")
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	//создание сокета
	int s = socket(PF_INET, type, ppe->p_proto); //создаем сокет
		if(mSocket < 0)
		{
			cout << "Ошибка создания сокета: " << strerror(errno) << endl;    //сообщение об ошибке
			return -1;
		}
	return s;//возвращаем дескриптор сокета
}

int Client::connectSocket()
{
	//попытка подключить сокет
	if(connect(mSocket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		{
            cout << "Не удалось подключится к серверу: " << strerror(errno) << endl;//сообщение об ошибке
			return -1;
		}
    return mSocket;
}

int Client::send(const string& msg)
{
    return sendto(mSocket,msg.c_str(),msg.size(),0,(struct sockaddr *) &sin, sizeof(sin)); //отправляем заданное сообщение и возвращаем количество переданных байт
}

const string& Client::receive()
{
    char buffer[MAX_SIZE];
    int rcvd = recvfrom(mSocket, buffer, sizeof(buffer), 0, NULL, NULL);//принимаем сообщение
    if(rcvd>0) //проверяем, что сообщение принято
    {
        received = buffer;
        return received;//возвращаем принятое сообщение
    }
    else return "Сообщение не получено.";
}

int Client::MAX_SIZE = 65536; //максимальная длина сообщения
