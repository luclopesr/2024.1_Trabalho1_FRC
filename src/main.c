#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_RETRIES 3
#define TIMEOUT 2

// Estrutura para o cabeçalho da consulta DNS
typedef struct {
    unsigned short id;
    unsigned short flags;
    unsigned short qcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
} DNS_HEADER;

// Função para enviar consulta DNS e receber resposta
void send_dns_query(const char *domain_name, const char *dns_server_ip) {
    // Criação do socket UDP
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Definição do endereço do servidor DNS
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53); // Porta padrão para serviço DNS
    inet_aton(dns_server_ip, &server_addr.sin_addr);

    // Inicialização do cabeçalho DNS
    DNS_HEADER dns_header;
    dns_header.id = (unsigned short) getpid();
    dns_header.flags = htons(0x0100); // Flags para consulta recursiva
    dns_header.qcount = htons(1); // Apenas 1 consulta
    dns_header.ancount = 0;
    dns_header.nscount = 0;
    dns_header.arcount = 0;

    // Montagem da consulta DNS
    char query[256]; // Tamanho máximo para o nome de domínio
    sprintf(query, "%s", domain_name);

    // Envio da consulta DNS
    ssize_t sent_bytes;
    sent_bytes = sendto(sockfd, &dns_header, sizeof(DNS_HEADER), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (sent_bytes < 0) {
        perror("Erro ao enviar consulta DNS");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Recebimento da resposta DNS
    char buffer[1024];
    struct sockaddr_in server_response;
    socklen_t server_response_len = sizeof(server_response);
    ssize_t recv_bytes;

    // Configuração do timeout para receber resposta
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // Tentativas de recebimento da resposta
    int attempts = 0;
    while (attempts < MAX_RETRIES) {
        recv_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_response, &server_response_len);
        if (recv_bytes >= 0) {
            // Processamento da resposta DNS e impressão dos resultados
            printf("%s <> %s\n", domain_name, buffer);
            break;
        } else {
            perror("Erro ao receber resposta DNS");
            attempts++;
        }
    }

    if (attempts == MAX_RETRIES) {
        printf("Não foi possível coletar entrada NS para %s\n", domain_name);
    }

    // Fechamento do socket
    close(sockfd);
}

int main(int argc, char *argv[]) {
    // Verificação dos argumentos da linha de comando
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <nome_dominio> <ip_servidor_dns>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Chamada da função para enviar consulta DNS
    send_dns_query(argv[1], argv[2]);

    return 0;
}
