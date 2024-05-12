import socket
import struct
import random
import sys

def dns_query(domain_name, dns_server):
    try:
        # Criar um socket UDP
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        client_socket.settimeout(2)  # Tempo de espera por resposta em segundos

        # Gerar um ID de transação aleatório de 16 bits
        transaction_id = random.randint(0, 65535)

        # Montar o payload da consulta DNS
        query_payload = struct.pack('!HHHHHH', transaction_id, 0x0100, 1, 0, 0, 0)
        # Adicionar o nome do domínio à consulta
        for part in domain_name.split('.'):
            query_payload += struct.pack('B', len(part))
            query_payload += part.encode()
        # Terminar o nome do domínio
        query_payload += b'\x00'
        # Adicionar o tipo de consulta NS
        query_payload += struct.pack('!HH', 2, 1)

        # Tentar resolver o nome até 3 vezes
        for _ in range(3):
            # Enviar a consulta para o servidor DNS
            client_socket.sendto(query_payload, (dns_server, 53))

            # Receber a resposta do servidor DNS
            try:
                response, _ = client_socket.recvfrom(1024)
            except socket.timeout:
                continue  # Tentar novamente se não houver resposta em 2 segundos
            else:
                break  # Se a resposta for recebida, sair do loop

        # Se não houver resposta após 3 tentativas, informar o usuário
        else:
            print(f"Nao foi possível coletar entrada NS para {domain_name}")
            return

        # Interpretar a resposta
        num_answers = struct.unpack('!H', response[6:8])[0]
        if num_answers == 0:
            print(f"Dominio {domain_name} nao encontrado")
        else:
            print(f"Resultados para {domain_name}:")
            offset = response.find(b'\xc0\x0c') + 12  # Início das respostas
            for _ in range(num_answers):
                # Verificar se estamos na seção de respostas de autoridade (NS)
                if response[offset] & 0xc0 == 0xc0:
                    break
                # Ler o comprimento do domínio e extrair o nome do servidor de e-mail
                domain_length = response[offset]
                mail_server = response[offset + 1: offset + 1 + domain_length]
                print(f"{domain_name} <> {mail_server.decode()}.{domain_name}")
                # Avançar para o próximo registro
                offset += 12 + domain_length

    except Exception as e:
        print(f"Erro: {e}")
    finally:
        client_socket.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python3 cliente_dns.py <nome_dominio> <ip_servidor_dns>")
        sys.exit(1)

    domain_name = sys.argv[1]
    dns_server = sys.argv[2]
    dns_query(domain_name, dns_server)
