#include "inimigo.h"

struct Controls
{
  char up = 72; //cima
  char left = 75; //esquerda
  char down = 80; //baixo
  char right = 77; //direita
  char bomb = ' '; //espaco

  void deletarEspacos(){
    if(up == spaceKey) up = 255;
    if(left == spaceKey) left = 255;
    if(down == spaceKey) down = 255;
    if(right == spaceKey) right = 255;
    if(bomb == spaceKey) bomb = 255;
  }
  void lerEspacos(){
    if(up == -1) up = spaceKey;
    if(left == -1) left = spaceKey;
    if(down == -1) down = spaceKey;
    if(right == -1) right = spaceKey;
    if(bomb == -1) bomb = spaceKey;
  }
  
  };



struct player
{
    obj objeto = {0,0,playerid};
    bomba bomba;   
    int status = 1; // 0 = morto, 1 = vivo
    bool ghostPowerup = false; 
    Controls controles;


    void copy(player newPlayer){
      objeto = newPlayer.objeto;
      controles = newPlayer.controles;
      bomba.copy (newPlayer.bomba);
      status = newPlayer.status;
      ghostPowerup = newPlayer.ghostPowerup;

    }
    void deletePlayer(){
      bomba.deleteBomba();
    };
    void createPlayer(){
      bomba.createBomba();
    }

    void resize(player newplayer){
      bomba.resizeRaio(newplayer.bomba.raio);
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
      bomba.raio++;
      bomba.resizeRaio(bomba.raio);
    }
};