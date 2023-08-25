/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/


#include "matrixFunctions.cpp"


#define QtdObjetos 2



int main()
{
    ///ALERTA: N�O MODIFICAR O TRECHO DE C�DIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR N�O FIQUE PISCANDO NA TELA
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
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO IN�CIO DA TELA
    ///ALERTA: N�O MODIFICAR O TRECHO DE C�DIGO, ACIMA.

    map m = {   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,0,0,0,1,1,1,1,1,1,1,0,1,
                1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
                1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,
                1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    };


    //Posi��o inicial do personagem no console
    obj player = {1,1,2};
    //Vari�vel para tecla precionada
    char tecla;
    
    while(true){
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        

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
            }
         }

         draw_map(SumMapItens(m,player));


    } //fim do laço do jogo

    return 0;
} //fim main
