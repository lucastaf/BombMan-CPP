#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

#define sizex 15
#define sizey 10
#define raio 1
#define vazioid 0
#define explosaoid 1
#define inimigoid 2
#define playerid 3
#define paredeid 4
#define paredefragilid 5
#define bombaid 6 

struct obj{
    int x;
    int y;
    int id;
};
typedef struct obj obj;

struct map{
    int mapa[sizey][sizex];
};
typedef struct map map;

struct bomba{
    obj bomba;
    char status; //0 nao existe, 1 existe, 2 explodiu
    clock_t set,trigger;
    obj explosao[4][raio];
};
typedef struct bomba bomba;

struct inimigo{
    obj inimigo;
    clock_t set, trigger;
    char status = 1; //0 = morto, 1 = parado, 2 = andando
    int numeroPassos;
    obj direcao;
};
typedef struct inimigo inimigo;

void draw_map(map mapa){
        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<sizey;i++){
            for(int j=0;j<sizex;j++){
                    switch (mapa.mapa[i][j]){
                        case vazioid: cout<<"\033[32m"<<char(219); break;//caminho - Verde
                        case explosaoid: cout<<"\033[31m"<<"#"; break;//explosao - Laranja
                        case inimigoid: cout<<"\033[31m"<<char(219); break;//inimigo - Vermelho
                        case playerid: cout<<"\033[33m"<<char(219); break;//Player - Amarelo
                        case paredeid: cout<<"\033[97m"<<char(219); break;//parede - cinza
                        case paredefragilid: cout<<"\033[37m"<<char(219); break;//parede fragil - branca
                        case bombaid: cout<<"\033[30m"<<char(219); break;//bomba - Preta
                        
                        default: cout<<"-"; //erro
                    } //fim switch
                }
                cout<<"\n";
            }
} //fim for mapa

obj moveObject(obj objeto,int Xmove, int Ymove, map mapa, bool isBomb = false){
    if (objeto.x + Xmove >= 0 && objeto.x + Xmove < sizex){
        if (mapa.mapa[objeto.y][objeto.x + Xmove] <= playerid){ //0 é vazio e 1 é explosao
            objeto.x += Xmove;
        }
        if(isBomb && mapa.mapa[objeto.y][objeto.x + Xmove] == paredefragilid){
            objeto.x += Xmove;
        }
    }
    if (objeto.y + Ymove >= 0 && objeto.y + Ymove < sizey){
        if(mapa.mapa[objeto.y+Ymove][objeto.x] <= playerid){
            objeto.y += Ymove;
        }
        if(isBomb && mapa.mapa[objeto.y+Ymove][objeto.x] == paredefragilid){
            objeto.y += Ymove;
        }
    }
    return objeto;
}

map SumMapItens(map &mapa, obj objeto){
    mapa.mapa[objeto.y][objeto.x] = objeto.id;
    return mapa; 
}

map SumMapExplosion(map &mapa, obj objeto[4][raio]){
    for (int i = 0; i < 4; i++){
        for(int j = 0;j < raio; j ++){
            mapa.mapa[objeto[i][j].y][objeto[i][j].x] = objeto[i][j].id;
        }
    }
    return mapa;
}

bool CanMove(obj objeto, int xMove, int yMove,map mapa){
    obj objetoMovido = moveObject(objeto,xMove,yMove,mapa);
    if(objetoMovido.x == objeto.x && objetoMovido.y == objeto.y){
        return false;
    }else{
        return true;
    }
}