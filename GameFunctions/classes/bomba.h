#include "coreFunctions.h"

struct bomba
{
    obj objeto = {0, 0, bombaid}; // Objeto bomba (centro)
    int status = 0;              // 0 nao existe, 1 existe, 2 explodiu
    clock_t set, trigger;         // Momento de armar a bomba e de explosão
    obj *explosao[4];             // Array com as particulas de explosão nas 4 direções
    int raio = 1;                 // raio da Bomba

    bomba()
    {
        resizeRaio(raio, false);
    }

    void copy(bomba newBomba, bool resizeNeeded = true){
        objeto = newBomba.objeto;
        status = newBomba.status;
        set = newBomba.set;
        trigger = newBomba.trigger;
        raio = newBomba.raio;
        if (resizeNeeded)
            resizeRaio(raio);
    }
    void resizeRaio(int newRaio, bool deleteOld = true)
    {
        if (deleteOld)
            deleteBomba();
        raio = newRaio;
        for (int i = 0; i < 4; i++)
        {
            explosao[i] = new obj[newRaio];
            for (int j = 0; j < newRaio; j++)
            {
                explosao[i][j].x = 0;
                explosao[i][j].y = 0;
                explosao[i][j].id = explosaoid;
            }
        }
    }

    void colocaBomba(obj player)
    {
        status = 1; // status = bomba existe
        objeto.x = player.x;
        objeto.y = player.y; // a bomba vai para a posição do player
        set = clock();       // é dado o set da bomba
    }

    void deleteBomba()
    {
        delete[] explosao[0];
        delete[] explosao[1];
        delete[] explosao[2];
        delete[] explosao[3];
    }

    void createBomba(){
        explosao[0] = new obj[raio];
        explosao[1] = new obj[raio];
        explosao[2] = new obj[raio];
        explosao[3] = new obj[raio];
    }

    void explodirBomba(map &mapa)
    {
        if (status == 2)
        {                        // se a bomba ja havia explodido
            status = 0;          // ela deixar de exisitr
            objeto.id = bombaid; // seu id vira o de uma bomba
            return;              // finaliza a função
        }

        // caso a bomba não tenha explodido ainda ==>
        set = clock();          // é dado o set a partir do momento da explosão
        bool canExpand;         // canExpand dira se a bomba podera continuar expandindo em uma direção
        status = 2;             // a bomba recebe o status de explodida
        objeto.id = explosaoid; // a bomba (nucleo da explosão) também vira uma explosão

        int direcoes[4][2]{
            // matriz com versores nas 4 direções
            0,
            -1,
            0,
            1,
            1,
            0,
            -1,
            0,
        };

        for (int i = 0; i < 4; i++)
        {                     // para cada versor de direção
            canExpand = true; // a bomba pode expandir

            for (int j = 0; j < raio; j++)
            { // para cada item dentro daquela direção
                explosao[i][j] = objeto;
                if (!canExpand)
                {
                    // se a bomba não poder expandir mais, a explosão sera colocada dentro do centro da bomba
                    continue;
                }
                // Caso ela ainda possa expandir
                // A instancia da explosão sera colocada na direção do versor atual vezes a itensidade atual definida por J
                explosao[i][j].move(direcoes[i][0] * (j + 1), direcoes[i][1] * (j + 1), mapa, true);

                // Caso não houve alteração na explosão após o MoveObject, a bomba irá parar de expandir
                if (explosao[i][j].x == objeto.x && explosao[i][j].y == objeto.y)
                    canExpand = false;

                if (mapa.mapa[explosao[i][j].y][explosao[i][j].x] == paredefragilid)
                {                                                            // se a explosão atingiu uma parede fragil
                    mapa.mapa[explosao[i][j].y][explosao[i][j].x] = vazioid; // aquele local do mapa passa a ser um vazio
                    canExpand = false;                                       // a bomba não pode mais expandir a partir dali
                }
            }
        }
    }
};