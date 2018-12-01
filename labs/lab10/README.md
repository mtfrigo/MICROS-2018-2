# Lab 10 - Inter Integrated Circuit (I2C)

## Notas

O I2C é um barramento de comunicação serial utilizado para pequenas distâncias, tipicamente na mesma placa de circuito impresso. Ele opera de forma bidirecional no modo mestre-escravo e suporta vários mestres. O SMBus, atualmente utilizado em  motherboards de PCs, é um subconjunto do I2C. Vários dispositivos são compatíveis com ambos barramentos.

O I2C é baseado em dois sinais, com drivers em dreno aberto com resistores de pull-up:

**SCL**: clock, gerado pelo mestre
**SDA**: linha de dados bidirecional

O Quark X1000 possui um barramento I2C que está disponível no conector de shield da Galileo Gen2 e é utilizado na própria placa para acessar os expansores de GPIO nos endereços 0x25, 0x26 e 0x27, o driver do PWM no endereço 0x47 e uma EEPROM serial de 1kB, ou seja, 8 kb, nos endereços 0x54-0x57.

O barramento I2C disponível na Galileo Gen 2 é acessado através do dispositivo /dev/i2c-0, que suporta as operações open(), close(), read(), write() e ioctl(). As operações de read() e write() são utilizadas para receber e transmitir dados, respectivamente e a operação ioctl() é usada para executar outros comandos no barramento. O mais importante é o comando I2C_SLAVE (constante definida em linux/i2c-dev.h), que é usado para definir o endereço do escravo a ser acessado. 


A EEPROM pode ser acessada através do pseudo-arquivo /sys/bus/i2c/ devices/0-0054/eeprom e pode ser lida e escita como se fosse um arquivo qualquer

## Experimento


