#include "Game.h"



/*Função para escolher o mapa. Exibe opções de mapas e permite ao jogador selecionar um.
A função aguarda eventos de teclado e atualiza a seleção do mapa conforme as teclas pressionadas.
Se a tecla ESC for pressionada, a função retorna ao menu principal.
Se a janela for fechada, o jogo é encerrado.*/
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

    while (!player1_selected)
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
            ALLEGRO_COLOR color = al_map_rgb(90, 90, 90); // Cor padrão (cinza)
            if (i == game->mapa_selecionado)
                color = al_map_rgb(255, 128, 0);          // Cor de seleção (laranja)
            int x = start_x + i * (square_size + spacing);
            al_draw_filled_rectangle(x, start_y, x + square_size, start_y + square_size, color);
        }
        if(game->mapa_selecionado == 2) al_draw_bitmap_region(bolinha, 904, 785, 40, 40, 320, 445, 1);
        if(game->mapa_selecionado == 0) al_draw_bitmap_region(bolinha, 904, 785, 40, 40, 540, 445, 0);
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
                    player1_selected = 1;
                }
                //printf("valor de game->mapa_selecionado %d\n", game->mapa_selecionado);
            }
            // quero voltar par o menu:
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                al_destroy_font(font1);
                al_destroy_font(font2);
                al_destroy_bitmap(bolinha);
                return;
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_bitmap(bolinha);
            exit(0);
        }
    }
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_bitmap(bolinha);
    inicia_rounds(game, queue);
}


/*
Função responsável por gerenciar o modo multiplayer do jogo. Exibe uma interface onde dois jogadores 
podem selecionar seus personagens utilizando as teclas de seta e confirmações. A função lida com eventos 
de teclado e de fechamento da janela. Após ambos os jogadores confirmarem suas seleções, a função 
transfere o controle para a escolha de mapas.

Cálculo de posição dos jogadores:
- As posições dos jogadores são determinadas pelo índice de seleção (selecao) de cada jogador.
- player1_x e player1_y são as coordenadas X e Y do jogador 1, calculadas com base na seleção (selecao % 2 e selecao / 2).
- player2_x e player2_y são as coordenadas X e Y do jogador 2, calculadas de forma semelhante.
- A interface gráfica desenha quadrados representando as seleções possíveis, e a posição dos jogadores 
  é destacada desenhando quadrados preenchidos nas cores verde (jogador 1) e azul (jogador 2).

Eventos de movimento dos jogadores:
- Jogador 1: Movimenta-se utilizando as teclas W (cima), A (esquerda), S (baixo) e D (direita).
- Jogador 2: Movimenta-se utilizando as teclas de seta (cima, esquerda, baixo e direita).
- A função impede que os jogadores se movam para uma posição já ocupada pelo outro jogador.

Eventos de seleção e retorno ao menu:
- Jogador 1 confirma sua seleção com a tecla P, e jogador 2 com a tecla Enter.
- Se qualquer jogador pressiona a tecla ESC, a função retorna ao menu principal.
*/
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
                if (event.keyboard.keycode == ALLEGRO_KEY_A) // Move para a esquerda
                {
                    if (game->player1->selecao % 2 > 0 && game->player1->selecao - 1 != game->player2->selecao)
                        game->player1->selecao--;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_D) // Move para a direita
                {
                    if (game->player1->selecao % 2 < 1 && game->player1->selecao + 1 != game->player2->selecao)
                        game->player1->selecao++;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_W) // Move para cima
                {
                    if (game->player1->selecao / 2 > 0 && game->player1->selecao - 2 != game->player2->selecao)
                        game->player1->selecao -= 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_S) // Move para baixo
                {
                    if (game->player1->selecao / 2 < 1 && game->player1->selecao + 2 != game->player2->selecao)
                        game->player1->selecao += 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_P) // Confirma.
                {
                    //printf("Player 1 selecionou o quadrado %d\n", game->player1->selecao);
                    player1_selected = 1;
                    carrega_sprite(game->player1, game->player1->selecao);
                }
            }

            if (!player2_selected)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) // Move para a esquerda.
                {
                    if (game->player2->selecao % 2 > 0 && game->player2->selecao - 1 != game->player1->selecao)
                        game->player2->selecao--;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) // Move para a direita.
                {
                    if (game->player2->selecao % 2 < 1 && game->player2->selecao + 1 != game->player1->selecao)
                        game->player2->selecao++;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_UP) // Move para a cima.
                {
                    if (game->player2->selecao / 2 > 0 && game->player2->selecao - 2 != game->player1->selecao)
                        game->player2->selecao -= 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) // Move para a baixo.
                {
                    if (game->player2->selecao / 2 < 1 && game->player2->selecao + 2 != game->player1->selecao)
                        game->player2->selecao += 2;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) // Confirma.
                {
                    //printf("Player 2 selecionou o quadrado %d\n", game->player2->selecao);
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
            //printf("Os dois jogadores selecionaram seus quadrados. Saindo do modo multiplayer...\n");
            break;
        }
    }
    escolher_mapas(game, queue, timer);
}


/*Função para carregar as animações dos personagens do jogo.
Cada animação é inicializada com suas coordenadas de origem na folha de sprites,
o número de quadros (frames), a largura e altura de cada quadro, e a duração de cada quadro.*/
void inicia_frames(Game *game)
{

    const float frame_duration = 0.05; // Duração de cada frame em segundos

    // Mobilidade geral personagem (1).
    init_animation(&game->player1->animations[0], 0, 0, 10, 384, 224, frame_duration);                  // Idle. Ok
    init_animation(&game->player1->animations[1], 0, 224, 11, 384, 224, frame_duration);                // Walk frente. OK
    init_animation(&game->player1->animations[2], 0, 224 * 17, 11, 384, 224, frame_duration);           // walk trás     
    init_animation(&game->player1->animations[3], 0, 448, 9, 384, 224, frame_duration);                 // Jump. ok
    init_animation(&game->player1->animations[4], (384 * 4), 672, 1, 384, 224, frame_duration);         // Crouch. ok
    init_animation(&game->player1->animations[5], 0, 224 * 4, 5, 384, 224, frame_duration);             // Soco normal. ok  
    init_animation(&game->player1->animations[6], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);     // Soco cima. ok 
    init_animation(&game->player1->animations[7], 0, 224 * 5, 6, 384, 224, frame_duration);             // Soco baixo. ok
    init_animation(&game->player1->animations[8], 0, 224 * 7, 10, 384, 224, frame_duration/1.5);        // Chute. ok
    init_animation(&game->player1->animations[9], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);     // Chute cima. ok
    init_animation(&game->player1->animations[10], 0, 224 * 8, 6, 384, 224, frame_duration);            // Chute baixo. ok
    init_animation(&game->player1->animations[11], (384 * 3), 224 * 10, 1, 384, 224, frame_duration);   // defesa. ok
    init_animation(&game->player1->animations[12], (384 * 1), 224 * 12, 1, 384, 224, frame_duration);   // defesa cima. ok
    init_animation(&game->player1->animations[13], (384 * 1), 224 * 11, 1, 384, 224, frame_duration);   // defesa baixo. ok
    init_animation(&game->player1->animations[14], (384 * 0), 224 * 14, 8, 384, 224, frame_duration);   // Toma normal. ok
    init_animation(&game->player1->animations[15], (384 * 0), 224 * 13, 6, 384, 224, frame_duration);   // Toma baixo. ok
    init_animation(&game->player1->animations[16], (384 * 0), 224 * 15, 12, 384, 224, frame_duration/2);// movimento particular (movimento especial). ok
    init_animation(&game->player1->animations[17], (384 * 0), 224 * 16, 5, 900, 224, frame_duration/2); // fatality.

    // MOVIMENTAÇÃO PARA PERSONAGEM/HERÓI (2)
    init_animation(&game->player2->animations[0], 0, 0, 10, 384, 224, frame_duration);                  // Idle. ok
    init_animation(&game->player2->animations[2], 0, 224, 11, 384, 224, frame_duration);                // Walk. frente. ok
    init_animation(&game->player2->animations[1], 0, 224 * 17, 11, 384, 224, frame_duration);           // walk trás. ok
    init_animation(&game->player2->animations[3], 0, 448, 9, 384, 224, frame_duration);                 // Jump. ok
    init_animation(&game->player2->animations[4], (384 * 4), 672, 1, 384, 224, frame_duration);         // Crouch. ok
    init_animation(&game->player2->animations[5], 0, 224 * 4, 5, 384, 224, frame_duration);             // Soco normal. ok 
    init_animation(&game->player2->animations[6], (384 * 1), 224 * 6, 1, 384, 224, frame_duration);     // Soco cima. ok
    init_animation(&game->player2->animations[7], 0, 224 * 5, 6, 384, 224, frame_duration);             // Soco baixo. ok
    init_animation(&game->player2->animations[8], 0, 224 * 7, 10, 384, 224, frame_duration/1.5);        // Chute. ok
    init_animation(&game->player2->animations[9], (384 * 1), 224 * 9, 7, 384, 224, frame_duration);     // Chute cima. ok
    init_animation(&game->player2->animations[10], 0, 224 * 8, 6, 384, 224, frame_duration);            // Chute baixo. ok
    init_animation(&game->player2->animations[11], (384 * 3), 224 * 10, 1, 384, 224, frame_duration);   // defesa. ok
    init_animation(&game->player2->animations[12], (384 * 1), 224 * 12, 1, 384, 224, frame_duration);   // defesa cima. ok
    init_animation(&game->player2->animations[13], (384 * 1), 224 * 11, 1, 384, 224, frame_duration);   // defesa baixo. ok
    init_animation(&game->player2->animations[14], (384 * 0), 224 * 14, 8, 384, 224, frame_duration);   // Toma normal. ok
    init_animation(&game->player2->animations[15], (384 * 0), 224 * 13, 6, 384, 224, frame_duration);   // Toma baixo. ok
    init_animation(&game->player2->animations[16], (384 * 0), 224 * 15, 12, 384, 224, frame_duration/2);// movimento particular (movimento especial). ok
    init_animation(&game->player2->animations[17], (384 * 0), 224 * 16, 5, 900, 224, frame_duration/2); // fatality. ok
}


void desenha_hit_box (Game *game)
{
    //Desenha hitboxes
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
    game->player1->animacao_atual = 0; // Inicia com a animação idle
    game->player2->animacao_atual = 0; // Inicia com a animação idle
    ALLEGRO_BITMAP *bolinha = al_load_bitmap("./img/bolinha.png");

    inicia_frames(game); // Funçaõ responsavel por iniciar todos os frames das jogadores.

    while (1)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == local_timer)
        {
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

            // Resolve colisão física entre os jogadores
            verifica_limites(game->player1, SCREEN_WIDTH);
            verifica_limites(game->player2, SCREEN_WIDTH);

            resolve_collision(game->player1, game->player2);

            movimento_final(game->player1, game->player2);

            diminuir_vida(game->player1, game->player2);


            // Exibe os personagens na tela com a animação correta
            // Bloco de exibiçaõ de informações.
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap_region(bolinha, 0, 600 * game->mapa_selecionado, 900, 600, 0, 0, 0); // fundo
            draw_animation(game->player1, &game->player1->animations[game->player1->animacao_atual], invert_flag, game->player1->animacao_atual);
            draw_animation(game->player2, &game->player2->animations[game->player2->animacao_atual], !invert_flag, game->player2->animacao_atual);

            // Barras de vida.
            al_draw_bitmap_region(bolinha, 937, 723, 304, 24, 98, 10, 0);
            al_draw_bitmap_region(bolinha, 937, 723, 304, 24, 800 - 299, 10, 0);
            
            for (int i = 0; i < game->player1->vida; i++){al_draw_bitmap_region(bolinha, 937, 701, 3, 20, 100 + (i * 3), 12, 0);}
            for (int i = 0; i < game->player2->vida; i++){al_draw_bitmap_region(bolinha, 937, 701, 3, 20, 800 - (i * 3), 12, 0);}

            for (int i = 0; i < game->player1->vitorias; i++){al_draw_bitmap_region(bolinha, 937, 639, 26, 28, 37 + (i * 30), 13, 0);}
            for (int i = 0; i < game->player2->vitorias; i++){al_draw_bitmap_region(bolinha, 937, 639, 26, 28, 840 - (i * 30), 13, 0);}
            if (game->player2->vida < 20 && game->player1->vida >= 60){al_draw_bitmap_region(bolinha, 963, 639, 25, 28, 1, 13, 0);}
            if (game->player1->vida < 20 && game->player2->vida >= 60){al_draw_bitmap_region(bolinha, 963, 639, 25, 28, 900 - 25, 13, 0);}
            update_animation(&game->player1->animations[game->player1->animacao_atual], delta_time);
            update_animation(&game->player2->animations[game->player2->animacao_atual], delta_time);

            //desenha_hit_box(game);


            al_flip_display();
           
            if (game->player1->vida < 0 || game->player2->vida < 0)
            {
                al_draw_text(font1, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT/ 2, ALLEGRO_ALIGN_CENTER, "FIM DO ROUND"); // round
                al_flip_display();
                al_rest(2);        
                al_destroy_font(font1);
                al_destroy_timer(local_timer);
                al_destroy_bitmap(bolinha);

                if (game->player1->vida < 0 && game->player2->vida < 0)
                {
                    game->player1->vitorias += 1;
                    game->player2->vitorias += 1;
                }
                else if (game->player1->vida < 0)
                    game->player2->vitorias += 1;
                else if (game->player2->vida < 0)
                    game->player1->vitorias += 1;
                return; // Sai da função.
            }           
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 0)
            {
                al_draw_text(font1, al_map_rgb(255, 119, 0), SCREEN_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, "PAUSA");
                al_flip_display(); // Atualiza a tela para que a escrita seja desenhada.
                al_stop_timer(local_timer);
                pause = 1;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && pause == 1)
            {
                al_start_timer(local_timer);
                pause = 0;
            }

            // Movimenta Player 1 padrão(W, A, S, D) ataque(u = soco, i=chute)
            if (event.keyboard.keycode == ALLEGRO_KEY_A)joystick_left(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_D)joystick_right(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_W)joystick_up(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_S)joystick_down(game->player1->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_H)soco(game->player1);
            else if (event.keyboard.keycode == ALLEGRO_KEY_J)chute(game->player1);
            else if (event.keyboard.keycode == ALLEGRO_KEY_K){joystick_defesa(game->player1->control);}
            else if (event.keyboard.keycode == ALLEGRO_KEY_Y){movimento_especial(game->player1);}
            else if (event.keyboard.keycode == ALLEGRO_KEY_U){joystick_fire(game->player1->control);}

            // Movimenta Player 2 (Setas direcionais)
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)joystick_left(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)joystick_right(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_UP)joystick_up(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)joystick_down(game->player2->control);
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1)soco(game->player2);
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2)chute(game->player2);
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_3){joystick_defesa(game->player2->control);}
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4){movimento_especial(game->player2);}
            else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_5){joystick_fire(game->player2->control);}
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_font(font1);
            al_destroy_timer(local_timer); // Destroi o temporizador específico
            exit(0);
        }
    }
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
        //printf("***\nEMPATE NESTA LUTA\n***\n");
    }
    else if (game->player1->vitorias == 2){
        al_draw_text(font1, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "GANHADOR");
        al_draw_bitmap_region(game->player1->face_sprite, 0, 400 * game->player1->selecao, 300, 400, 300, 100, 0);
        //printf("Player 1 venceu o luta!\n");
    }
    else if (game->player2->vitorias == 2){
        al_draw_text(font1, al_map_rgb(0, 255, 0), SCREEN_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "GANHADOR");
        al_draw_bitmap_region(game->player2->face_sprite, 0, 400 * game->player2->selecao, 300, 400, 300, 100, 0);
        //printf("Player 2 venceu o luta!\n");
    }
    al_flip_display();
    al_rest(2); // Tempo necessario para que o ganhador seja exibido na tela.

    // se o "x" do display foi pressionado, assim fecha o display e sai do jogo.
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        exit(0);

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
