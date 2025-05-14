#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib") // Llibreria per treballar amb Winsock

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    // Inicialitzar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicialitzar Winsock." << std::endl;
        return 1;
    }

    // Crear el socket del client
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar l'adreça del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Error al convertir l'adreça IP." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al connectar al servidor." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connexió establerta amb el servidor." << std::endl;

    // Enviar missatge al servidor
    const char* message = "Hola servidor!";
    if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar el missatge." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Enviant missatge: " << message << std::endl;

    // Rebre resposta del servidor
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Assegurar finalització de cadena
        std::cout << "Resposta del servidor: " << buffer << std::endl;
    } else {
        std::cerr << "Error al rebre la resposta del servidor." << std::endl;
    }

    // Tancar el socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
