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

struct map;


struct obj
{
    // Struct de um objeto: possui um vetor x,y e um ID
    int x;
    int y;
    int id;

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

bool isEven(int i)
{
    return !(i % 2); // Função simples para saber se um número é par
};
