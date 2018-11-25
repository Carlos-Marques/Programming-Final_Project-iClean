#ifndef STRUCTS_H
#define STRUCTS_H

/**Definiçao da struct de posiçao*/
typedef struct
{
    int x;
    int y;
} position;

/**Definiçao da struct que representa um elemento do stack*/
typedef struct char_stack
{

    position pos;

    struct char_stack * next;

} no_stack;
typedef no_stack * pstack;

/**Definiçao da struct que representa um quadrado*/
typedef struct
{
    int state;
    int is_robot;
    char naRobot[3];
    int target;
} square;

/**Definiçao da struct que representa um elemento da lista de robots*/
typedef struct robot
{
    int limpos;
    int ocupados;
    char name[3];
    position current;
    position target;
    float bat;
    int newtarget;
    pstack pos_past;
    struct robot *next;
} robot;


#endif
