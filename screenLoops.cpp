#include "gameFunctions.cpp"

//Define gamestatus ==>
#define InMenu 0
#define InGame 1
#define InMapEditor 2 
#define Defeat 3
#define Vicotry 4
//
char gameStatus = InMenu;

    map m;
    map currentframe = m;
    obj player = playerInicial;  //Posicao inicial do personagem no consoled
    bomba bomba1;
    inimigo inimigos[QtdInimigos];
    char tecla;

void MainGameLoop(){
    currentframe = m;
    SumMapItens(currentframe,player); // adiciona o player na tela
    if(bomba1.status) SumMapItens(currentframe,bomba1.bomba); //Adiciona a bomba se ela existe
    if(bomba1.status == 2) SumMapExplosion(currentframe,bomba1.explosao); //adiciona o raio da explosão se ela explodiu
    for (int i = 0; i < QtdInimigos; i ++){
        if(inimigos[i].status) SumMapItens(currentframe,inimigos[i].inimigo); //Adiciona o inimigo se ele está vivo
    }
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
        for (int i = 0; i < QtdInimigos; i++){
        if (inimigos[i].status) //se inimigo esta vivo
        {
                if (Colide(inimigos[i].inimigo,currentframe,explosaoid)){
                    inimigos[i].status = 0;
                    contInimigos --;
                }
                inimigos[i].trigger = clock();
                if (inimigos[i].status == 1) 
                {
                    inimigos[i].direcao = novaDirecaoInimigo(inimigos[i],currentframe);
                    inimigos[i].numeroPassos = (rand()%3)+1;
                    inimigos[i].set = clock();
                    inimigos[i].status = 2;
                }else if (inimigos[i].status == 2){
                  if(inimigos[i].numeroPassos) {
                     if((inimigos[i].trigger - inimigos[i].set)/CLOCKS_PER_SEC == 1)
                      MoveInimigo(inimigos[i],currentframe);
                   }else{
                      inimigos[i].status = 1;
                    }
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
        if(tecla == 'r' || tecla == 27){
            system("cls");
            gameRestart(player,inimigos,m,bomba1);
            if(tecla == 'r')gameStatus = InGame;
            if(tecla == 27)gameStatus = InMenu;
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

void mainMenuLoop(){
    if ( _kbhit() ){
        tecla = getch();
        if(tecla == ' '){
            system("cls");
            gameRestart(player,inimigos,m,bomba1);
            gameStatus = InGame;
            return;
         }
        if(tecla == 'z'){
            system("cls");
            gameStatus = InMapEditor;
            return;
        }
    }

    cout << "\nBomberMan - C++ 1.0 \n\n";
        cout << "Press Space to Play"<<"\n\n";
    
    cout << "Press 'z' to Map Editor";

}

void mapEditorLoop(){

        currentframe = m;
        ///executa os movimentos
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': ///cima
                    if (isInsideMap(player,0,-1)) player.y --;
                break;
                case 80: case 's': ///baixo
                    if(isInsideMap(player,0,1)) player.y ++;
                break;
                case 75:case 'a': ///esquerda
                    if(isInsideMap(player,-1,0)) player.x --;
                break;
                case 77: case 'd': ///direita
                    if(isInsideMap(player,1,0)) player.x ++;
                break;
                case 'z':
                    m.mapa[player.y][player.x] = vazioid;
                break;
                case 'x':
                    m.mapa[player.y][player.x] = paredeid;
                break;
                case 'c':
                    m.mapa[player.y][player.x] = paredefragilid;
                break;
                case 27: //ESC
                    gameRestart(player,inimigos,m,bomba1);
                    system("cls");
                    gameStatus = InMenu;
                    return;
                break;
                case 13: //Enter
                    MapaInicial = m;
                    gameRestart(player,inimigos,m,bomba1);
                    system("cls");
                    gameStatus = InMenu;
                    return;
                break;
            }
         }
        //------------------

        currentframe = SumMapItens(currentframe,player);
        draw_map(currentframe);
}