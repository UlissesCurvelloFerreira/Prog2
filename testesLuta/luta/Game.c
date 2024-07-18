#include "Game.h"


// Função para header game.
void escolher_mapas(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
    
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    ALLEGRO_BITMAP *bolinha = al_load_bitmap("./img/bolinha.png");
    int player1_selected = 0;

    
    const int square_size = 30;
    const int spacing = 30;
    const int num_squares = 3;
    const int total_width = num_squares * square_size + (num_squares - 1) * spacing;
    const int start_x = (SCREEN_WIDTH - total_width) / 2;
    const int start_y = 450;
    ALLEGRO_FONT *font1 = al_load_ttf_font("./fonte/2fonte.ttf", 24, 0);
    ALLEGRO_FONT *font2 = al_load_ttf_font("./fonte/2fonte.ttf", 18, 0);

    while (1)
    {
        al_wait_for_event(queue, &event);

        
        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_draw_bitmap_region(bolinha, 900, 0, 900, 600, 0, 0, 0);

        al_draw_text(font1, al_map_rgb(255, 119, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "SELECIONE O MAPA");
        al_draw_text(font2, al_map_rgb(255, 119, 0), SCREEN_WIDTH / 2 , SCREEN_HEIGHT - 30, ALLEGRO_ALIGN_CENTER,  "ESC VOLTA PARA O MENU");
        
        
        al_draw_scaled_bitmap(bolinha, 0, 0 + (600 * game->mapa_selecionado), 900, 600, 225, 100, 450, 300, 0);


        // Desenha os quadrados
        for (int i = 0; i < num_squares; i++)
        {
            ALLEGRO_COLOR color = al_map_rgb(90, 90, 90); // Cor padrão (branca)
            if (i == game->mapa_selecionado)
            {
                color = al_map_rgb(255, 128, 0); // Cor de seleção (vermelha)
            }

            int x = start_x + i * (square_size + spacing);
            al_draw_filled_rectangle(x, start_y, x + square_size, start_y + square_size, color);
        }
        if(game->mapa_selecionado == 2)
            al_draw_bitmap_region(bolinha, 904, 785, 40, 40, 320, 445, 1);
        if(game->mapa_selecionado == 0)
            al_draw_bitmap_region(bolinha, 904, 785, 40, 40, 540, 445, 0);

            
        al_flip_display();
    
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (!player1_selected)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                { 
                    if( game->mapa_selecionado >= 1)game->mapa_selecionado -= 1;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    if (game->mapa_selecionado < 2)game->mapa_selecionado += 1;
                }

                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    printf("Player 2 selecionou o quadrado %d\n", game->player2->selecao);
                    player1_selected = 1;
                    carrega_sprite(game->player2, game->player2->selecao);
                }
                printf("valor de game->mapa_selecionado %d\n", game->mapa_selecionado);
            }

            // quero voltar par o menu:
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                al_destroy_bitmap(bolinha);
                return;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_bitmap(bolinha);
            exit(0);
        }
        // mapa selecionado atraves de game. mapa.
        if (player1_selected)
        {
            printf("Os dois jogadores selecionaram seus quadrados. Saindo do modo multiplayer...\n");
            break;
        }
    }
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_bitmap(bolinha);
    inicia_rounds(game, queue);
}

// Função para header game.
void run_multiplayer(Game *game, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_TIMER *timer)
{
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    int player1_selected = 0;
    int player2_selected = 0;
    ALLEGRO_FONT *font1 = al_load_ttf_font("./fonte/2fonte.ttf", 24, 0);
    ALLEGRO_FONT *font2 = al_load_ttf_font("./fonte/2fonte.ttf", 18, 0);

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
                                     al_map_rgb(0, 255, 0));
            al_draw_filled_rectangle(start_x + player2_x * (SQUARE_SIZE + SQUARE_PADDING), start_y + player2_y * (SQUARE_SIZE + SQUARE_PADDING),
                                     start_x + player2_x * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE, start_y + player2_y * (SQUARE_SIZE + SQUARE_PADDING) + SQUARE_SIZE,
                                     al_map_rgb(0, 0, 255));
                                     

            
            al_draw_bitmap_region(game->player1->face_sprite, 306, 2, 900, 600, 0, 0, 0);

            al_draw_text(font1, al_map_rgb(255, 128, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "SELECIONE OS HERÓIS");

            al_draw_text(font1, al_map_rgb(0, 255, 0), (SCREEN_WIDTH / 2) - 270 , 70, ALLEGRO_ALIGN_CENTER, "Player 1");
            al_draw_text(font1, al_map_rgb(0, 0, 255), (SCREEN_WIDTH / 2) + 270, 70, ALLEGRO_ALIGN_CENTER, "Player 2");

            al_draw_text(font2, al_map_rgb(255, 128, 0), SCREEN_WIDTH / 2 , SCREEN_HEIGHT - 30, ALLEGRO_ALIGN_CENTER,  "ESC VOLTA PARA O MENU");

            al_draw_bitmap_region(game->player1->face_sprite, 0, (game->player1->selecao) * 400, 300, 400, 20, 100, 0);
            al_draw_bitmap_region(game->player2->face_sprite, 0, (game->player2->selecao) * 400, 300, 400, SCREEN_WIDTH - (300 + 20), 100, 1);
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

            // quero voltar par o menu:
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                al_destroy_font(font1);
                al_destroy_font(font2);
                return;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font1);
            al_destroy_font(font2);
            exit(0);
        }

        if (player1_selected && player2_selected)
        {
            printf("Os dois jogadores selecionaram seus quadrados. Saindo do modo multiplayer...\n");
            break;
        }
    }
    escolher_mapas(game, queue, timer);
    // tela_controle(game, queue, timer);
}

void tela_controle(Game *game, ALLEGRO_EVENT_QUEUE *queue)
{
    ALLEGRO_EVENT event;
    ALLEGRO_FONT *font1 = al_load_ttf_font("./fonte/2fonte.ttf", 24, 0);
    ALLEGRO_TIMER *local_timer = al_create_timer(1.0 / 30.0); // Temporizador específico para função atual.
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

    // Mobilidade geral personagem (1).
    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration);               // Idle.
    init_animation(&game->player1->animations[1], 0, 224, 10, 384, 224, frame_duration);             // Walk. esquerda
    //init_animation(&game->player1->animations[2], 0, 448, 9, 384, 224, frame_duration);            // walk direita
    init_animation(&game->player1->animations[3], 0, 448, 9, 384, 224, frame_duration);              // Jump.
    init_animation(&game->player1->animations[4], (384 * 4), 672, 1, 384, 224, frame_duration);      // Crouch.
    init_animation(&game->player1->animations[5], 0, 224 * 4, 5, 384, 224, frame_duration);         // Soco. normal  ok
    init_animation(&game->player1->animations[6], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);   // Soco. cima ok 
    init_animation(&game->player1->animations[7], 0, 224 * 5, 6, 384, 224, frame_duration);          // Soco baixo. ok
    init_animation(&game->player1->animations[8], 0, 224 * 7, 10, 384, 224, frame_duration);         // Chute.
    init_animation(&game->player1->animations[9], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);  // Chute cima.
    init_animation(&game->player1->animations[10], 0, 224 * 8, 6, 384, 224, frame_duration);          // Chute baixo.
    init_animation(&game->player1->animations[11], (384 * 3), 224 * 10, 1, 384, 224, frame_duration); // defesa.
    init_animation(&game->player1->animations[12], (384 * 1), 224 * 12, 1, 384, 224, frame_duration); // defesa cima.
    init_animation(&game->player1->animations[13], (384 * 1), 224 * 11, 1, 384, 224, frame_duration); // defesa baixo.
    init_animation(&game->player1->animations[14], (384 * 0), 224 * 14, 8, 384, 224, frame_duration); // Toma normal. ok
    init_animation(&game->player1->animations[15], (384 * 0), 224 * 13, 6, 384, 224, frame_duration); // Toma baixo. ok
    init_animation(&game->player1->animations[16], (384 * 0), 224 * 16, 12, 384, 224, frame_duration);  // movimento particular (movimento especial)
    init_animation(&game->player1->animations[17], (384 * 6), 224 * 15, 1, 900, 224, frame_duration);  // fatality.

    // MOVIMENTAÇÃO PARA PERSONAGEM/HERÓI (2)
    init_animation(&game->player2->animations[0], 0, 0, 10, 384, 224, frame_duration);               // Idle.
    init_animation(&game->player2->animations[1], 0, 224, 10, 384, 224, frame_duration);             // Walk. esquerda
    //init_animation(&game->player2->animations[2], 0, 448, 9, 384, 224, frame_duration);            // walk direita
    init_animation(&game->player2->animations[3], 0, 448, 9, 384, 224, frame_duration);              // Jump.
    init_animation(&game->player2->animations[4], (384 * 4), 672, 1, 384, 224, frame_duration);      // Crouch.
    init_animation(&game->player2->animations[5], 0, 224 * 4, 5, 384, 224, frame_duration);         // Soco. normal  ok
    init_animation(&game->player2->animations[6], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);   // Soco. cima ok 
    init_animation(&game->player2->animations[7], 0, 224 * 5, 6, 384, 224, frame_duration);          // Soco baixo. ok
    init_animation(&game->player2->animations[8], 0, 224 * 7, 10, 384, 224, frame_duration);         // Chute.
    init_animation(&game->player2->animations[9], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);  // Chute cima.
    init_animation(&game->player2->animations[10], 0, 224 * 8, 6, 384, 224, frame_duration);          // Chute baixo.
    init_animation(&game->player2->animations[11], (384 * 3), 224 * 10, 1, 384, 224, frame_duration); // defesa.
    init_animation(&game->player2->animations[12], (384 * 1), 224 * 12, 1, 384, 224, frame_duration); // defesa cima.
    init_animation(&game->player2->animations[13], (384 * 1), 224 * 11, 1, 384, 224, frame_duration); // defesa baixo.
    init_animation(&game->player2->animations[14], (384 * 0), 224 * 14, 8, 384, 224, frame_duration); // Toma normal. ok
    init_animation(&game->player2->animations[15], (384 * 0), 224 * 13, 6, 384, 224, frame_duration); // Toma baixo. ok
    init_animation(&game->player2->animations[16], (384 * 0), 224 * 16, 12, 384, 224, frame_duration);  // movimento particular (movimento especial)
    init_animation(&game->player2->animations[17], (384 * 6), 224 * 15, 1, 900, 224, frame_duration);  // fatality.

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

            movimento_no_ar(game->player1);
            movimento_no_ar(game->player2);

            movimento_left_right(game->player1);
            movimento_left_right(game->player2);

            movimento_ataque_verifica(game->player1);
            movimento_ataque_verifica(game->player2);

            movimento_idle(game->player1);
            movimento_idle(game->player2);

            movimento_baixo_cima(game->player1);
            movimento_baixo_cima(game->player2);

            movimento_defesa(game->player1);
            movimento_defesa(game->player2);


            // fatality
            if (game->player1->control->fire  && !game->player1->no_ar && game->player2->vida < 20){game->player1->animacao_atual = 17;}
            if (game->player2->control->fire  && !game->player2->no_ar && game->player1->vida < 20){game->player2->animacao_atual = 17;}

            // Verifica colisão entre hitboxes
            if (check_player_collision(game->player1, game->player2))
            {
                if (game->player1->ataque && !game->player2->control->defesa)
                {
                    game->player2->vida -= 5;
                    leva_dano(game->player2); 
                }
                else if (game->player2->ataque && !game->player1->control->defesa)
                {
                    game->player1->vida -= 5;
                    leva_dano(game->player1);
                }
            }


            // Resolve colisão física entre os jogadores
            resolve_collision(game->player1, game->player2);
            // Exibe os personagens na tela com a animação correta
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // fundo
            al_draw_bitmap_region(bolinha, 0, 600 * game->mapa_selecionado, 900, 600, 0, 0, 0);
            // al_draw_bitmap_region(bolinha, 0, 664 * 1, 900, 600, 0, 0, 0);
            
            // Exibe os personagens.
            draw_animation(game->player1, &game->player1->animations[game->player1->animacao_atual], invert_flag, game->player1->animacao_atual);
            draw_animation(game->player2, &game->player2->animations[game->player2->animacao_atual], !invert_flag, game->player2->animacao_atual);

            // Exibe vidas e quantidade de vitorias por raund.
            al_draw_bitmap_region(bolinha, 937, 723, 304, 24, 98, 10, 0);
            al_draw_bitmap_region(bolinha, 937, 723, 304, 24, 800 - 299, 10, 0);
            
            for (int i = 0; i < game->player1->vida; i++){al_draw_bitmap_region(bolinha, 937, 701, 3, 20, 100 + (i * 3), 12, 0);}
            for (int i = 0; i < game->player2->vida; i++){al_draw_bitmap_region(bolinha, 937, 701, 3, 20, 800 - (i * 3), 12, 0);}

            // vitórias // ok.
            for (int i = 0; i < game->player1->vitorias; i++){al_draw_bitmap_region(bolinha, 937, 639, 26, 28, 37 + (i * 30), 13, 0);}
            for (int i = 0; i < game->player2->vitorias; i++){al_draw_bitmap_region(bolinha, 937, 639, 26, 28, 840 - (i * 30), 13, 0);}

            //fatality // ok
            if (game->player2->vida <= 20){al_draw_bitmap_region(bolinha, 963, 639, 25, 28, 1, 13, 0);}
            if (game->player1->vida <= 20){al_draw_bitmap_region(bolinha, 963, 639, 25, 28, 900 - 25, 13, 0);}

            // Verifica qul esta com vida menor ou igual a zero.
            if (game->player1->vida < 0 && game->player2->vida < 0)
            { // Exibe a imagem de empate
                al_draw_text(font1, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, ALLEGRO_ALIGN_CENTER, "FIM DO RAUND");
                al_flip_display(); // Atualiza a tela para que a imagem seja desenhada
                al_rest(2);
                al_destroy_font(font1);
                al_destroy_timer(local_timer); // Destrói o temporizador específico
                game->player1->vitorias += 1;
                game->player2->vitorias += 1;
                al_destroy_bitmap(bolinha);
                return; // Sai da função para voltar ao menu
            }
            if (game->player1->vida < 0 || game->player2->vida < 0)
            {
   
                al_draw_text(font1, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, ALLEGRO_ALIGN_CENTER, "FIM DO RAUND");
                al_flip_display(); // Atualiza a tela para que a imagem seja desenhada
                
                al_rest(2);        // Espera por 5 segundos
                printf("Um dos dois esta com a vida baixa\n");
                al_destroy_font(font1);
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
            if (event.keyboard.keycode == ALLEGRO_KEY_K){movimento_especial(game->player1);}
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4){movimento_especial(game->player2);}
            if (event.keyboard.keycode == ALLEGRO_KEY_H){joystick_fire(game->player1->control);}
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_5){joystick_fire(game->player2->control);}

            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 0)
            {
                al_draw_text(font1, al_map_rgb(255, 119, 0), SCREEN_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, "PAUSA");
                al_flip_display(); // Atualiza a tela para que a imagem seja desenhada
                al_stop_timer(local_timer);
                pause = 1;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 1)
            {
                al_start_timer(local_timer);
                pause = 0;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_O){joystick_defesa(game->player1->control);printf("defesa p2 press\n");}
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_6){joystick_defesa(game->player2->control);printf("defesa p2 press\n");}

            // Movimenta Player 1 padrão(W, A, S, D) ataque(u = soco, i=chute)
            if (event.keyboard.keycode == ALLEGRO_KEY_A)joystick_left(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_D)joystick_right(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_W)joystick_up(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_S)joystick_down(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_U)soco(game->player1);
            else if (event.keyboard.keycode == ALLEGRO_KEY_I)chute(game->player1);

            // Movimenta Player 2 (Setas direcionais)
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)joystick_left(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)joystick_right(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP)joystick_up(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)joystick_down(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1)soco(game->player2);
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2)chute(game->player2);
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font1);
            al_destroy_timer(local_timer); // Destroi o temporizador específico
            exit(0);
        }
    }
}

// Funções para a header game.
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
    game->mapa_selecionado = 0;
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

void exibe_ganhador(Game *game, ALLEGRO_EVENT_QUEUE *queue)
{
    
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *bolinha = al_load_bitmap("./img/bolinha.png");
    ALLEGRO_FONT *font1 = al_load_ttf_font("./fonte/2fonte.ttf", 24, 0);
    
    al_wait_for_event(queue, &event);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap_region(bolinha, 900, 0, 900, 600, 0, 0, 0);

    
    if (game->player1->vitorias == 2 && game->player2->vitorias == 2){
        al_draw_text(font1, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "EMPATE");
        al_draw_bitmap_region(game->player1->face_sprite, 0, 400 * game->player1->selecao, 300, 400, 100, 100, 0);
        al_draw_bitmap_region(game->player2->face_sprite, 0, 400 * game->player2->selecao, 300, 400, 500, 100, 0);
        printf("***\nEMPATE NESTA LUTA\n***\n");
    }
    else if (game->player1->vitorias == 2){
        al_draw_text(font1, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "GANHADOR");
        al_draw_bitmap_region(game->player1->face_sprite, 0, 400 * game->player1->selecao, 300, 400, 300, 100, 0);
        printf("Player 1 venceu o luta!\n");
    }
    else if (game->player2->vitorias == 2){
        al_draw_text(font1, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "GANHADOR");
        al_draw_bitmap_region(game->player2->face_sprite, 0, 400 * game->player2->selecao, 300, 400, 300, 100, 0);
        printf("Player 2 venceu o luta!\n");
    }
    al_flip_display();
    al_rest(2);

    // se o "x" do display foi pressionado, assim fecha o display e sai do jogo.
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        exit(0);
    
    // libera memoria.
    al_destroy_bitmap(bolinha);
    al_destroy_font(font1);
}

void inicia_rounds(Game *game, ALLEGRO_EVENT_QUEUE *queue)
{
    int rounds_num = 0;
    while (game->player1->vitorias < 2 && game->player2->vitorias < 2 && rounds_num < 3)
    {
        tela_controle(game, queue);
        rounds_num += 1;
    }  
    exibe_ganhador(game, queue);
    game->player1->vitorias = 0;
    game->player2->vitorias = 0;
}


