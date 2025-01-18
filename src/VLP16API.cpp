#include <iostream>
#include <iomanip>       // For std::hex and std::setw
#include <cstring>       // For memset
#include <cstdlib>       // For exit
#include <stdio.h>
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in
#include <unistd.h>      // For close()
#include <bitset>
#include <mutex>
#include <thread>
#include "../include/VLP16API.h"


std::mutex packetMutex;

Packet readPacket(Packet &packet){

    return packet;

}


void initUDP(){
    
    std::thread UDPThread(UDP);

    UDPThread.join();

}

void UDP() { 
    
    const int PORT = 2368;
    const int BUFFER_SIZE = 1206;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket.\n";
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket.\n";
        close(sock);
    }

    std::cout << "Listening for UDP packets on port " << PORT << "...\n";

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    Packet packet;

    dataBlock* blocks[] = {
        &packet.block0, &packet.block1, &packet.block2, &packet.block3,
        &packet.block4, &packet.block5, &packet.block6, &packet.block7,
        &packet.block8, &packet.block9, &packet.block10, &packet.block11,
        &packet.block12, &packet.block13, &packet.block14, &packet.block15
    };

    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Zero-initialize the buffer

        ssize_t bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE, 0,
                                         reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

        if (bytesReceived < 0) {
            std::cerr << "Error receiving data.\n";
            break;
        }

        std::cout << "Received " << bytesReceived << " bytes as hex:\n";
        if(bytesReceived == BUFFER_SIZE){
            for(int i = 0 ; i < 12 ; i++){
                memcpy(blocks[i] , buffer + i * 100 , 100);
            }
            memcpy(&packet.timeStamp , buffer + 1200, 4);
            printPacket(packet);
        }else{
            std::cout<<"Packet Failed expected size " << BUFFER_SIZE << "bytes : recieved "<< bytesReceived << " bytes";
        }
    }
    close(sock);
}
