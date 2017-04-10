
//Client.h
#pragma once
#include <netinet/in.h>
#include <iostream>
#include <string>

using namespace std;

class Client
{
    public:
    Client(const string& host, const string& port, const string& tprotocol);
    ~Client();
    bool bad();                     //функция проверки наличия ошибок
    int send(const string& msg);    //функция отправки сообщения
    const string& receive();        //функция принятия сообщения
    static int MAX_SIZE;

    private:
    int sock(const string& host, const string& port,const string& tprotocol); //создание сокета
    int connectSocket();     //соединение с сервером
    struct sockaddr_in sin; //структура IP-адреса оконечной точки
    int mSocket; //дескриптор сокета
    int type; // тип протокола
    string received; //полученное сообщение
};
