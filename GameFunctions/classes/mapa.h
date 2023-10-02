#include "coreFunctions.h"

struct bomba;

struct map
{
    // Struct Map: possui apenas uma array de mapa para ser utilizado como return
    int **mapa;
    int sizeX = 10;
    int sizeY = 10;

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

    void SumItens(obj objeto)
    {
        // Essa função adiciona um único objeto dentro de um mapa
        mapa[objeto.y][objeto.x] = objeto.id;
    }

    void SumExplosion(bomba bomba)
    {
        // Essa função adiciona todos as instancias de uma explosão,
        // as explosões são colocadas dentro de uma matriz dentro da struct bomba
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < bomba.raio; j++)
            {
                mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] = bomba.explosao[i][j].id;
            }
        }
    }
};
