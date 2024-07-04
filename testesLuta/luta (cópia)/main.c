#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "Joystick.h"


/*        move  ||    soco  || chute ||
player 1: wads, u, i
player 2; setas direito crtl, direito shift


make;./a

Personagem 2: não sabe chutar abaixado
*/

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_WIDTH 684
#define PLAYER_HEIGHT 524
#define SQUARE_SIZE 70
#define SQUARE_PADDING 10
#define POSICAO_P1 0.05
#define POSICAO_P2 0.95
#define PERCENTUAL_INFERIOR 0.05
#define GRAVIDADE 6.0f
#define PULO_VELOCIDADE -60.0f
#define MAX_VELOCIDADE_CAIDA 100.0f

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
    int selecao;
    int x, y;              // Coordenadas do personagem
    int is_attacking;      // Indica se o personagem está atacando
    int no_ar;             // Indica se o personagem está no ar
    float velocidade_y;    // Velocidade vertical do personagem
    joystick *control;     // Joystick para controle
    Animation *animations; // Array de animações
    int current_animation; // Índice da animação atual
} Player;

typedef struct
{
    Player *player1;
    Player *player2;
} Game;

void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer);
Game *create_game();
void destroy_game(Game *game);
Player *create_player();
void destroy_player(Player *player);
void carrega_sprite(Player *player, int selecao);
void exibe_personagem(Player *player, int x, int y, int flag);
void exibe_face(Player *player, int x, int y, int face_index, int flag);
void flash_text(ALLEGRO_FONT *font, const char *text, int x, int y, int flashes, ALLEGRO_COLOR color, ALLEGRO_COLOR bg_color, ALLEGRO_EVENT_QUEUE *queue);

/* Para animar os personagens. */
void init_animation(Animation *anim, int x, int y, int frame_count, int frame_width, int frame_height, float frame_duration);
void update_animation(Animation *anim, float delta_time);
void draw_animation(Player *player, Animation *anim, int flag);

int main()
{
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

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
            al_clear_to_color(al_map_rgb(50, 50, 50));

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

            al_draw_scaled_bitmap(game->player1->face_sprite, 0, (game->player1->selecao) * 200, 150, 200, 10, 100, 300, 400, 0);
            al_draw_scaled_bitmap(game->player2->face_sprite, 0, (game->player2->selecao) * 200, 150, 200, SCREEN_WIDTH - (150 * 2) - 10, 100, 300, 400, 0);

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
    tela_controle(game, queue, timer);
}

void soco(Player *p)
{

    if (!p->is_attacking)
    {
        p->is_attacking = 1;
        if (p->control->down)
        {
            p->current_animation = 6; // Soco baixo
        }
        else
        {
            p->current_animation = 4; // Soco
        }
        p->animations[p->current_animation].current_frame = 0;
    }
}

void chute(Player *p)
{
    if (!p->is_attacking)
    {
        p->is_attacking = 1;
        if (p->control->down)
        {
            p->current_animation = 7; // Chute baixo
        }
        else
        {
            p->current_animation = 5; // Chute
        }
        p->animations[p->current_animation].current_frame = 0;
    }
}

void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer) {
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    // Inicializa as posições dos personagens
    game->player1->x = 0;
    game->player1->y = SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT);
    game->player2->x = SCREEN_WIDTH - PLAYER_WIDTH;
    game->player2->y = SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT);

    const float frame_duration = 0.033333333; // Duração de cada frame em segundos

    // Inicializa as animações
    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration);          // Idle
    init_animation(&game->player1->animations[1], 0, 224, 10, 384, 224, frame_duration);        // Walk
    init_animation(&game->player1->animations[2], 0, 448, 9, 384, 224, frame_duration);         // Jump
    init_animation(&game->player1->animations[3], (384 * 4), 672, 1, 384, 224, frame_duration); // Crouch
    init_animation(&game->player1->animations[4], 0, 224 * 4, 5, 384, 224, frame_duration);     // soco
    init_animation(&game->player1->animations[5], 0, 224 * 7, 10, 384, 224, frame_duration);    // chute
    init_animation(&game->player1->animations[6], 0, 224 * 5, 6, 384, 224, frame_duration);     // soco baixo
    init_animation(&game->player1->animations[7], 0, 224 * 8, 6, 384, 224, frame_duration);     // chute baixo

    init_animation(&game->player2->animations[0], 0, 0, 10, 384, 224, frame_duration);          // Idle
    init_animation(&game->player2->animations[1], 0, 224, 10, 384, 224, frame_duration);        // Walk
    init_animation(&game->player2->animations[2], 0, 448, 9, 384, 224, frame_duration);         // Jump
    init_animation(&game->player2->animations[3], (384 * 3), 672, 1, 384, 224, frame_duration); // Crouch
    init_animation(&game->player2->animations[4], 0, 224 * 4, 5, 384, 224, frame_duration);     // soco
    init_animation(&game->player2->animations[5], 0, 224 * 7, 10, 384, 224, frame_duration);    // chute
    init_animation(&game->player2->animations[6], 0, 224 * 5, 6, 384, 224, frame_duration);     // soco baixo
    init_animation(&game->player2->animations[7], 0, 224 * 8, 6, 384, 224, frame_duration);     // chute baixo

    game->player1->current_animation = 0; // Inicia com a animação idle
    game->player2->current_animation = 0; // Inicia com a animação idle

    while (1) {
        while (!al_is_event_queue_empty(queue)) {
            al_wait_for_event(queue, &event);

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                // Movimenta Player 1 padrão(W, A, S, D) ataque(u = soco, i=chute)
                if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                    joystick_left(game->player1->control);
                    game->player1->current_animation = 1; // Walk
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                    joystick_right(game->player1->control);
                    game->player1->current_animation = 1; // Walk
                } else if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                    joystick_up(game->player1->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                    joystick_down(game->player1->control);
                    game->player1->current_animation = 3; // Crouch
                } else if (event.keyboard.keycode == ALLEGRO_KEY_U) {
                    soco(game->player1);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_I) {
                    chute(game->player1);
                }

                // Movimenta Player 2 (Setas direcionais)
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                    joystick_left(game->player2->control);
                    game->player2->current_animation = 1; // Walk
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    joystick_right(game->player2->control);
                    game->player2->current_animation = 1; // Walk
                } else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    joystick_up(game->player2->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    joystick_down(game->player2->control);
                    game->player2->current_animation = 3; // Crouch
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RCTRL) { // Soco
                    soco(game->player2);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) { // Chute
                    chute(game->player2);
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    return; // Sai da função para voltar ao menu
                }
            } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                // Libera Player 1 (W, A, S, D)
                if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                    joystick_left(game->player1->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                    joystick_right(game->player1->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_W) {
                    joystick_up(game->player1->control);
                    if (game->player1->no_ar) {
                        game->player1->velocidade_y = 0;
                    }
                } else if (event.keyboard.keycode == ALLEGRO_KEY_S) {
                    joystick_down(game->player1->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_U) {
                    game->player1->is_attacking = 0;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_I) {
                    game->player1->is_attacking = 0;
                }

                // Libera Player 2 (Setas direcionais)
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                    joystick_left(game->player2->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    joystick_right(game->player2->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    joystick_up(game->player2->control);
                    if (game->player2->no_ar) {
                        game->player2->velocidade_y = 0;
                    }
                } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    joystick_down(game->player2->control);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RCTRL) {
                    game->player2->is_attacking = 0;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RSHIFT) {
                    game->player2->is_attacking = 0;
                }
            } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                exit(0);
            } else if (event.type == ALLEGRO_EVENT_TIMER) {
                // Atualiza as animações
                float delta_time = 1.0 / 30.0;
                update_animation(&game->player1->animations[game->player1->current_animation], delta_time);
                update_animation(&game->player2->animations[game->player2->current_animation], delta_time);

                // Verifica se o ataque terminou
                if (game->player1->is_attacking) {
                    Animation *anim = &game->player1->animations[game->player1->current_animation];
                    if (anim->current_frame == anim->frame_count - 1) {
                        game->player1->is_attacking = 0;
                        if (game->player1->control->down) {
                            game->player1->current_animation = 3; // Crouch
                        } else {
                            game->player1->current_animation = 0; // Idle
                        }
                    }
                }

                if (game->player2->is_attacking) {
                    Animation *anim = &game->player2->animations[game->player2->current_animation];
                    if (anim->current_frame == anim->frame_count - 1) {
                        game->player2->is_attacking = 0;
                        if (game->player2->control->down) {
                            game->player2->current_animation = 3; // Crouch
                        } else {
                            game->player2->current_animation = 0; // Idle
                        }
                    }
                }

                // Aplica a movimentação dos jogadores
                if (game->player1->control->left) {
                    game->player1->x -= 10;
                    if (!game->player1->is_attacking && !game->player1->no_ar)
                        game->player1->current_animation = 1; // Walk
                }
                if (game->player1->control->right) {
                    game->player1->x += 10;
                    if (!game->player1->is_attacking && !game->player1->no_ar)
                        game->player1->current_animation = 1; // Walk
                }

                // Atualiza a física do pulo do Player 1
                if (game->player1->no_ar) {
                    game->player1->y += game->player1->velocidade_y;
                    game->player1->velocidade_y += GRAVIDADE;

                    if (game->player1->y >= SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT)) {
                        game->player1->y = SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT);
                        game->player1->no_ar = 0;
                        game->player1->velocidade_y = 0;
                        if (!game->player1->is_attacking) {
                            if (game->player1->control->down) {
                                game->player1->current_animation = 3; // Crouch
                            } else {
                                game->player1->current_animation = 0; // Idle
                            }
                        }
                    }
                } else if (game->player1->control->up) {
                    game->player1->velocidade_y = PULO_VELOCIDADE;
                    game->player1->no_ar = 1;
                }

                if (game->player1->control->up) {
                    if (!game->player1->is_attacking && game->player1->no_ar)
                        game->player1->current_animation = 2; // Jump
                }
                if (game->player1->control->down) {
                    if (!game->player1->is_attacking && !game->player1->no_ar)
                        game->player1->current_animation = 3; // Crouch
                }
                if (!game->player1->control->left && !game->player1->control->right &&
                    !game->player1->control->up && !game->player1->control->down && !game->player1->is_attacking && !game->player1->no_ar) {
                    game->player1->current_animation = 0; // Idle
                }

                if (game->player2->control->left) {
                    game->player2->x -= 10;
                    if (!game->player2->is_attacking && !game->player2->no_ar)
                        game->player2->current_animation = 1; // Walk
                }
                if (game->player2->control->right) {
                    game->player2->x += 10;
                    if (!game->player2->is_attacking && !game->player2->no_ar)
                        game->player2->current_animation = 1; // Walk
                }

                // Atualiza a física do pulo do Player 2
                if (game->player2->no_ar) {
                    game->player2->y += game->player2->velocidade_y;
                    game->player2->velocidade_y += GRAVIDADE;

                    if (game->player2->y >= SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT)) {
                        game->player2->y = SCREEN_HEIGHT - PLAYER_HEIGHT - (PERCENTUAL_INFERIOR * SCREEN_HEIGHT);
                        game->player2->no_ar = 0;
                        game->player2->velocidade_y = 0;
                        if (!game->player2->is_attacking) {
                            if (game->player2->control->down) {
                                game->player2->current_animation = 3; // Crouch
                            } else {
                                game->player2->current_animation = 0; // Idle
                            }
                        }
                    }
                } else if (game->player2->control->up) {
                    game->player2->velocidade_y = PULO_VELOCIDADE;
                    game->player2->no_ar = 1;
                }

                if (game->player2->control->up) {
                    if (!game->player2->is_attacking && game->player2->no_ar)
                        game->player2->current_animation = 2; // Jump
                }
                if (game->player2->control->down) {
                    if (!game->player2->is_attacking && !game->player2->no_ar)
                        game->player2->current_animation = 3; // Crouch
                }
                if (!game->player2->control->left && !game->player2->control->right &&
                    !game->player2->control->up && !game->player2->control->down && !game->player2->is_attacking && !game->player2->no_ar) {
                    game->player2->current_animation = 0; // Idle
                }

                // Exibe os personagens na tela com a animação correta
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_animation(game->player1, &game->player1->animations[game->player1->current_animation], 0);
                draw_animation(game->player2, &game->player2->animations[game->player2->current_animation], 1);
                al_flip_display();
            }
        }
    }
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

void draw_animation(Player *player, Animation *anim, int flag)
{
    int frame_x = anim->x + anim->current_frame * anim->frame_width;
    al_draw_scaled_bitmap(player->img_sprite, frame_x, anim->y, anim->frame_width, anim->frame_height,
                          player->x, player->y, PLAYER_WIDTH, PLAYER_HEIGHT, flag);
}

Game *create_game()
{
    Game *game = (Game *)malloc(sizeof(Game));
    if (game)
    {
        game->player1 = create_player();
        game->player2 = create_player();
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

Player *create_player()
{
    Player *player = (Player *)malloc(sizeof(Player));
    if (player)
    {
        player->img_sprite = NULL;
        player->selecao = -1;
        player->x = 0;
        player->y = 0;
        player->is_attacking = 0;
        player->no_ar = 0;
        player->velocidade_y = 0;
        player->control = joystick_create();                  // Inicializa o joystick
        player->face_sprite = al_load_bitmap("img/face.png"); // Carrega o sprite da face

        // Inicializa as animações // 4 igual ao numero de animações atual pula baixa esq dir
        player->animations = (Animation *)malloc(8 * sizeof(Animation));
        if (!player->animations)
        {
            fprintf(stderr, "Erro ao alocar memória para as animações.\n");
            free(player);
            return NULL;
        }
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
        player->img_sprite = al_load_bitmap("img/testeMove.png");
        break;
    case 1:
        player->img_sprite = al_load_bitmap("img/testeMove.png");
        break;
    case 2:
        player->img_sprite = al_load_bitmap("img/testeMove.png");
        break;
    case 3:
        player->img_sprite = al_load_bitmap("img/testeMove.png");
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





