#CC = gcc -O3
CC = gcc 
CFLAGS = -Wall -Wextra -g

# Definição das flags de linkagem
# Inclui as bibliotecas necessárias para o Allegro
LDFLAGS = -lallegro -lallegro_main -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_primitives

SRC = main.c Joystick.c Player.c Game.c
OBJ = $(SRC:.c=.o)
EXEC = a

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJ)  # Remover arquivos objeto também
