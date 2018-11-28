# Lab 04 - Convenções de Função

## Notas

Makefile: 

Note que as opções --32, para o assembler e -m32 para o compilador, forçam a geração de código de 32 bits. Isto é necessário porque o código em Assembly está utilizando as convenções de chamada para 32 bits. Aqui isto não é estritamente necessário porque as ferramentas de desenvolvimento para a Galileo geram apenas código para 32 bits. Note que a compilação do programa em C é feita sem a flag -O2.



## Experimento

* Gere o programa executável com `make`
  
* Carregue o programa na Galileo para depuração remota com o comando

HOST = nome do host
PORT = porta TCP utilizada para comunicação da Galileo com o host.
`gbdserver <HOST>:<PORT> callasm 1 2`

* Carregue o kdbg no host para depuração remota

TARGET = nome da galileo
`kdbg -r <TARGET>:<PORT> callasm`

* Carregue o código fonte no programa no kgdb
  
* Insira um breakpoint no início do programa e clique nos sinais de + a esquerda para visualizar o código em Assembly do programa. 

* Execute o programa passo-a-passo para verificar o seu funcionamento e como é feita a passagem dos parâmetros nos dois casos. Observe, particularmente o funcionamento da pilha do sistema.

* Insira a flag -O2 nas flags de compilação e repita os passos 5 a 10. 
Obs: antes de executar o make execute um `touch callasm.c`, para forçar a compilação já que o próprio Makefile não é uma dependência para gerar o executável.

* Uma string em C é uma sequência de bytes com o código ASCII dos caracteres terminada por um byte como valor 0x00. Faça um programa em C para imprimir na tela em letras maiúsculas uma string passada na linha de comando. A conversão para letras maiúsculas e a impressão na tela deve ser feita por uma função implementada em Assembly. Dica: O ASCII de uma letra maiúscula diferencia-se da respectiva minúscula por apenas 1 bit.