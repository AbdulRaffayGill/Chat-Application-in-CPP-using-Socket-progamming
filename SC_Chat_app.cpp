#include <iostream>
#include <winsock2.h>
using namespace std;

class server
{
    SOCKET server_socket;
    SOCKET client_socket;
    string send_message;
    char rec_message[5000];
    char name[1024];
    int d;

public:
    string client_name;

    server()
    {
        WSADATA wsa_var;
        WSAStartup(MAKEWORD(2, 2), &wsa_var);
    }

    void connection()
    {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(5000);
        serv.sin_addr.s_addr = INADDR_ANY;
        bind(server_socket, (struct sockaddr *)&serv, sizeof(serv));
        listen(server_socket, 5);
    }

    void lis_for_conn()
    {
        sockaddr_in client_addr;
        int n = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &n);
        recv(client_socket, name, sizeof(name) - 1, 0);
        name[sizeof(name) - 1] = '\0';
        client_name = string(name);
        cout << client_name << " connected with you\n";
    }

    string take_mess()
    {
        string s;
        cout << "Raffay:";
        getline(cin, s);
        return s;
    }

    void encrypt(char *buffer, int size)
    {
        char key = 0;
        for (int i = 0, n = 1; i < size; i++,n++)
        {
            key = 6 * n - 1;
            *(buffer + i) = ~(*(buffer + i));
            *(buffer + i) ^= key;
        }
    
    }

    void decrypt(char *buffer, int size)
    {
        char key = 6 * size - 1;
        for (int i = size - 1; i >= 0; i--)
        {
            *(buffer + i) ^= key;
            *(buffer + i) = ~(*(buffer + i));
            key -= 6;
        }
    }

    void send_mess(string s)
    {
        encrypt(&s[0], s.size());
        send(client_socket, s.c_str(), s.length(), 0);
    }

    void receive(bool &t)
    {
        char s[5000];
        int bytesReceived = recv(client_socket, s, sizeof(s), 0);
        if (bytesReceived > 0)
        {
            decrypt(s, bytesReceived);
            s[bytesReceived] = '\0';
            cout << client_name << ": " << s << endl;
        }
        if (s[0] == ';')
        {
            closesocket(client_socket);
            closesocket(server_socket);
            WSACleanup();
            t = true;
            return;
        }
    }

    ~server()
    {
        string s = "\nChat ended, other person left\n";
        send(client_socket, s.c_str(), s.length(), 0);
        closesocket(client_socket);
        closesocket(server_socket);
        WSACleanup();
    }

    friend class client;
};

class client
{
private:
    SOCKET cl_socket;
    string name_to_send;
    char name_to_receive[1024];
    char data_receive[5000];
    string my_name = "";
    server obj;
    struct sockaddr_in client_addr;

public:
    void conn()
    {
        cl_socket = socket(AF_INET, SOCK_STREAM, 0);
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(5000);
        client_addr.sin_addr.s_addr = inet_addr("Add server IPv4 here");
    }

    void client_conn()
    {
        if (connect(cl_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR)
        {
            cerr << "Connection failed!" << endl;
            closesocket(cl_socket);
            WSACleanup();
        }
        else
        {
            cout << "Raffay Connected\n";
            cout << "Enter your name:\n";
            getline(cin, my_name);
            send(cl_socket, my_name.c_str(), my_name.length(), 0);
        }
    }

    void send_mess()
    {
        string temp;
        cout << my_name << ": ";
        getline(cin, temp);
        obj.encrypt(&temp[0], temp.size());
        send(cl_socket, temp.c_str(), temp.length(), 0);
    }

    ~client()
    {
        string temp = "\nChat ended, other person left\n";
        send(cl_socket, temp.c_str(), temp.length(), 0);
        closesocket(cl_socket);
        WSACleanup();
    }

    void receive_mess(bool &t)
    {
        int bytesReceived = recv(cl_socket, data_receive, sizeof(data_receive), 0);
        if (bytesReceived > 0)
        {
            data_receive[bytesReceived] = '\0';
            obj.decrypt(data_receive, bytesReceived);
            cout << "Raffay: " << data_receive << endl;
        }
        if (data_receive[0] == ';')
        {
            closesocket(cl_socket);
            WSACleanup();
            t = true;
            return;
        }
    }
};

int server_module()
{
    bool yes = false;
    server a;
    a.connection();
    a.lis_for_conn();
    system("cls");
    for (int i = 0;; i++)
    {
        a.send_mess(a.take_mess());
        a.receive(yes);
        if (yes)
        {
            cout << "Chat Closing, other person left\n";
            Sleep(2000);
            return 0;
        }
    }
}

int client_module()
{
    bool yes = false;
    client a;
    a.conn();
    a.client_conn();
    system("cls");
    for (int i = 0;; i++)
    {
        a.receive_mess(yes);
        if (yes)
        {
            cout << "Chat closing, other person left\n";
            Sleep(2000);
            return 0;
        }
        a.send_mess();
    }
}

int main()
{
m:
    cout << R"(

       _____   _____                   

      / ____| / ____|              
     | (___  | |        
      \___ \ | |   
      ____) || |____ 
     |_____/  \_____|


   ____ _   _    _    _____  
  / ___| | | |  / \  |_   _| 
 | |   | |_| | / _ \   | |   
 | |___|  _  |/ ___ \  | |  
  \____|_| |_/_/   \_\ |_| 
                                  
)" << endl;

    cout << "Welcome to ScChat, Your own Secure chatting application, to start, Select either of the options:\n1. You want to connect\n2. You want to listen\n";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice != 1 && choice != 2)
    {
        cout << "Invalid choice\n, Going back to main menu:\n";
        Sleep(800);
        goto m;
    }
    else if (choice == 1)
    {
        client_module();
    }
    else
    {
        server_module();
    }
}
