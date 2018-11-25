#ifndef FUNCTIONS_DECLARATION_H
#define FUNCTIONS_DECLARATION_H

/**Funçao que le do ficheiro e cria um mapa e listas de robots*/
void readfile(FILE *fl);

/**Funçao que adiciona um robot por ordem alfabetica a lista de robots*/
void add_lista(int posY, int posX);

/**Funçao que adiciona actualiza o mapa consoante as listas*/
void check_lista(robot *head);

/**Funçao que adiciona retira um robot da lista se este tiver a bateria a zero*/
robot* clean_if_zero();

/**Funçao que adiciona limpa uma lista*/
robot* clean_lista();

/**Funçao que da render no logo*/
int RenderLogo(int x, int y, SDL_Renderer* _renderer);

/**Funçao que da render em textp*/
int RenderText(int x, int y, const char *text, TTF_Font *font, SDL_Renderer* _renderer);

/**Funçao que da render no map*/
void RenderMap(int nSquareW, int nSquareH, square **map, SDL_Renderer* _renderer);

/**Funçao que inicializa os graficos*/
int InitEverything(int width, int height, SDL_Window** _window, SDL_Renderer** _renderer);

/**Funçao que inicializa o SDL*/
int InitSDL();

/**Funçao que inicializa a Font*/
int InitFont();

/**Funçao que cria Window de SDL consoante a width e height dadas*/
SDL_Window* CreateWindow(int width, int height);

/**Funçao que cria render*/
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window* _window);

/**Funçao que cria Stack*/
pstack create_stack();

/**Funçao que empurra para o topo do Stack*/
void push(pstack topo, position pos);

/**Funçao que move ou nao um robot dependendo das condiçoes*/
void MoveRobot(int *_xc, int *_yc, int xt, int yt, int *a);

/**Funçao que cria novo target*/
void new_target(int *x);

/**Funçao que move todos os robots*/
void MoveRobots();

#endif
