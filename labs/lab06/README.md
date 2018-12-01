# Lab 06 - Pulse-Width-Modulation – PWM

## Configurando interruções

Os PWMs são acessados no espaço do usuário através da interface sysfs exposta em /sys/class/pwm/pwmchip0.

Neste diretório existem os seguintes pseudo-arquivos:

* npwm: O número de canais de PWM suportados
* export: Exporta um canal de PWM
* unexport: “Desexporta” um canal de PWM
* device/pwm_period: período do PWM em nanosegundos. No caso do PCA9685 todos os PWMs usam o mesmo período.
    Os canais de PWM são numerados de 0 a npwm-1.

Quando um canal de PWM é exportado, será criado um diretório pwmX dentro de /sys/class/pwm/pwmchip0, onde X é o número do canal exportado.
Os seguintes arquivos existirão no diretório /sys/class/pwm/pwmchip0/pwmX:

* period: período do PWM em nanosegundos. No caso do PCA9685 todos os PWMs usam o mesmo período, portanto este arquivo não pode ser escrito. O período deve ser configurado através do arquivo /sys/class/pwm/pwmchip0/device/pwm_period, como explicado acima.
* duty_cycle: duração do ciclo de trabalho do PWM em nanosegundos. Obviamente, deve ser menor do que o período.
* polarity: troca a polaridade do sinal de PWM. Funciona apenas se o hardware suportar. A polaridade só pode ser trocada com o PWM desabilitado. Os valores possíveis são as strings: "normal" ou "inversed".
* enable: habilita/desabilita o PWM. 0 desabilita, 1 habilita


## Experimento

* Logar como root na galileo, criar o grupo 'pwm' e incluir o seu usuário nesse grupo.
```
ssh root@<galileoXX>
groupadd -r pwm
groupmems -g pwm -a <LOGIN>

```

* Verifique no mapa de configuração dos pinos da Galileo Gen2 como configurar os multiplexadores para que o sinal pwm1 seja roteado para o pino IO3;

* O script de inicialização esta na pasta init e deve ser configurado

No host:
`scp eng10032lab06_1 root@<galileoXX>:`

Na galileo(como root)
```
cp eng10032lab06_1 /etc/init.d
chmod +x /etc/init.d/eng10032lab06_1
update-rc.d eng10032lab06_1 defaults
reboot
```
