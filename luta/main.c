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
#include "Game.h"

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


/* Usado na função show_fading_image. */
#define INCREMENT_VALUE 0.01f
#define MAX_DELAY_COUNT 20

/*Função que exibe uma imagem com um efeito de fade in e fade out. A imagem é desenhada em uma posição específica na tela, 
começando totalmente transparente e gradualmente se tornando opaca. Após alcançar opacidade total, a imagem permanece 
por um curto período antes de começar a desaparecer. A função também pode opcionalmente exibir um texto indicando que o 
jogador ganhou a luta. A função lida com eventos de temporizador para controlar o efeito de fade e de fila de eventos para a sincronização.*/
void show_fading_image(ALLEGRO_BITMAP *image, ALLEGRO_EVENT_QUEUE *event_queue, int src_x, int src_y, int src_w, int src_h, int x, int y)
{
    float alpha = 0.0f;
    int delayCount = 0;
    ALLEGRO_FONT *font1 = al_load_ttf_font("./fonte/2fonte.ttf", 50, 0);

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
            al_draw_tinted_bitmap_region(image, al_map_rgba_f(alpha, alpha, alpha, alpha), src_x, src_y, src_w, src_h, x, y, 0);
            al_flip_display();
        }
    }
    al_destroy_font(font1);
}


/*
Função que exibe um texto piscante na tela. A função desenha o texto em uma posição específica, 
alternando entre o texto visível e invisível para criar um efeito de piscar. 
O número de piscadas e a duração de cada piscar podem ser ajustados. 
A função também lida com eventos de fechamento da janela, encerrando o programa se necessário.*/
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
        al_rest(0.1);               // Ajuste a duração do piscar conforme necessário
        while (al_get_next_event(queue, &event))
        {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                exit(0);
            
        }
    }
}



/*Função principal do jogo que inicializa a biblioteca Allegro e seus addons, configura o timer, fila de eventos e display.
Define o título da janela, carrega fontes e imagens, e exibe um menu interativo com três opções:
Multiplayer, Singleplayer e Sair/Fechar. A seleção do menu é feita através das setas do teclado e a escolha é confirmada com a tecla Enter.
Ao selecionar uma opção, é exibido um efeito de piscar do texto da opção selecionada, seguido pela execução da ação correspondente.
A função lida com eventos de teclado e de fechamento da janela, garantindo a limpeza e destruição dos recursos alocados antes de sair.*/
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
        printf("Falha ao carregar a fonte.\n");
        return -1;
    }

    ALLEGRO_BITMAP *background = al_load_bitmap("img/1img.png");
    if (!background)
    {
        printf("Falha ao carregar a imagem de fundo.\n");
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
    //show_fading_image(background, queue, 900, 0, 900, 600, 0, 0);
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
