#include <winsock2.h>
#include <ws2tcpip.h> // Necesario para inet_pton
#include <iostream>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib") // Enlazamos la biblioteca Winsock

int main() {
    WSADATA wsaSetupData; // Estructura de datos para la configuración de Winsock
    SOCKET connectionSocket; // Socket para la conexión con el servidor
    sockaddr_in remoteServerAddr; // Estructura de dirección del servidor remoto
    char rxBuffer[1024] = { 0 }; // Buffer para los datos recibidos

    // Iniciar la librería Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaSetupData) != 0) {
        std::cerr << "Fallo al iniciar el entorno Winsock." << std::endl;
        return 1;
    }

    // Crear el socket de comunicación para el cliente
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectionSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket de conexión." << std::endl;
        WSACleanup(); // Limpiar recursos
        return 1;
    }

    // Configurar la dirección del servidor al que nos queremos conectar
    remoteServerAddr.sin_family = AF_INET; // IPv4
    remoteServerAddr.sin_port = htons(8080); // Puerto del servidor, en orden de bytes de red

    // Convertir la dirección IP de texto a formato binario
    if (inet_pton(AF_INET, "127.0.0.1", &remoteServerAddr.sin_addr) <= 0) {
        std::cerr << "Error al procesar la dirección IP del servidor." << std::endl;
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }

    // Intentar establecer la conexión con el servidor
    if (connect(connectionSocket, (struct sockaddr*)&remoteServerAddr, sizeof(remoteServerAddr)) == SOCKET_ERROR) {
        std::cerr << "No se pudo conectar al servidor." << std::endl;
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "¡Conexión establecida con el servidor remoto!" << std::endl;

    // Definir y enviar un mensaje al servidor
    const char* outgoingMessage = "Saludos cordiales desde el cliente!";
    if (send(connectionSocket, outgoingMessage, strlen(outgoingMessage), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar el mensaje al servidor." << std::endl;
        closesocket(connectionSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Enviando información: \"" << outgoingMessage << "\"" << std::endl;

    // Recibir la respuesta que el servidor nos envíe
    int receivedBytes = recv(connectionSocket, rxBuffer, sizeof(rxBuffer) - 1, 0);
    if (receivedBytes > 0) {
        rxBuffer[receivedBytes] = '\0'; // Asegurar la terminación de la cadena
        std::cout << "Mensaje recibido del servidor: " << rxBuffer << std::endl;
    } else {
        std::cerr << "Fallo al recibir la respuesta del servidor." << std::endl;
    }

    // Cerrar el socket y limpiar recursos
    closesocket(connectionSocket);
    WSACleanup();
    return 0;
}
