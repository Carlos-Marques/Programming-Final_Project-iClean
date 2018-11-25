/** INCLUDE DIRECTIVES: inclui todos os ficheiros .h e .c necessarios ao funcionamento do programa */
#include "Directives.c"

/**MAIN: core do programa a ser executado, tem como argumentos argvs que são atribuidos na compilaçao*/
int main( int argc, char* args[] )
{
    /**Inicialização de variaveis necessarias ao funcionamento do main e outras funçoes*/

    /**Variaveis do SDL*/
    SDL_Event event;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    /**Variaveis de auxilio aos calculos e a construçao da janela*/
    int width = 0;
    int height = 0;
    int delay = 1000;
    int key,i,posX,posY,quit=0;
    int pause=0, x, y, quits;
    robot *aux;

    /**Set da seed como 456*/
    srand(456);

    /**Passagem do segundo argumento fornecido na compilaçao para o int maxbat, maximo de deteorizaçao da bataria por move*/
    maxbat = atoi(args[2]);
    maxbat = maxbat * 10;

    /**Declaraçao de dois ficheiros fl e fe*/
    FILE *fe;
    FILE *fl;

    /**Leitura do map fornecido na compilaçao do programa*/

    fl = fopen(args[1], "r");

    /**Criaçao do mapa com os obstaculos e das listas de robots*/
    readfile(fl);

    fclose(fl);

    // calculate the window size
    width = SQUARE_SIZE*nSquareW;
    height = SQUARE_SIZE*nSquareH;

    // initialize graphics
    if ( !InitEverything(width, height, &window, &renderer) )
        return -1;
    key=-1;
    while( quit==0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;
                break;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                case SDLK_DOWN:
                    // speed-down
                    delay += 50;

                    break;
                case SDLK_UP:
                    // speed-up
                    if(delay>0)
                    {
                        delay -= 50;
                    }

                    break;

                case SDLK_q:
                    //quit program
                    quit = 1;

                    break;

                case SDLK_e:
                    //escreve ficheiro de estatisticas
                    fe = fopen("estatisticas.txt", "w");

                    aux = head;
                    while(aux != NULL)
                    {
                        fprintf(fe,"%s\n",aux->name);
                        fprintf(fe,"%d\n",aux->limpos);
                        fprintf(fe,"%d\n\n",aux->ocupados);
                        aux = aux->next;
                    }

                    fclose(fe);

                    break;

                case SDLK_i:
                    //reinicia a simulaçao
                    clean_lista();

                    fl = fopen(args[1], "r");

                    readfile(fl);

                    fclose(fl);

                    break;
                case SDLK_p:
                    //poe a simulaçao em pause
                    pause=1;

                    break;
                default:
                    pause=0;
                    break;
                }
            }
        }

        while(pause == 1 && quit == 0)
        {
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    quit = 1;
                    break;
                }
                else if ( event.type == SDL_KEYDOWN )
                {
                    switch ( event.key.keysym.sym )
                    {
                    case SDLK_a:
                        //permite adicionar robot como o rato
                        key=0;

                        break;
                    case SDLK_o:
                        //permite adicionar obstaculo com o rato
                        key=1;

                        break;
                    case SDLK_q:
                        //faz quit da simulaçao
                        quit = 1;

                        break;
                    case SDLK_e:
                        //escreve ficheiro de estatisticas
                        fe = fopen("estatisticas.txt", "w");

                        aux = head;
                        while(aux != NULL)
                        {
                            fprintf(fe,"%s\n",aux->name);
                            fprintf(fe,"%d\n",aux->limpos);
                            fprintf(fe,"%d\n\n",aux->ocupados);
                            aux = aux->next;
                        }

                        fclose(fe);

                        break;
                    case SDLK_p:
                        //retira da pause
                        pause=0;

                        break;
                    case SDLK_i:
                        //reinicializa a simulaçao
                        clean_lista();

                        fl = fopen(args[1], "r");

                        readfile(fl);

                        fclose(fl);

                        break;
                    default:
                        key = -1;
                        break;
                    }
                }
                else if(event.type==SDL_MOUSEBUTTONDOWN)//caso o botao do rato esteja primido
                {
                    SDL_GetMouseState(&posX,&posY);//tira as coordenadas X Y em pixeis
                    posX=posX/SQUARE_SIZE ;
                    posY= posY/SQUARE_SIZE;

                    if(key==1)
                    {
                        printf("Position selected:%d %d\n",posX,posY);
                        if(posX<nSquareW && posY<nSquareH)//caso esteja dentro do mapa (porque tambem ha a parte da tela do simobolo do tecnico..
                        {
                            //Para meter um obstaculo
                            map[posY][posX].state=OBSTACLE;
                        }
                        key=-1;

                    }
                    if(key==0)
                    {
                        printf("Position selected:%d %d\n",posX,posY);
                        if(posX<nSquareW && posY<nSquareH)
                        {
                            //adicionar robot
                            add_lista(posY, posX);
                        }
                    }
                }
            }
            /**Actualiza o mapa consoante as listas*/
            check_lista(head);

            /**Faz render do map*/
            RenderMap(nSquareW, nSquareH, map, renderer);
        }

        /**Faz render do map*/
        RenderMap(nSquareW, nSquareH, map, renderer);

        /**Faz o move de todos os robots, escolhendo targets novos quando o movimento nao e possivel*/
        MoveRobots();

        /**Faz render do map*/
        RenderMap(nSquareW, nSquareH, map, renderer);

        /**Limpa da lista os robots que tem a bateria a zero*/
        clean_if_zero();

        /**Actualiza o mapa consoante as listas*/
        check_lista(head);

        /**Faz render do map*/
        RenderMap(nSquareW, nSquareH, map, renderer);

        /**Caso todas as posiçoes fiquem limpas da quit no programa*/
        quits =0;
        for(x=0; x < nSquareW; x++)
        {
            for(y=0; y < nSquareH; y++)
            {
                if (map[y][x].state == DIRTY)
                {
                    quits++;
                }
            }
        }
        if(quits == 0)
        {
            quit = 1;
        }

        /**Caso nao existam mais robots na simulaçao e a sala ainda nao se encontre completamente limpa poe a simulaçao em pausa e pede ao utilizador para acrescentar
        robots*/
        if(head == NULL)
        {
            fprintf(stderr,"Insira mais robots para continuar a simulação\n");
            pause = 1;
        }

// add a delay
        SDL_Delay( delay );
    }
    //free do map
    for(i=0; i<nSquareH; i++)
        free(map[i]);
    free(map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
}
