

Funções da lib:

Funções LCD:

* lcd_init();
Função para inicializar o LCD;


* lcd_backlight_init(int fd);
Função para inicializar o backlight do LCD;

*fd* => handler do arquivo do LCD;

* lcd_write_words(int fd_lcd, char* word1, char* word2);
Função para escrever no LCD;

*fc_lcd* => handler do arquivo de LCD;
*word1* => string a ser escrita na primeira linha do led;
*word2* => string a ser escrita na segunda linha do led;


* lcd_backlight_set(int fd, int red, int blue, int green);
Função para configurar a cor do backlight do LCD;
*fd* => handler do lcd
*red* => R
*green* => G
*blue* => B

Funções EEMPROM:

* eeprom_write(char *data);
Função para escrever no EEMPROM;
*data* => string a ser escrita;

* eeprom_read(char data[EEPROM_LENGTH], int length);
Função para ler a EEPROM
*data* => string que guardará o que foi lido da EEMPROM
*length* => tamanho da mensagem a ser lida;
