# Lab 05 - Interrupções

## Configurando interruções

### Habilitando interruções

O arquivo /sys/class/gpio/gpioXX/edge é utilizado para habilitar e desabilitar a interrupção e configurar a borda em que ela será ativa.

As seguintes strings podem ser escritas no arquivo para configurar a borda da interrupção:

* "rising": interrupção na borda de subida do sinal
* "falling": interrupção na borda de descida do sinal
* "both": interrupção em ambas bordas do sinal
* "none": interrupção desabilitada

### Recebendo a interrupção

Uma vez habilitada, a interrupção pode ser recebida por um programa no espaço do usuário através de um polling no arquivo /sys/class/gpio/gpioXX/value com o uso da função poll() para esperar por um evento POLLPRI


## Experimento

* Executar o Makefile da pasta lib para gerar a biblioteca.

* Olhando a tabela, verifica-se que o IO4 tem o gpio6 como pino de interrupções

* O script de inicialização esta na pasta init e deve ser configurado

No host:
`scp eng10032lab05_1 root@<galileoXX>:`

Na galileo
```
cp ~/eng10032lab05_1 /etc/init.d
chmod +x /etc/init.d/eng10032lab05_1
update-rc.d eng10032lab05_1 defaults
reboot
```

* Conectar o pushbutton no D4 do shield da galileo

* Dar make no diretório count para fazer a contagem das bordas de descida 

* Transfira o programa compilado para a galileo com `scp count <LOGIN>@<galileoXX>:`

* Faça login na galileo `ssh <LOGIN>@<galileoXX>` 

* Execute o programa

