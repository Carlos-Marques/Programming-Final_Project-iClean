#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Structs.h"
#include "Constants.h"


void readfile(FILE *fl)
{
    /**Declaraçao de variaveis auxiliares*/
    int pos[2], x, y, full, nRobot, width, height;
    float batRobot;
    char teststr[MAXINPUT], naRobot[3];

    /**Leitura da largura e altura do map e as posiçoes ocupadas por obstaculos*/
    fgets(teststr, MAXINPUT, fl);
    sscanf(teststr, "%d %d %d", &nSquareW, &nSquareH, &full );
    width = nSquareW;
    height = nSquareH;

    /**Alocaçao da memoria do map*/
    /**Alocaçao de uma coluna primeiro*/
    map = (square**)malloc(height*sizeof(square*));

    /**Caso a alocaçao de erro*/
    if(map == NULL)
    {
        printf("Sem memória\n");
        exit(EXIT_FAILURE);
    }

    /**Alocaçao linha a linha apartir da coluna*/
    for(x=0; x < height; x++)
    {
        map[x] = (square*)malloc(width*sizeof(square));

        /**Caso a alocaçao de erro*/
        if(map == NULL)
        {
            printf("Sem memória\n");
            exit(EXIT_FAILURE);
        }
    }

/**Inicializaçao das posiçoes com os valores default*/
    for(x=0; x < width; x++)
    {
        for(y=0; y < height; y++)
        {
            map[y][x].state = 1;
            map[y][x].is_robot = 0;
            map[y][x].naRobot[2] = 0;
            map[y][x].naRobot[1] = 0;
            map[y][x].naRobot[0] = 0;
            map[y][x].target = 0;
        }
    }

/**Leitura das posiçoes com obstaculo e actualizaçao do mapa com os obstaculos*/
    for(x=0; x < full; x++)
    {
        fgets(teststr, MAXINPUT, fl);
        sscanf(teststr, "[%d, %d]", &pos[0], &pos[1]);
        map[pos[1]][pos[0]].state = 2;
    }

/**Leitura do numero de Robos inicial*/
    fgets(teststr, MAXINPUT, fl);
    sscanf(teststr, "%d", &nRobot);
    nRobots = nRobot-1;

/**Leitura do nome, bateria e posiçao dos robots e criaçao das listas de robots alfabeticamente e respectivos stacks de posiçoes anteriores*/
    for(x=0; x < nRobot; x++)
    {
        fgets(teststr, MAXINPUT, fl);
        sscanf(teststr, "%s [%d, %d] %f", naRobot, &pos[0], &pos[1], &batRobot);

        new_robot = (robot*)malloc(sizeof(robot));
        pilhas[x] = create_stack();

        if (new_robot == NULL)
        {
            printf("Memory ERROR");
            exit(EXIT_FAILURE);
        }

        new_robot -> name[0] = naRobot[0];
        new_robot -> name[1] = naRobot[1];
        new_robot -> name[2] = '\0';
        new_robot -> bat = batRobot;
 	new_robot ->limpos = 0;
	new_robot ->ocupados = 0;
        new_robot->current.x = pos[0];
        new_robot->current.y = pos[1];
        new_robot->target.x = rand() % nSquareW;
        new_robot->target.y = rand() % nSquareH;
        new_robot->pos_past = pilhas[x];

        push(pilhas[x], new_robot->current);

        if(head == NULL)
        {
            head = new_robot;
            new_robot -> next = NULL;
            continue;
        }

        if( head->name[1] >= naRobot[1])
        {
            new_robot->next = head;
            head = new_robot;
        }
        //insert in the middle/end of the list
        else
        {
            //initialize two pointers, one pointing to a node after the other
            aux1 = head;
            aux2 = head->next;
            //finding the place in the list where new_node
            //must be inserted
            while( aux2 != NULL && aux2->name[1] <= naRobot[1])
            {
                aux1 = aux1->next;
                aux2 = aux2->next;
            }
            //finaly, insert it in the list!
            new_robot->next = aux2;
            aux1->next = new_robot;
        }
    }
}

void add_lista(int posY, int posX)
{
    /**Declaraçao de variaveis auxiliares*/
    float batRobot;
    char robotname[3]= {0,0,0}, input[100];
    int repeatrobot =0;
    robot *aux = head;

/**Randomize da bateria do novo robot entre 50 e 100*/
    batRobot = ((rand() % 50) + 50);

/**Alocaçao do novo robot e criaçao da respectiva pilha*/
    new_robot = (robot*)malloc(sizeof(robot));
    nRobots++;
    pilhas[nRobots] = create_stack();

/**Caso a alocaçao falhe*/
    if (new_robot == NULL)
    {
        printf("Memory ERROR");
        exit(EXIT_FAILURE);
    }

/**Leitura do nome do robot do terminal*/
    while(strlen(robotname)>2 || robotname[0] != 82)
    {
        printf("Enter the name of the robot: \n");
        fgets(input,MAXINPUT,stdin);
        sscanf(input,"%s",robotname);
    }

/**Actualizaçao das caracteristicas do novo robot*/
    new_robot -> name[0] = robotname[0];
    new_robot -> name[1] = robotname[1];
    new_robot -> name[2] = '\0';
    new_robot -> bat = batRobot;
    new_robot -> limpos = 0;
    new_robot -> ocupados = 0;
    new_robot->current.x = posX;
    new_robot->current.y = posY;
    new_robot->target.x = rand() % nSquareW;
    new_robot->target.y = rand() % nSquareH;
    new_robot->pos_past = pilhas[nRobots];

/**Push para a pilha da posiçao onde o robot começa*/
    push(pilhas[nRobots], new_robot->current);

/**Check para robot repetido*/

    repeatrobot = 0;
    while(aux != NULL)
    {
	if(robotname[1] == aux->name[1])
	{
		repeatrobot = 1;
		fprintf(stderr, "Robot repetido, tente outro nome\n");
	}
	aux = aux->next;
     }

/**Organizaçao da lista ja presente para incluir o novo robot na posiçao correcta alfabeticamente*/
    if(head == NULL)
    {
        new_robot -> next = NULL;
        head = new_robot;
    }

    if( head->name[1] > robotname[1] && repeatrobot == 0)
    {
        new_robot->next = head;
        head = new_robot;
    }
    //insert in the middle/end of the list
    else if ( head->name[1] < robotname[1] && repeatrobot == 0)
    {
        //initialize two pointers, one pointing to a node after the other
        aux1 = head;
        aux2 = head->next;
        //finding the place in the list where new_node
        //must be inserted
        while( aux2 != NULL && aux2->name[1] < robotname[1])
        {
            aux1 = aux1->next;
            aux2 = aux2->next;
        }
        //finaly, insert it in the list!
        new_robot->next = aux2;
        aux1->next = new_robot;
    }
}

void check_lista(robot *head)
{
    robot *aux = head;

/**Actualizaçao do mapa consoante a lista de robots*/
    while(aux != NULL)
    {
        map[aux->target.y][aux->target.x].target = 1;
        map[aux->current.y][aux->current.x].is_robot = 1;
	map[aux->target.y][aux->target.x].naRobot[0] = aux->name[0];
        map[aux->target.y][aux->target.x].naRobot[1] = aux->name[1];
        map[aux->current.y][aux->current.x].naRobot[0] = aux->name[0];
        map[aux->current.y][aux->current.x].naRobot[1] = aux->name[1];
        map[aux->current.y][aux->current.x].state = 0;
        aux = aux->next;
    }

}

robot* clean_if_zero()
{
    robot *tmp;

    robot *aux = head;

    for(; aux != NULL; aux = aux->next)
    {
        /**Limpeza da primeira posiçao da lista*/
        if( aux == head && aux->bat == 0 )
        {
            map[aux->current.y][aux->current.x].is_robot = 0;
            map[aux->target.y][aux->target.x].target = 0;
            head = head->next;
            free(aux);
        }
        /**Limpeza de outra posiçao da lista*/
        else
        {
            if(aux->next != NULL)
            {
                if( aux->next->bat == 0 )
                {
                    map[aux->next->current.y][aux->next->current.x].is_robot = 0;
                    map[aux->next->target.y][aux->next->target.x].target = 0;
                    tmp = aux->next;
                    aux->next = aux->next->next;
                    free(tmp);
                }
            }

        }

    }
    return head;
}

robot* clean_lista()
{
    robot *aux = head;

    /**Free das listas e respetivos stacks*/
    while(aux != NULL)
    {
        aux = head;
        if(head->next != NULL)
        {
            head = head->next;
        }
        else
        {
            head = NULL;
            free(aux->pos_past);
            free(aux);
            break;
        }
        free(aux->pos_past);
        free(aux);
    }
    return head;
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Surface *img_IST;
    SDL_Rect boardPos;

    // renders IST Logo
    img_IST = SDL_LoadBMP("ist_logo.bmp");
    if (img_IST == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(-5);
    }
    // square where the Logo is placed
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = img_IST->w;
    boardPos.h = img_IST->h;

    // creates a texture and renders it in the screen
    text_IST = SDL_CreateTextureFromSurface(_renderer, img_IST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy texture and surface
    SDL_DestroyTexture(text_IST);
    SDL_FreeSurface(img_IST);
    return img_IST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *font, SDL_Renderer* _renderer)
{
    SDL_Color color = { 0, 0, 0 };
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;

    // creates a surface with some text
    text_surface = TTF_RenderText_Blended(font,text,color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(-5);
    }

    // creates a texture from the surface and renders it !
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

    // destroy texture and surface
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * RenderMap function: Renders the map on the window screen according to their size
 * \param nSquareW number of squares to render (width)
 * \param nSquareH number of squares to render (height)
 * \param pos_robot position of the robot (ID of the square)
 * \param robot_name name of the robot (max. 2 letters)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderMap(int nSquareW, int nSquareH, square **map, SDL_Renderer* _renderer)
{
    TTF_Font *sans;
    TTF_Font *serif;
    SDL_Rect gridPos;
    int i,j, height, id = 0;
    char batrobot[5];

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    // opens a font style and sets a size
    sans = TTF_OpenFont("FreeSans.ttf", 24);
    serif = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!sans || !serif)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(-5);
    }
    // render the IST Logo
    height = RenderLogo(nSquareW*SQUARE_SIZE, 0, _renderer);

    // render the student name
    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myName, serif, _renderer);

    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myNumber, serif, _renderer);

    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myName1, serif, _renderer);

    // render the student name
    height += RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, myNumber1, serif, _renderer);

    robot *aux = head;

    while(aux != NULL)
    {
        RenderText(nSquareW*SQUARE_SIZE+3*MARGIN, height, aux->name, serif, _renderer);
        snprintf(batrobot, 5, "%f", aux->bat);
        height += RenderText(nSquareW*SQUARE_SIZE+8*MARGIN, height, batrobot, serif, _renderer);
        aux = aux->next;
    }



    // grid position
    gridPos.w = SQUARE_SIZE;
    gridPos.h = SQUARE_SIZE;
    gridPos.y = 0;

    // iterate over all squares
    for (i = 0; i < nSquareH; i++)
    {
        gridPos.x = 0;
        for (j = 0; j < nSquareW; j++)
        {
            if(map[i][j].state==DIRTY)
            {

                // writes a dirty square
                SDL_SetRenderDrawColor( _renderer, 0, 204, 204, 255 );
                SDL_RenderFillRect( _renderer, &gridPos );
                SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
                SDL_RenderDrawRect( _renderer, &gridPos );

            }
            else if(map[i][j].state==CLEAN)
            {
                SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
                SDL_RenderFillRect( _renderer, &gridPos );
                SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
                SDL_RenderDrawRect( _renderer, &gridPos );

            }
            else if(map[i][j].state==OBSTACLE)
            {
                //writes an obstacle square
                SDL_SetRenderDrawColor( _renderer, 64, 64, 64, 255 );
                SDL_RenderFillRect( _renderer, &gridPos );
                SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
                SDL_RenderDrawRect( _renderer, &gridPos );

            }
            if (map[i][j].target == 1)
            {
                SDL_SetRenderDrawColor(_renderer, 200, 100, 0, 255 );
                SDL_RenderFillRect(_renderer,&gridPos);
                SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(_renderer,&gridPos);
		RenderText(gridPos.x+MARGIN, gridPos.y, map[i][j].naRobot, sans, _renderer);
		
            }
            if (map[i][j].is_robot==1)
            {
                SDL_SetRenderDrawColor(_renderer, 0, 153, 0, 255 );
                SDL_RenderFillRect(_renderer,&gridPos);
                SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(_renderer,&gridPos);
                RenderText(gridPos.x+MARGIN, gridPos.y, map[i][j].naRobot, sans, _renderer);
            }
            gridPos.x += SQUARE_SIZE;
            id++;
        }
        gridPos.y += SQUARE_SIZE;
    }

    // render the changes above
    SDL_RenderPresent( _renderer);

    // destroy everything
    TTF_CloseFont(sans);
    TTF_CloseFont(serif);
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
int InitEverything(int width, int height, SDL_Window** _window, SDL_Renderer** _renderer)
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if ( !InitSDL() )
        return 0;

    if ( !InitFont() )
        return 0;

    window = CreateWindow(width, height);
    if ( window == NULL )
        return 0;

    renderer = CreateRenderer(width, height, window);
    if ( renderer == NULL )
        return 0;

    *_window = window;
    *_renderer = renderer;

    return 1;
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
int InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
int InitFont()
{
    // init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 0;
    }

    return 1;
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window* window = NULL;
    // init window
    window = SDL_CreateWindow( "iClean", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );

    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        return NULL;
    }

    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window* _window)
{
    SDL_Renderer* renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        return NULL;
    }
    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

    return renderer;
}


pstack create_stack()

{

    pstack novo;

    /**Alocaçao da memoria do novo stack*/
    novo = (pstack)malloc(sizeof(no_stack));

    /**Caso a alocaçao de erro*/
    if (novo == NULL)

    {

        printf("Erro no malloc\n");

        exit(EXIT_FAILURE);

    }

    novo->next = NULL;

    return novo;

}


void push(pstack topo, position pos)

{

    pstack novo;

    /**Alocaçao da nova posiçao do stack*/
    novo = (pstack)malloc(sizeof(no_stack));

    /**Caso a alocaçao de erro*/
    if (novo == NULL)

    {

        printf("Erro no malloc\n");

        exit(EXIT_FAILURE);

    }

    /**Empurrar para o topo do stack*/
    novo->pos = pos;

    novo->next = topo->next;

    topo->next = novo;

}

// Function to move the robot
// To be used in the programming project final version
// \param _xc X coordinate computed: new robot position
// \param _yc Y coordinate computed: new robot position
// \param xt X coordinate: old robot position
// \param yt Y coordinate: old robot position

void MoveRobot(int *_xc, int *_yc, int xt, int yt, int *a)
{
    int xc, yc, x, y;
    float dbat;
    double angle;
    pstack auxstack;
    robot *aux = *a;
    auxstack = aux->pos_past;

    // just copy the values
    xc = *_xc;
    yc = *_yc;

    map[yc][xc].is_robot = 0;

    // calculate the angle
    angle = atan2((double)(yc-yt), (double)(xt-xc));

    // calculate the new position
    x = floor(xc + cos(angle)+0.5);
    y = floor(yc - sin(angle)+0.5);


    /**Caso possa mover (nao tem obstaculo nem robot) move e deteora a bateria consoante o fornecido na compilaçao*/
    if(map[y][x].state != 2 && map[y][x].is_robot == 0)
    {

        if(map[y][x].state == DIRTY)
        {
            aux->limpos++;
        }

        aux->ocupados++;

        *_xc = x;
        *_yc = y;
        while(dbat == 0)
        {
            dbat = rand() % maxbat;
        }

        aux->bat -= (dbat/10);
        dbat = 0;

        if(aux->bat < 0)
        {
            aux->bat = 0;
        }

        push(auxstack, aux->current);
    }
    /**Caso contrario cria novo target*/
    else
    {
        aux->newtarget = 1;
    }
}

void new_target(int *x)
{
    int H, W, y;
    pstack auxstack;
    robot *aux = *x;
    auxstack = aux->pos_past;

/**Caso a posiçao actual seja igual ao target ou caso seja preciso gerar novo target se existir um obstaculo ou robot no caminho*/
    if((aux->current.x == aux->target.x && aux->current.y == aux->target.y) || aux->newtarget == 1)
    {
        map[aux->target.y][aux->target.x].target=0;

        /**Gera posiçoes target ate gerar uma que nao esteja no stack de posiçoes passadas*/
        for(y=0; y < 1; y++)
        {
            auxstack = aux->pos_past;
            H = rand() % (nSquareH);
            W = rand() % (nSquareW);

            while (auxstack != NULL)
            {
                if(H == auxstack->pos.y && W == auxstack->pos.x)
                {
                    y--;
                    break;
                }
                auxstack = auxstack->next;
            }
        }
        aux->target.x = W;
        aux->target.y = H;
        aux->newtarget = 0;
        map[aux->target.y][aux->target.x].target = 1;
	map[aux->target.y][aux->target.x].naRobot[0] = aux->name[0];
        map[aux->target.y][aux->target.x].naRobot[1] = aux->name[1];
    }
}

void MoveRobots()
{
    robot *aux = head;

    /**Move todos os robots e actualiza o mapa*/
    while(aux != NULL)
    {
        new_target(&aux);
        MoveRobot(&aux->current.x, &aux->current.y, aux->target.x, aux->target.y, &aux);
        check_lista(head);
        aux = aux->next;
    }
}
