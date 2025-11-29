#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>   // struct iphdr
#include <netinet/tcp.h>  // struct tcphdr
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include <cctype> // for isprint

// Function to print payload as hex + ASCII
void print_payload(const char* payload, int len) {
    const int bytes_per_line = 16;
    for (int i = 0; i < len; i += bytes_per_line) {
        // Print offset
        std::cout << std::setw(4) << std::setfill('0') << i << "  ";

        // Print hex bytes
        for (int j = 0; j < bytes_per_line; ++j) {
            if (i + j < len)
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << (unsigned int)(unsigned char)payload[i + j] << " ";
            else
                std::cout << "   ";
        }

        std::cout << " ";

        // Print ASCII characters
        for (int j = 0; j < bytes_per_line; ++j) {
            if (i + j < len) {
                unsigned char c = payload[i + j];
                std::cout << (isprint(c) ? c : '.');
            }
        }

        std::cout << std::dec << std::endl; // reset number format
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd == -1) {
        perror("Error creating socket");
        return 1;
    }

    char buffer[65536];
    std::cout << "Listening for TCP packets... (Press Ctrl+C to stop)\n";

    while (true) {
        int recv_len = recv(sockfd, buffer, sizeof(buffer), 0);
        if (recv_len < 0) {
            perror("recv failed");
            break;
        }

        // Parse IP header
        struct iphdr* ip = (struct iphdr*)buffer;
        struct tcphdr* tcp = (struct tcphdr*)(buffer + ip->ihl * 4);

        char src_ip[INET_ADDRSTRLEN];
        char dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip->saddr, src_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &ip->daddr, dst_ip, INET_ADDRSTRLEN);

        std::cout << "\nPacket: " << src_ip << ":" << ntohs(tcp->source)
                  << " -> " << dst_ip << ":" << ntohs(tcp->dest)
                  << ", Length: " << recv_len << std::endl;

        // Compute payload
        int ip_header_len = ip->ihl * 4;
        int tcp_header_len = tcp->doff * 4;
        int payload_offset = ip_header_len + tcp_header_len;
        int payload_len = recv_len - payload_offset;

        if (payload_len > 0) {
            std::cout << "Payload (" << payload_len << " bytes):\n";
            print_payload(buffer + payload_offset, payload_len);
        } else {
            std::cout << "Payload: 0 bytes\n";
        }

        std::cout << "----------------------------------------\n";
    }

    close(sockfd);
    return 0;
}
