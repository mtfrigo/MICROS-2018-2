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