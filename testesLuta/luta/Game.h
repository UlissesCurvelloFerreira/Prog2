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
#define CORTE 20 // 30 corte para ajustar o personagem da direita e da esquerda dado que a imagem não esta alinhada.


typedef struct
{
    Player *player1;
    Player *player2;
    int mapa_selecionado;
} Game;


void escolher_mapas(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue);
Game *create_game();
void destroy_game(Game *game);
void exibe_ganhador(Game *game, ALLEGRO_EVENT_QUEUE *queue);
void inicia_rounds(Game *game, ALLEGRO_EVENT_QUEUE *queue);


#endif // PLAYER_H
