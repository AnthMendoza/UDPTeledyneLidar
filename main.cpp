#include <iostream>
#include <iomanip>       // For std::hex and std::setw
#include <cstring>       // For memset
#include <cstdlib>       // For exit
#include <stdio.h>
#include <sys/socket.h>  // For socket functions
#include <netinet/in.h>  // For sockaddr_in
#include <unistd.h>      // For close()
#include <bitset>


struct dataBlock{

    uint8_t flagFF;
    uint8_t flagEE;

    uint16_t azimuth;

    uint16_t distChannel0;
    uint8_t reflectChennel0;
    uint16_t distChannel1;
    uint8_t reflectChennel1;
    uint16_t distChannel2;
    uint8_t reflectChennel2;
    uint16_t distChannel3;
    uint8_t reflectChennel3;
    uint16_t distChannel4;
    uint8_t reflectChennel4;
    uint16_t distChannel5;
    uint8_t reflectChennel5;
    uint16_t distChannel6;
    uint8_t reflectChennel6;
    uint16_t distChannel7;
    uint8_t reflectChennel7;
    uint16_t distChannel8;
    uint8_t reflectChennel8;
    uint16_t distChannel9;
    uint8_t reflectChennel9;
    uint16_t distChannel10;
    uint8_t reflectChennel10;
    uint16_t distChannel11;
    uint8_t reflectChennel11;
    uint16_t distChannel12;
    uint8_t reflectChennel12;
    uint16_t distChannel13;
    uint8_t reflectChennel13;
    uint16_t distChannel14;
    uint8_t reflectChennel14;
    uint16_t distChannel15;
    uint8_t reflectChennel15;
};


//data block is a struct which takes data to construct, thus memcpy wont fill properllly 




uint16_t reverse16(uint16_t &byte){
    return ((byte & 0x00FF) << 8) | 
           ((byte & 0xFF00) >> 8)  ;
}
uint32_t reverse32(uint32_t &byte){
    return ((byte & 0x000000FF) << 24) | 
           ((byte & 0x0000FF00) << 8)  | 
           ((byte & 0x00FF0000) >> 8)  | 
           ((byte & 0xFF000000) >> 24);
}

//void formatBlock(dataBlock &block){
//    block.azimuth = reverse16(block.azimuth);
//    block.distChannel0 = reverse16(block.distChannel0);
//}

void printPacket(const dataBlock &block){
    //std::cout<<static_cast<int>(packet.block0.azimuth/100);
    //std::cout<<static_cast<int>(packet.block0.distChannel0);
    //std::cout<<static_cast<int>(packet.block0.distChannel1);
    //std::cout<<static_cast<int>(packet.block0.distChannel2);
    //std::cout<<static_cast<int>(packet.block0.distChannel3);
    std::cout<<static_cast<int>(block0.flagEE);
    std::cout<<static_cast<int>(block0.flagFF);
    std::cout<<static_cast<int>(block1.flagEE);
    std::cout<<static_cast<int>(block1.flagFF);
    std::cout<<static_cast<int>(block2.flagEE);
    std::cout<<static_cast<int>(block2.flagFF);
}


int main() { 
    const int PORT = 2368;
    const int BUFFER_SIZE = 1206;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket.\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket.\n";
        close(sock);
        return 1;
    }

    std::cout << "Listening for UDP packets on port " << PORT << "...\n";

    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

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
                dataBlock block;
                memcpy(&block , buffer + i * 100 , 100);
                printPacket(block);
            }
        }else{
            std::cout<<"Packet Failed expected size " << BUFFER_SIZE << "bytes : recieved "<< bytesReceived << " bytes";
        }
    }
    close(sock);
    return 0;
}

