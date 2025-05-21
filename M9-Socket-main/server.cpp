#include <winsock2.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "Ws2_32.lib") // Enlazamos con la librería Winsock

int main() {
    WSADATA networkData; // Información de inicialización de Winsock
    SOCKET hostSocket, peerSocket; // Sockets para el servidor y el cliente conectado
    sockaddr_in serverAddress, clientAddressInfo; // Estructuras de dirección para el servidor y el cliente
    char messageBuffer[1024] = { 0 }; // Buffer para almacenar mensajes recibidos/enviados

    // Inicializar la API de sockets de Windows
    if (WSAStartup(MAKEWORD(2, 2), &networkData) != 0) {
        std::cerr << "Fallo al iniciar el subsistema Winsock." << std::endl;
        return 1;
    }

    // Creación del socket principal para el servidor
    hostSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hostSocket == INVALID_SOCKET) {
        std::cerr << "Error al establecer el socket principal." << std::endl;
        WSACleanup(); // Limpiar recursos de Winsock
        return 1;
    }

    // Configurar los detalles de la dirección del servidor
    serverAddress.sin_family = AF_INET; // Familia de direcciones IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Aceptar conexiones desde cualquier interfaz de red
    serverAddress.sin_port = htons(8080); // Puerto de escucha en orden de bytes de red

    // Vincular el socket del servidor a la dirección y puerto especificados
    if (bind(hostSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "No se pudo vincular el socket al puerto especificado." << std::endl;
        closesocket(hostSocket); // Cerrar el socket antes de salir
        WSACleanup();
        return 1;
    }

    // Poner el socket en modo de escucha para conexiones entrantes
    if (listen(hostSocket, 1) == SOCKET_ERROR) { // El '1' indica el tamaño máximo de la cola de conexiones pendientes
        std::cerr << "Error al poner el socket en modo de escucha." << std::endl;
        closesocket(hostSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Servidor esperando conexiones entrantes..." << std::endl;

    // Aceptar la primera conexión entrante de un cliente
    int clientAddressSize = sizeof(clientAddressInfo);
    peerSocket = accept(hostSocket, (struct sockaddr*)&clientAddressInfo, &clientAddressSize);
    if (peerSocket == INVALID_SOCKET) {
        std::cerr << "Fallo al aceptar la conexión del cliente." << std::endl;
        closesocket(hostSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "¡Un cliente se ha conectado satisfactoriamente!" << std::endl;

    // Recibir datos enviados por el cliente
    int bytesRead = recv(peerSocket, messageBuffer, sizeof(messageBuffer) - 1, 0);
    if (bytesRead > 0) {
        messageBuffer[bytesRead] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
        std::cout << "Mensaje recibido del cliente remoto: " << messageBuffer << std::endl;
    } else {
        std::cerr << "No se pudo recibir el mensaje del cliente." << std::endl;
    }

    // Preparar y enviar una respuesta al cliente
    const char* serverReply = "Un saludo desde el servidor TCP!";
    if (send(peerSocket, serverReply, strlen(serverReply), 0) == SOCKET_ERROR) {
        std::cerr << "Error al enviar la respuesta al cliente conectado." << std::endl;
    } else {
        std::cout << "Respuesta enviada al cliente conectado." << std::endl;
    }

    // Cerrar los sockets y limpiar Winsock
    closesocket(peerSocket); // Cerrar el socket del cliente
    closesocket(hostSocket); // Cerrar el socket del servidor
    WSACleanup();
    return 0;
}
