#include <stdlib.h>
#include "Joystick.h"

joystick* joystick_create(){														//Implementação da função "joystick_create"

	joystick *element = (joystick*) malloc (sizeof(joystick));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;														//Se a alocação não deu certo, retorna erro
	element->right = 0;																//Insere o estado de desligado para o botão de movimentação à esquerda
	element->left = 0;																//Insere o estado de desligado para o botão de movimentação à direita
	element->up = 0;																//Insere o estado de desligado para o botão de movimentação para cima
	element->down = 0;																//Insere o estado de desligado para o botão de movimentação para baixo
	element->soco = 0;																//Insere o estado de desligado para o botão de movimentação para soco
	element->fire = 0;;																//Insere o estado de desligado para o botão de movimentação para especial
	element->chute = 0;;															//Insere o estado de desligado para o botão de movimentação para chute
	element->soco_baixo = 0;;														//Insere o estado de desligado para o botão de movimentação para soco baixo.
	element->defesa = 0;															//Insere o estado de desligado para o botão de movimentção para defesa
	return element;																	//Retorna o novo controle
}

void joystick_destroy(joystick *element){ free(element);}							//Implementação da função "joystick_destroy"; libera a memória do elemento na heap

void joystick_left(joystick *element){ element->left = element->left ^ 1;}			//Implementação da função "joystick_left"; muda o estado do botão

void joystick_right(joystick *element){ element->right = element->right ^ 1;}		//Implementação da função "joystick_right"; muda o estado do botão

void joystick_up(joystick *element){ element->up = element->up ^ 1;}				//Implementação da função "joystick_up"; muda o estado do botão

void joystick_down(joystick *element){ element->down = element->down ^ 1;}			//Implementação da função "joystick_down"; muda o estado do botão

void joystick_soco(joystick *element){ element->soco = element->soco ^ 1;}			//Implementação da função "joystick_soco"; muda o estado do botão

void joystick_soco_baixo(joystick *element){ element->soco_baixo = element->soco_baixo ^ 1;} //Implementação da função "joystick_soco_baixo"; muda o estado do botão

void joystick_chute(joystick *element){ element->chute = element->chute ^ 1;}		//Implementação da função "joystick_chute"; muda o estado do botão

void joystick_fire(joystick *element){ element->fire = element->fire ^ 1;}			//Implementação da função "joystick_fire"; muda o estado do botão

void joystick_defesa(joystick *element){ element->defesa = element->defesa ^ 1;} 	//Implementação da função "joystick_defesa"; muda o estado do botão