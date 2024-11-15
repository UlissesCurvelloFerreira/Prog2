#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "Joystick.h"
#include <time.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define PLAYER_WIDTH 484
#define PLAYER_HEIGHT 324
#define SQUARE_SIZE 80
#define SQUARE_PADDING 10
#define GRAVIDADE 7.0f
#define PULO_VELOCIDADE -60.0f
#define PISO 220
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
    int id;
    // int facing_left;       // Indica se o personagem está virado para a esquerda
} Player;

typedef struct
{
    Player *player1;
    Player *player2;
} Game;

void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer, int mapa_aleat);
Game *create_game();
void destroy_game(Game *game);
Player *create_player(int id);
void destroy_player(Player *player);
void carrega_sprite(Player *player, int selecao);
void exibe_personagem(Player *player, int x, int y, int flag);
void exibe_face(Player *player, int x, int y, int face_index, int flag);
void flash_text(ALLEGRO_FONT *font, const char *text, int x, int y, int flashes, ALLEGRO_COLOR color, ALLEGRO_COLOR bg_color, ALLEGRO_EVENT_QUEUE *queue);

/* Para animar os personagens. */
void init_animation(Animation *anim, int x, int y, int frame_count, int frame_width, int frame_height, float frame_duration);
void update_animation(Animation *anim, float delta_time);
void draw_animation(Player *player, Animation *anim, int flag, int especial);
void verifica_limites(Player *player, int largura_tela);
void update_hitbox(Player *player, int flag);
int check_collision(Hitbox *hitbox1, Hitbox *hitbox2);
int check_player_collision(Player *p1, Player *p2);
void resolve_collision(Player *p1, Player *p2);

///////
#define INCREMENT_VALUE 0.01f
#define MAX_DELAY_COUNT 20

int gerarNumeroAleatorio()
{
    return rand() % 2 + 1; // Gera um número aleatório entre 0 e 1, e depois adiciona 1
}

void show_fading_image(ALLEGRO_BITMAP *image, ALLEGRO_EVENT_QUEUE *event_queue, int src_x, int src_y, int src_w, int src_h)
{
    float alpha = 0.0f;
    int delayCount = 0;

    for (bool increasing = true; increasing || alpha > 0.0f;)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            alpha += increasing ? INCREMENT_VALUE : -INCREMENT_VALUE;

            if (increasing && alpha >= 1.0f)
            {
                delayCount = (delayCount >= MAX_DELAY_COUNT) ? 0 : delayCount + 1;
                increasing = !(delayCount == 0);
            }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_tinted_bitmap_region(image, al_map_rgba_f(alpha, alpha, alpha, alpha), src_x, src_y, src_w, src_h, 0, 0, 0);
            al_flip_display();
        }
    }
}
///////

/* MINHAS CRIAÇÕES. */
void inicia_rounds(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);

int main()
{
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    srand(1);

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    al_set_window_title(disp, "TP prog.");

    ALLEGRO_FONT *font = al_load_ttf_font("fonte/2fonte.ttf", 35, 0);
    if (!font)
    {
        fprintf(stderr, "Falha ao carregar a fonte.\n");
        return -1;
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("img/1img.png");
    if (!background)
    {
        fprintf(stderr, "Falha ao carregar a imagem de fundo.\n");
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    ALLEGRO_COLOR selected_color = al_map_rgb(250, 90, 0);
    ALLEGRO_COLOR bg_color = al_map_rgb(0, 0, 0);
    al_start_timer(timer);

    int menuSelection = 0;
    // show_fading_image(background, queue, 900, 0, 900, 600);
    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, 0, 0, 0);
            al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, 200, ALLEGRO_ALIGN_CENTER, "MENU:");
            al_draw_text(font, (menuSelection == 0) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 250, ALLEGRO_ALIGN_CENTER, "Multiplayer");
            al_draw_text(font, (menuSelection == 1) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 310, ALLEGRO_ALIGN_CENTER, "Singleplayer");
            al_draw_text(font, (menuSelection == 2) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 370, ALLEGRO_ALIGN_CENTER, "Sair/Fechar");
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                menuSelection = (menuSelection + 1) % 3;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                menuSelection = (menuSelection - 1 + 3) % 3;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                if (menuSelection == 0)
                {
                    flash_text(font, "Multiplayer", SCREEN_WIDTH / 2, 250, 3, selected_color, bg_color, queue);
                    Game *game = create_game();
                    run_multiplayer(game, queue, timer);
                    inicia_rounds(game, queue, timer);
                    // add ganhador
                    // tela_controle(game, queue, timer);
                    destroy_game(game);
                }
                else if (menuSelection == 1)
                {
                    flash_text(font, "Singleplayer", SCREEN_WIDTH / 2, 310, 3, selected_color, bg_color, queue);
                    printf("Singleplayer selecionado!\n");
                }
                else if (menuSelection == 2)
                {
                    flash_text(font, "Sair/Fechar", SCREEN_WIDTH / 2, 370, 3, selected_color, bg_color, queue);
                    printf("Saindo do jogo...\n");
                    break;
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
    }

    al_destroy_bitmap(background);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

void flash_text(ALLEGRO_FONT *font, const char *text, int x, int y, int flashes, ALLEGRO_COLOR color, ALLEGRO_COLOR bg_color, ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    for (int i = 0; i < flashes * 2; ++i)
    {
        al_clear_to_color(bg_color);
        if (i % 2 == 0)
        {
            al_draw_text(font, color, x, y, ALLEGRO_ALIGN_CENTER, text);
        }
        al_flip_display();
        al_rest(0.1); // Ajuste a duração do piscar conforme necessário
        while (al_get_next_event(queue, &event))
        {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                exit(0);
            }
        }
    }
}

void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    int player1_selected = 0;
    int player2_selected = 0;

    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int start_x = (SCREEN_WIDTH - (2 * SQUARE_SIZE + SQUARE_PADDING)) / 2;
            int start_y = (SCREEN_HEIGHT - (2 * SQUARE_SIZE + SQUARE_PADDING)) / 2;

            // Desenha os quatro quadrados em duas linhas
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    al_draw_rectangle(start_x + j * (SQUARE_SIZE + SQUARE_PADDING), start_y + i * (SQUARE_SIZE + SQUARE_PADDING),
                                      start_x + j * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE, start_y + i * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE,
                                      al_map_rgb(101, 101, 101), 2);
                }
            }

            // Desenha os jogadores
            int player1_x = game->player1->selecao % 2;
            int player1_y = game->player1->selecao / 2;
            int player2_x = game->player2->selecao % 2;
            int player2_y = game->player2->selecao / 2;

            al_draw_filled_rectangle(start_x + player1_x * (SQUARE_SIZE + SQUARE_PADDING), start_y + player1_y * (SQUARE_SIZE + SQUARE_PADDING),
                                     start_x + player1_x * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE, start_y + player1_y * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE,
                                     al_map_rgb(191, 11, 8));
            al_draw_filled_rectangle(start_x + player2_x * (SQUARE_SIZE + SQUARE_PADDING), start_y + player2_y * (SQUARE_SIZE + SQUARE_PADDING),
                                     start_x + player2_x * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE, start_y + player2_y * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE,
                                     al_map_rgb(251, 255, 0));

            ALLEGRO_FONT *font = al_create_builtin_font();

            al_draw_text(font, al_map_rgb(255, 119, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "SELECIONE SEUS HEROIS");
            /* aprocimadfamenet SCREEN_WIDTH/6*/
            al_draw_text(font, al_map_rgb(255, 0, 0), 150, 520, ALLEGRO_ALIGN_CENTER, "PLAYER 1");
            al_draw_text(font, al_map_rgb(208, 255, 0), SCREEN_WIDTH - 150, 520, ALLEGRO_ALIGN_CENTER, "PLAYER 2");

            al_draw_bitmap_region(game->player1->face_sprite, 306, 2, 900, 600, 0, 0, 0);

            al_draw_bitmap_region(game->player1->face_sprite, 0, (game->player1->selecao) * 400, 300, 400, 20, 100, 0);
            al_draw_bitmap_region(game->player2->face_sprite, 0, (game->player2->selecao) * 400, 300, 400, SCREEN_WIDTH - (300 + 20), 100, 1);

            /// al_draw_scaled_bitmap(game->player1->face_sprite, 0, (game->player1->selecao) * 400, 300, 200, 10, 100, 300, 400, 0);
            // al_draw_scaled_bitmap(game->player2->face_sprite, 0, (game->player2->selecao) * 400, 300, 200, SCREEN_WIDTH - (150 * 2) - 10, 100, 300, 400, 1);

            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (!player1_selected)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_A)
                {
                    if (game->player1->selecao % 2 > 0 && game->player1->selecao - 1 != game->player2->selecao)
                        game->player1->selecao--;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_D)
                {
                    if (game->player1->selecao % 2 < 1 && game->player1->selecao + 1 != game->player2->selecao)
                        game->player1->selecao++;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_W)
                {
                    if (game->player1->selecao / 2 > 0 && game->player1->selecao - 2 != game->player2->selecao)
                        game->player1->selecao -= 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_S)
                {
                    if (game->player1->selecao / 2 < 1 && game->player1->selecao + 2 != game->player2->selecao)
                        game->player1->selecao += 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_J)
                {
                    printf("Player 1 selecionou o quadrado %d\n", game->player1->selecao);
                    player1_selected = 1;
                    carrega_sprite(game->player1, game->player1->selecao);
                }
            }

            if (!player2_selected)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    if (game->player2->selecao % 2 > 0 && game->player2->selecao - 1 != game->player1->selecao)
                        game->player2->selecao--;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    if (game->player2->selecao % 2 < 1 && game->player2->selecao + 1 != game->player1->selecao)
                        game->player2->selecao++;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    if (game->player2->selecao / 2 > 0 && game->player2->selecao - 2 != game->player1->selecao)
                        game->player2->selecao -= 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    if (game->player2->selecao / 2 < 1 && game->player2->selecao + 2 != game->player1->selecao)
                        game->player2->selecao += 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    printf("Player 2 selecionou o quadrado %d\n", game->player2->selecao);
                    player2_selected = 1;
                    carrega_sprite(game->player2, game->player2->selecao);
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            exit(0);
        }

        if (player1_selected && player2_selected)
        {
            printf("Os dois jogadores selecionaram seus quadrados. Saindo do modo multiplayer...\n");
            break;
        }
    }
    // tela_controle(game, queue, timer);
}

void soco(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down)
        {
            p->animacao_atual = 6; // Soco baixo.
        }
        else if (p->no_ar)
        {
            p->animacao_atual = 14; // soco cima.
        }
        else
        {
            p->animacao_atual = 4; // Soco.
        }
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void chute(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down)
        {
            p->animacao_atual = 7; // Chute baixo
        }
        else if (p->no_ar)
        {
            p->animacao_atual = 15; // chute cima
        }
        else
        {
            p->animacao_atual = 5; // Chute
        }
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void leva_dano(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down)
        {
            p->animacao_atual = 11; // animação de dano abaixado.
        }
        else
        {
            p->animacao_atual = 10; // animação de dano.
        }
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void verifica_limites(Player *player, int largura_tela)
{
    int offset_x = 200; // Espaço em branco nas laterais
    if (player->x < -offset_x)
    {
        player->x = -offset_x;
    }
    else if (player->x + PLAYER_WIDTH - offset_x > largura_tela)
    {
        player->x = largura_tela - (PLAYER_WIDTH - offset_x);
    }
}

int check_collision(Hitbox *hitbox1, Hitbox *hitbox2)
{
    if (hitbox1->x < hitbox2->x + hitbox2->width &&
        hitbox1->x + hitbox1->width > hitbox2->x &&
        hitbox1->y < hitbox2->y + hitbox2->height &&
        hitbox1->y + hitbox1->height > hitbox2->y)
    {
        return 1;
    }
    return 0;
}

int check_player_collision(Player *p1, Player *p2)
{
    if (check_collision(&p1->hitbox_superior, &p2->hitbox_superior) ||
        check_collision(&p1->hitbox_superior, &p2->hitbox_inferior) ||
        check_collision(&p1->hitbox_inferior, &p2->hitbox_superior) ||
        check_collision(&p1->hitbox_inferior, &p2->hitbox_inferior))
    {
        return 1;
    }
    return 0;
}

int invert_flag_control(Player *p1, Player *p2)
{
    if (p1->x > p2->x)
        return 1;
    return 0;

    // int p = 1; // Se 0 150, se 1 -150;
    // int esc = p ? -150 : 150;
    // if (esc == -150)
    //     printf("ESCOLHEU O -150\n");
    // if (esc == 150)
    //     printf("ESCOLHEU O +150\n");
}

void resolve_collision(Player *p1, Player *p2)
{
    if (check_player_collision(p1, p2))
    {
        if (p1->x < p2->x)
        {
            p1->x -= 10;
            p2->x += 10;
        }
        else
        {
            p1->x += 10;
            p2->x -= 10;
        }
    }
}

// CRIADAS AGORA SABADO NOITE 06/09 (23:10)

void movimento_idle(Player *p)
{
    if (!p->control->left &&
        !p->control->right &&
        !p->control->up &&
        !p->control->down &&
        !p->ataque &&
        !p->no_ar &&
        !p->control->defesa)
    {
        p->animacao_atual = 0; // Idle
    }
}

void movimento_ataque_verifica(Player *p)
{
    if (p->ataque)
    {
        Animation *anim = &p->animations[p->animacao_atual];
        if (anim->current_frame == anim->frame_count - 1)
        {
            p->ataque = 0;
            if (p->control->down)
            {
                p->animacao_atual = 3; // Crouch
            }
            // else
            // {
            //     p->animacao_atual = 0; // Idle
            // }
        }
    }
}

void movimento_no_ar(Player *p)
{
    // Atualiza a física do pulo do Player 1
    if (p->no_ar)
    {
        p->y += p->velocidade_y;
        p->velocidade_y += GRAVIDADE;

        if (p->y >= PISO)
        {
            p->y = PISO;
            p->no_ar = 0;
            p->velocidade_y = 0;
            if (!p->ataque)
            {
                if (p->control->down)
                {
                    p->animacao_atual = 3; // Crouch
                }
                else
                {
                    p->animacao_atual = 0; // Idle
                }
            }
        }
    }
    else if (p->control->up)
    {
        p->velocidade_y = PULO_VELOCIDADE;
        p->no_ar = 1;
    }
}

void movimento_baixo_cima(Player *p)
{
    if (p->control->up)
    {
        if (!p->ataque && p->no_ar)
            p->animacao_atual = 2; // Jump
    }
    if (p->control->down)
    {
        if (!p->ataque && !p->no_ar)
            p->animacao_atual = 3; // Crouch
    }
}

void movimento_left_right(Player *p)
{
    // Aplica a movimentação dos jogadores
    if (p->control->left && !p->control->down && !p->control->defesa)
    {
        p->x -= 15;
        verifica_limites(p, SCREEN_WIDTH); // Verifica os limites do player 1
        if (!p->ataque && !p->no_ar)
            p->animacao_atual = 1; // Walk
    }
    if (p->control->right && !p->control->down && !p->control->defesa)
    {
        p->x += 15;
        verifica_limites(p, SCREEN_WIDTH); // Verifica os limites do player 1
        if (!p->ataque && !p->no_ar)
            p->animacao_atual = 1; // Walk
    }
}


void movimento_defesa(Player *p)
{
    // movimentos de defesa para ambos os personagens.
    if (p->control->defesa)
    {
        if (p->control->down)
            p->animacao_atual = 12; // defesa baixo.
        else if (p->no_ar)
            p->animacao_atual = 13; // defesa cima.
        else
            p->animacao_atual = 8; // defesa cima.
        // printf("valor da defesa %d\n ",p->control->defesa);
    }
}


// CRIADAS AGORA SABADO NOITE 06/09 (23:10)



void update_hitbox(Player *player, int flag)
{
    // int hitbox_offset = player->facing_left ? -150 : 150; // Define a direção da expansão da hitbox

    switch (player->animacao_atual)
    {
    case 0: // Idle
    case 1: // Walk
    case 2: // Jump
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200;
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 85;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;
    case 3:
    case 12:
        // Hitbox superior desativada // Crouch
        player->hitbox_superior.width = 0;
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170; ///
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;
    case 4:
        // Hitbox superior // Soco
        player->hitbox_superior.x = player->x + 200 + (-65 * flag); // 65 pois é a diferença entre a largura 150-85 = 65
                                                                    // player->hitbox_superior.x = player->x + 200 + (flag ? -65 : 0)//
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 150;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;
    case 5: 
        // Hitbox superior // Chute
        player->hitbox_superior.x = player->x + 200 + (-75 * flag) /*+ (player->facing_left ? -50 : 50)*/; // Ajusta a direção
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 160;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;
    case 6:                                // Soco baixo
        player->hitbox_superior.width = 0; // Hitbox superior desativada
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-55 * flag); // Ajusta a direção
        player->hitbox_inferior.y = player->y + 170;                ///
        player->hitbox_inferior.width = 140;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;
    case 7:                                // Chute baixo
    case 15: 
        // Hitbox superior
        // player->hitbox_superior.x = player->x + 200;
        // player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 0;
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-75 * flag); // Ajusta a direção
        player->hitbox_inferior.y = player->y + 170;                ///
        player->hitbox_inferior.width = 160;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;
    case 9:
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200 + (-765 * flag);
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 850;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-765 * flag);
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 850;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;

    default:
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200;
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 85;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;
    }
}








void movimento_especial(Player *p)
{

    if (!p->ataque)
    {
        //p->x +=200;
        //p->y += 800;
        p->ataque = 1;
        if (p->control->down)
        {
            p->animacao_atual = 16; // Soco baixo.
        }
        else if (p->no_ar)
        {
            p->animacao_atual = 16; // soco cima.
        }
        else
        {
            p->animacao_atual = 16; // Soco.
        }
        p->animations[p->animacao_atual].current_frame = 0;
    }

}


void movimento_dano (Player *p)
{
    

    if (p->control->down)
    {
        p->animacao_atual = 10; // Soco baixo.
    }
    else if (p->no_ar)
    {
        p->animacao_atual = 11; // soco cima.
    }
    else
    {
        p->animacao_atual = 11; // Soco.
    }
    p->animations[p->animacao_atual].current_frame = 0;
    
    
}


void movimento_queda (Player *p)
{
    //init_animation(&game->player1->animations[11], (384 * 0), 224 * 14, 8, 384, 224, frame_duration); // Toma normal. ok
     p->animacao_atual = 17;
}







void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *main_timer, int mapa_aleat)
{
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *local_timer = al_create_timer(1.0 / 30.0); // Temporizador específico para tela_controle
    al_register_event_source(queue, al_get_timer_event_source(local_timer));

    al_start_timer(local_timer); // Inicia o temporizador específico
    int invert_flag;
    int pause = 0;

    // Inicializa as posições dos personagens
    game->player1->x = -150;
    game->player1->y = PISO;
    game->player2->x = 550;
    game->player2->y = PISO;

    game->player1->vida = 100;
    game->player2->vida = 100;

    const float frame_duration = 0.05; // Duração de cada frame em segundos

    // Inicializa as animações // 16 para queda.
    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration);               // Idle.
    init_animation(&game->player1->animations[1], 0, 224, 10, 384, 224, frame_duration);             // Walk.
    init_animation(&game->player1->animations[2], 0, 448, 9, 384, 224, frame_duration);              // Jump.
    init_animation(&game->player1->animations[3], (384 * 4), 672, 1, 384, 224, frame_duration);      // Crouch.
    init_animation(&game->player1->animations[4], 0, 224 * 4, 5, 384, 224, frame_duration);          // Soco.
    init_animation(&game->player1->animations[5], 0, 224 * 7, 10, 384, 224, frame_duration);         // Chute.
    init_animation(&game->player1->animations[6], 0, 224 * 5, 6, 384, 224, frame_duration);          // Soco baixo.
    init_animation(&game->player1->animations[7], 0, 224 * 8, 6, 384, 224, frame_duration);          // Chute baixo.
    init_animation(&game->player1->animations[8], (384 * 3), 224 * 10, 1, 384, 224, frame_duration); // defesa.
    init_animation(&game->player1->animations[9], (384 * 6), 224 * 15, 1, 900, 224, frame_duration);  // fatality.
    // 
    // move left OK, right não.
    //init_animation(&game->player1->animations[15], (384 * 1), 224 * 9, 9, 384, 224, frame_duration);  // toma dano no alto.
    init_animation(&game->player1->animations[10], (384 * 0), 224 * 13, 6, 384, 224, frame_duration); // Toma baixo. ok
    init_animation(&game->player1->animations[11], (384 * 0), 224 * 14, 8, 384, 224, frame_duration); // Toma normal. ok

    init_animation(&game->player1->animations[17], (384 * 0), 224 * 17, 12, 384, 224, frame_duration*2); // Toma . ok

    init_animation(&game->player1->animations[12], (384 * 1), 224 * 11, 1, 384, 224, frame_duration); // defesa baixo.
    init_animation(&game->player1->animations[13], (384 * 1), 224 * 12, 1, 384, 224, frame_duration); // defesa cima.
    init_animation(&game->player1->animations[14], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);  // soco cima.
    init_animation(&game->player1->animations[15], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);  // chute cima.
    init_animation(&game->player1->animations[16], (384 * 0), 224 * 16, 12, 384, 224, frame_duration);  // movimento particular (movimento especial)


    /*
    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration); // idle.
    init_animation(&game->player1->animations[1], 0, 0, 10, 384, 224, frame_duration); // moviemnto esquerda. 
    init_animation(&game->player1->animations[2], 0, 0, 10, 384, 224, frame_duration); // moviemnto direita.
    init_animation(&game->player1->animations[3], 0, 0, 10, 384, 224, frame_duration); // pulo.
    init_animation(&game->player1->animations[4], 0, 0, 10, 384, 224, frame_duration); // abaixado.

    init_animation(&game->player1->animations[5], 0, 0, 10, 384, 224, frame_duration); // soco normal.
    init_animation(&game->player1->animations[6], 0, 0, 10, 384, 224, frame_duration); // soco cima.
    init_animation(&game->player1->animations[7], 0, 0, 10, 384, 224, frame_duration); // soco baixo.

    init_animation(&game->player1->animations[8], 0, 0, 10, 384, 224, frame_duration); // chute normal.
    init_animation(&game->player1->animations[9], 0, 0, 10, 384, 224, frame_duration); // chute cima.
    init_animation(&game->player1->animations[10], 0, 0, 10, 384, 224, frame_duration); // chute baixo.

    init_animation(&game->player1->animations[11], 0, 0, 10, 384, 224, frame_duration); // defende normal.
    init_animation(&game->player1->animations[12], 0, 0, 10, 384, 224, frame_duration); // defende cima.
    init_animation(&game->player1->animations[13], 0, 0, 10, 384, 224, frame_duration); // defende baixo.

    init_animation(&game->player1->animations[14], 0, 0, 10, 384, 224, frame_duration); // toma dano normal/baixo.
    init_animation(&game->player1->animations[15], 0, 0, 10, 384, 224, frame_duration); // toma dano biaxo.

    init_animation(&game->player1->animations[16], 0, 0, 10, 384, 224, frame_duration); // movimento especial.
    init_animation(&game->player1->animations[17], 0, 0, 10, 384, 224, frame_duration); // movimento fatality.
    

    */


















    // MOVIMENTAÇÃO PARA PERSONAGEM/HERÓI (2)
    init_animation(&game->player2->animations[0], 0, 0, 10, 384, 224, frame_duration);               // Idle ok
    init_animation(&game->player2->animations[1], 0, 224, 10, 384, 224, frame_duration);             // Walk ok
    init_animation(&game->player2->animations[2], 0, 448, 9, 384, 224, frame_duration);              // Jump ok
    init_animation(&game->player2->animations[3], (384 * 4), 672, 1, 384, 224, frame_duration);      // Crouch ok
    init_animation(&game->player2->animations[4], 0, 224 * 4, 5, 384, 224, frame_duration);          // Soco ok
    init_animation(&game->player2->animations[5], 0, 224 * 7, 10, 384, 224, frame_duration);         // Chute não
    init_animation(&game->player2->animations[6], 0, 224 * 5, 6, 384, 224, frame_duration);          // Soco baixo
    init_animation(&game->player2->animations[7], 0, 224 * 8, 6, 384, 224, frame_duration);          // Chute baixo
    init_animation(&game->player2->animations[8], (384 * 3), 224 * 10, 1, 384, 224, frame_duration); // defesa
    init_animation(&game->player2->animations[9], (384 * 6), 224 * 15, 1, 900, 224, frame_duration); // fatality.
    // init_animation(&game->player2->animations[9], (384 * 6), 224 * 15, 1, 900, 224, frame_duration); // movimento particular (movimento especial)


    init_animation(&game->player2->animations[10], (384 * 1), 224 * 14, 3, 384, 224, frame_duration); // Toma dano.
    init_animation(&game->player2->animations[11], (384 * 1), 224 * 13, 3, 384, 224, frame_duration); // Toma dano baixo.

    // por que n funciona em personagem 2. (AGORA FUNCIONA)
    init_animation(&game->player2->animations[12], (384 * 1), 224 * 11, 1, 384, 224, frame_duration); // defesa baixo.
    init_animation(&game->player2->animations[13], (384 * 1), 224 * 12, 1, 384, 224, frame_duration); // defesa cima.

    // ATAQUE CIMA.
    init_animation(&game->player2->animations[14], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);  // soco cima.
    init_animation(&game->player2->animations[15], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);  // chute cima.
    
    init_animation(&game->player2->animations[16], (384 * 0), 224 * 16, 13, 384, 224, frame_duration);  // movimento particular (movimento especial)


    game->player1->animacao_atual = 0; // Inicia com a animação idle
    game->player2->animacao_atual = 0; // Inicia com a animação idle
    ALLEGRO_BITMAP *bolinha = al_load_bitmap("./img/bolinha.png");

    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == local_timer)
        {
            // Atualiza as animações
            float delta_time = 1.0 / 30.0;

            // Atualiza as hitboxes
            invert_flag = invert_flag_control(game->player1, game->player2);
            update_hitbox(game->player1, invert_flag);
            update_hitbox(game->player2, !invert_flag);

            movimento_left_right(game->player1);
            movimento_left_right(game->player2);

            movimento_ataque_verifica(game->player1);
            movimento_ataque_verifica(game->player2);

            movimento_no_ar(game->player1);
            movimento_no_ar(game->player2);

            movimento_idle(game->player1);
            movimento_idle(game->player2);

            movimento_baixo_cima(game->player1);
            movimento_baixo_cima(game->player2);

            movimento_defesa(game->player1);
            movimento_defesa(game->player2);



            // movimentos de defesa para ambos os personagens.
            if (game->player1->control->fire  && !game->player1->no_ar)
            {
                
                game->player1->animacao_atual = 9; // defesa cima.
 
            // game->player1rintf("valor da defesa %d\n ",game->player1->control->defesa);
            }

                        // movimentos de defesa para ambos os personagens.
            if (game->player2->control->fire  && !game->player2->no_ar)
            {
                
                game->player2->animacao_atual = 9; // defesa cima.
 
            // game->player2rintf("valor da defesa %d\n ",game->player2->control->defesa);
            }

            // Verifica colisão entre hitboxes
            if (check_player_collision(game->player1, game->player2))
            {
                if (game->player1->ataque && !game->player2->control->defesa)
                {
                    game->player2->vida -= 5;
                    game->player2->animacao_atual = 10; 


                }
                else if (game->player2->ataque && !game->player1->control->defesa)
                {
                    game->player1->vida -= 5;
                    if(game->player1->vida < 0)
                    {
                        printf("querda\n");
                        movimento_queda(game->player1);
                    }
                    movimento_dano(game->player1);

                    //game->player1->animacao_atual = 10; 
                }
            }

            // Resolve colisão física entre os jogadores
            resolve_collision(game->player1, game->player2);
            // Exibe os personagens na tela com a animação correta
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // fundo
            al_draw_bitmap_region(bolinha, 0, 600 * 0/*mapa_aleat*/, 900, 600, 0, 0, 0);
            // al_draw_bitmap_region(bolinha, 0, 664 * 1, 900, 600, 0, 0, 0);
            draw_animation(game->player1, &game->player1->animations[game->player1->animacao_atual], invert_flag, game->player1->animacao_atual);
            draw_animation(game->player2, &game->player2->animations[game->player2->animacao_atual], !invert_flag, game->player2->animacao_atual);
            // Exibe vidas e quantidade de vitorias por raund.
            al_draw_bitmap_region(bolinha, 0, 1285, 304, 24, 98, 10, 0);
            al_draw_bitmap_region(bolinha, 0, 1285, 304, 24, 800 - 299, 10, 0);
            for (int i = 0; i < game->player1->vida; i++)
            {
                al_draw_bitmap_region(bolinha, 0, 1263, 3, 20, 100 + (i * 3), 12, 0);
            }
            for (int i = 0; i < game->player2->vida; i++)
            {
                al_draw_bitmap_region(bolinha, 0, 1263, 3, 20, 800 - (i * 3), 12, 0);
            }

            // vitórias
            for (int i = 0; i < game->player1->vitorias; i++)
            {
                al_draw_bitmap_region(bolinha, 0, 1205, 24, 24, 37 + (i * 30), 13, 0);
            }
            for (int i = 0; i < game->player2->vitorias; i++)
            {
                al_draw_bitmap_region(bolinha, 0, 1205, 24, 24, 840 - (i * 30), 13, 0);
            }


            // fatality
            // if (game->player2->vida <= 20)
            // {
            //     printf("vido do p2 menor ou igual a 20\n");
            //     al_draw_bitmap_region(bolinha, 33, 1203, 26, 24, 1, 13, 0);
            // }
            // if (game->player1->vida <= 20)
            // {
            //     printf("vido do p1 menor ou igual a 20\n");
            //     al_draw_bitmap_region(bolinha, 33, 1203, 26, 24, 900 - 26, 13, 0);
            // }



            //Desenha hitboxes para depuração
            // al_draw_bitmap_region(bolinha, 49, 1564, 464 - 49, 100, 250, 250, 0);
            al_draw_rectangle(game->player1->hitbox_superior.x, game->player1->hitbox_superior.y,
                              game->player1->hitbox_superior.x + game->player1->hitbox_superior.width,
                              game->player1->hitbox_superior.y + game->player1->hitbox_superior.height, al_map_rgb(0, 255, 0), 2);
            al_draw_rectangle(game->player1->hitbox_inferior.x, game->player1->hitbox_inferior.y,
                              game->player1->hitbox_inferior.x + game->player1->hitbox_inferior.width,
                              game->player1->hitbox_inferior.y + game->player1->hitbox_inferior.height, al_map_rgb(255, 0, 0), 2);
            al_draw_rectangle(game->player2->hitbox_superior.x, game->player2->hitbox_superior.y,
                              game->player2->hitbox_superior.x + game->player2->hitbox_superior.width,
                              game->player2->hitbox_superior.y + game->player2->hitbox_superior.height, al_map_rgb(0, 255, 0), 2);
            al_draw_rectangle(game->player2->hitbox_inferior.x, game->player2->hitbox_inferior.y,
                              game->player2->hitbox_inferior.x + game->player2->hitbox_inferior.width,
                              game->player2->hitbox_inferior.y + game->player2->hitbox_inferior.height, al_map_rgb(0, 0, 255), 2);

            // Verifica qul esta com vida menor ou igual a zero.
            // if (game->player1->vida < 0 && game->player2->vida < 0)
            // { // Exibe a imagem de empate
            //     al_rest(15);
            //     al_destroy_timer(local_timer); // Destrói o temporizador específico
            //     game->player1->vitorias += 1;
            //     game->player2->vitorias += 1;
            //     al_destroy_bitmap(bolinha);
            //     return; // Sai da função para voltar ao menu
            // }
            // al_draw_bitmap_region(bolinha, 49, 1564, 464 - 49, 100, 250, 250, 0);
            if (game->player1->vida < 0 || game->player2->vida < 0)
            {
                movimento_queda(game->player1);
                al_draw_bitmap_region(bolinha, 3, 1326, 200, 36, 350, 300, 0);
                al_flip_display(); // Atualiza a tela para que a imagem seja desenhada
                movimento_queda(game->player1);
                //al_rest(5);        // Espera por 5 segundos
                printf("Um dos dois esta com a vida baixa\n");

                al_destroy_timer(local_timer); // Destroi o temporizador específico

                if (game->player1->vida < 0)
                    game->player2->vitorias += 1;
                else if (game->player2->vida < 0)
                    game->player1->vitorias += 1;

                al_destroy_bitmap(bolinha);
                return; // Sai da função para voltar ao menu
            }
            update_animation(&game->player1->animations[game->player1->animacao_atual], delta_time);
            update_animation(&game->player2->animations[game->player2->animacao_atual], delta_time);
            al_flip_display();
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
        {

            if (event.keyboard.keycode == ALLEGRO_KEY_K)
            {
                movimento_especial(game->player1);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4)
            {
                movimento_especial(game->player2);
            }


            if (event.keyboard.keycode == ALLEGRO_KEY_H)
            {
                joystick_fire(game->player1->control);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_5){joystick_fire(game->player2->control);}

            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 0)
            {
                // al_draw_bitmap_region(bolinha, 1, 1370, 200, 25, 400, 300, 0);
                // al_flip_display(); // Atualiza a tela para que a imagem seja desenhada
                al_stop_timer(local_timer);
                pause = 1;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 1)
            {
                al_start_timer(local_timer);
                pause = 0;
            }

            // if (event.keyboard.keycode == ALLEGRO_KEY_H){joystick_fire(game->player1->control);}
            // if (event.keyboard.keycode == ALLEGRO_KEY_PAD_5){joystick_fire(game->player2->control);}

            if (event.keyboard.keycode == ALLEGRO_KEY_O)
            {
                joystick_defesa(game->player1->control);
                printf("defesa p2 press\n");
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_6)
            {
                joystick_defesa(game->player2->control);
                printf("defesa p2 press\n");
            }


            // Movimenta Player 1 padrão(W, A, S, D) ataque(u = soco, i=chute)
            if (event.keyboard.keycode == ALLEGRO_KEY_A)
            {
                joystick_left(game->player1->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_D)
            {
                joystick_right(game->player1->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_W)
            {
                joystick_up(game->player1->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_S)
            {
                joystick_down(game->player1->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_U)
            {
                soco(game->player1);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_I)
            {
                chute(game->player1);
            }

            // Movimenta Player 2 (Setas direcionais)
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                joystick_left(game->player2->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                joystick_right(game->player2->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                joystick_up(game->player2->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                joystick_down(game->player2->control);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1)
            {
                soco(game->player2);
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2)
            {
                chute(game->player2);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_timer(local_timer); // Destroi o temporizador específico
            exit(0);
        }
    }
}


void draw_animation(Player *player, Animation *anim, int flag, int fatality)
{
    int frame_x = anim->x + (anim->current_frame * anim->frame_width);

    if (fatality == 9)
    {
        al_draw_scaled_bitmap(player->img_sprite, frame_x, anim->y, anim->frame_width - CORTE, anim->frame_height,
                player->x + (- 800 * player->id), player->y, PLAYER_WIDTH + 800, PLAYER_HEIGHT, flag ? ALLEGRO_FLIP_HORIZONTAL : 0);
        //printf("fatality\n");
        // de aumentar o player->x o personagem vai para frente.
    }
    else
        al_draw_scaled_bitmap(player->img_sprite, frame_x, anim->y, anim->frame_width - CORTE, anim->frame_height,
                          player->x, player->y, PLAYER_WIDTH, PLAYER_HEIGHT, flag ? ALLEGRO_FLIP_HORIZONTAL : 0);
}


void init_animation(Animation *anim, int x, int y, int frame_count, int frame_width, int frame_height, float frame_duration)
{
    anim->x = x;
    anim->y = y;
    anim->frame_count = frame_count;
    anim->frame_width = frame_width;
    anim->frame_height = frame_height;
    anim->current_frame = 0;
    anim->frame_duration = frame_duration;
    anim->elapsed_time = 0;
}

void update_animation(Animation *anim, float delta_time)
{

    anim->elapsed_time += delta_time;
    if (anim->elapsed_time >= anim->frame_duration)
    {
        anim->elapsed_time = 0;
        anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
    }
}



Game *create_game()
{
    Game *game = (Game *)malloc(sizeof(Game));
    if (game)
    {
        game->player1 = create_player(0);
        game->player2 = create_player(1);
        game->player1->selecao = 0;
        game->player2->selecao = 1;
    }
    return game;
}

void destroy_game(Game *game)
{
    if (game)
    {
        destroy_player(game->player1);
        destroy_player(game->player2);
        free(game);
    }
}

Player *create_player(int id)
{
    Player *player = (Player *)malloc(sizeof(Player));
    if (player)
    {
        player->img_sprite = NULL;
        player->selecao = -1;
        player->x = 0;
        player->y = 0;
        player->ataque = 0;
        player->no_ar = 0;
        player->velocidade_y = 0;
        player->vitorias = 0;
        player->id = id;
        // player->facing_left = 0;
        player->control = joystick_create();                  // Inicializa o joystick
        player->face_sprite = al_load_bitmap("img/face.png"); // Carrega o sprite da face

        // Inicializa as animações
        player->animations = (Animation *)malloc(20 * sizeof(Animation));
        if (!player->animations)
        {
            fprintf(stderr, "Erro ao alocar memória para as animações.\n");
            free(player);
            return NULL;
        }

        // Inicializa as hitboxes
        player->hitbox_superior.x = 0;
        player->hitbox_superior.y = 0;
        player->hitbox_superior.width = 0;
        player->hitbox_superior.height = 0;
        player->hitbox_inferior.x = 0;
        player->hitbox_inferior.y = 0;
        player->hitbox_inferior.width = 0;
        player->hitbox_inferior.height = 0;

        player->vida = 20;
    }
    return player;
}

void destroy_player(Player *player)
{
    if (player)
    {
        if (player->img_sprite)
        {
            al_destroy_bitmap(player->img_sprite);
        }
        if (player->face_sprite)
        {
            al_destroy_bitmap(player->face_sprite);
        }
        joystick_destroy(player->control); // Destroi o joystick
        free(player->animations);          // Libera a memória das animações
        free(player);
    }
}

void carrega_sprite(Player *player, int selecao)
{
    // Destrói o sprite atual se existir
    if (player->img_sprite)
    {
        al_destroy_bitmap(player->img_sprite);
    }

    // Carrega o sprite com base na seleção do quadrado
    switch (selecao)
    {
    case 0:
        player->img_sprite = al_load_bitmap("img/ken.png");
        break;
    case 1:
        player->img_sprite = al_load_bitmap("img/ken.png");
        break;
    case 2:
        player->img_sprite = al_load_bitmap("img/ken.png");
        break;
    case 3:
        player->img_sprite = al_load_bitmap("img/ken.png");
        break;
    default:
        player->img_sprite = al_load_bitmap("img/default_sprite.png");
        break;
    }

    al_convert_mask_to_alpha(player->img_sprite, al_map_rgb(248, 0, 248));
    if (!player->img_sprite)
    {
        fprintf(stderr, "Falha ao carregar o sprite para a seleção %d.\n", selecao);
    }
}

/* MINHAS CRIAÇÕES. */
void inicia_rounds(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
    int rounds_num = 0;
    int mapa = (rand() % 2);
    while (game->player1->vitorias < 2 && game->player2->vitorias < 2 && rounds_num < 3)
    {
        tela_controle(game, queue, timer, mapa);
        rounds_num += 1;
        game->player1->vida = 20;
        game->player2->vida = 20;
        // Declara o vencedor
        if (game->player1->vitorias == 2 && game->player2->vitorias == 2)
            printf("***************************\nEMPATE NESTA LUTA\n***************************\n");
        else if (game->player1->vitorias == 2)
        {
            printf("Player 1 venceu o luta !\n");
            show_fading_image(game->player1->face_sprite, queue, 0, 0, 300, 400);
        }
        else if (game->player2->vitorias == 2)
        {
            printf("Player 2 venceu o luta!\n");
        }
        else
        {
            if (game->player1->vitorias == 1)
            {
                printf("Player 1 venceu a raund!\n");
            }
            else if (game->player2->vitorias == 1)
            {
                printf("Player 2 venceu a raund!\n");
            }
            else
                printf("Caso impossivel !\n");
        }
    }

    game->player1->vitorias = 0;
    game->player2->vitorias = 0;
}


    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration);              // idle. ok
    init_animation(&game->player1->animations[1], 0, 224, 10, 384, 224, frame_duration);            // moviemnto esquerda. ok
    init_animation(&game->player1->animations[2], 0, 0, 10, 384, 224, frame_duration);              // moviemnto direita. ok
    init_animation(&game->player1->animations[3], 0, 448, 9, 384, 224, frame_duration);             // pulo. ok
    init_animation(&game->player1->animations[4],  (384 * 4), 672, 1, 384, 224, frame_duration);    // abaixado. ok

    init_animation(&game->player1->animations[5], 0, 224 * 4, 5, 384, 224, frame_duration);         // soco normal. ok
    init_animation(&game->player1->animations[6], (384 * 1), 224 * 6, 1, 384, 224, frame_duration); // soco cima. ok
    init_animation(&game->player1->animations[7], 0, 224 * 5, 6, 384, 224, frame_duration);         // soco baixo. ok

    init_animation(&game->player1->animations[8], 0, 224 * 7, 10, 384, 224, frame_duration);         // chute normal. ok
    init_animation(&game->player1->animations[9], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);  // chute cima. ok
    init_animation(&game->player1->animations[10],  0, 224 * 8, 6, 384, 224, frame_duration);        // chute baixo. ok

    init_animation(&game->player1->animations[11],(384 * 3), 224 * 10, 1, 384, 224, frame_duration); // defende normal. ok
    init_animation(&game->player1->animations[12],(384 * 1), 224 * 12, 1, 384, 224, frame_duration); // defende cima. ok
    init_animation(&game->player1->animations[13],(384 * 1), 224 * 11, 1, 384, 224, frame_duration); // defende baixo. ok

    init_animation(&game->player1->animations[14], (384 * 0), 224 * 14, 8, 384, 224, frame_duration);// toma dano normal/baixo. ok
    init_animation(&game->player1->animations[15], (384 * 0), 224 * 13, 6, 384, 224, frame_duration);// toma dano biaxo. ok

    init_animation(&game->player1->animations[16],(384 * 0), 224 * 16, 12, 384, 224, frame_duration); // movimento especial. ok
    init_animation(&game->player1->animations[17],(384 * 6), 224 * 15, 1, 900, 224, frame_duration); // movimento fatality. ok

    init_animation(&game->player1->animations[17], (384 * 0), 224 * 17, 12, 384, 224, frame_duration*2); // Queda. ok