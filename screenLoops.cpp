#include "gameFunctions.cpp"

char gameStatus;
//Define gamestatus ==>
#define InGame 0
#define Defeat 1
#define Vicotry 2
//

    map m
    ;
    map currentframe = m;
    obj player = playerInicial;  //Posicao inicial do personagem no consoled
    bomba bomba1;
    inimigo inimigo1 = inimigoInicial; //posição inicial do inimigo no console
    char tecla;

void MainGameLoop(){
        currentframe = m;
         SumMapItens(currentframe,player); // adiciona o player na tela
         if(inimigo1.status) SumMapItens(currentframe,inimigo1.inimigo); //Adiciona o inimigo se ele está vivo
         if(bomba1.status) SumMapItens(currentframe,bomba1.bomba); //Adiciona a bomba se ela existe
         if(bomba1.status == 2) SumMapExplosion(currentframe,bomba1.explosao); //adiciona o raio da explosão se ela explodiu
        
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
                    if (!bomba1.status) colocaBomba(player,bomba1);
                break;
            }
         }
        //------------------
        if (bomba1.status){
            bomba1.trigger = clock();
            if ((bomba1.trigger - bomba1.set)/CLOCKS_PER_SEC == 2){
                explodirBomba(bomba1, m);
            }
        }     
        
        
        //PROGRAMAÇÃO DO INIMIGO
        if (inimigo1.status) //se inimigo esta vivo
        {
            if (Colide(inimigo1.inimigo,currentframe,explosaoid)){
                inimigo1.status = 0;
                contInimigos --;
            }
            inimigo1.trigger = clock();
            if (inimigo1.status == 1) 
            {
                inimigo1.direcao = novaDirecaoInimigo(inimigo1,currentframe);
                inimigo1.numeroPassos = (rand()%3)+1;
                inimigo1.set = clock();
                inimigo1.status = 2;
            }else if (inimigo1.status == 2){
                if(inimigo1.numeroPassos) {
                    if((inimigo1.trigger - inimigo1.set)/CLOCKS_PER_SEC == 1)
                    MoveInimigo(inimigo1,currentframe);
                }else{
                    inimigo1.status = 1;
                }
            }
        }
        //FIM DO INIMIGO

        if(Colide(player,currentframe,explosaoid) || Colide(player,currentframe,inimigoid)){
            gameStatus = Defeat;
            system("cls");
            return;
        }
        if(!contInimigos){
            gameStatus = Vicotry;
            system("cls");
            return;
        }

    draw_map(currentframe);


    //fim do laço do jogo
}

void GameOverLoop(char IsWinner){
    
    if ( _kbhit() ){
        tecla = getch();
        if(tecla == 'r'){
            system("cls");
            gameRestart(player,inimigo1,m,bomba1);
            gameStatus = InGame;
            return;
         }
    }

    cout << "\nGame Over \n\n";
    if(IsWinner){
        cout << "You Win!"<<"\n\n";
    }
    else{
        cout << "You Lose!" << "\n\n";
    }
    cout << "Press R to restart";
}