#include<iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<ws2tcpip.h>
#include<string.h>
#include<thread>

#pragma comment (lib,"ws2_32.lib")


using namespace std;

#define PORT 9000

void sockFun(SOCKET cSocket) {
    while (true) {
        int bRec = 0;
        char buf[1023];
        bRec = recv(cSocket, buf, 1023, 0);
        if (bRec == SOCKET_ERROR) {
            cout << "ERROR : Can't Receive Message" << endl;
            cout << "Error # : " << WSAGetLastError();
            return;
        }
        cout << string(buf, 0, bRec) << endl;
    }
}

int main() {

    // Initialize Winsock.

    WSAData wd;
    int val = WSAStartup(MAKEWORD(2, 2), &wd);
    if (val != 0) {
        cout << "ERROR : Can't Initialize Winsock" << endl;
        cout << "Error # : " << WSAGetLastError();
        return 0;
    }

    cout << "Winsock Initialized Successfully" << endl;

    // Initialzie Socket.

    SOCKET cSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (cSocket == SOCKET_ERROR) {
        cout << "ERROR : Can't Initialize Socket" << endl;
        cout << "Error # : " << WSAGetLastError();
        return 0;
    }

    cout << "Socket Initialized Successfully" << endl;

    // Connect to server.

    sockaddr_in lst;

    lst.sin_port = htons(PORT);
    lst.sin_family = AF_INET;
    lst.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    int cVal = connect(cSocket, (sockaddr*)&lst, sizeof(lst));
    if (cVal == SOCKET_ERROR) {
        cout << "ERROR : Can't Connect to Server" << endl;
        cout << "Error # : " << WSAGetLastError();
        return 0;
    }

    cout << "Connected to Server on Port # : " << ntohs(lst.sin_port) << endl;

    // Receiving Welcome Message

    char wmsg[1023];

    int wMsg = recv(cSocket, wmsg, 1023, 0);

    if (wMsg == SOCKET_ERROR) {
        cout << "ERROR : Error in receiving welcome message" << endl;
        return 0;
    }

    cout << endl << string(wmsg, 0, wMsg) << endl;

    cout << endl << "-------------------------CLIENT-------------------------" << endl;

    // Send & Receive Response.

    while (true) {

        char msg[1023];

        thread t2(sockFun, cSocket);

        cin.getline(msg, 1023);

        int sd = send(cSocket, msg, strlen(msg) + 1, 0);

        if (sd == SOCKET_ERROR) {
            cout << "ERROR : Can't Send Message" << endl;
            cout << "Error # : " << WSAGetLastError() << endl;
            return 0;
        }

        t2.detach();
    }

    // Close Socket.

    closesocket(cSocket);

    // Close Winsock.

    WSACleanup();

    return 0;
}

