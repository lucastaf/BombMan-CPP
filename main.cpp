/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)

    Trabalho Bomberman feito por:
    Lucas Bittencourt Rauch
    Milca Leite Pereira Barreto

    Link do Github: https://github.com/lucastaf/BombMan-CPP

    Modificações propositais:
-O player é capaz de colocar uma bomba em cima de mesmo, mas não é capaz de andar por cima da bomba depois disso,
essa era a mecanica do game original
*/

#include "GameFunctions/screenLoops.h"

int main()
{
    /// ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
    // INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
    srand(time(NULL));
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
    // FIM: COMANDOS PARA QUE O CURSOR NÃO FIQUE PISCANDO NA TELA
    // INÍCIO: COMANDOS PARA REPOSICIONAR O CUSOR NO INÍCIO DA TELA
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    // FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    /// ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.
    if (QtdInimigos > ((sizex + 1) / 2) * ((sizey + 1) / 2) - 1)
        return 0; // essa linha serve para crashar o jogo propositalmente caso o número de inimigos seja maior que a qunatidade de espaços livres

    gerarMapaInicial(MapaInicial);
    gerarInimigosIniciais(inimigsoIniciais, inimigos);
    preencherObjetos();
    m = MapaInicial;

    while (true) // Loop principal do Jogo
    {
        /// Posiciona a escrita no início do console
        SetConsoleCursorPosition(out, coord);
        switch (gameStatus)
        {
        case InMenu:
            mainMenuLoop();
            break;
        case InGame:
            MainGameLoop();
            break;
        case Defeat:
            GameOverLoop(0);
            break;
        case Vicotry:
            GameOverLoop(1);
            break;
        case InMapEditor:
            mapEditorLoop();
            break;
        case tutorial:
            tutorialLoop();
            break;
        case fileScreen:
            fileLoop();
            break;
        }

    } // fim do loop
    return 0;
}
