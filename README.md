
<div style="display: flex; flex-direction: row-reverse; align-items: flex-start;">
  <div style="width: 30%; position: relative;">
    <div style="position: absolute; top: 0; left: 0; width: 100%; height: 200px; background-color: red;"></div>
    <div style="position: absolute; bottom: 0; left: 0; width: 100%; height: 50px; background-color: red;"></div>
  </div>
  <div style="width: 70%;">
  </div>
</div>

<div style="width: 100%; height: 100px; background-color: red; margin-top: 20px;">
</div>




# Street Fighter

Este projeto é uma implementação do jogo "Street Fighter" desenvolvida como parte do curso de Programação 2. O objetivo principal é aplicar conceitos de programação, como estruturas de dados, manipulação de arquivos e desenvolvimento de interfaces gráficas para criar uma versão funcional do jogo.

## Linguagem Utilizada

- O código foi escrito na linguagem **C**.

## Requisitos Mínimos

- Sistema operacional compatível com compiladores C (**Linux, macOS ou Windows** com MinGW ou Cygwin).
- **Compilador GCC** instalado.
- **Biblioteca Allegro 5** instalada.

## Bibliotecas Necessárias

- **Allegro 5**: Biblioteca gráfica usada para renderização, entrada de usuário e manipulação de áudio.

## Comandos do Jogo

- **Setas direcionais**: Movimentação do personagem.
- **Tecla 'A'**: Soco.
- **Tecla 'S'**: Chute.
- **Tecla 'D'**: Defesa.
- **Tecla 'Q'**: Ataque especial.

## Explicação do Trabalho

O projeto consiste no desenvolvimento de um jogo de luta inspirado no clássico **Street Fighter**. O jogador pode controlar um personagem que se movimenta, ataca e defende-se contra um oponente. A lógica de combate, os gráficos e os sons foram integrados utilizando a biblioteca **Allegro 5**.

## Organização do Projeto

A estrutura do projeto é organizada da seguinte forma:

```
street_fighter/
├── src/                # Código-fonte do jogo
│   ├── main.c          # Função principal que inicializa o jogo
│   ├── game.c          # Lógica principal do jogo
│   ├── game.h          # Cabeçalho do game.c
│   ├── player.c        # Implementação do personagem do jogador
│   ├── player.h        # Cabeçalho do player.c
│   ├── enemy.c         # Implementação do personagem inimigo
│   ├── enemy.h         # Cabeçalho do enemy.c
│   ├── graphics.c      # Funções de renderização gráfica
│   ├── graphics.h      # Cabeçalho do graphics.c
├── assets/             # Recursos do jogo
│   ├── images/         # Sprites e imagens do jogo
│   ├── sounds/         # Sons e músicas do jogo
├── Makefile            # Script de compilação do jogo
└── README.md           # Documentação do projeto
```

## Explicação do Makefile

O `Makefile` automatiza o processo de compilação do projeto. Suas funcionalidades incluem:

- **Compilação do jogo**: Gera o executável compilando todos os arquivos C necessários.
- **Organização dos arquivos objeto**: Compila os arquivos separadamente para melhor modularidade.
- **Execução do linker**: Junta os arquivos compilados e as bibliotecas necessárias, gerando um único arquivo executável.
- **Limpeza de arquivos temporários**: Remove arquivos `.o` e o executável do jogo quando solicitado.

Para compilar o projeto, utilize o comando:

```bash
make
```

Para limpar os arquivos gerados, utilize:

```bash
make clean
```

## O Que o Código Faz

1. **Inicializa o Allegro 5** e carrega os recursos necessários.
2. **Cria uma janela gráfica** e um loop principal do jogo.
3. **Captura eventos do teclado** para movimentação e ataques.
4. **Atualiza o estado do jogo** com base nas interações do jogador e do inimigo.
5. **Renderiza gráficos e sons** para criar uma experiência dinâmica.
6. **Verifica as condições de vitória ou derrota** e finaliza o jogo adequadamente.

---
*Este `README.md` descreve o projeto de forma detalhada e pode ser ajustado conforme necessário para refletir as especificidades do código.*


-------
-------
-------

<br>
<br>
<br>
<br>
<br>

# 🔥 Pontos Adicionais para um README.md Completo  

## 🚀 Instalação das Dependências  
Explicação passo a passo sobre como instalar as bibliotecas necessárias no sistema operacional.  
Exemplo de comando para instalar o **Allegro 5** no Linux:  
```bash
sudo apt-get install liballegro5-dev
```  

## 🛠 Modo de Execução do Jogo  
- Explicar como rodar o jogo após a compilação (`./street_fighter`).  
- Mencionar se há parâmetros opcionais para a execução.  

## 📂 Explicação Detalhada dos Arquivos  
- Pequena descrição do que cada arquivo `.c` e `.h` faz no projeto.  

## 🎨 Personalização do Jogo  
- Como modificar os **sprites e sons** dentro da pasta `assets/`.  

## 📜 Histórico e Motivação do Projeto  
- Contexto do desenvolvimento (acadêmico ou pessoal).  

## 🕵️‍♂️ Debugging e Erros Comuns  
- Explicação sobre erros que podem ocorrer na compilação ou execução, com possíveis soluções.  

## 🖥️ Capturas de Tela / GIFs  
- Adicionar imagens do jogo rodando para ilustrar o funcionamento.  

## 🎮 IA do Inimigo (Se Houver)  
- Explicar como funciona a inteligência artificial do inimigo.  

## 📜 Licença  
- Informar sobre o licenciamento do código (ex: **MIT, GPL**).  

## 🎯 Objetivos de Aprendizado  
- Conceitos que foram praticados no desenvolvimento (ex: **eventos, gráficos, structs**).  

## 📊 Desempenho do Jogo  
- Se houver otimizações, descrever como o jogo foi estruturado para ser eficiente.  

## 🤝 Contribuição  
- Como outras pessoas podem colaborar no projeto.  

## 📑 Referências  
- Links úteis para a documentação das bibliotecas utilizadas.  

## 💡 Possíveis Melhorias Futuras  
- Ideias para expandir o projeto (**novos personagens, multiplayer, mais golpes, etc.**).  





<br>
<br>
<br>
<br>
<br>

-------
-------
-------




<img src="https://github.com/user-attachments/assets/6cbb31ba-1d3b-4c33-b35c-e797d9a3d0e4" alt="Imagem" width="700"/>
