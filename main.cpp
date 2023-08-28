/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/


#include "gameFunctions.cpp"


#define QtdObjetos 2



int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        srand(time(NULL));
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

    
    map m = {   4,4,4,4,4,4,4,4,4,4,4,4,4,0,4,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                4,0,4,0,4,0,4,4,4,4,4,4,4,0,0,
                4,0,4,0,4,0,4,4,4,4,4,4,4,0,0,
                4,0,4,0,4,5,4,4,4,4,4,4,4,5,4,
                4,0,4,0,0,0,4,4,4,4,4,4,4,0,4,
                4,0,4,0,4,0,4,4,4,4,4,0,0,0,4,
                4,0,4,0,4,0,4,4,4,4,4,0,0,0,4,
                4,0,0,0,0,0,0,0,5,0,0,0,0,0,4,
                4,4,4,0,0,0,4,4,4,4,4,4,4,0,4
    };

    map currentframe = m;


   
    obj player = {1,1,playerid};  //Posicao inicial do personagem no console
    inimigo inimigo = {12,12,inimigoid}; //posição inicial do inimigo no console
    bomba bomba =  {0,0,bombaid,0};
    for(int x=0;x<=raio*4;x++){
        bomba.explosao[0][x].id = explosaoid;
    }
    //Variavel para tecla precionada
    char tecla;
    
    while(true) //Loop principal do Jogo
    {
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        currentframe = m;
         SumMapItens(currentframe,player); // adiciona o player na tela
         if(bomba.status) SumMapItens(currentframe,bomba.bomba); //Adiciona a bomba se ela existe
         if(bomba.status == 2) SumMapExplosion(currentframe,bomba.explosao); //adiciona o raio da explosão se ela explodiu
         if(inimigo.status) SumMapItens(currentframe,inimigo.inimigo); //Adiciona o inimigo se ele está vivo

        ///executa os movimentos
         if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    player = moveObject(player,0,-1,currentframe);
                break;
                case 80: case 's': ///baixo
                    player = moveObject(player,0,1,currentframe);
                break;
                case 75:case 'a': ///esquerda
                    player = moveObject(player,-1,0,currentframe);
                break;
                case 77: case 'd': ///direita
                    player = moveObject(player,1,0,currentframe);
                break;
                case ' ':
                    if (!bomba.status) colocaBomba(player,bomba);
                break;
            }
         }
        //------------------
        if (bomba.status){
            bomba.trigger = clock();
            if ((bomba.trigger - bomba.set)/CLOCKS_PER_SEC == 2){
                explodirBomba(bomba, m);
            }
        }     
        
        
        if (inimigo.status) //se inimigo esta vivo
        {
            if (inimigo.status == 1 && (inimigo.trigger - inimigo.set)/CLOCKS_PER_SEC == 1)
            {
                inimigo.direcao = novaDirecaoInimigo(inimigo,currentframe);
                inimigo.numeroPassos = (rand()%10)+1;
                inimigo.status = 2;
            }else{
                inimigo.trigger = clock();
                if(inimigo.numeroPassos) MoveInimigo(inimigo,currentframe);
            }
            
        }



         draw_map(currentframe);


    } //fim do laço do jogo

    return 0;
} //fim main
