#include <winsock2.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib") // Llibreria per treballar amb Winsock

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    char buffer[1024] = { 0 };

    // Inicialitzar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicialitzar Winsock." << std::endl;
        return 1;
    }

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar l'adreça del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Lligar el socket al port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al lligar el socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Escoltar connexions entrants
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Error al escoltar connexions." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Esperant connexions..." << std::endl;

    // Acceptar una connexió
    int clientSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al acceptar la connexió." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connectat!" << std::endl;

    // Rebre el missatge del client
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Assegurar finalització de cadena
        std::cout << "Missatge rebut del client: " << buffer << std::endl;
    } else {
        std::cerr << "Error al rebre el missatge del client." << std::endl;
    }

    // Enviar una resposta al client
    const char* response = "Hola des del servidor!";
    if (send(clientSocket, response, strlen(response), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar la resposta al client." << std::endl;
    } else {
        std::cout << "Resposta enviada al client." << std::endl;
    }

    // Tancar sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
