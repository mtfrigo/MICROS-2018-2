# Lab 03 - GPIO

## Configurando porta gpio

### Exportar porta

Escreve-se o número da porta GPIO em m /sys/class/gpio/export
```
echo -n "XX" > /sys/class/gpio/export
```
Com isso surgirá um diretório correspondente à porta. Por exemplo: /sys/class/gpio/gpioXX

### Desexportar a porta

Escreve-se o número da porta no arquivo o /sys/class/gpio/unexport.
```
echo -n "XX" > /sys/class/gpio/unexport
```

### Direção da porta

Escrevendo-se "in" ou "out" em /sys/class/gpio/gpioXX/direction.
```
echo -n "out" > /sys/class/gpio/gpioXX/direction
ou
echo -n "in" > /sys/class/gpio/gpioXX/direction
```

As portas de número igual ou maior do que 64 não possuem o pseudo-arquivo direction, pois são sempre saída já que são usadas apenas para controlar multiplexadores e buffers.

### Ler ou escrever

Escrevendo-se ou lendo-se em /sys/class/gpio/gpioXX/value.
```
echo -n "0" > /sys/class/gpio/gpio46/value
ou
cat /sys/class/gpio/gpio46/value
```
Para as portas que controlam multiplexadores ou direção dos buffers também é possível escrever "low" ou "high" no pseudo-arquivo direction. Isso é equivalente a configurar simultaneamente direction para "out" e value para "0" ou "1",  respectivamente.

## Ajuste de permissões

Por default, os arquivos em /sys/class/gpio só podem ser escritos pelo superusuário.
A configuração destas e o ajuste das permissões será feito através dos scripts de inicialização
Será criado o grupo gpio e as permissões serão ajustadas para que os usuários membros deste grupo possam acessar os arquivos adequados em /sys/class/gpio.


## Experimento 

* Transferir o script eng10032lab03_1 para a galileo
`scp eng10032lab03_1 <galileoXX>:`

* Logar na Galileo como super usuário e crie o grupo gpio, inclua seu suario no grupo

```
groupadd -r gpio
groupmems -g gpio -a <LOGIN>
```

* Logar na galileo com seu usuário
`ssh <LOGIN>@<galileoXX>`

* Copiar o arquivo eng10032lab03_1 para o diretorio /etc/init.d, tornar o script executável e configurar para o Linux chamar o script durante a inicialização
```
cp ~/eng10032lab03_1 /etc/init.d
chmod +x /etc/init.d/eng10032lab03_1
update-rc.d eng10032lab03_1 defaults
reboot
```

* Como já descrito na seção 2.1, o pino IO13 (onde está conectado o LED) é controlado pela porta gpio7, que é roteada para este pino colocando-se a gpio46 em nível lógico baixo. A gpio30 controla a direção do buffer associado (nível lógico baixo para saída e alto para entrada). Neste caso, é irrelavante o estado do resistor de pull-up ou pull-down, controlado por gpio31.

* Verifique que o script da listagem 1 configura o pino IO13 como GPIO e como saída e portanto capaz de acionar o LED.

* COMO DESCOBRIR EM QUAL PINO DO SHIELD É O IO13? 

IO13 é o LED fixo que está ao lado do conector USB host.

* Copiar o arquivo eng10032lab03_2 para o diretorio /etc/init.d, tornar o script executável e configurar para o Linux chamar o script durante a inicialização

No host:
`scp eng10032lab03_2 <galileoXX>:`

Na galileo
```
update-rc.d -f eng10032lab03_1 remove
cp ~/eng10032lab03_2 /etc/init.d
chmod +x /etc/init.d/eng10032lab03_2
update-rc.d eng10032lab03_2 defaults
reboot
```

* Verifique que o script da listagem 2 configura o pino IO3 como GPIO e como saída e portanto capaz de acionar o LED.

* COMO DESCOBRIR EM QUAL PINO DO SHIELD É O IO3? 

ACHO que no shield D03 = IO3

* Copiar o arquivo eng10032lab03_3 para o diretorio /etc/init.d, tornar o script executável e configurar para o Linux chamar o script durante a inicialização

No host:
`scp eng10032lab03_3 <galileoXX>:`

Na galileo
```
update-rc.d -f eng10032lab03_2 remove
cp ~/eng10032lab03_3 /etc/init.d
chmod +x /etc/init.d/eng10032lab03_3
update-rc.d eng10032lab03_3 defaults
reboot
```

* Ligar o LED na porta D03 do shield e o pushbutton na porta D02 do shield e verificar o funcionamento

* Ao final do experimento não esquecer de dar o seguinte commando

`update-rc.d -f eng10032lab03_3 remove`
