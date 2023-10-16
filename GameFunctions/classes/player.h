#include "inimigo.h"

struct Controls
{
  char up = 72; //cima
  char left = 75; //esquerda
  char down = 80; //baixo
  char right = 77; //direita
  char bomb = ' '; //espaco
};



struct player
{
    obj objeto = {0,0,playerid};
    Controls controles;
    bomba bomba;   
    int status = 1; // 0 = morto, 1 = vivo
    bool ghostPowerup = false; 

    void deletePlayer(){
      bomba.deleteBomba();
    };

    void keyPress (char key, map mapa){
      if(key == controles.up) objeto.move(0,-1,mapa, ghostPowerup);
      if(key == controles.down) objeto.move(0,1,mapa, ghostPowerup);
      if(key == controles.left) objeto.move(-1,0,mapa, ghostPowerup);
      if(key == controles.right) objeto.move(1,0,mapa, ghostPowerup);
      if(key == controles.bomb){
      if(!bomba.status) bomba.colocaBomba(objeto);
      }
    }

    void expandbomb(){
      bomba.resizeRaio(bomba.raio++);
    }
};