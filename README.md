# Microcontroladores 2018/2


Professor: Walter Fetter Lages.

Disciplina: Microcontroladores (ENG10032).

Semestre: 2018/2.

http://www.ece.ufrgs.br/~fetter/eng10032/

https://moodle.ece.ufrgs.br/course/view.php?id=10

# Tabela com os MACs

98:4F:EE:01:D6:7D	galileo1

98:4F:EE:01:EA:F4	galileo2

98:4F:EE:01:EB:58	galileo3

98:4F:EE:01:EB:61	galileo4

98:4F:EE:01:EB:62	galileo5

98:4F:EE:01:EB:68	galileo6

98:4F:EE:01:EB:ED	galileo7

98:4F:EE:01:EC:2C	galileo8

98:4F:EE:01:EC:30	galileo9

98:4F:EE:01:EC:8A	galileo10

98:4F:EE:01:EC:FC	galileo11

98:4F:EE:01:ED:4B	galileo12

98:4F:EE:01:EE:28	galileo13

98:4F:EE:01:EE:2A	galileo14

98:4F:EE:01:EE:31	galileo15

98:4F:EE:01:EE:56	galileo16

98:4F:EE:01:EE:7A	galileo17

98:4F:EE:01:EE:8D	galileo18

98:4F:EE:01:EE:90	galileo19

98:4F:EE:01:EF:02	galileo20

98:4F:EE:01:EF:2A	galileo21

98:4F:EE:01:EF:2F	galileo22

98:4F:EE:01:EF:30	galileo23

98:4F:EE:01:EF:34	galileo24

# Lab 01

Passo a passo lab 01:
* Inserir cartao microSD na Galileo
* Verificar na etiqueta da Galileo o MAC, e descobrir o seu nome pela tabela
* abrir terminal no HOST e usar o comando `ssh root@<galileoname>`
* configurar senha para o super usuario atraves do comando `passwd`
* criar um usuario comum `useradd -c "Nome por Extenso" -s /bin/bash -m <login>`
* pode-se excluir o usuario com `userdel -r <login>`
* configurar senha para tal usuario com `passwd <login>`
* sair do galileo `exit`
* fazer login na galileo com o login criado `ssh <login>@<galileoname>`
* incluir diretorio no PATH `echo "export PATH=.:$PATH" >> .profile`
* sair da galileo `exit`

cansei na real



