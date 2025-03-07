# Street Fighter - Projeto de Programação 2
## Linguagem Utilizada
🟦 **Linguagem:** C  
🟦 **Compilador:** GCC
---
## Estrutura do Projeto
Abaixo está a estrutura de diretórios e arquivos do projeto **Prog 2**:
```
Prog2/
├── street_fighter/       # Código do jogo
│   ├── main.c            # Função principal que inicializa o jogo
│   ├── Game.c            # Lógica principal do jogo
│   ├── Game.h            # Cabeçalho do game.c
│   ├── Player.c          # Implementação do personagem do jogador
│   ├── Player.h          # Cabeçalho do player.c
│   ├── Makefile          # Código para compilação
│   ├── fonte/            # Fonte usada no jogo
│   │   └── 2fonte.ttf    # Arquivo de fonte TrueType
│   ├── assets/           # Recursos do jogo
│   │   ├── 1img.png      # Imagem do jogo
│   │   ├── akuma.png     # Imagem do personagem Akuma
│   │   ├── bolinha.png   # Imagem de item ou objeto
│   │   ├── face.png      # Imagem do rosto de um personagem
│   │   ├── ken.png       # Imagem do personagem Ken
│   │   ├── ryu.png       # Imagem do personagem Ryu
│   │   ├── sean.png      # Imagem do personagem Sean
└── README.md             # Documentação do projeto
```
---


##  Instalação das Dependências
Para configurar o ambiente de desenvolvimento e garantir que o projeto funcione corretamente, siga os passos abaixo para instalar as dependências necessárias:

### 1. Instalar o Compilador GCC
Se você ainda não tem o **GCC** instalado, execute o seguinte comando:
```bash
sudo apt update
sudo apt install build-essential
```
🟢  Verificar se foi instalado corretamente: 
```
gcc --version
```

### 2. Instalar a Biblioteca Allegro5
A biblioteca **Allegro5** é necessária para gráficos, áudio, entrada e outras funcionalidades. Instale as dependências com o comando:
```bash
sudo apt install liballegro5-dev liballegro-font-dev liballegro-image-dev liballegro-ttf-dev liballegro-primitives-dev
```
🟢 Verificar se foi instalado corretamente: 
```
dpkg -l | grep liballegro5
```









-----


## Modo de Execução do Jogo


### 1. Compilar o Jogo

Execute o comando abaixo para compilar o projeto utilizando o **Makefile**, Isso irá compilar todos os arquivos `.c` presentes na pasta e gerar o executável **`a`**

```bash
make
```
🟢 Após a compilação bem-sucedida, execute o jogo com o seguinte comando:

```bash
./a
```
Limpar Arquivos de Compilação (Opcional)
Isso removerá os arquivos gerados durante a compilação, garantindo um ambiente limpo.
```bash
make clean
```

------





# Comandos do Jogo

## Jogador 1
- **W**: Pular
- **A**: Mover para a esquerda
- **D**: Mover para a direita
- **S**: Abaixar (Crouch)
- **J**: Soco
- **K**: Chute
- **Shift**: Defesa
- **L**: Ataque Especial

## Jogador 2
- **Seta para cima**: Pular
- **Seta para a esquerda**: Mover para a esquerda
- **Seta para a direita**: Mover para a direita
- **Seta para baixo**: Abaixar (Crouch)
- **Numpad 1**: Soco
- **Numpad 2**: Chute
- **Ctrl**: Defesa
- **Numpad 0**: Ataque Especial


-----
🟢🔵🔴🟠🟡🟣🟤⚪
---
| Jogador 1              |   &nbsp;   | Jogador 2              |
|:----------------------:|:----------:|:----------------------:|
| **Ação**               |            | **Ação**               |
| Pular                  |            | Pular                  |
| Mover para a esquerda  |            | Mover para a esquerda  |
| Mover para a direita   |            | Mover para a direita   |
| Abaixar (Crouch)       |            | Abaixar (Crouch)       |
| Soco                   |            | Soco                   |
| Chute                  |            | Chute                  |
| Defesa                 |            | Defesa                 |
| Ataque Especial        |            | Ataque Especial        |
| **Tecla**              |            | **Tecla**              |
| W                      |            | ↑                      |
| A                      |            | ←                      |
| D                      |            | →                      |
| S                      |            | ↓                      |
| J                      |            | Numpad 1              |
| K                      |            | Numpad 2              |
| Shift                  |            | Ctrl                   |
| L                      |            | Numpad 0              |



<details>
  <summary>Clique aqui para ver mais</summary>
  Aqui está o conteúdo oculto dentro da caixa! Você pode colocar o que quiser aqui, como listas, imagens ou textos.
</details>
