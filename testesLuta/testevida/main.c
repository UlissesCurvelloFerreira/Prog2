#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>

struct Personagem {
    int vida;
    int x, y;
};

struct Personagem *cria_personagem() {
    struct Personagem *new = malloc(sizeof(struct Personagem));
    new->vida = 100;
    new->x = 300;
    new->y = 300;
    return new;
}

void destroi_personagem(struct Personagem *p) {
    free(p);
}

void imprime_vida(struct Personagem *p) {
    int vida = 50; // Posição inicial de onde a imagem vai ser impressa.
    ALLEGRO_BITMAP *image = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *image2 = al_load_bitmap("2.png");

    al_draw_bitmap_region(image, 0, 0, 499, 30, 48, 50, 0); // Centraliza a imagem

    for (int i = 0; i < p->vida; i++) {
        al_draw_bitmap_region(image2, 0, 0, 3, 26, vida, 52, 0);
        vida += 3;
    }

    al_destroy_bitmap(image);
    al_destroy_bitmap(image2);
}

int main() {


    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    ALLEGRO_DISPLAY *display = al_create_display(900, 600);
    ALLEGRO_BITMAP *image = al_load_bitmap("1.png");
    ALLEGRO_BITMAP *image2 = al_load_bitmap("2.png");
    struct Personagem *p = cria_personagem();

    // Configurar a fila de eventos
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    bool running = true;
    while (running) {

        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        imprime_vida(p);
        al_flip_display();

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE || p->vida <= 0) {
                running = false;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_D) {
                p->vida -= 100;
                if (p->vida < 0) p->vida = 0;
            }
        }
    }

    destroi_personagem(p);
    al_destroy_bitmap(image);
    al_destroy_bitmap(image2);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
