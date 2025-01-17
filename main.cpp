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

    uint16_t AdistChannel0;
    uint8_t  AreflectChennel0;
    uint16_t AdistChannel1;
    uint8_t  AreflectChennel1;
    uint16_t AdistChannel2;
    uint8_t  AreflectChennel2;
    uint16_t AdistChannel3;
    uint8_t  AreflectChennel3;
    uint16_t AdistChannel4;
    uint8_t  AreflectChennel4;
    uint16_t AdistChannel5;
    uint8_t  AreflectChennel5;
    uint16_t AdistChannel6;
    uint8_t  AreflectChennel6;
    uint16_t AdistChannel7;
    uint8_t  AreflectChennel7;
    uint16_t AdistChannel8;
    uint8_t  AreflectChennel8;
    uint16_t AdistChannel9;
    uint8_t  AreflectChennel9;
    uint16_t AdistChannel10;
    uint8_t  AreflectChennel10;
    uint16_t AdistChannel11;
    uint8_t  AreflectChennel11;
    uint16_t AdistChannel12;
    uint8_t  AreflectChennel12;
    uint16_t AdistChannel13;
    uint8_t  AreflectChennel13;
    uint16_t AdistChannel14;
    uint8_t  AreflectChennel14;
    uint16_t AdistChannel15;
    uint8_t  AreflectChennel15;

    uint16_t BdistChannel0;
    uint8_t  BreflectChennel0;
    uint16_t BdistChannel1;
    uint8_t  BreflectChennel1;
    uint16_t BdistChannel2;
    uint8_t  BreflectChennel2;
    uint16_t BdistChannel3;
    uint8_t  BreflectChennel3;
    uint16_t BdistChannel4;
    uint8_t  BreflectChennel4;
    uint16_t BdistChannel5;
    uint8_t  BreflectChennel5;
    uint16_t BdistChannel6;
    uint8_t  BreflectChennel6;
    uint16_t BdistChannel7;
    uint8_t  BreflectChennel7;
    uint16_t BdistChannel8;
    uint8_t  BreflectChennel8;
    uint16_t BdistChannel9;
    uint8_t  BreflectChennel9;
    uint16_t BdistChannel10;
    uint8_t  BreflectChennel10;
    uint16_t BdistChannel11;
    uint8_t  BreflectChennel11;
    uint16_t BdistChannel12;
    uint8_t  BreflectChennel12;
    uint16_t BdistChannel13;
    uint8_t  BreflectChennel13;
    uint16_t BdistChannel14;
    uint8_t  BreflectChennel14;
    uint16_t BdistChannel15;
    uint8_t  BreflectChennel15;
};

struct Packet{
    dataBlock block0;
    dataBlock block1;
    dataBlock block2;
    dataBlock block3;
    dataBlock block4;
    dataBlock block5;
    dataBlock block6;
    dataBlock block7;
    dataBlock block8;
    dataBlock block9;
    dataBlock block10;
    dataBlock block11;
    dataBlock block12;
    dataBlock block13;
    dataBlock block14;
    dataBlock block15;
};





void reverse16(uint16_t &byte){

    byte = ((byte & 0x00FF) << 8) | 
           ((byte & 0xFF00) >> 8)  ;

}
void reverse32(uint32_t &byte){

    byte = ((byte & 0x000000FF) << 24) | 
           ((byte & 0x0000FF00) << 8)  | 
           ((byte & 0x00FF0000) >> 8)  | 
           ((byte & 0xFF000000) >> 24);

}

void printPacket( Packet &packet){
    reverse16(packet.block0.azimuth);
    std::cout<<packet.block0.azimuth/100<< "\n";
    reverse16(packet.block1.azimuth);
    std::cout<<packet.block1.azimuth/100<< "\n";
    reverse16(packet.block2.azimuth);
    std::cout<<packet.block2.azimuth/100<< "\n";
    reverse16(packet.block3.azimuth);
    std::cout<<packet.block3.azimuth/100<< "\n";
    reverse16(packet.block4.azimuth);
    std::cout<<packet.block4.azimuth/100<< "\n";
    reverse16(packet.block5.azimuth);
    std::cout<<packet.block5.azimuth/100<< "\n";
    reverse16(packet.block6.azimuth);
    std::cout<<packet.block6.azimuth/100<< "\n";
    std::cout<<static_cast<int>(packet.block0.flagEE);
    std::cout<<static_cast<int>(packet.block0.flagFF);
    std::cout<<static_cast<int>(packet.block1.flagEE);
    std::cout<<static_cast<int>(packet.block1.flagFF);
    std::cout<<static_cast<int>(packet.block2.flagEE);
    std::cout<<static_cast<int>(packet.block2.flagFF);
    std::cout<<static_cast<int>(packet.block3.flagEE);
    std::cout<<static_cast<int>(packet.block3.flagFF);

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
            printPacket(packet);
        }else{
            std::cout<<"Packet Failed expected size " << BUFFER_SIZE << "bytes : recieved "<< bytesReceived << " bytes";
        }
    }
    close(sock);
    return 0;
}

