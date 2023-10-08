#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
using namespace std;

// Ids de objetos
#define vazioid 0
#define explosaoid 1
#define inimigoid 2
#define playerid 3
#define paredeid 4
#define paredefragilid 5
#define bombaid 6
#define ghostid 7

// Cada objeto do cenario possui um ID proprio, esses IDs são categorizados e a programação do jogo gira em torno deles//
// Player e inimigo não possuem colisão, eles podem ser atravessado, isso garante q seja detectado quando o player ou inimigo colidem um com o outro
// Note que os objetos com colisão possuem um ID superior ao PlayerID, isso será útil no futuro para realizar testes de colisão
struct objcore{
    int x;
    int y;
    int id;
};

bool isEven(int i)
{
    return !(i % 2); // Função simples para saber se um número é par
};

struct map
{
    // Struct Map: possui apenas uma array de mapa para ser utilizado como return
    int sizeX = 10;
    int sizeY = 10;
    int **mapa;

    map(){
        mapa = new int*[sizeY];
        for (int i = 0; i < sizeY; i++){
            mapa[i] = new int[sizeX];
            for(int j = 0; j < sizeX; j++){
                mapa[i][j] = vazioid;
            }
        }
        gerarMapa();

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
                switch (mapa[i][j])
                { // Desenha o id daquele objeto
                case vazioid:
                    cout << "\033[32m" << char(219);
                    break; // caminho - Verde
                case explosaoid:
                    cout << "\033[31m"
                         << "#";
                    break; // explosao - Laranja
                case inimigoid:
                    cout << "\033[31m" << char(219);
                    break; // inimigo - Vermelho
                case playerid:
                    cout << "\033[33m" << char(219);
                    break; // Player - Amarelo
                case paredeid:
                    cout << "\033[97m" << char(219);
                    break; // parede - cinza
                case paredefragilid:
                    cout << "\033[37m" << char(219);
                    break; // parede fragil - branca
                case bombaid:
                    cout << "\033[30m" << char(219);
                    break; // bomba - Preta
                case ghostid:
                    cout << "\033[36m" << char(219);
                    break; // fantasma - ciano

                default:
                    cout << "-"; // erro
                }                // fim switch
            }
            cout << "\n";
        }

    } // fim for mapa

    void SumItens(objcore objeto)
    {
        // Essa função adiciona um único objeto dentro de um mapa
        mapa[objeto.y][objeto.x] = objeto.id;
    }

    void copyMap(map originmap){
        for(int i = 0; i < sizeY; i++){
            for(int j = 0; j < sizeX; j++){
                mapa[i][j] = originmap.mapa[i][j];
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

    objcore toCore(){
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
        int i = true;
        if (xMove != 0)
        {
            if (!(x + xMove >= 0 && x + xMove < mapa.sizeX))
            {
                i = false;
            }
        }

        if (yMove != 0)
        {
            if (!(y + yMove >= 0 && y + yMove < mapa.sizeY))
            {
                i = false;
            }
        }
        return i;
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

void draw_hud(obj item, int Iditem)
{
    // A função de desenhar o hud serve para imprimir a primeira linha do editor de mapas
    cout << "\033[37m>> ";
    switch (item.id)
    {
        // Essa função desenha apenas se o item q está selecionado é um player ou um inimigo
    case playerid:
        cout << "\033[33m" << char(219);
        break;
    case inimigoid:
        cout << "\033[31m" << char(219);
        break;
    }
    cout << "\033[37m <<";
    cout << "Item Selecionado: " << Iditem << "\n";
}


