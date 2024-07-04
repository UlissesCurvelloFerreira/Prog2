#ifndef __JOYSTICK__ 																												//Guardas de inclusão 
#define __JOYSTICK__																												//Guardas de inclusão 

typedef struct {																													//Definição da estrutura de um controle 
	unsigned char right;																											//Botão de movimentação à direta 
	unsigned char left;																												//Botão de movimentação à esquerda 
	unsigned char up;																												//Botão de movimentação para cima 
	unsigned char down;
	unsigned char soco;
	unsigned char chute;																													//Botão de disparo (!)																											//Botão de movimentação para baixo 
	unsigned char fire;
	unsigned char soco_baixo;
	unsigned char defesa;

} joystick;																															//Definição do nome da estrutura 

joystick* joystick_create();																										//Protótipo da função de criação de um controle 
void joystick_destroy(joystick *element);																							//Protótipo da função de destruição de um controle 
void joystick_right(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à direita 
void joystick_left(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação à esquerda 
void joystick_up(joystick *element);																								//Protótipo da função de ativação/desativação do botão de movimentação para cima 
void joystick_down(joystick *element);
void joystick_soco(joystick *element);
void joystick_soco_baixo(joystick *element);
void joystick_chute(joystick *element);																											//Protótipo da função de ativação/desativação do botão de movimentação para baixo 
void joystick_fire(joystick *element);																								//Protótipo da função de ativação/desativação do botão de disparo (!)
void joystick_defesa(joystick *element);
#endif																																//Guardas de inclusão 