#include "inimigo.h"

struct player
{
    obj objeto = {0,0,playerid};
    bomba bomba;
    int status = 1; // 0 = morto, 1 = vivo

    void colocabomba(){
      bomba.colocaBomba(objeto);  
    };
};