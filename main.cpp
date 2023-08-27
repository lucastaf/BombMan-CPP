/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/


#include "matrixFunctions.cpp"


#define QtdObjetos 2



int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NÃO FIQUE PISCANDO NA TELA
        //INÍCIO: COMANDOS PARA REPOSICIONAR O CUSOR NO INÍCIO DA TELA
        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.

    
    map m = {   2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                2,0,2,0,2,0,2,2,2,2,2,2,2,0,0,
                2,0,2,0,2,0,2,2,2,2,2,2,2,0,0,
                2,0,2,0,2,0,2,2,2,2,2,2,2,0,2,
                2,0,2,0,0,0,2,2,2,2,2,2,2,0,2,
                2,0,2,0,2,0,2,2,2,2,2,2,2,0,2,
                2,0,2,0,2,0,2,2,2,2,2,2,2,0,2,
                2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                2,2,2,0,0,0,2,2,2,2,2,2,2,0,2
    };

    map currentframe = m;


   
    obj player = {1,1,4};  //Posicao inicial do personagem no console
    bomba bomba =  {0,0,5,0};
    for(int x=0;x<=raio*4;x++){
        bomba.explosao[x].id = 1;
    }
    //Variavel para tecla precionada
    char tecla;
    
    while(true) //Loop principal do Jogo
    {
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        currentframe = m;
        

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    player = moveObject(player,0,-1,m);
                break;
                case 80: case 's': ///baixo
                    player = moveObject(player,0,1,m);
                break;
                case 75:case 'a': ///esquerda
                    player = moveObject(player,-1,0,m);
                break;
                case 77: case 'd': ///direita
                    player = moveObject(player,1,0,m);
                break;
                case ' ':
                    if (!bomba.existe) colocaBomba(player,bomba);
                break;
            }
         }
        //------------------
        if (bomba.existe){
            bomba.tigger = clock();
            if ((bomba.tigger - bomba.set)/CLOCKS_PER_SEC == 2){
                explodirBomba(bomba, m);
            }
        }     


         SumMapItens(currentframe,player);
         if(bomba.existe) SumMapItens(currentframe,bomba.bomba);
         draw_map(currentframe);


    } //fim do laço do jogo

    return 0;
} //fim main
