# Lab 08 - Serial Peripheral Interface (SPI)

## Notas

O SPI é um barramento de comunicação serial síncrono utilizado para pequenas distâncias. Ele opera em full-duplex no modo mestre-escravo

O SPI é baseado em quatro sinais

**SCLK**: clock, saída do mestre
**MOSI**: Saída do mestre, entrada no escravo
**MISO**: Entrada no mestre, saída do escravo
**#SS**: Seleção do escravo, saída do mestre

## Polaridade e Fase do Clock

O SPI pode operar com duas polaridades e duas fases do sinal de clock

**CPOL**: Polaridade do clock
**CPHA**: Fase do clock

Para CPOL=0, o estado inativo do clock é em nível lógico baixo. Neste caso, quando CPHA=0, os dados são alterados na borda de descida do clock e amostrados na borda de subida do clock, como mostra a figura 3(a). Quando CPHA=1 os dados são alterados na subida do clock e amostrados na descida do clock, como mostra a figura 3(b).


Para CPOL=1, o estado inativo do clock é em nível lógico alto. Neste caso, quando CPHA=0, os dados são alterados na borda de subida do clock e amostrados na borda de descida do clock, como mostra a figura 3(c). Quando CPHA=1 os dados são alterados na descida do clock e amostrados na subida do clock, como mostra a figura 3(d).

## Implementação na Gen2

O protocolo SPI poderia ser implementado por software utilizando 4 portas GPIO quaisquer. 

No entanto, neste caso, a taxa máxima de transferência seria limitada pela velocidade de execução do software. Maiores taxas de transferência podem ser obtidas por implementações em hardware. O Quark X1000 possui 3 interfaces SPI. Uma é utilizada pela memória flash, outra é utilizada pelo conversor A/D e a terceira está disponível no conector de shield, conforme pinagem na tabela 2.

Pino Sinal
IO10 #SS
IO11 MOSI
IO12 MISO
IO13 SCLK

O barramento SPI disponível no conector de shield da Galileo Gen 2 é acessado através do dispositivo /dev/spidev1.0, que suporta as operações open(), close(), read(), write() e ioctl(). As operações de read() e write() são utilizadas para  receber e transmitir dados, respectivamente, no modo halfduplex. 

A operação ioctl() é usada para configurar a porta SPI e para transmitir e receber dados no modo full-duplex, conforme as operações mostradas na tabela 3, cujas constantes estão definidas no arquivo de cabeçalho linux/spi/spidev.h.

**SPI_IOC_RD_MODE**: Lê o modo do clock
**SPI_IOC_WR_MODE**: Escreve o modo do clock
**SPI_IOC_RD_LSB_FIRST**: Lê a justificação dos bits
**SPI_IOC_WR_LSB_FIRST**: Escreve a justificação dos bits
**SPI_IOC_RD_BITS_PER_WORD**: Lê o tamanho da palavra
**SPI_IOC_WR_BITS_PER_WORD**: Escreve o tamanho da palavra
**SPI_IOC_RD_MAX_SPEED_HZ**: Lê a taxa de transferência máxima
**SPI_IOC_WR_MAX_SPEED_HZ**: Escreve a taxa de transferência máxima
**SPI_IOC_MESSAGE(N)**: Envia/recebe N mensagens


## Experimento


