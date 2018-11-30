# Microcontroladores 2018/2


Professor: Walter Fetter Lages.

Disciplina: Microcontroladores (ENG10032).

Semestre: 2018/2.

http://www.ece.ufrgs.br/~fetter/eng10032/

https://moodle.ece.ufrgs.br/course/view.php?id=10

# Utilidades

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
INSTALL_DIR=`pwd` sdk-relocator/relocate_sdk.sh
```

* !!os nomes dos links tem que ser exatamente estes!!



# Configurando porta gpio

## Exportar porta

Escreve-se o número da porta GPIO em m /sys/class/gpio/export
```
echo -n "XX" > /sys/class/gpio/export
```
Com isso surgirá um diretório correspondente à porta. Por exemplo: /sys/class/gpio/gpioXX

## Desexportar a porta

Escreve-se o número da porta no arquivo o /sys/class/gpio/unexport.
```
echo -n "XX" > /sys/class/gpio/unexport
```

## Direção da porta

Escrevendo-se "in" ou "out" em /sys/class/gpio/gpioXX/direction.
```
echo -n "out" > /sys/class/gpio/gpioXX/direction
ou
echo -n "in" > /sys/class/gpio/gpioXX/direction
```

As portas de número igual ou maior do que 64 não possuem o pseudo-arquivo direction, pois são sempre saída já que são usadas apenas para controlar multiplexadores e buffers.

## Ler ou escrever

Escrevendo-se ou lendo-se em /sys/class/gpio/gpioXX/value.
```
echo -n "0" > /sys/class/gpio/gpio46/value
ou
cat /sys/class/gpio/gpio46/value
```
Para as portas que controlam multiplexadores ou direção dos buffers também é possível escrever "low" ou "high" no pseudo-arquivo direction. Isso é equivalente a configurar simultaneamente direction para "out" e value para "0" ou "1",  respectivamente.

# Ajuste de permissões

Por default, os arquivos em /sys/class/gpio só podem ser escritos pelo superusuário.
A configuração destas e o ajuste das permissões será feito através dos scripts de inicialização
Será criado o grupo gpio e as permissões serão ajustadas para que os usuários membros deste grupo possam acessar os arquivos adequados em /sys/class/gpio.
