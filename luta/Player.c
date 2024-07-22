#include "Player.h"

Player *create_player()
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
        player->control = joystick_create();                  // Inicializa o joystick
        player->face_sprite = al_load_bitmap("img/face.png"); // Carrega o sprite da face

        // Inicializa as animações
        player->animations = (Animation *)malloc(18 * sizeof(Animation));
        if (!player->animations)
        {
            printf("Erro ao alocar memória para as animações.\n");
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

        player->vida = 0;
    }
    return player;
}

void destroy_player(Player *player)
{
    if (player)
    {
        if (player->img_sprite)
            al_destroy_bitmap(player->img_sprite);
        
        if (player->face_sprite)
            al_destroy_bitmap(player->face_sprite);
        
        joystick_destroy(player->control); // Destroi o joystick
        free(player->animations);          // Libera a memória das animações
        free(player);
    }
}

void draw_animation(Player *player, Animation *anim, int flag, int fatality)
{
    int frame_x = anim->x + (anim->current_frame * anim->frame_width);

    if (fatality == 17) //Fatality muda a largura, assim mudando a impressão também.
    {
        al_draw_scaled_bitmap(player->img_sprite, frame_x, anim->y, anim->frame_width - CORTE, anim->frame_height,
                player->x + (- 800 * flag), player->y, PLAYER_WIDTH + 800, PLAYER_HEIGHT, flag ? ALLEGRO_FLIP_HORIZONTAL : 0);
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

void carrega_sprite(Player *player, int selecao)
{
    // Destrói o sprite atual se existir
    if (player->img_sprite){al_destroy_bitmap(player->img_sprite);}
    // Carrega o sprite com base na seleção do quadrado
    switch (selecao)
    {
    case 0:
        player->img_sprite = al_load_bitmap("img/sean.png");
        break;
    case 1:
        player->img_sprite = al_load_bitmap("img/ryu.png");
        break;
    case 2:
        player->img_sprite = al_load_bitmap("img/ken.png");
        break;
    case 3:
        player->img_sprite = al_load_bitmap("img/akuma.png");
        break;
    default:
        player->img_sprite = al_load_bitmap("img/default_sprite.png");
        break;
    }
    // Deixa a cor rosa transparente.
    al_convert_mask_to_alpha(player->img_sprite, al_map_rgb(248, 0, 248));
    if (!player->img_sprite){printf("Falha ao carregar o sprite.\n");}
}

void verifica_limites(Player *player, int largura_tela)
{
    int recorte = 200; // Espaço em branco nas laterais
    if (player->x < -recorte)player->x = -recorte;
    else if (player->x + PLAYER_WIDTH - recorte > largura_tela){
    player->x = largura_tela - (PLAYER_WIDTH - recorte);}
    
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

void resolve_collision(Player *p1, Player *p2)
{
    if (check_player_collision(p1, p2))
    {
        if (p1->x < p2->x)
        {
            p1->x -= 15;
            p2->x += 15;
        }
        else
        {
            p1->x += 15;
            p2->x -= 15;
        }
    }
}

void update_hitbox(Player *player, int flag)
{
    switch (player->animacao_atual)
    {
    case 0: // Idle
    case 1: // Walk esq
    case 2: // Walk dir
    case 3: // Junp
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
    
    case 4: // abaixado.
    case 13: // defesa abaixado.
    case 15: // toma dano abaixado.
        // Hitbox superior desativada
        player->hitbox_superior.width = 0;
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;

    case 5: // soco normal.
    case 6: // soco cima.
        // Hitbox superior
        // 65 pois é a diferença entre a largura 150-85 = 65 
        // player->hitbox_superior.x = player->x + 200 + (flag ? -65 : 0)//
        player->hitbox_superior.x = player->x + 200 + (-65 * flag);                                                           
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 150;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;

    case 8: // chute normal.
        // Hitbox superior 
        player->hitbox_superior.x = player->x + 200 + (-75 * flag);
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 160;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200;
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 85;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;

    case 7:  // Soco baixo
        // Hitbox superior desativada
        player->hitbox_superior.width = 0; 
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-55 * flag);
        player->hitbox_inferior.y = player->y + 170;               
        player->hitbox_inferior.width = 140;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;

    case 9: // chute cima.
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200;
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 85;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-75 * flag);
        player->hitbox_inferior.y = player->y + 170;               
        player->hitbox_inferior.width = 160;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;

    case 10: // Chute baixo
        // Hitbox superior
        player->hitbox_superior.width = 0;
        player->hitbox_superior.height = 0;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-75 * flag);
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 160;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 225;
        break;

    case 17:
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200 + (-815 * flag);
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 900;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 + (-815 * flag);
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 900;
        player->hitbox_inferior.height = PLAYER_HEIGHT - 220;
        break;

    
    case 16: // movimento particular.
        // Hitbox superior
        player->hitbox_superior.x = player->x + 200 + (-65 * flag);  
        player->hitbox_superior.y = player->y + 108;
        player->hitbox_superior.width = 150;
        player->hitbox_superior.height = 60;

        // Hitbox inferior
        player->hitbox_inferior.x = player->x + 200 +(-65 * flag);
        player->hitbox_inferior.y = player->y + 170;
        player->hitbox_inferior.width = 150;
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

void soco(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down) p->animacao_atual = 7; // Soco baixo.
        else if (p->no_ar) p->animacao_atual = 6;    // soco cima.
        else p->animacao_atual = 5;                  // Soco.
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void chute(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down) p->animacao_atual = 10; // Chute baixo
        else if (p->no_ar) p->animacao_atual = 9;     // chute cima
        else p->animacao_atual = 8;                   // Chute
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void movimento_especial(Player *p)
{
    if (!p->ataque)
    {
        p->ataque = 1;
        if (p->control->down) p->animacao_atual = 16; // Soco baixo.
        else if (p->no_ar) p->animacao_atual = 16;    // soco cima.
        else p->animacao_atual = 16;                  // Soco.
        p->animations[p->animacao_atual].current_frame = 0;
    }
}

void leva_dano(Player *p)
{
    if (p->control->down)p->animacao_atual = 15; // animação de dano abaixado.
    else p->animacao_atual = 14;                 // animação de dano (normal/no ar).
    p->animations[p->animacao_atual].current_frame = 0;
}

void movimento_idle(Player *p)
{//Busca tratar todos os casos para, assim, voltar par o move idle.
    if (!p->control->left &&
        !p->control->right &&
        !p->control->up &&
        !p->control->down &&
        !p->ataque &&
        !p->no_ar &&
        !p->control->defesa)
    {p->animacao_atual = 0;} //Idle.
}

void movimento_defesa(Player *p)
{// movimentos de defesa para ambos os personagens.
    if (p->control->defesa)
    {
        if (p->control->down) p->animacao_atual = 13; // defesa baixo.
        else if (p->no_ar) p->animacao_atual = 12;    // defesa cima.
        else p->animacao_atual = 11;                  // defesa.
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
            if (p->control->down) p->animacao_atual = 4; //Abaixado.
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
                if (p->control->down) p->animacao_atual = 4; // abaixado.
                else p->animacao_atual = 0; // Idle
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
        if (!p->ataque && p->no_ar) p->animacao_atual = 3; // Jump.
    if (p->control->down)
        if (!p->ataque && !p->no_ar) p->animacao_atual = 4;// abaixado.
}

void movimento_left_right(Player *p)
{
    // Aplica a movimentação dos jogadores
    if (p->control->left && !p->control->down && !p->control->defesa)
    {
        p->x -= 15;
        verifica_limites(p, SCREEN_WIDTH); // Verifica os limites do player
        if (!p->ataque && !p->no_ar)
            p->animacao_atual = 2;         // Walk
    }
    if (p->control->right && !p->control->down && !p->control->defesa)
    {
        p->x += 15;
        verifica_limites(p, SCREEN_WIDTH); // Verifica os limites do player
        if (!p->ataque && !p->no_ar)
            p->animacao_atual = 1;         // Walk
    }
}

void movimento_final(Player *p1, Player *p2)
{
    if (p1->control->fire  && !p1->no_ar && p2->vida < 20 && p1->vida >= 60)
    {
        if(!p1->ataque)
        {
            p1->ataque = 1;
            p1->animacao_atual = 17;
        }
    }
    if (p2->control->fire  && !p2->no_ar && p1->vida < 20 && p2->vida >= 60)
    {
        if(!p2->ataque)
        {
            p2->ataque = 1;
            p2->animacao_atual = 17;
        }
    }
}

/*Função responsavel por baixar a vida quando é um ataque do jogador adversário. */
void diminuir_vida (Player *p1, Player *p2)
{
    // Verifica colisão entre hitboxes
    if (check_player_collision(p1, p2))
    {
        if (p1->ataque && !p2->control->fire)
        {
            // Se adversario fatality
            if (p1->control->fire) 
            {
                p2->vida -= 1;
                printf("p1 aplica 1 de dano\n");
            }// Se esta defendendo.      
            else if (p2->control->defesa) 
            {
                p2->vida -= 2;  
                    printf("p1 aplica 2 de dano\n");
            }
            else // Dano normal
            {   p2->vida -= 3;
                    printf("p1 aplica 3 de dano\n");
            }                                        
            leva_dano(p2); 
        }
        else if (p2->ataque && !p1->control->defesa && !p1->control->fire)
        {
            // Se adversario fatality
            if (p2->control->fire) 
            {
                p1->vida -= 1;
                printf("p2 aplica 1 de dano\n");
            }// Se esta defendendo.      
            else if (p1->control->defesa) 
            {
                p1->vida -= 2;  
                printf("p2 aplica 2 de dano\n");
            }
            else // Dano normal
            {   p1->vida -= 3;
                printf("p2 aplica 3 de dano\n");
            }        
            leva_dano(p1);
        }
    }
}

int invert_flag_control(Player *p1, Player *p2)
{
    if (p1->x > p2->x)return 1;
    return 0;
}


///////////////////////////////////===================================///////////////////////////////////////==================================////////////////////////////


/*

        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
        {

            if (event.keyboard.keycode == ALLEGRO_KEY_K){movimento_especial(game->player1);}
            if (event.keyboard.keycode == ALLEGRO_KEY_PAD_4){movimento_especial(game->player2);}
            if (event.keyboard.keycode == ALLEGRO_KEY_H){joystick_fire(game->player1->control);}
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
            al_destroy_timer(local_timer); // Destroi o temporizador específico
            exit(0);
        }
    }
}
*/



/*

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

*/
