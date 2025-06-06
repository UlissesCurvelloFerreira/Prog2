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

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
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

    int enviado = send(sock, buffer, sizeof(struct ether_header) + 4 + tamanho, 0);
    if (enviado > 0) {
        printf("[SENDER] Frame enviado - Tipo: %d | Seq: %d | Tamanho: %d bytes\n", tipo, sequencia, tamanho);
    } else {
        perror("[SENDER] Erro ao enviar frame");
    }
}

void envia_arquivo(int sock, const char* interface, const uint8_t* dest_mac, const char* caminho) {
    FILE* f = fopen(caminho, "rb");
    if (!f) {
        perror("Arquivo");
        exit(1);
    }

    uint8_t dados[MAX_DADOS];
    uint8_t sequencia = 0;

    printf("[SENDER] Iniciando envio do arquivo: %s\n", caminho);

    while (1) {
        int lidos = fread(dados, 1, MAX_DADOS, f);
        if (lidos <= 0) break;

        envia_frame(sock, dest_mac, interface, TIPO_DADOS, sequencia, dados, lidos);
        sequencia = (sequencia + 1) % 32;
        usleep(10000); // delay para estabilidade
    }

    envia_frame(sock, dest_mac, interface, TIPO_FIM, sequencia, NULL, 0);
    printf("[SENDER] Envio concluído.\n");
    fclose(f);
}

void recebe_frames(int sock) {
    uint8_t buffer[1600];
    FILE* out = fopen("recebido.txt", "wb");
    if (!out) {
        perror("[RECEIVER] Falha ao abrir arquivo de saída");
        exit(1);
    }

    printf("[RECEIVER] Aguardando pacotes na interface...\n");

    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len <= 0) continue;

        struct ether_header* eth = (struct ether_header*) buffer;
        if (ntohs(eth->ether_type) != PROTOCOLO_CUSTOM) {
            printf("[RECEIVER] Frame ignorado (EtherType diferente: 0x%04x)\n", ntohs(eth->ether_type));
            continue;
        }

        uint8_t* payload = buffer + sizeof(struct ether_header);
        if (payload[0] != MARCADOR_INICIO) {
            printf("[RECEIVER] Frame ignorado (sem marcador de início)\n");
            continue;
        }

        uint8_t tamanho = payload[1] & 0x7F;
        uint8_t seq = (payload[2] >> 3) & 0x1F;
        uint8_t tipo = payload[2] & 0x0F;
        uint8_t checksum = payload[3];

        uint8_t soma = payload[1] + payload[2];
        for (int i = 0; i < tamanho; ++i) soma += payload[4 + i];
        if (soma != checksum) {
            printf("[RECEIVER] Checksum incorreto - Ignorado\n");
            continue;
        }

        if (tipo == TIPO_DADOS) {
            fwrite(&payload[4], 1, tamanho, out);
            printf("[RECEIVER] Dados recebidos - Seq: %d | Tamanho: %d bytes\n", seq, tamanho);
        } else if (tipo == TIPO_FIM) {
            printf("[RECEIVER] Fim de arquivo recebido.\n");
            break;
        } else {
            printf("[RECEIVER] Tipo desconhecido: %d\n", tipo);
        }
    }

    fclose(out);
    printf("[RECEIVER] Arquivo salvo como 'recebido.txt'\n");
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
    if (addr.sll_ifindex == 0) {
        perror("[ERRO] Interface inválida");
        exit(1);
    }

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        exit(1);
    }

    printf("[SOCKET] Vinculado à interface %s (index %d)\n", interface, addr.sll_ifindex);
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

        // Substitua pelos 6 bytes do MAC da outra máquina (receptor)
        uint8_t dest_mac[6] = {0x84, 0x7b, 0xeb, 0xfa, 0xd9, 0xd3}; // MAC da interface enp3s0 do receptor


        envia_arquivo(sock, interface, dest_mac, argv[3]);

    } else if (strcmp(modo, "receiver") == 0) {
        recebe_frames(sock);
    } else {
        printf("Modo inválido\n");
    }

    close(sock);
    return 0;
}


/*
mint@mint:~/Downloads$ ip link show
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: enp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 84:47:09:31:3b:c8 brd ff:ff:ff:ff:ff:ff
3: wlp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP mode DORMANT group default qlen 1000
    link/ether a0:b3:39:d8:31:3c brd ff:ff:ff:ff:ff:ff
*/
