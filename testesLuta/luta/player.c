
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
        player->animations = (Animation *)malloc(10 * sizeof(Animation));
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

        player->vida = 100;
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
