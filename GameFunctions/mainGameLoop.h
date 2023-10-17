#include "mapEditorLoop.h"



char tecla;

void MainGameLoop()
{
    currentframe.copyMap(currentGame.mapa);

    currentGame.AddItensToMap(currentframe);

    /// executa os movimentos
    if (_kbhit()) currentGame.KeyboardHitActions(currentframe);
    //------------------

    currentGame.bombFrameAction(currentframe);

    currentGame.playerFrameAction(currentframe);

    currentGame.inimigosFrameAction(currentframe);
    
    if (!currentGame.contPlayers){
        //se não há mais players, é game over
        gameStatus = Defeat;
        system("cls");
        return;
    }
    
    if (!currentGame.contInimigos)
    {
        // se não houverem mais inimigos, o player ganha
        gameStatus = Vicotry;
        system("cls");
        return;
    }

    currentframe.draw_map(); // desenha o mapa

    // fim do laço do jogo
}

