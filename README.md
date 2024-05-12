# Cliente DNS - Trabalho 01 (FRC)

<div style="text-align:center"><img src= "docs/assets/inicio.png"/></div>

## Sobre a disciplina
<p style="text-align: justify;">Documentação reservada ao trabalho 1 "Camada de Aplicação: Cliente DNS" da disciplina de Fundamentos de Redes de Computadores (FRC) da Universidade de Brasília (UnB - FGA), lecionada pelo professor Tiago Alves. Este trabalho tem como objetivo estudar a implementação de esquemas de comunicações entre processos (IPC), especificamente a utilização da interface socket.</p>

<p style="text-align: justify;">A disciplina de Fundamentos de Redes de Computadores trata de diversos tópicos relativos a Sistemas Operacionais, sistemas que provêem uma forma intuitiva de se utilizar as funcionalidades de computadores digitais sem que seja necessário ao usuário ou programador ter profundo
conhecimento das interações entre os diferentes hardwares que compõem um computador.</p>


## Contribuidores

| **Foto** | **Nome** | **Matrícula** | **GitHub** |
| :------: | :------: | :-----------: | :--------: |
| <a href="https://github.com/ArturVinicius"><img src="https://avatars.githubusercontent.com/u/58870950?v=4" height="auto" width="90" style="border-radius:50%"></a> | Artur Vinicius Dias Nunes | 190142421 | [ArturVinicius](https://github.com/ArturVinicius) |
| <a href="https://github.com/joaobisi"><img src="https://avatars.githubusercontent.com/u/111386960?v=4" height="auto" width="90" style="border-radius:50%"></a> | João Victor Max Bisinotti de Oliveira | 170069991 | [joaobisi](https://github.com/joaobisi) |
| <a href="https://github.com/kessJhones"><img src="https://avatars.githubusercontent.com/u/49600484?v=4" height="auto" width="90" style="border-radius:50%"></a> | Kess Jhones Gomes Tavares | 180124498 | [kessJhones](https://github.com/kessJhones) |
| <a href="https://github.com/luclopesr"><img src="https://avatars.githubusercontent.com/u/88348202?v=4" height="auto" width="90" style="border-radius:50%"></a> | Lucas Lopes Rocha | 202023903 | [luclopesr](https://github.com/luclopesr) |


# Documentação da aplicação

Para melhor visualização desta documentação, acesso a página Wiki deste projeto acessando o [LINK](https://luclopesr.github.io/2024.1_Trabalho1_FRC/).

## Ferramentas Utilizadas

Para construção dessa aplicação foi utilizado o sistema operacional Ubuntu (Linux). Além disso, o ambiente de desenvolvimento usado foi o <a href="https://code.visualstudio.com/">Visual Studio Code</a>.

## Construção da Aplicação

1. **Ler os argumentos da linha de comando:** Para ler os argumentos da linha de comando em Python, podemos usar o módulo `sys`. O código para isso seria algo assim:

    ```py
    import sys

    if len(sys.argv) != 3:
        print("Uso: python3 cliente_dns.py <nome_dominio> <ip_servidor_dns>")
        sys.exit(1)

    domain_name = sys.argv[1]
    dns_server = sys.argv[2]
    ```
    Este código verifica se foram passados dois argumentos na linha de comando. Se não, ele exibe uma mensagem de uso e sai do programa. Caso contrário, ele atribui os valores dos argumentos a duas variáveis: `domain_name` e `dns_server`.


2. **Criar o payload da consulta DNS:** Utilize o módulo `struct para formatar os campos do payload UDP de acordo com as especificações fornecidas. Isso inclui o Transaction ID, Flags, Question, Answer RRs, Authority RRs e Additional RRs.

3. **Enviar a consulta DNS:** Utilize sockets UDP para enviar o payload ao servidor DNS especificado. Você pode usar o módulo `socket` do Python para isso.

4. **Esperar pela resposta:** Defina um tempo limite para esperar pela resposta do servidor DNS. Se a resposta não for recebida dentro desse tempo, faça uma nova tentativa (se ainda houver tentativas restantes).

5. **Processar a resposta:** Analise a resposta recebida do servidor DNS. Se for bem-sucedida, extraia as informações necessárias (nome do servidor de e-mail) e exiba-as no formato especificado. Se a consulta falhar por qualquer motivo (domínio não encontrado, entrada NS não encontrada, etc.), exiba a mensagem apropriada.

## Pré-requisitos e Execução da Aplicação
Antes de executar a aplicação, certifique-se de ter o Python3 instalado no seu sistema. Você pode instalar o [Python3](https://www.python.org/downloads/source/) seguindo as instruções específicas para o Ubuntu.

1. Abra um terminal e clone o repositório em que se encontra o projeto.

    ```bash
    git clone https://github.com/luclopesr/2024.1_Trabalho1_FRC
    ```

2. Navegue até o diretório onde o clone do repositório está localizado.

3. Execute o seguinte comando, substituindo `<nome_dominio>` pelo nome do domínio que deseja consultar e `<ip_servidor_dns>` pelo endereço IP do servidor DNS:

    ```bash
    python3 src/cliente_dns.py <nome_dominio> <ip_servidor_dns>
    ```

    Exemplo:
    ```bash
    python3 src/cliente_dns.py google.com 8.8.8.8
    ```

4. Aguarde a execução da consulta DNS e observe os resultados exibidos no terminal.

## Telas
Esta aplicação é executada inteiramente no terminal. Após a execução do comando acima, você receberá a saída diretamente no terminal, conforme os exemplos de interação fornecidos nas instruções do trabalho.

## Limitações Conhecidas

- A aplicação só suporta consultas do tipo NS (servidor de nomes autoritativo para o domínio).
- Firewalls podem impedir que pacotes de consulta alcancem servidores localizados fora da rede de acesso.
- Infelizmente, não foi possível aplicar a verificação para casos em que o domínio não possui entrada NS.