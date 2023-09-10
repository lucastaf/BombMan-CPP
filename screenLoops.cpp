#include "gameFunctions.cpp"

//Define gamestatus ==>
#define InMenu 0
#define InGame 1
#define InMapEditor 2 
#define Defeat 3
#define Vicotry 4
#define tutorial 5
//
int Iditemselecionado = 0; //Id do item selecionado no editor de mapas
char gameStatus = InMenu; //gameStatus irá definir qual tela será renderizada

    map m;
    map currentframe = m;
    bomba bomba1;
    obj ghost = {0,0,ghostid};

    char tecla;

void MainGameLoop(){
    currentframe = m;
    SumMapItens(currentframe,player); // adiciona o player na tela
    for (int i = 0; i < QtdInimigos; i ++){
        if(inimigos[i].status) SumMapItens(currentframe,inimigos[i].inimigo); //Adiciona o inimigo se ele está vivo
    }
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
                case ' ': //Barra de espaço
                    if (!bomba1.status) colocaBomba(player,bomba1); //Coloca a bomba se ela não existe
                break;
            }
         }
        //------------------
        if (bomba1.status){ //se a bomba existe
            bomba1.trigger = clock();
            if ((bomba1.trigger - bomba1.set)/CLOCKS_PER_SEC == 2){ //Após 2 segundos, explode a bomba
                explodirBomba(bomba1, m); 
            }
        }     
        
        
        //PROGRAMAÇÃO DO INIMIGO
        for (int i = 0; i < QtdInimigos; i++){ //para cada um dos inimigos
        if (inimigos[i].status) //se inimigo esta vivo
        {
                if (Colide(inimigos[i].inimigo,currentframe,explosaoid)){ //se o inimigo colide com uma explosão
                    inimigos[i].status = 0; //ele morre
                    contInimigos --; //a quantidade de inimigos diminui
                }
                inimigos[i].trigger = clock();
                if (inimigos[i].status == 1) //se ele está parado
                {
                    inimigos[i].direcao = novaDirecaoInimigo(inimigos[i],currentframe); //gera uma nova direção
                    inimigos[i].numeroPassos = (rand()%3)+1; //o número de passos é aleatorio
                    inimigos[i].set = clock(); 
                    inimigos[i].status = 2; //o inimigo passa a se mover
                }else if (inimigos[i].status == 2) //se ele está se movendo
                {
                  if(inimigos[i].numeroPassos) //se ainda há passos a serem andados
                  {
                     if((inimigos[i].trigger - inimigos[i].set)/CLOCKS_PER_SEC == 1)
                      MoveInimigo(inimigos[i],currentframe);
                   }else{
                      inimigos[i].status = 1; //o inimigo passa a ficar parado
                    }
                }
            }
        }
        //FIM DO INIMIGO

        if(Colide(player,currentframe,explosaoid) || Colide(player,currentframe,inimigoid)){ 
            //se o player colide com uma explosão ou com um inimigo, ele perde
            gameStatus = Defeat;
            system("cls");
            return;
        }
        if(!contInimigos){
            //se não houverem mais inimigos, o player ganha
            gameStatus = Vicotry;
            system("cls");
            return;
        }

    draw_map(currentframe); //desenha o mapa


    //fim do laço do jogo
}

void GameOverLoop(char IsWinner){
    //a tela de fim de jogo serve para caso de vitoria e derrota
    if ( _kbhit() ){
        tecla = getch();
        if(tecla == 'r' || tecla == 27){
            system("cls");
            gameRestart(player,inimigos,m,bomba1);
            if(tecla == 'r')gameStatus = InGame;
            if(tecla == 27)gameStatus = InMenu; //esc
            return;
         }
    }
    
    cout << "\033[37m";
    cout << "\nGame Over \n\n";
    if(IsWinner){
        cout << "You Win!"<<"\n\n";
    }
    else{
        cout << "You Lose!" << "\n\n";
    }
    cout << "Press R to restart\n";
    cout << "Press Esc to menu";
}

void mainMenuLoop(){
    if ( _kbhit() ){
        tecla = getch();
        if(tecla == ' '){ //barra de espaço
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
        if(tecla == 'x'){
            system("cls");
            gameStatus = tutorial;
            return;
        }
    }
    cout << "\033[37m";
    cout << "\nBomberMan - C++ 1.0 \n\n";
    cout << "Press Space to Play"<<"\n\n";
    
    cout << "Press 'z' to Map Editor\n";
    cout << "Press 'x' to tutorial";

}

void mapEditorLoop(){

        currentframe = m;
        ///executa os movimentos
        if ( _kbhit() ){
            tecla = getch();
            switch(tecla)
            {
                case 72: case 'w': if (isInsideMap(ghost,0,-1)) ghost.y --; break; ///cima
                case 80: case 's': if(isInsideMap(ghost,0,1)) ghost.y ++; break; ///baixo
                case 75: case 'a':  if(isInsideMap(ghost,-1,0)) ghost.x --; break; ///esquerda
                case 77: case 'd': if(isInsideMap(ghost,1,0)) ghost.x ++; break; ///direita
                
                //A array objetos possui QtdInimigos+1 itens, logo QtdInimigos é o último ID da array
                case 'q': //seleciona o proximo item
                    if (Iditemselecionado>0){
                        Iditemselecionado--;
                    } else{
                        Iditemselecionado = QtdInimigos; //Ultimo ID
                    }
                break;
                case 'e': //seleciona o item anterior
                    if(Iditemselecionado<QtdInimigos){ //Id selecionado menor q o ultimo ID
                        Iditemselecionado++;
                    }else{
                        Iditemselecionado = 0;
                    }
                break;
                case ' ': //Bara de espaço, coloca o item selecionado
                    objetos[Iditemselecionado]->x = ghost.x;
                    objetos[Iditemselecionado]->y = ghost.y;
                break;
                case 'z': m.mapa[ghost.y][ghost.x] = vazioid; break;
                case 'x': m.mapa[ghost.y][ghost.x] = paredeid;break;
                case 'c': m.mapa[ghost.y][ghost.x] = paredefragilid; break;
                case 27: //ESC
                    gameRestart(player,inimigos,m,bomba1);
                    system("cls");
                    gameStatus = InMenu;
                    return;
                break;
                case 13: //Enter
                    //Define as variaveis padrao para as variaveis editadas
                    MapaInicial = m;
                    playerInicial = player;
                    for(int i = 0;i<QtdInimigos;i++){
                        inimigsoIniciais[i] = inimigos[i];
                    }
                    //reseta o game
                    gameRestart(player,inimigos,m,bomba1);
                    system("cls");
                    gameStatus = InMenu;
                    return;
                break;
            }
         }
        //------------------
        for(int i = 0; i <= QtdInimigos; i++){ //Qtdinimigos é o ultimo ID da array objetos
            currentframe = SumMapItens(currentframe,*objetos[i]);
        }
        currentframe = SumMapItens(currentframe,ghost);
        draw_hud(*objetos[Iditemselecionado],Iditemselecionado);
        draw_map(currentframe);
}

void tutorialLoop(){
    if ( _kbhit() ){
    tecla = getch();
    if(tecla == 27){ //ESC
            system("cls");
            gameStatus = InMenu;
            return;
    }
}
    cout << "\033[37m";
    cout << "\nO jogo de bomberman consiste em varios inimigos(em vermelho) e um player(em amarelo), \n";
    cout << "Para vencer o jogo, o jogador deve derrotar todos os inimigos,\n";
    cout << "Se o jogador encostar em um inimigo ou em sua bomba, ele perde\n";
    cout << "\nMova o player com wasd ou com as setinhas, coloque uma bomba utilizando barra de espaco\n";
    cout << "\n\nEditor de Mapas:\n\n";
    cout << "pressione Q e E para alternar entre os itens selecionados,\n";
    cout << "pressione Barra de Espaco para colocar um item\n";
    cout << "Os cenarios sao construidos com Z,X e C ->\n";
    cout << "Z = Caminho vazio\n";
    cout << "X = Parede solida\n";
    cout << "C = Parede fragil\n";
    cout << "\nPressione ESC para retornar ao menu";

}