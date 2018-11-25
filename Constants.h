#ifndef CONSTANTS_H
#define CONSTANTS_H

/**Declaraçao de constantes globais*/
#define STRING_SIZE 100
#define MAP_SIZE 1000
#define WINDOW_POSX 500
#define WINDOW_POSY 250
#define SQUARE_SIZE 35
#define EXTRASPACE 150
#define MARGIN 5
#define MAXINPUT 100
#define CLEAN 0
#define DIRTY 1
#define OBSTACLE 2

/**Declaraçao dos nomes e numeros dos alunos*/
const char myName[] = "Carlos Silva";
const char myName1[] = "Sergio Marinheiro";

const char myNumber[] = "IST81323";
const char myNumber1[] = "IST81332";

/**Declaraçao de variaveis globais*/
square **map=0;
int nSquareW=0, nSquareH=0, nRobots, xf, yf;
robot * new_robot = NULL;
robot * head = NULL;
robot * aux1 = NULL;
robot * aux2 = NULL;
pstack pilhas[MAXINPUT];
int newtarget=0;
int maxbat=0;

#endif
