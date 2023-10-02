#include "classes/player.h"

//obj player = playerInicial; // Posicao inicial do personagem no consoled

bool Colide(obj objeto, map mapa, int id)
{
    // Verifica se um objeto colide com outro objeto dentro do mapa
    if (mapa.mapa[objeto.y][objeto.x] == id)
    { // Se o x e y do mapa naquele ponto for igual ao id enviado
        return true;
    }
    return false;
}

struct gameState
{
    int QtdPlayers = 1;
    int QtdInimigos = 2;
    int contInimigos = QtdInimigos;
    map mapa;
    inimigo *inimigos;
    player *players = new player[1];
    obj *objetos = new obj[QtdInimigos + QtdPlayers]; // A array com todos os objetos será útil para o editor de mapas
    gameState()
    {
        preencherObjetos();
    }

    void Restart(gameState gameOriginal)
    {
        // Define todas as variaveis para as condições inicias
        QtdPlayers = gameOriginal.QtdPlayers;
        QtdInimigos = gameOriginal.QtdInimigos;
        mapa = gameOriginal.mapa;
        contInimigos = QtdInimigos;

        players = new player[QtdPlayers];
        inimigos = new inimigo[QtdInimigos];
        for (int i = 0; i < QtdPlayers; i++)
        {
            players[i] = gameOriginal.players[i];
        }
        for (int i = 0; i < QtdInimigos; i ++){
            inimigos[i] = gameOriginal.inimigos[i];
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

    void gerarMapa()
    {
        int i, j;
        // o mapa é gerado formando com paredes padronizadas e paredesfrageis de forma aleatoria
        for (i = 0; i < mapa.sizeY; i++)
        {
            for (j = 0; j < mapa.sizeX; j++)
            {
                if (!isEven(i) && !isEven(j))
                {
                    mapa.mapa[i][j] = paredeid;
                }
                if (isEven(i) != isEven(j))
                {
                    if (!(rand() % 3))
                        mapa.mapa[i][j] = paredefragilid;
                }
            }
        }
    };

    void preencherObjetos()
    {
        // Preenche os ponteiros da array objetos com os endereços do player e de todos inimigos;
        for (int i = 0; i < QtdPlayers; i++){
            //Do ID 0 até o ID qtd de players
            objetos[i] = players[i].objeto;
        }
        for (int i = QtdPlayers - 1; i < QtdPlayers-1 + QtdInimigos ; i++)
        {
            //Do ID após a quantidade de players ate o ulitmo ID(Players+inimigos);
            objetos[i] = inimigos[i].objeto;
        }
    }
};