#include "classes/player.h"

//obj player = playerInicial; // Posicao inicial do personagem no consoled



struct gameState
{
    int QtdPlayers = 1;
    int QtdInimigos = 2;
    int contInimigos = QtdInimigos;
    int contPlayers = QtdPlayers;
    map mapa;
    inimigo *inimigos = new inimigo[QtdInimigos];
    player *players = new player[QtdPlayers];
    obj **objetos; // A array com todos os objetos será útil para o editor de mapas
    gameState()
    {
        //preencherObjetos();
    }

    void Restart(gameState gameOriginal)
    {
        // Define todas as variaveis para as condições inicias
        delete players;
        delete inimigos;
        QtdPlayers = gameOriginal.QtdPlayers;
        QtdInimigos = gameOriginal.QtdInimigos;
        mapa.copyMap(gameOriginal.mapa);
        contInimigos = QtdInimigos;
        contPlayers = QtdPlayers;
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

    void preencherObjetos()
    {
        delete []objetos;
        objetos = new obj*[QtdInimigos + QtdPlayers];
        // Preenche os ponteiros da array objetos com os endereços do player e de todos inimigos;
        for (int i = 0; i < QtdPlayers; i++){
            //Do ID 0 até o ID qtd de players
            objetos[i] = &players[i].objeto;
        }
        for (int i = QtdPlayers; i < QtdPlayers + QtdInimigos; i++)
        {
            //Do ID após a quantidade de players ate o ulitmo ID(Players+inimigos);
            objetos[i] = &inimigos[i - QtdPlayers].objeto;
        }
    }

    void inimigosFrameAction(map currentframe){
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
                inimigos[i].numeroPassos = (rand() % 3) + 1;                         // o número de passos é aleatorio
                inimigos[i].set = clock();
                inimigos[i].status = 2; // o inimigo passa a se mover
            }
            else if (inimigos[i].status == 2) // se ele está se movendo
            {
                if (inimigos[i].numeroPassos) // se ainda há passos a serem andados
                {
                    if ((inimigos[i].trigger - inimigos[i].set) / CLOCKS_PER_SEC == 1)
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

    void KeyboardHitActions(map currentframe){
        char tecla = getch();
        switch (tecla)
        {
        case 72:
        case 'w': /// cima
            players[0].objeto.move(0, -1, currentframe);
            break;
        case 80:
        case 's': /// baixo
            players[0].objeto.move(0, 1, currentframe);
            break;
        case 75:
        case 'a': /// esquerda
            players[0].objeto.move(-1, 0, currentframe);
            break;
        case 77:
        case 'd': /// direita
            players[0].objeto.move(1, 0, currentframe);
            break;
        case ' ': // Barra de espaço
            if (!players[0].bomba.status)
                players[0].colocabomba(); // Coloca a bomba se ela não existe
            break;
        }
    }

    void AddItensToMap(map &currentframe){
            currentframe.SumItens(players[0].objeto.toCore()); // adiciona o player na tela
    for (int i = 0; i < QtdInimigos; i++)
    {
        if (inimigos[i].status)
            currentframe.SumItens(inimigos[i].objeto.toCore()); // Adiciona o inimigo se ele está vivo
    }
    if (players[0].bomba.status)
        currentframe.SumItens(players[0].bomba.objeto.toCore()); // Adiciona a bomba se ela existe
    if (players[0].bomba.status == 2)
        SumExplosion(players[0].bomba, currentframe); // adiciona o raio da explosão se ela explodiu
    }

    void SumExplosion(bomba bomba, map &currentframe)
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

    void resizeMap(int newX, int newY){
        mapa.resize(newX, newY);
        preencherObjetos();
        for(int i = 0; i < QtdInimigos + QtdPlayers; i ++){
            if (objetos[i]->x >= newX){
                objetos[i]->x = newX - 1;
            }
            if(objetos[i]->y >= newY){
                objetos[i]->y = newY - 1;
            }
        }
    }
};