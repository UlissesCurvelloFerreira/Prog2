#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "Joystick.h"
#include "Player.h"


#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define PLAYER_WIDTH 484
#define PLAYER_HEIGHT 324
#define SQUARE_SIZE 80
#define SQUARE_PADDING 10
#define GRAVIDADE 7.0f
#define PULO_VELOCIDADE -60.0f
#define PISO 300
#define CORTE 20 


typedef struct
{   
    Player *player1;
    Player *player2;
    int mapa_selecionado;
} Game;


/*Cria uma nova instância do jogo e inicializa os jogadores e a seleção do mapa.
Retorna um ponteiro para o novo objeto Game.*/
Game *create_game();


/*Libera a memória alocada para o objeto Game e seus jogadores.*/
void destroy_game(Game *game);


/*Função que gerencia a seleção dos personagens em um modo multiplayer. A função exibe uma tela onde os jogadores
podem escolher seus personagens. Ela utiliza um temporizador para atualizar a tela periodicamente. Durante a execução,
os jogadores podem mover a seleção usando as teclas direcionais e confirmar suas escolhas com as teclas apropriadas.
A função desenha a interface gráfica com quadrados de seleção e mostra os personagens selecionados. 
Quando ambos os jogadores fazem suas seleções, a função encerra o modo de seleção e prossegue para a tela de escolha de mapas.
Se a tecla ESC for pressionada, a função retorna ao menu principal. Além disso, a função lida com o fechamento da janela do jogo,
encerrando o programa se necessário.*/
void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);


/*Função para escolher o mapa. Exibe opções de mapas e permite ao jogador selecionar um.
A função aguarda eventos de teclado e atualiza a seleção do mapa conforme as teclas pressionadas.
Se a tecla ESC for pressionada, a função retorna ao menu principal.
Se a janela for fechada, o jogo é encerrado.*/
void inicia_rounds(Game *game, ALLEGRO_EVENT_QUEUE *queue);


/*Função responsável por controlar a tela de combate do jogo, gerenciar a entrada do usuário e atualizar o estado dos jogadores.
Inicializa as posições e estados dos jogadores. Entra em um loop principal onde espera e trata eventos,
como temporizador e entrada do teclado. Atualiza as animações dos jogadores, verifica colisões, atualiza a interface
e gerencia o estado de pausa do jogo.*/
void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue);


/*Inicia os rounds do jogo, atualizando a tela e verificando as vitórias dos jogadores.
O loop continua enquanto nenhum jogador alcançar 2 vitórias e o número de rounds for menor que 3.
Após o término dos rounds, exibe o ganhador e zera as vitórias dos jogadores.*/
void escolher_mapas(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);


/*Exibe o ganhador na tela após o término dos rounds.
Carrega e desenha o fundo, o texto indicando o resultado e as imagens dos jogadores.
O resultado é baseado no número de vitórias de cada jogador.
Se houver empate, exibe "EMPATE".*/
void exibe_ganhador(Game *game, ALLEGRO_EVENT_QUEUE *queue);


#endif // PLAYER_H
