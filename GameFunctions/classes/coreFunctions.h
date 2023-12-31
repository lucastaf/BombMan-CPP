#include "defines.h"

// Cada objeto do cenario possui um ID proprio, esses IDs são categorizados e a programação do jogo gira em torno deles//
// Player e inimigo não possuem colisão, eles podem ser atravessado, isso garante q seja detectado quando o player ou inimigo colidem um com o outro
// Note que os objetos com colisão possuem um ID superior ao PlayerID, isso será útil no futuro para realizar testes de colisão
struct objcore
{
    int x;
    int y;
    int id;
};

void drawPixel(int id)
{
    switch (id)
    { // Desenha o id daquele objeto
    case vazioid:
        cout << "\033[32m" << char(219) << "\u001b[0m";
        break; // caminho - Verde
    case explosaoid:
        cout << "\033[31m"
             << "#"
             << "\u001b[0m";
        break; // explosao - Laranja
    case inimigoid:
        cout << "\033[31m" << char(219) << "\u001b[0m";
        break; // inimigo - Vermelho
    case playerid:
        cout << "\033[33m" << char(219) << "\u001b[0m";
        break; // Player - Amarelo
    case paredeid:
        cout << "\033[97m" << char(219) << "\u001b[0m";
        break; // parede - cinza
    case paredefragilid:
        cout << "\033[37m" << char(219) << "\u001b[0m";
        break; // parede fragil - branca
    case bombaid:
        cout << "\033[30m" << char(219) << "\u001b[0m";
        break; // bomba - Preta
    case ghostid:
        cout << "\033[36m" << char(219) << "\u001b[0m";
        break; // fantasma - ciano
    case ghostPowerupId:
        cout << "\u001b[46m\u001b[97m" << char(247) << "\u001b[0m";
        break; // Power up fantasma
    case bombExpanderId:
        cout << "\u001b[45m\u001b[97m"
             << "+"
             << "\u001b[0m";
        break; // Aumentar bomba
    case ghostBombId:
        cout << "\u001b[45m\u001b[97m"
             << char(247)
             << "\u001b[0m";
        break;
    default:
        cout
            << "-"; // erro
    }               // fim switch
}

bool isEven(int i)
{
    return !(i % 2); // Função simples para saber se um número é par
};

struct map
{
    // Struct Map: possui apenas uma array de mapa para ser utilizado como return
    int sizeX = 0;
    int sizeY = 0;
    int **mapa;

    map()
    {
        resize(10, 10);
        gerarMapa();
    };

    void resize(int newX, int newY)
    {
        int **oldmap = mapa;
        int oldX = sizeX, oldY = sizeY;
        sizeX = newX;
        sizeY = newY;
        // Declara nova matriz
        mapa = new int *[sizeY];
        for (int i = 0; i < sizeY; i++)
        {
            mapa[i] = new int[sizeX];
        }
        // repassa os valores
        for (int i = 0; i < sizeY; i++)
        {
            if (i < oldY)
            {
                for (int j = 0; j < sizeX; j++)
                {
                    if (j < oldX)
                    {
                        mapa[i][j] = oldmap[i][j];
                    }
                    else
                    {
                        mapa[i][j] = vazioid;
                    }
                }
            }
            else
            {
                for (int j = 0; j < sizeX; j++)
                {
                    mapa[i][j] = vazioid;
                }
            }
        }
        // Apaga o mapa antigo
        for (int i = 0; i < oldY; i++)
        {
            delete[] oldmap[i];
        }
        delete[] oldmap;
    };

    void gerarMapa()
    {
        // o mapa é gerado formando com paredes padronizadas e paredesfrageis de forma aleatoria
        for (int i = 0; i < sizeY; i++)
        {
            for (int j = 0; j < sizeX; j++)
            {
                if (!isEven(i) && !isEven(j))
                {
                    mapa[i][j] = paredeid;
                }
                if (isEven(i) != isEven(j))
                {
                    if (!(rand() % 3))
                        mapa[i][j] = paredefragilid;
                }
            }
        }
    };

    void draw_map()
    {
        /// Imprime o jogo: mapa e personagem.
        for (int i = 0; i < sizeY; i++)
        { // Para cada linha
            for (int j = 0; j < sizeX; j++)
            { // Para cada coluna da linha
                drawPixel(mapa[i][j]);
            }
            cout << "\n";
        }

    } // fim for mapa

    void SumItens(objcore objeto)
    {
        // Essa função adiciona um único objeto dentro de um mapa
        mapa[objeto.y][objeto.x] = objeto.id;
    }

    void copyMap(map originalmap, bool resizeNeeded = false)
    {
        if (resizeNeeded)
            resize(originalmap.sizeX, originalmap.sizeY);
        for (int i = 0; i < sizeY; i++)
        {
            for (int j = 0; j < sizeX; j++)
            {
                mapa[i][j] = originalmap.mapa[i][j];
            }
        }
    }
};

struct obj
{
    // Struct de um objeto: possui um vetor x,y e um ID
    int x;
    int y;
    int id;

    objcore toCore()
    {
        objcore i;
        i.x = x;
        i.y = y;
        i.id = id;
        return i;
    }

    void move(int xMove, int yMove, map mapa, bool isBomb = false)
    {
        // A função MoveObject verifica se o local que está tentando se mover é vazio,
        // Se o local for vazio, ele se move, caso o contrario, retorna a instancia na direção original

        // Testa colisão em X
        if (isInsideMap(xMove, yMove, mapa))
        {
            // Testa colisões em X
            if (mapa.mapa[y][x + xMove] <= playerid)
            {               // Qualquer objeto com ID <= PlayerID é um objeto sem colisão
                x += xMove; // Se não houve colisão em X, o objeto se move em X
            }
            else if (isBomb && mapa.mapa[y][x + xMove] == paredefragilid)
            {               // Se o objeto for uma bomba, ele pode atravessar paredes frageis
                x += xMove; // Se colisão da bomba foi com uma parede fragil, ele se move
            }

            // Testa colisão em Y
            // O código é o mesmo de cima, só muda para Y
            if (mapa.mapa[y + yMove][x] <= playerid)
            {
                y += yMove;
            }
            else if (isBomb && mapa.mapa[y + yMove][x] == paredefragilid)
            {
                y += yMove;
            }
        }
        //----------
    };

    bool isInsideMap(int xMove, int yMove, map mapa)
    {
        // Inimide map testa se um objeto estara dentro do mapa após ele se mover
        if (xMove != 0)
        {
            if (!(x + xMove >= 0 && x + xMove < mapa.sizeX))
            {
                return false;
            }
        }

        if (yMove != 0)
        {
            if (!(y + yMove >= 0 && y + yMove < mapa.sizeY))
            {
                return false;
            }
        }
        return true;
    };

    bool CanMove(int xMove, int yMove, map mapa)
    {
        // A função CanMove testa se um objeto pode se mover a partir da logica da moveObject:
        // A função MoveObject só move se for possível se mover,
        // então se o objeto retornado for igual ao enviado, ele não se moveu (logo não pode se mover)
        obj tempobj;
        tempobj.x = x;
        tempobj.y = y;
        if (xMove == 0 && yMove == 0)
        { // Movimentar em 0 em x e y ele sempre podera se mover
            return true;
        }
        tempobj.move(xMove, yMove, mapa);
        if (x == tempobj.x && y == tempobj.y)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool Colide(map mapa, int id)
    {
        // Verifica se um objeto colide com outro objeto dentro do mapa
        if (mapa.mapa[y][x] == id)
        { // Se o x e y do mapa naquele ponto for igual ao id enviado
            return true;
        }
        return false;
    }

    bool Colide(map mapa, int id, int id2)
    {
        // Verifica se um objeto colide com outro objeto dentro do mapa
        if (mapa.mapa[y][x] == id || mapa.mapa[y][x] == id2)
        { // Se o x e y do mapa naquele ponto for igual ao id enviado
            return true;
        }
        return false;
    }
};

void draw_hud(int itemId, int itemIndex)
{
    // A função de desenhar o hud serve para imprimir a primeira linha do editor de mapas
    cout << "\033[37m>> ";
    drawPixel(itemId);
    cout << "\033[37m <<";
    cout << "Item Selecionado: " << itemIndex << "\n";
}

void rotateItem(int &item, int max, int ammount)
{
    int i = item + ammount;
    if (item + ammount >= max)
    {
        i = 0;
    }
    if (item + ammount < 0)
    {
        i = max - 1;
    }
    item = i;
}