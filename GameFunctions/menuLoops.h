#include "fileFunctions.h"
// Define gamestatus ==>
#define InMenu 0
#define InGame 1
#define InMapEditor 2
#define InMapEditorConfig 3
#define Defeat 4
#define Vicotry 5
#define tutorial 6
#define fileScreen 7
//
char gameStatus = InMenu; // gameStatus irá definir qual tela será renderizada

gameState defaultGame;
gameState currentGame;
map currentframe;
obj ghost = {0, 0, ghostid};

void GameOverLoop(char IsWinner)
{
    // a tela de fim de jogo serve para caso de vitoria e derrota
    if (_kbhit())
    {
        char tecla = getch();
        if (tecla == 'r' || tecla == escKey)
        {
            system("cls");
            currentGame.Restart(defaultGame, false, true);
            if (tecla == 'r')
                gameStatus = InGame;
            if (tecla == 27)
                gameStatus = InMenu; // esc
            return;
        }
    }

    cout << "\033[37m";
    cout << "\nGame Over \n\n";
    if (IsWinner)
    {
        cout << "You Win!"
             << "\n\n";
    }
    else
    {
        cout << "You Lose!"
             << "\n\n";
    }
    cout << "Press R to restart\n";
    cout << "Press Esc to menu";
}

void mainMenuLoop()
{
    if (_kbhit())
    {
        char tecla = getch();
        switch (tecla)
        {
        case ' ': // barra de espaço
            system("cls");
            gameStatus = InGame;
            currentGame.unpause();
            return;
            break;
        case 'z': // barra de espaço
            system("cls");
            gameStatus = InGame;
            currentGame.Restart(defaultGame,false,true);
            return;
            break;
        case 'x':
            system("cls");
            gameStatus = InMapEditor;
            return;
            break;
        case 'c':
            system("cls");
            gameStatus = tutorial;
            return;
            break;
        case 'v':
            system("cls");
            gameStatus = fileScreen;
            return;
            break;
        }
    }

    cout << "\033[37m";
    cout << "\nBomberMan - C++ 1.0 \n\n";
    cout << "Press Space to Continue \n";
    cout << "Press Z to New Game";
    cout << "\n\n";

    cout << "Press 'x' to Map Editor\n";
    cout << "Press 'c' to tutorial\n";
    cout << "Press 'v' to import a file";
}

void tutorialLoop()
{
    if (_kbhit())
    {
        char tecla = getch();
        if (tecla == 27)
        { // ESC
            system("cls");
            gameStatus = InMenu;
            return;
        }
    }
    cout << "\033[37m";
    cout << "Trabalho feito por Lucas Bittencourt Rauch e Milca Leite Pereira Barreto \n";
    cout << "\nO jogo de bomberman consiste em varios inimigos(em vermelho) e um player(em amarelo), \n";
    cout << "Para vencer o jogo, o jogador deve derrotar todos os inimigos,\n";
    cout << "Se o jogador encostar em um inimigo ou em sua bomba, ele perde\n";
    cout << "\nMova o player com as setinhas, coloque uma bomba utilizando barra de espaco\n";
    cout << "\n\nEditor de Mapas:\n\n";
    cout << "pressione Q e E para alternar entre os itens selecionados,\n";
    cout << "pressione Barra de Espaco para colocar o item no cenario\n";
    cout << "Os objetos nao estaticos sao selecionados utilizando Z e C\n";
    cout << "Coloque um objeto utilizando a tecla X\n";
    cout << "Pressionando a tecla T voce pode abrir o menu de configuracoes \n";
    cout << "O menu de configuracoes te permite alterar os tamanhos e as propriedades dos objetos\n";
    cout << "\nPressione ESC para retornar ao menu";
}

void fileLoop()
{
    char escolha;
    string arquivo;
    cout << "Seletor de arquivos >> Digite 1 para importar e 2 para exportar um arquivo,\n";
    cout << "Entre qualquer outro valor para retornar ao menu \n";
    cin >> escolha;

    switch (escolha)
    {
    case '1':
        cout << "Digite o nome do arquivo que deseja importar \n";
        cin >> arquivo;
        if(importFile(arquivo, defaultGame)){
            currentGame.Restart(defaultGame, true, true);
            currentframe.resize(currentGame.mapa.sizeX, currentGame.mapa.sizeY);
            for(int i = 0; i < currentGame.QtdPlayers; i++){
            if(currentGame.players[i].bomba.status == 2){
                currentGame.players[i].bomba.explodirBomba(currentGame.mapa,true);
                defaultGame.players[i].bomba.status = 0;
            }
        }
        system("cls");
    }
        gameStatus = InMenu;
        return;
        break;
    case '2':
        cout << "Digite o nome do arquivo que sera gerado \n";
        cin >> arquivo;
        exportFile(arquivo, currentGame);
        system("cls");
        gameStatus = InMenu;
        return;
        break;
    default:
        system("cls");
        gameStatus = InMenu;
        return;
        break;
    }
}