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
#define CORTE 20 
// 30 corte para ajustar o personagem da direita e 
//da esquerda dado que a imagem não esta alinhada.


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
    unsigned char ataque;         // Indica se o personagem está atacando
    unsigned char no_ar;          // Indica se o personagem está no ar
    float velocidade_y; // Velocidade vertical do personagem
    int animacao_atual; // Índice da animação atual // animacao_atual
    int vida;           // Vida do personagem
    int vitorias;       // Contador de vitórias.
} Player;


/* Função para criar e inicializar um jogador.
Aloca memória para uma nova estrutura Player.
Inicializa os atributos do jogador, incluindo posição, estado e controle.
Carrega o sprite da face do jogador e inicializa as animações e hitboxes.
Se ocorrer um erro ao alocar memória para as animações, a memória do jogador é liberada.
Retorna um ponteiro para a estrutura Player inicializada ou NULL em caso de erro.*/
Player *create_player();


/* Função responsável por liberar a memória da estrutura player.
Desta forma, verifica se a estrutura realmente foi criada e começa o processo de liberação.
Libera as imagens alocadas, o joystick, animações e, por fim, a estrutura player. */
void destroy_player(Player *player);


/*Função que verifica se o jogador 1 ultrapassou o jogador 2.
Funçaõ fundamental para que os jogadores fiquem frente a frente das suas animações.*/
int invert_flag_control(Player *p1, Player *p2);


/*Função que tem o objetivo de modar/modificar a forma dos jogadores, desta forma cada 
animação tem um correspondente especifico, algumas podem ser iguais como idle, 
andando frente ou  andando trás, mas todos tem uma forma previamente especificada.*/
void update_hitbox(Player *player, int flag);


/*Função responsável por carregar os sprites dos jogadores.
Pode optar por escolher entre: 0-Sean, 1-Ryu, 2-Ken e 3-Akuma.*/
void carrega_sprite(Player *player, int selecao);


/*Função que inicia a animação de soco do jogador.
Dependendo do estado do jogador (no ar, abaixado, ou normal), 
A função só altera o estado de ataque se o jogador não estiver atacando atualmente.*/
void soco(Player *p);


/*Função que inicia a animação de chute do jogador.
Dependendo do estado do jogador (no ar, abaixado, ou normal), 
A função só altera o estado de ataque se o jogador não estiver atacando atualmente.*/
void chute(Player *p);


/*Função que inicia a animação especial do jogador.
A função só altera o estado de ataque se o jogador não estiver atacando atualmente.*/
void movimento_especial(Player *p);


/*Função que inicia a animação dano do jogador (sofrer dano).
Dependendo do estado do jogador (abaixado, ou normal/no ar), */
void leva_dano(Player *p);


/*Função que define a animação de idle do jogador.
Verifica se o jogador não está se movendo (esquerda, direita, cima, baixo), 
atacando, no ar ou em estado de defesa. Se todas essas condições forem verdadeiras,
a função define a animação atual como a de idle.*/
void movimento_idle(Player *p);


/*Funçaõ que define os movimentos de defesa de ambos os jogadores.
Dependendo do estado do jogador (no ar, abaixado, ou normal)*/
void movimento_defesa(Player *p);


/* Função que verifica se a animação de ataque do jogador chegou ao fim.
Se o jogador está em estado de ataque, a função verifica o quadro atual da animação.
Se o quadro atual for o último quadro da animação, o estado de ataque é desativado.
Se o controle de movimento para baixo estiver ativo, a animação atual é definida como "abaixado".*/
void movimento_ataque_verifica(Player *p);


/*Funçaõ que verifica se o jogador esta se movendo para a direita ou esquerda.
Esta muda conform o jogador, pois a direita e a esquerda deve se inverter.*/
void movimento_left_right(Player *p);

/*Função responsavel por exibir fatality*/
void movimento_final(Player *p1, Player *p2);

/*Função responsavel por detectar colisão e baixar a vida dos jogadores.*/
void diminuir_vida (Player *p1, Player *p2);

/* Função para desenhar uma animação de um jogador.
Desenha um frame da animação especificada na posição do jogador.
Se o modo fatality estiver ativo (fatality == 17), ajusta a largura do sprite e a posição.
Caso contrário, desenha a animação normalmente.*/
void draw_animation(Player *player, Animation *anim, int flag, int fatality);


/* Inicializa uma estrutura de animação com os valores fornecidos.
Define a posição inicial, contagem de frames, dimensões dos frames e duração de cada frame.*/
void init_animation(Animation *anim, int x, int y, int frame_count, int frame_width, int frame_height, float frame_duration);


/* Verifica se duas hitboxes estão colidindo.
Compara as posições e dimensões das duas hitboxes para determinar se há uma interseção.
Retorna 1 se as hitboxes colidirem e 0 caso contrário.*/
int check_collision(Hitbox *hitbox1, Hitbox *hitbox2);


/* Verifica se dois jogadores estão colidindo.
Utiliza a função check_collision para verificar a colisão entre as hitboxes superiores
e inferiores dos dois jogadores.
Retorna 1 se qualquer par de hitboxes colidirem e 0 caso contrário.*/
int check_player_collision(Player *p1, Player *p2);


/* Resolve a colisão entre dois jogadores.
Se os jogadores estiverem colidindo, ajusta as posições dos jogadores para afastá-los.*/
void resolve_collision(Player *p1, Player *p2);


/*Verifica e corrige a posição do jogador para que ele 
não ultrapasse os limites da tela.*/
void verifica_limites(Player *player, int largura_tela);


/*Atualiza a animação do jogador para "Jump" ou "abaixado" com base nos controles de movimento vertical.*/
void movimento_baixo_cima(Player *p);


/* Se o jogador está no ar, a função atualiza sua posição vertical com base na
velocidade e na gravidade. Se o jogador atinge o piso, a função redefine a posição
e a velocidade, e ajusta a animação para "abaixado" ou "idle" conforme o controle.
Se o jogador não está no ar e o controle de pulo está ativo, inicia o pulo.*/
void movimento_no_ar(Player *p);


/* Atualiza a animação com base no tempo decorrido.
Esta função gerencia a troca de frames da animação com base no tempo decorrido desdea última atualização. 
Adiciona o tempo passado (delta_time) ao acumulador de tempo(elapsed_time). 
Se o acumulador atinge ou excede a duração de exibição de um frame(frame_duration),
o acumulador é reiniciado e o quadro atual da animação é avançado.
A mudança de quadro é feita ciclicamente, retornando ao primeiro quadro quando o últimoé alcançado.

O cálculo da variável `elapsed_time` é feito da seguinte forma:
- `elapsed_time` é incrementado pelo valor de `delta_time`, que representa o tempo decorrido desde a última atualização da animação.
- Se `elapsed_time` for maior ou igual a `frame_duration`, significa que o tempo necessário para exibir o quadro atual passou. Então:
- `elapsed_time` é resetado para 0, para começar a contagem para o próximo quadro.
- `current_frame` é atualizado para o próximo quadro na sequência. Se o quadro atual é o último (definido por `frame_count`), 
ele retorna ao primeiro quadro, criando um efeito de loop contínuo na animação.*/
void update_animation(Animation *anim, float delta_time);

#endif