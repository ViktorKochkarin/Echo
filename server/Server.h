//Server.h
#pragma once
#include <string>
using namespace std;

class Server
{
	public:
	Server(const string& port = "3123", const string& tprotocol = "tcp", int qlen = 5);
	~Server();
	bool bad(); //проверка наличия ошибок
	void start();//запуск сервера
	void process(const string& str);//дополнительная обработка принятого сообщения

	private:
	int sock(const string& port, const string& tprotocol, int qlen);//создание сокета и привязка к интерфейсу
	int mSocket;//дескриптор сокета
	int type;   //тип протокола
	static int MAX_SIZE; //максимальная длина сообщения
};

