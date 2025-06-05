
/*
 * Exemplo básico de envio e recebimento de arquivos entre duas máquinas
 * via raw sockets, usando bit-packing com protocolo definido.
 * Este código suporta dois modos:
 * - sender <interface> <arquivo>
 * - receiver <interface>
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_DADOS 127
#define MARCADOR_INICIO 0x7E
#define PROTOCOLO_CUSTOM 0x88B5
#define TIPO_DADOS 5
#define TIPO_FIM   9
#define TIPO_ACK   0

void envia_frame(int sock, const uint8_t* dest_mac, const char* interface, uint8_t tipo, uint8_t sequencia, uint8_t* dados, uint8_t tamanho) {
    uint8_t buffer[1600];
    struct ether_header *eth = (struct ether_header *)buffer;
    struct ifreq ifr;

    strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
    ioctl(sock, SIOCGIFHWADDR, &ifr);
    memcpy(eth->ether_shost, ifr.ifr_hwaddr.sa_data, 6);
    memcpy(eth->ether_dhost, dest_mac, 6);
    eth->ether_type = htons(PROTOCOLO_CUSTOM);

    uint8_t* payload = buffer + sizeof(struct ether_header);
    payload[0] = MARCADOR_INICIO;
    payload[1] = tamanho & 0x7F;
    payload[2] = ((sequencia & 0x1F) << 3) | (tipo & 0x0F);

    uint8_t checksum = payload[1] + payload[2];
    for (int i = 0; i < tamanho; ++i) {
        payload[4 + i] = dados[i];
        checksum += dados[i];
    }

    payload[3] = checksum;

    send(sock, buffer, sizeof(struct ether_header) + 4 + tamanho, 0);
}

void envia_arquivo(int sock, const char* interface, const uint8_t* dest_mac, const char* caminho) {
    FILE* f = fopen(caminho, "rb");
    if (!f) {
        perror("Arquivo");
        exit(1);
    }

    uint8_t dados[MAX_DADOS];
    uint8_t sequencia = 0;

    while (1) {
        int lidos = fread(dados, 1, MAX_DADOS, f);
        if (lidos <= 0) break;

        envia_frame(sock, dest_mac, interface, TIPO_DADOS, sequencia, dados, lidos);
        sequencia = (sequencia + 1) % 32;
        usleep(10000); // delay para estabilidade
    }

    envia_frame(sock, dest_mac, interface, TIPO_FIM, sequencia, NULL, 0);
    fclose(f);
}

void recebe_frames(int sock) {
    uint8_t buffer[1600];
    FILE* out = fopen("recebido.txt", "wb");

    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len <= 0) continue;

        struct ether_header* eth = (struct ether_header*) buffer;
        if (ntohs(eth->ether_type) != PROTOCOLO_CUSTOM) continue;

        uint8_t* payload = buffer + sizeof(struct ether_header);
        if (payload[0] != MARCADOR_INICIO) continue;

        uint8_t tamanho = payload[1] & 0x7F;
        uint8_t seq = (payload[2] >> 3) & 0x1F;
        uint8_t tipo = payload[2] & 0x0F;
        uint8_t checksum = payload[3];

        uint8_t soma = payload[1] + payload[2];
        for (int i = 0; i < tamanho; ++i) soma += payload[4 + i];
        if (soma != checksum) continue;

        if (tipo == TIPO_DADOS) {
            fwrite(&payload[4], 1, tamanho, out);
        } else if (tipo == TIPO_FIM) {
            break;
        }
    }

    fclose(out);
    printf("Arquivo salvo como 'recebido.txt'\n");
}

int cria_socket(const char* interface) {
    int s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (s < 0) {
        perror("Socket");
        exit(1);
    }

    struct sockaddr_ll addr = {0};
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_ifindex = if_nametoindex(interface);
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        exit(1);
    }

    return s;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso:\n  sender <interface> <arquivo>\n  receiver <interface>\n");
        return 1;
    }

    const char* modo = argv[1];
    const char* interface = argv[2];
    int sock = cria_socket(interface);

    if (strcmp(modo, "sender") == 0) {
        if (argc < 4) {
            printf("Arquivo não especificado\n");
            return 1;
        }

        // MAC da outra máquina (EXEMPLO - substitua!)
        uint8_t dest_mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
        envia_arquivo(sock, interface, dest_mac, argv[3]);

    } else if (strcmp(modo, "receiver") == 0) {
        recebe_frames(sock);
    } else {
        printf("Modo inválido\n");
    }

    close(sock);
    return 0;
}
