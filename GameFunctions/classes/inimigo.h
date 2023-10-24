#include "bomba.h"

struct inimigo
{
    obj objeto = {0, 0, inimigoid}; // Objeto do inimigo
    clock_t set, trigger; // Inicio da mudança de estado / final
    clock_t tempoDePasso;
    int status = 1;                // 0 = morto, 1 = parado, 2 = andando
    int numeroPassos;               // Passos a serem andados
    obj direcao;                    // Direção a ser andada (serve apenas como vetor, o ID é descartavel)

    inimigo(){
        tempoDePasso = 1.5;
    }

    void move(map mapa)
    {
        if (!objeto.CanMove(direcao.x, direcao.y, mapa))
        {
            // Se o inimigo não poder se mover na direção dada, a direção irá gira 180°
            direcao.x = direcao.x * -1;
            direcao.y = direcao.y * -1;
        }

        objeto.move(direcao.x, direcao.y, mapa);
        set = clock();
        numeroPassos--;
    }

    void novaDirecaoInimigo(map mapa)
    {
        // A função NovaDirecaoInimigo verifica quais direções o inimigo pode se mover
        // e escolhe uma direção aleatorio para ele se mover

        int i = 0;       // numero de direções possível
        obj direcoes[4]; // array com as direções possíveis

        // Para cada uma das direções possíveis, a direção será adicionada ao vetor
        if (objeto.CanMove(0, -1, mapa))
        {
            direcoes[i].x = 0;
            direcoes[i].y = -1;
            i++;
        }
        if (objeto.CanMove(0, 1, mapa))
        {
            direcoes[i].x = 0;
            direcoes[i].y = 1;
            i++;
        }
        if (objeto.CanMove(-1, 0, mapa))
        {
            direcoes[i].x = -1;
            direcoes[i].y = 0;
            i++;
        }
        if (objeto.CanMove(1, 0, mapa))
        {
            direcoes[i].x = 1;
            direcoes[i].y = 0;
            i++;
        }

        if (i == 0)
        { // se não houver direção possível
            // a nova direção será (0,0)
            direcao.x = 0;
            direcao.y = 0;
            return;
        }

        int novaDirecaoId = rand() % i;

        direcao.x = direcoes[novaDirecaoId].x;
        direcao.y = direcoes[novaDirecaoId].y;
    }
};

