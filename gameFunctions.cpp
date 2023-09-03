#include "coreFunctions.cpp"

#define QtdInimigos 1

map MapaInicial = { 4,4,4,4,4,4,4,4,4,4,4,4,4,0,4,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    4,0,4,0,4,0,4,4,4,4,4,4,4,0,0,
                    4,0,4,0,4,0,4,4,4,4,4,4,4,0,0,
                    4,0,4,0,4,5,4,4,4,4,4,4,4,5,4,
                    4,0,4,0,0,0,4,4,4,4,4,4,4,0,4,
                    4,0,4,0,4,0,4,4,4,4,4,0,0,0,4,
                    4,0,4,0,4,0,4,4,4,4,4,0,0,0,4,
                    4,0,0,0,0,0,0,0,5,0,0,0,0,0,4,
                    4,4,4,0,0,0,4,4,4,4,4,4,4,0,4
    };
    obj playerInicial = {1,2,playerid};
    inimigo inimigoInicial = {5,1,inimigoid};

void colocaBomba(obj player, bomba &bomba){
    bomba.status = 1; //status = bomba existe
    bomba.bomba.x = player.x; 
    bomba.bomba.y = player.y; //a bomba vai para a posição do player
    bomba.set = clock(); //é dado o set da bomba

}

void explodirBomba(bomba &bomba, map &mapa){
    if(bomba.status == 2){ //se a bomba ja havia explodido
        bomba.status = 0; //ela deixar de exisitr
        bomba.bomba.id = bombaid; //seu id vira o de uma bomba
        return; //finaliza a função
    }

    //caso a bomba não tenha explodido ainda ==>
    bomba.set = clock(); //é dado o set a partir do momento da explosão
    bool canExpand; //canExpand dira se a bomba podera continuar expandindo em uma direção
    bomba.status = 2;  //a bomba recebe o status de explodida
    bomba.bomba.id = explosaoid; //a bomba (nucleo da explosão) também vira uma explosão

    int direcoes[4][2]{ //matriz com versores nas 4 direções
        0,-1,
        0,1,
        1,0,
        -1,0,
    };

    for (int i = 0; i < 4; i ++){ //mara cada versor de direção
        canExpand = true; //a bomba pode expandir

        for (int j = 0; j < raio; j ++){ //para cada item dentro daquela direção
            if(!canExpand){
                //se a bomba não poder expandir mais, a explosão sera colocada dentro do centro da bomba
                bomba.explosao[i][j] = bomba.bomba; 
                continue;
            }
            //Caso ela ainda possa expandir
            //A instancia da explosão sera colocada na direção do versor atual vezes a itensidade atual definida por J
            bomba.explosao[i][j] = moveObject(bomba.bomba,direcoes[i][0]*(j+1),direcoes[i][1]*(j+1),mapa,true); 
            
            //poderia usar a função CanExpand, mas verificar o X e Y seria mais simples neste caso
            if (bomba.explosao[i][j].x == bomba.bomba.x && bomba.explosao[i][j].y == bomba.bomba.y )canExpand = false;
            
            if (mapa.mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] == paredefragilid){ //se a explosão atingiu uma parede fragil
                mapa.mapa[bomba.explosao[i][j].y][bomba.explosao[i][j].x] = vazioid; //aquele local do mapa passa a ser um vazio
                canExpand = false; //a bomba não pode mais expandir a partir dali
            }
        }
    }
}

obj novaDirecaoInimigo(inimigo inimigo, map mapa){
    //A função NovaDirecaoInimigo verifica quais direções o inimigo pode se mover
    //e escolhe uma direção aleatorio para ele se mover
    
    int i = 0; //numero de direções possível
    obj direcoes[4]; //array com as direções possíveis
    obj novaDirecao;

    //Para cada uma das direções possíveis, a direção será adicionada ao vetor
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

    if (i==0){ //se não houver direção possível
    // a nova direção será (0,0)
        novaDirecao.x = 0;
        novaDirecao.y = 0;
    }

    return novaDirecao;
}

void MoveInimigo(inimigo &inimigo, map mapa){
    if(!CanMove(inimigo.inimigo,inimigo.direcao.x,inimigo.direcao.y,mapa)){
        inimigo.direcao.x = inimigo.direcao.x * -1;
        inimigo.direcao.y = inimigo.direcao.y * -1;
    }

    inimigo.inimigo = moveObject(inimigo.inimigo,inimigo.direcao.x,inimigo.direcao.y,mapa);
    inimigo.set = clock();
    inimigo.numeroPassos--;
}

bool Colide(obj objeto, map mapa, int id){
    if(mapa.mapa[objeto.y][objeto.x] == id){
        return true;
    }
    return false;
}

void gameRestart(obj &player, inimigo &inimigo, map &mapa, bomba &bomba){
    player = playerInicial;
    inimigo = inimigoInicial;
    mapa = MapaInicial;
    bomba.status = 0;
    bomba.bomba.id = bombaid;
}

