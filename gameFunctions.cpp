#include "coreFunctions.cpp"


void colocaBomba(obj player, bomba &bomba){
    bomba.status = 1;
    bomba.bomba.x = player.x;
    bomba.bomba.y = player.y;
    bomba.set = clock();

}

void explodirBomba(bomba &bomba, map &mapa){
    if(bomba.status == 2){
        bomba.status = 0;
        bomba.bomba.id = bombaid;
        return;
    }
    bomba.set = clock();
    bool canExpand;
    bomba.status = 2;
    bomba.bomba.id = explosaoid;
    int direcoes[4][2]{
        0,-1,
        0,1,
        1,0,
        -1,0,
    };

    for (int i = 0; i < 4; i ++){
        canExpand = true;
        for (int j = 0; j < raio; j ++){
            if(!canExpand){
                bomba.explosao[i][j] = bomba.bomba;
                continue;
            }
            bomba.explosao[i][j] = moveObject(bomba.bomba,direcoes[i][0]*(j+1),direcoes[i][1]*(j+1),mapa,true);
            if (bomba.explosao[i][j].x == bomba.bomba.x && bomba.explosao[i][j].y == bomba.bomba.y )canExpand = false;
            if (mapa.mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] == paredefragilid){
                canExpand = false;
                mapa.mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] = vazioid;

            }
        }
    }
}

obj novaDirecaoInimigo(inimigo inimigo, map mapa){
    int i = 0;
    obj direcoes[4];
    obj novaDirecao;
    if(CanMove(inimigo.inimigo,0,-1,mapa)){
        direcoes[i].x = 0;
        direcoes[i].y = -1;
        i++;
    }
    if(CanMove(inimigo.inimigo,0,1,mapa)){
        direcoes[i].x = 0;
        direcoes[i].y = 1;
        i++;
    }
    if(CanMove(inimigo.inimigo,-1,0,mapa)){
        direcoes[i].x = -1;
        direcoes[i].y = 0;
        i++;
    }
    if(CanMove(inimigo.inimigo,1,0,mapa)){
        direcoes[i].x = 1;
        direcoes[i].y = 0;
        i++;
    }

    int novaDirecaoId = rand()%i;

    novaDirecao.x = direcoes[novaDirecaoId].x;
    novaDirecao.y = direcoes[novaDirecaoId].y;

    if (i==0){
        novaDirecao.x = 0;
        novaDirecao.y = 0;
    }

    return novaDirecao;
}

void MoveInimigo(inimigo &inimigo, map mapa){
    if(CanMove(inimigo.inimigo,inimigo.direcao.x,inimigo.direcao.y,mapa)){
        inimigo.inimigo = moveObject(inimigo.inimigo,inimigo.direcao.x,inimigo.direcao.y,mapa);
    }else{
        inimigo.inimigo = moveObject(inimigo.inimigo,inimigo.direcao.x*-1,inimigo.direcao.y*-1,mapa);
    }
}