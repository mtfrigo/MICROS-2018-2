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

#Utilidades

## Conectar na galileo como root

```
ssh root@<galileoXX>
passwd
useradd -c <LOGIN> -s /bin/bash -m <LOGIN>
passwd <LOGIN>
exit
```

## Adicionar usuário

```
useradd -c "Nome" -s /bin/bash -m <LOGIN>
userdel -r <LOGIN> (caso criado errado)
passwd <LOGIN>
exit
ssh <LOGIN>@<galileoXX>
echo "export PATH=.:$PATH" >> .profile
exit
```

## Adicionar grupo

```
groupadd -r <NOME_DO_GRUPO>
```

## Adicionar membro ao grupo 

```
groupmems -g <NOME_DO_GRUPO> -a <LOGIN>
```

## Adicionar scripts de inicialização (script na galileo)
```
cp ~/<NOME_SCRIPT> /etc/init.d
chmod +x /etc/init.d/<NOME_SCRIPT>
update-rc.d <NOME_SCRIPT> defaults
reboot
update-rc.d <NOME_SCRIPT> remove -> feito no fim do lab
```

## Configurar o host para o uso da Galileo

Levando em consideração o uso do script *iss_setup.sh*, faça o seguinte comando: 
```
source iss_setup.sh
```

## Configurar o compilador para o compilador nativo

```
export CROSS_COMPILE=
```

## Copiar arquivo do host para a Galileo

Levando em consideração que o arquivo está no HOST!
```
scp <FILE> <LOGIN>@<galileoXX>:
```

## Copiar arquivo da Galileo para o Host

Levando em consideração que o arquivo está na Galileo!
```
scp <LOGIN>@<galileoXX>:<FILE> <path_host>
```

## Depuração Remota

Executando na Galileo
HOST = nome do host
PORT = porta TCP usada para comunicação (escolher porta acima de 1024)
```
gdbserver <HOST>:<PORT> <PROGRAMA> 
```

Executando no Host
TARGET = nome da galileo
PORT = porta TCP usada para comunicação (escolher porta acima de 1024)
```
kdgb -r <TAGET>:<PORT> <PROGRAMA> 
```

## Instalação do ambiente em máquinas fora do Lab


* Baixe e descompacte o arquivo como o Intel System Studio IoT Edition no
diretório /opt:

```
cd /opt
tar -xjf iss-iot-linux_03-24-16.tar.bz2
rm iss-iot-linux_03-24-16.tar.bz2
```

* Ajuste as permissões dos arquivos descompactados:
```
chown -R root.root iss-iot-linux
chmod -R go-w iss-iot-linux
chmod -R -s iss-iot-linux
```

* Ainda no diretório /opt, crie um link para a versão atual do Intel System
Studio Iot Edition:
```
ln -s iss-iot-linux iot-devkit
```

* Crie um link para o diretório das ferramentas de desenvolvimento:
```
cd iot-devkit
ln -s devkit-x86 1.7.2
```

* Execute a relocação das ferramentas de desenvolvimento:
```
INSTALL_DIR=‘pwd‘ sdk-relocator/relocate_sdk.sh
```

* !!os nomes dos links tem que ser exatamente estes!!
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

# Lab 02 - Ambiente de Desenvolvimento

* configurar as variáveis de ambiente do seu usuário para utilizar o Intel System Studio IoT Edition 
* `export DEVKIT=/opt/iot-devkit/devkit-x86` 
* `export PATH=$PATH:$DEVKIT/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux`
* esses comandos podem ser inseridos no arquivo ~/.profile ou num script
* caso for feito um script (com o nome iss_setup.sh, por exemplo) usar o comando 
* `source iss_setup.sh`
* para compilar o programa para a Galileo, deve-se configurar a variavel de ambiente a seguir
* `export CROSS_COMPILE=i586-poky-linux-`
* esta variável também pode ser inserida no arquivo ~/.profile ou iss_setup.sh
* `make`
* transfira o programa compilado para a galileo com `scp hello <LOGIN>@<galileoXX>:`
* faça login na galileo `ssh <LOGIN>@<galileoXX>` 
* execute o programa
* fazer a depuração 









