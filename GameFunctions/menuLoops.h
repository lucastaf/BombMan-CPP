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
        if (tecla == 'r' || tecla == 27)
        {
            system("cls");
            currentGame.Restart(defaultGame);
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
            currentGame.Restart(defaultGame);
            gameStatus = InGame;
            return;
            break;
        case 'z':
            system("cls");
            gameStatus = InMapEditor;
            return;
            break;
        case 'x':
            system("cls");
            gameStatus = tutorial;
            return;
            break;
        case 'c':
            system("cls");
            gameStatus = fileScreen;
            return;
            break;
        }
    }

    cout << "\033[37m";
    cout << "\nBomberMan - C++ 1.0 \n\n";
    cout << "Press Space to Play"
         << "\n\n";

    cout << "Press 'z' to Map Editor\n";
    cout << "Press 'x' to tutorial\n";
    cout << "Press 'c' to import a file";
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
        // importFile(arquivo);
        system("cls");
        // gameRestart(player, inimigos, m, bomba1);
        gameStatus = InMenu;
        return;
        break;
    case '2':
        cout << "Digite o nome do arquivo que sera gerado \n";
        cin >> arquivo;
        // exportFile(arquivo);
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