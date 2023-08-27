#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

#define sizex 15
#define sizey 10
#define raio 1

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
    bool existe;
    clock_t set,tigger;
    obj explosao[raio*4+1];
};
typedef struct bomba bomba;



void draw_map(map mapa){
        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<sizey;i++){
            for(int j=0;j<sizex;j++){
                    switch (mapa.mapa[i][j]){
                        case 0: cout<<"\033[32m"<<char(219); break; //caminho - Verde
                        case 1: cout<<"\033[48:5:208m%s"<<char(219); break;//explosao - Laranja
                        case 2: cout<<"\033[97m"<<char(219); break; //parede - cinza
                        case 3: cout<<"\033[37m"<<char(219); break;//parede fragil - branca
                        case 4: cout<<"\033[33m"<<char(219);break;//Player - Amarelo
                        case 5: cout<<"\033[30m"<<char(219);break;//bomba - Preta
                        case 6: cout<<"\033[31m"<<char(219); break;//inimigo - Vermelho
                        default: cout<<"-"; //erro
                    } //fim switch
                }
                cout<<"\n";
            }
} //fim for mapa

map SumMapItens(map &mapa, obj objeto){
    mapa.mapa[objeto.y][objeto.x] = objeto.id;
    return mapa; 
}

obj moveObject(obj objeto,int Xmove, int Ymove, map mapa){
    if (objeto.x + Xmove >= 0 && objeto.x + Xmove < sizex){
        if (mapa.mapa[objeto.y][objeto.x + Xmove] <= 1){ //0 é vazio e 1 é explosao
            objeto.x += Xmove;
        }
    }
    if (objeto.y + Ymove >= 0 && objeto.y + Ymove < sizey){
        if(mapa.mapa[objeto.y+Ymove][objeto.x] <= 1){
            objeto.y += Ymove;
        }
    }
    return objeto;
}

void colocaBomba(obj player, bomba &bomba){
    bomba.existe = true;
    bomba.bomba.x = player.x;
    bomba.bomba.y = player.y;
    bomba.set = clock();

}

void explodirBomba(bomba &bomba, map &mapa){
    bomba.existe = false;
    bomba.explosao[0] = moveObject(bomba.bomba,0,0,mapa);
    bomba.explosao[1] = moveObject(bomba.bomba,1,0,mapa);
    bomba.explosao[2] = moveObject(bomba.bomba,-1,0,mapa);
    bomba.explosao[3] = moveObject(bomba.bomba,0,1,mapa);
    bomba.explosao[4] = moveObject(bomba.bomba,0,-1,mapa);
    
}