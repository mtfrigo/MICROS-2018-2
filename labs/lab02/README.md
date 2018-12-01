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