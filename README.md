# IOEditor (Igor's Own Editor)

Editor de texto minimalista feito em C, necessaria biblioteca ncurses, e autotools para compilar.

## Para instalar :

```sh
./configure
make
sudo make install
```

## Comandos (os comandos sao otimizados para layout colemak):

- CTRL + Q  Sair

### Comandos de navegacao

- CTRL+T => Ir para o inicio da linha
- CTRL+V => Ir para o fim da linha

- CTRL+C => Ir para a proxima linha
- CTRL+X => Ir para a linha anterior

- CTRL+\[ => Ir para inicio do arquivo
- CTRL+\] => Ir para o final do arquivo

### Modo seleçao

- CTRL+I => Liga/Desliga o modo seleçao
- CTRL+Y => Colar
