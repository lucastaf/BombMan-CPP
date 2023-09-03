/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/


#include "gameFunctions.cpp"






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

    
    

    map m = MapaInicial;

    map currentframe = m;



    obj player = playerInicial;  //Posicao inicial do personagem no console
    
    bomba bomba =  {0,0,bombaid,0};
    for(int x=0;x<=raio*4;x++){
        //estabelece todas as explosões da bomba com um ID de uma explosão
        bomba.explosao[0][x].id = explosaoid;
    }
    inimigo inimigo = inimigoInicial; //posição inicial do inimigo no console
    inimigo.set = clock();
    //Variavel para tecla precionada
    char tecla;
    
    while(true) //Loop principal do Jogo
    {
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        
        currentframe = m;
         SumMapItens(currentframe,player); // adiciona o player na tela
         if(inimigo.status) SumMapItens(currentframe,inimigo.inimigo); //Adiciona o inimigo se ele está vivo
         if(bomba.status) SumMapItens(currentframe,bomba.bomba); //Adiciona a bomba se ela existe
         if(bomba.status == 2) SumMapExplosion(currentframe,bomba.explosao); //adiciona o raio da explosão se ela explodiu
        
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
        
        
        //PROGRAMAÇÃO DO INIMIGO
        if (inimigo.status) //se inimigo esta vivo
        {
            if (Colide(inimigo.inimigo,currentframe,explosaoid)) inimigo.status = 0;
            inimigo.trigger = clock();
            if (inimigo.status == 1) 
            {
                inimigo.direcao = novaDirecaoInimigo(inimigo,currentframe);
                inimigo.numeroPassos = (rand()%3)+1;
                inimigo.set = clock();
                inimigo.status = 2;
            }else if (inimigo.status == 2){
                if(inimigo.numeroPassos) {
                    if((inimigo.trigger - inimigo.set)/CLOCKS_PER_SEC == 1)
                    MoveInimigo(inimigo,currentframe);
                }else{
                    inimigo.status = 1;
                }
            }
        }
        //FIM DO INIMIGO


        if(Colide(player,currentframe,explosaoid) || Colide(player,currentframe,inimigoid)){
            gameRestart(player,inimigo,m,bomba);
        }

         draw_map(currentframe);


    } //fim do laço do jogo

    return 0;
} //fim main
