# Lab 07 - Conversor Analógico/Digital (ADC)

## Notas

As entradas do conversor A/D estão mapeadas nos pinos IO14-IO19 do conector de shield.

O resistor de pull-up/pull-down associado ao pino deve ser configurado em off para não influenciar a medida.

A resolução do A/D é de 10 bits, mas os dados são escalonados para 12 bits para serem apresentados na interface do espaço do usuário.

## Modo One-Shot

No modo one-shot é feita apenas uma conversão por requisição.

O acesso aos canais do conversor A/D através do espaço do usuário é feito através de arquivos no diretório /sys/bus/iio/devices/iio:device0:

* in_voltageN_raw: Valor bruto da medida em ASCII.
* in_voltageN_scale: Multiplicador para converter o valor bruto da medida para mV.

(N é o número do canal do conversor A/D.)

## Modo Contínuo

No modo contínuo são feitas conversões contínuas em sequência em todos os canais habilitados

Os dados são lidos do arquivo /dev/iio:device0 em binário.

A configuração do buffer para receber os dados e dos canais que serão amostrados também é feita através de arquivos no diretório /sys/bus/iio/devices/iio: device0:

* buffer/length: Número de conjuntos de amostras do buffer.
* buffer/enable: Habilita/desabilita as conversões no modo contínuo.
* scan_elements/in_timestamp_en: Habilita o timestamp dos dados.
* scan_elements/in_timestamp_index: Índice da posição do timestamp no buffer.
* scan_elements/in_timestamp_type: Formato com o qual o timestamp é armazenado no buffer
* scan_elements/in_voltageN_en: Habilita a amostragem do canal N.
* scan_elements/in_voltageN_index: Índice da posição do canal N no buffer.
* scan_elements/in_voltageN_type: Formato com o qual os dados do canal N são armazenados no buffer.
* trigger/current_trigger: Configura o trigger a ser usado para as disparar as conversões.

N é o número do canal do conversor A/D

O formato dos dados é descrito por uma string semelhante a:

be:u12/16>>0

onde:
* be: representa a endianness do dado, be para big endian e le para low endian
* u: indica se o valor é com sinal (s) ou sem sinal (u)
* 12: indica o número de bits com informação relevante
* 16: indica o número de bits usados para armazenar o dado
* 0: indica o alinhamento dos bits de informação

Para realizar conversões no modo contínuo, deve-se habilitar os canais desejados, configurar o tamanho do buffer (número de amostras) e habilitar o  buffer. Os pseudo-arquivos com os índices são de leitura apenas e somente os canais habilitados são realmente inseridos no buffer.

As conversões são disparadas por um trigger. Os triggers disponíveis aparecem em /sys/bus/iio/devices quando o módulo do kernel correspondente é carregado. Estes módulos não são carregados no boot da Galileo. É necessário carrega-los explicitamente com os comandos modprobe ou insmod.

Os triggers disponíveis na Galileo Gen2 são:

* iio-trig-sysfs: totalmente baseado em software. Permite disparar por software cada amostragem, como no modo one-shot, mas com a API do modo contínuo.

* iio-trig-hrtimer: baseado no temporizador HPET. permite configurar a frequência com que será feita a amostragem.

Quando os módulos dos triggers são corregados, surgem os seguintes arquivos no diretório /sys/bus/iio/devices:

* iio_sysfs_trigger/add_trigger : Usado para criar um trigger do tipo iio_sysfs_trigger, ao se escrever um número inteiro no arquivo.
* iio_sysfs_trigger/remove_trigger : Usado para remover um trigger do tipo iio_sysfs_trigger, ao se escrever um número inteiro no arquivo.
* iio_hrtimer_trigger/add_trigger : Usado para criar um trigger do tipo iio_hrtimer_trigger, ao se escrever um número inteiro no arquivo.
* iio_hrtimer_trigger/remove_trigger : Usado para remover um trigger do tipo iio_hrtimer_trigger, ao se escrever um número inteiro no arquivo.

Ao ser criado um trigger é criado o diretório /sys/bus/iio/devices/trigger<n>, onde <n> é um número crescente a partir de 0, para cada trigger criado. Note que <n> não é o número que foi escrito no arquivo add_trigger para criação do trigger, mas um número que é incrementado a cada trigger criado. O conteúdo deste diretório depende do tipo do trigger:

* name: nome do trigger, usado para configurar o trigger a ser usado.
* trigger_now: qualquer coisa escrita neste arquivo dispara o trigger. Só existe para triggers do tipo iio_sysfs_trigger.
* frequency: configura a frequência do trigger. Só existe para triggers do tipo iio_hrtimer_trigger.

Quando é usado um trigger do tipo iio_sysfs_trigger, depois de habilitado o buffer, a cada escrita no arquivo trigger_now é realizada uma amostragem. Quando é usado um trigger do tipo iio_hrtimer_trigger, ao ser habilitado o buffer começa a amostragem com a frequência programada no arquivo frequency.

As conversões são paradas desabilitando-se o buffer e removendo-se o trigger.

## Experimento 1

* Logar como root na galileo, criar o grupo 'adc' e incluir o seu usuário nesse grupo.
```
ssh root@<galileoXX>
groupadd -r adc
groupmems -g adc -a <LOGIN>

```

* Verifique no mapa de configuração dos pinos da Galileo Gen2 como configurá-la para usar o ADC_A0.

Note que, para não alterar o valor medido pelo conversor A/D, não deve haver resistor de pull-up nem de pull-down no pino.

* Faça um script de inicialização para configurar o uso do ADC_A0 em modo one shot. Configure permissões de leitura para o grupo adc nos arquivos in_voltage0_raw e in_voltage0_scale.

* O script de inicialização esta na pasta rotary e deve ser configurado

No host:
`scp rotary_init root@<galileoXX>:/etc/init.d`

Na galileo(como root)
```
cd /etc/init.d
chmod +x rotary_init
update-rc.d rotary_init defaults
reboot
```

* Rodar o Makefile da pasta lib

* Rodar o Makefile da pasta rotary

* Copiar o arquivo rotary para a galileo

`scp rotary <LOGiN>@<galileoXX>:`

* Logar na galileo e rodar o código!
```
ssh <LOGIN>@<galileoXX>
./rotary
```

## Experimento 2

* O script de inicialização esta na pasta continuo e deve ser configurado

No host:
`scp continuo_init root@<galileoXX>:/etc/init.d`

Na galileo(como root)
```
cd /etc/init.d
chmod +x continuo_init
update-rc.d continuo_init defaults
reboot
```

* Rodar o Makefile da pasta lib

* Rodar o Makefile da pasta continuo

* Copiar o arquivo continuo para a galileo

`scp continuo <LOGiN>@<galileoXX>:`

* Logar na galileo e rodar o código!
```
ssh <LOGIN>@<galileoXX>
./continuo medidas.csv
```

* Copiar para o Host o arquivo gerado
  ```
  scp -r <LOGIN>@<galileoXX>:/home/<LOGIN>/<FILE> <PATH_HOST>
  ```

* Instalar o GNU Plot
`sudo apt-get install gnuplot`

* Plotar o gráfico
`sh plot.sh medidas.csv`
