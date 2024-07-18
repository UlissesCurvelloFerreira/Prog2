#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "Joystick.h"


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
    int x, y, width, height; // Coordenadas e dimensões da hitbox
} Hitbox;

typedef struct
{
    int x, y;             // Coordenadas da animação no sprite sheet
    int frame_count;      // Número de frames na animação
    int frame_width;      // Largura de cada frame
    int frame_height;     // Altura de cada frame
    int current_frame;    // Frame atual da animação
    float frame_duration; // Duração de cada frame
    float elapsed_time;   // Tempo decorrido desde o último frame
} Animation;

typedef struct
{
    ALLEGRO_BITMAP *img_sprite;
    ALLEGRO_BITMAP *face_sprite;
    Hitbox hitbox_superior; // Hitbox superior do personagem
    Hitbox hitbox_inferior; // Hitbox inferior do personagem
    joystick *control;      // Joystick para controle
    Animation *animations;  // Array de animações
    int selecao;
    int x, y;           // Coordenadas do personagem para movimentação
    int ataque;         // Indica se o personagem está atacando
    int no_ar;          // Indica se o personagem está no ar
    float velocidade_y; // Velocidade vertical do personagem
    int animacao_atual; // Índice da animação atual // animacao_atual
    int vida;           // Vida do personagem
    int vitorias;
    //int id;
    // int facing_left;       // Indica se o personagem está virado para a esquerda
} Player;


Player *create_player();
void destroy_player(Player *player);
void draw_animation(Player *player, Animation *anim, int flag, int fatality);
void init_animation(Animation *anim, int x, int y, int frame_count, int frame_width, int frame_height, float frame_duration);
void update_animation(Animation *anim, float delta_time);
void carrega_sprite(Player *player, int selecao);
void verifica_limites(Player *player, int largura_tela);
int check_collision(Hitbox *hitbox1, Hitbox *hitbox2);
int check_player_collision(Player *p1, Player *p2);
int invert_flag_control(Player *p1, Player *p2);
void resolve_collision(Player *p1, Player *p2);
void update_hitbox(Player *player, int flag);
void soco(Player *p);
void chute(Player *p);
void leva_dano(Player *p);
void movimento_especial(Player *p);
void movimento_idle(Player *p);
void movimento_ataque_verifica(Player *p);
void movimento_no_ar(Player *p);
void movimento_baixo_cima(Player *p);
void movimento_left_right(Player *p);
void movimento_defesa(Player *p);



#endif // PLAYER_H