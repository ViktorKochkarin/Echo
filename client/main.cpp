//main.cpp
#include "Client.h"

int main(int argc, char **argv)
{
    if(argc == 2)
    {
        string host = "localhost";  //имя хоста, можно переделать под передачу через аргументы main
        string port = "3123";       //порт сервера, можно переделать под передачу через аргументы main, должен совпадать с портом указанным на сервере
        string tprotocol = argv[1]; //передаем протокол работы через параметр main
        Client cl(host,port,tprotocol);//создаем объект класса Client. По факту создаем сокет и (в случае TCP) соединяемся с сервером
        if (cl.bad())   //проверяем на наличие ошибок
            return -1;
        else
        {
                string msg;
                cout << "Введите сообщение: " << endl;
                char buffer[Client::MAX_SIZE];
                cin.getline(buffer,sizeof(buffer),'\n'); //считывает сообщение с консоли
                msg = buffer;
                if(msg.size()>Client::MAX_SIZE)         //проверяем, чтобы сообщение было не больше 64 кб
                msg.resize(Client::MAX_SIZE);           //обрезаем его в случае, если оно превышает 64 кб
                if(cl.send(msg) < 0)                           //отправляем сообщение
                    cout << "Сообщение не отправлено." << endl;
                const string& rcv = cl.receive();              //принимаем ответ
                cout << "Получено сообщение: " << rcv << endl; //выводим ответ
        }
    }
    return 0;
}
