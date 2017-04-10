//Server.cpp
#include "Server.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <iostream>

bool cDig(char c);

Server::Server(const string& port, const string& tprotocol, int qlen)
{
    mSocket = sock(port,tprotocol,qlen);//создаем сокет и привязываем его к интерфейсу, запускаем прослушивание
}

Server::~Server()
{
    shutdown(mSocket,SHUT_RDWR);//закрываем сокет
}

bool Server::bad()
{
    return (mSocket < 0);//проверка наличия ошибок
}

void Server::start()
{
    cout << "Сервер запущен." << endl;
    int client, rcvd;   //дескриптор сокета клиента и количество принятых байт
    struct sockaddr_in  clientAddress;			                //структура IP-адреса клиента
	unsigned int clientAddressSize = sizeof(clientAddress);	    //размер структуры адреса
	char buffer[MAX_SIZE];					                        //буфер сообщения
	string msg;                                                //строка для обработки

	while(true)
	{
        if(type == SOCK_STREAM) //если используется протокол TCP, принимаем подключение
        {
            client = accept(mSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);	//принимаем входящее подключение, адрес клиента в clientAddress
            if(client < 0)		//проверяем результат
            {
                cout << "Ошибка принятия подключения: " << strerror(errno) << endl;   //сообщение об ошибке
                break;
            }
        }
        else
        {
            client = mSocket; //приравниваем дескриптора сокета клиента к дескриптору сокета сервера в случае использования протокола UDP
        }
        rcvd = recvfrom(client, &buffer, sizeof(buffer), 0, (struct sockaddr *) &clientAddress, &clientAddressSize); //принимаем сообщение
        if(rcvd>0)
            {
                msg = buffer;
                process(msg);               //дополнительная работа сервера
                sendto(client, buffer, rcvd, 0, (struct sockaddr *) &clientAddress, clientAddressSize);                 //отправляем сообщение обратно
            }
	}
}

int Server::sock(const string& port, const string& tprotocol, int qlen)
{
	struct protoent *ppe;   //указатель на запись с информацией о протоколе
	struct sockaddr_in sin; //структура IP-адреса оконечной точки
	int s; //будущий дескриптор сокета
	//обнуляем структуру адреса
	memset(&sin, 0, sizeof(sin));
	//указываем тип адреса IPv4, для IPv6 необходимо указать AF_INET6
	sin.sin_family = AF_INET;
	//указываем в качестве адреса шаблон INADDR_ANY - все сетевые интерфейсы
	sin.sin_addr.s_addr = INADDR_ANY;
	//задаем порт
	sin.sin_port = htons((unsigned short)atoi(port.c_str()));
	//преобразовываем имя транспортного протокола в номер протокола
	if ((ppe = getprotobyname(tprotocol.c_str())) == 0)
	{
        cout << "Ошибка преобразования имени транспортного протокола: " << strerror(errno) << endl; //сообщение об ошибке
		return -1;
	}
	//используем имя протокола для определения типа сокета
	if (tprotocol == "udp")
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	//создаем сокет
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
	{
        cout << "Ошибка создания сокета: " << strerror(errno) << endl;    //сообщение об ошибке
		return -1;
	}
	//привязка сокета с проверкой результата
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
        cout << "Ошибка связывания сокета: " << strerror(errno) << endl;  //сообщение об ошибке
		return -1;
	}
	//запуск прослушивания с проверкой результата
	if (type == SOCK_STREAM && listen(s, qlen) <0)
	{
        cout << "Ошибка прослушивания сокета: " << strerror(errno) << endl;//сообщение об ошибке
		return -1;
	}
	return s;	//возвращаем дескриптор сокета
}

void Server::process(const string& str) //функция дополнительной обработки сообщения
{
    vector<int> digits; //создаем вектор для всех вхождений чисел в сообщение
    string::const_iterator it = str.begin();
    while(it<str.end()) //обходим все сообщение в поиске символов чисел
    {
        it = find_if(it,str.end(),cDig); //находим первый символ числа
        if(it!=str.end())   //проверяем, был ли найден хотя бы один символ числа
            digits.push_back(*it-48);//добавляем соответствующее значение числа в вектор
        it++;//сдвигаем итератор для дальнейшего поиска
    }
    double sum = accumulate(digits.begin(),digits.end(),0);//вычисляем сумму всех чисел в векторе
    cout << "Сумма всех чисел в сообщении: " << sum << endl;//вводим сумму
    sort(digits.begin(),digits.end(),greater<int>());//сортируем вектор по убыванию
    for(vector<int>::iterator it = digits.begin(); it != digits.end();++it)//выводим все числа в векторе в порядке убывания
    {
        cout << *it << ' ';
    }
    cout << endl;
    cout << "Максимальное значение: " << *(digits.begin()) << endl; //выводим максимальное и минимальное значение в векторе (просто выводим первое и последнее число, так как вектор
    cout << "Минимальное значение: " << *(digits.end()-1) << endl;  //был предварительно отсортирован
}

bool cDig(char c) //спомогательная функция, передаваяема функции find_if в качестве предиката
{
    int ch = int(c);
    return isdigit(ch)!=0;//возвращаем true если символ - число, иначе false
}

int Server::MAX_SIZE = 65536; //максимальная длина сообщения

