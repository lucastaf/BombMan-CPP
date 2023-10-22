#include "classes/player.h"

// obj player = playerInicial; // Posicao inicial do personagem no consoled

void SumMapExplosion(bomba bomba, map &currentframe)
{
    // Essa função adiciona todos as instancias de uma explosão,
    // as explosões são colocadas dentro de uma matriz dentro da struct bomba
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < bomba.raio; j++)
        {
            currentframe.mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] = bomba.explosao[i][j].id;
        }
    }
}

struct gameState
{
    int QtdPlayers = 1;
    int QtdInimigos = 2;
    int contInimigos = QtdInimigos;
    int contPlayers = QtdPlayers;
    map mapa;
    inimigo *inimigos = new inimigo[QtdInimigos];
    player *players = new player[QtdPlayers];
    obj **objetos = new obj *[QtdInimigos + QtdPlayers]; // A array com todos os objetos será útil para o editor de mapas
    gameState()
    {
        // preencherObjetos();
    }

    void Restart(gameState gameOriginal, bool resizeNeeded = false, bool resetArrays = false)
    {

        // Define todas as variaveis para as condições inicias
        if (resizeNeeded)
        {
            resizePlayers(gameOriginal.QtdPlayers);
            resizeInimigos(gameOriginal.QtdInimigos);
            for (int i = 0; i < QtdPlayers; i++)
            {
                players[i].resize(gameOriginal.players[i]);
            }
        }
        QtdPlayers = gameOriginal.QtdPlayers;
        QtdInimigos = gameOriginal.QtdInimigos;
        mapa.copyMap(gameOriginal.mapa, resizeNeeded);
        contInimigos = QtdInimigos;
        contPlayers = QtdPlayers;
        for (int i = 0; i < QtdPlayers; i++)
        {
            players[i].copy(gameOriginal.players[i]);
        }
        for (int i = 0; i < QtdInimigos; i++)
        {
            inimigos[i] = gameOriginal.inimigos[i];
        }

        if (resetArrays)
        {
            for (int i = 0; i < QtdPlayers; i++)
            {
                players[i].deletePlayer();
                players[i].createPlayer();
            }
        }
    }

    void gerarInimigos()
    {
        bool isNewSpace;
        int x, y;
        // Essa função gera inimigos dentro dos espaço em branco que sobraram do mapa,
        // e testa se o local gerado não havia spawnado outro inimigo anteriormente
        for (int i = 0; i < QtdInimigos; i++)
        {

            do
            {
                isNewSpace = true;

                x = (rand() % ((mapa.sizeX + 1) / 2)) * 2;
                y = (rand() % ((mapa.sizeY + 1) / 2)) * 2;
                if (x == 0 && y == 0)
                { // 0,0 é o spawn do player
                    isNewSpace = false;
                }
                for (int j = 0; j < i; j++)
                {
                    if (x == inimigos[j].objeto.x && y == inimigos[j].objeto.y)
                    {
                        isNewSpace = false;
                    }
                }
            } while (!isNewSpace);
            inimigos[i].objeto.x = x;
            inimigos[i].objeto.y = y;
        }
    }

    void preencherObjetos()
    {
        delete[] objetos;
        objetos = new obj *[QtdInimigos + QtdPlayers];
        // Preenche os ponteiros da array objetos com os endereços do player e de todos inimigos;
        for (int i = 0; i < QtdPlayers; i++)
        {
            // Do ID 0 até o ID qtd de players
            objetos[i] = &players[i].objeto;
        }
        for (int i = QtdPlayers; i < QtdPlayers + QtdInimigos; i++)
        {
            // Do ID após a quantidade de players ate o ulitmo ID(Players+inimigos);
            objetos[i] = &inimigos[i - QtdPlayers].objeto;
        }
    }

    void playerFrameAction(map currentframe)
    {
        for (int i = 0; i < QtdPlayers; i++)
        {
            if (players[i].objeto.Colide(currentframe, explosaoid) || players[i].objeto.Colide(currentframe, inimigoid))
            {
                players[i].status = 0;
                contPlayers--;
            }
            if (players[i].objeto.Colide(mapa, ghostPowerupId))
            {
                mapa.mapa[players[i].objeto.y][players[i].objeto.x] = vazioid;
                players[i].ghostPowerup = true;
            }
            if (players[i].objeto.Colide(mapa, bombExpanderId))
            {
                mapa.mapa[players[i].objeto.y][players[i].objeto.x] = vazioid;
                players[i].expandbomb();
            }
            if(players[i].objeto.Colide(mapa,ghostBombId)){
                mapa.mapa[players[i].objeto.y][players[i].objeto.x] = vazioid;
                players[i].bomba.atravessaParede = true;
            }
        }
    }

    void inimigosFrameAction(map currentframe)
    {
        // PROGRAMAÇÃO DO INIMIGO
        for (int i = 0; i < QtdInimigos; i++)
        {                           // para cada um dos inimigos
            if (inimigos[i].status) // se inimigo esta vivo
            {
                if (inimigos[i].objeto.Colide(currentframe, explosaoid))
                {                           // se o inimigo colide com uma explosão
                    inimigos[i].status = 0; // ele morre
                    contInimigos--;         // a quantidade de inimigos diminui
                }
                inimigos[i].trigger = clock();
                if (inimigos[i].status == 1) // se ele está parado
                {
                    inimigos[i].novaDirecaoInimigo(currentframe); // gera uma nova direção
                    inimigos[i].numeroPassos = (rand() % 3) + 1;  // o número de passos é aleatorio
                    inimigos[i].set = clock();
                    inimigos[i].status = 2; // o inimigo passa a se mover
                }
                else if (inimigos[i].status == 2) // se ele está se movendo
                {
                    if (inimigos[i].numeroPassos) // se ainda há passos a serem andados
                    {
                        if ((inimigos[i].trigger - inimigos[i].set) / CLOCKS_PER_SEC == inimigos[i].tempoDePasso)
                            inimigos[i].move(currentframe);
                    }
                    else
                    {
                        inimigos[i].status = 1; // o inimigo passa a ficar parado
                    }
                }
            }
        }
        // FIM DO INIMIGO
    }

    void bombFrameAction(map currentframe)
    {
        for (int i = 0; i < QtdPlayers; i++)
        {
            if (players[i].bomba.status)
            { // se a bomba existe
                players[i].bomba.trigger = clock();
                if ((players[i].bomba.trigger - players[i].bomba.set) / CLOCKS_PER_SEC == 2)
                { // Após 2 segundos, explode a bomba
                    players[i].bomba.explodirBomba(mapa);
                }
            }
        }
    }

    void KeyboardHitActions(map currentframe, char tecla)
    {
        for (int i = 0; i < QtdPlayers; i++)
        {
            players[i].keyPress(tecla, currentframe);
        }
    }

    void AddItensToMap(map &currentframe)
    {
        for (int i = 0; i < QtdPlayers; i++)
        {
            if (players[i].status)
                currentframe.SumItens(players[i].objeto.toCore()); // adiciona o player na tela
        }
        for (int i = 0; i < QtdInimigos; i++)
        {
            if (inimigos[i].status)
                currentframe.SumItens(inimigos[i].objeto.toCore()); // Adiciona o inimigo se ele está vivo
        }
        for (int i = 0; i < QtdPlayers; i++)
        {
            if (players[i].bomba.status)
                currentframe.SumItens(players[i].bomba.objeto.toCore()); // Adiciona a bomba se ela existe
            if (players[i].bomba.status == 2)
                SumMapExplosion(players[i].bomba, currentframe); // adiciona o raio da explosão se ela explodiu
        }
    }

    void resizeMap(int newX, int newY)
    {
        mapa.resize(newX, newY);
        preencherObjetos();
        for (int i = 0; i < QtdInimigos + QtdPlayers; i++)
        { // Joga os objetos que foram jogados para fora do mapa de volta para a borda
            if (objetos[i]->x >= newX)
            {
                objetos[i]->x = newX - 1;
            }
            if (objetos[i]->y >= newY)
            {
                objetos[i]->y = newY - 1;
            }
        }
    }

    void resizePlayers(int newQtd)
    {
        player *oldplayers = players;
        players = new player[newQtd];
        int oldQtd = QtdPlayers;
        QtdPlayers = newQtd;
        for (int i = 0; i < newQtd; i++)
        {
            if (i < oldQtd)
            {
                players[i] = oldplayers[i];
            }
        }
        for (int i = newQtd; i < oldQtd; i++)
        {
            oldplayers[i].deletePlayer();
        }
        delete[] oldplayers;
        preencherObjetos();
    }

    void resizeInimigos(int newQtd)
    {
        inimigo *oldInimigos = inimigos;
        inimigos = new inimigo[newQtd];
        int oldQtd = QtdInimigos;
        QtdInimigos = newQtd;
        for (int i = 0; i < newQtd; i++)
        {
            if (i < oldQtd)
            {
                inimigos[i] = oldInimigos[i];
            }
        }
        delete[] oldInimigos;
        preencherObjetos();
    }

    void pause(){
        for(int i = 0; i < QtdPlayers; i ++){
            players[i].bomba.set = players[i].bomba.trigger - players[i].bomba.set;
        }
        for(int i = 0; i < QtdInimigos; i++){
            inimigos[i].set = players[i].bomba.trigger - inimigos[i].set;
        }
    }


    void unpause(){
        for(int i = 0; i < QtdPlayers; i ++){
            players[i].bomba.set = clock() - players[i].bomba.set;
        }
        for(int i = 0; i < QtdInimigos; i++){
            inimigos[i].set = clock();
        }
    }
};
