typedef struct
{
    ALLEGRO_BITMAP *img_sprite;
    ALLEGRO_BITMAP *face_sprite;
    Hitbox hitbox_superior;     // Hitbox superior do personagem
    Hitbox hitbox_inferior;  // Hitbox inferior do personagem
    joystick *control;     // Joystick para controle
    Animation *animations; // Array de animações
    int selecao;
    int x, y;              // Coordenadas do personagem para movimentação
    int ataque;             // Indica se o personagem está atacando
    int no_ar;             // Indica se o personagem está no ar
    float velocidade_y;    // Velocidade vertical do personagem
    int animacao_atual; // Índice da animação atual // animacao_atual
    int vida;              // Vida do personagem
    int vitorias;
    int id;
    // int facing_left;       // Indica se o personagem está virado para a esquerda
} Player;

