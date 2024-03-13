#include <iostream>
#include <string>
#include <WS2tcpip.h> // Include winsock library

int main() {
    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        std::cerr << "Can't initialize winsock! Quitting" << std::endl;
        return -1;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        WSACleanup();
        return -1;
    }

    // Connect to server
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // Port
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {
        std::cerr << "Can't connect to server! Quitting" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    // While loop to send and receive data
    char buf[4096];
    std::string userInput;

    do {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput.size() > 0) {
            // Send the text
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // Wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0) {
                    std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
                }
            }
        }

    } while (userInput.size() > 0);

    // Gracefully close everything
    closesocket(sock);
    WSACleanup();

    return 0;
}
