#include<iostream>
#include<windows.h>
#include<conio.h>
using namespace std;

#define sizex 15
#define sizey 10

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



void draw_map(map mapa){
        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<sizey;i++){
            for(int j=0;j<sizex;j++){
                    switch (mapa.mapa[i][j]){
                        case 0: cout<<" "; break; //caminho
                        case 1: cout<<"\033[37m"<<char(219); break; //parede
                        case 2: cout<<"\033[33m"<<"P";break;//Player
                        //default: cout<<"-"; //erro
                    } //fim switch
                }
                cout<<"\n";
            }
} //fim for mapa

map SumMapItens(map mapa, obj objeto){
    mapa.mapa[objeto.y][objeto.x] = objeto.id;
    return mapa; 
}

obj moveObject(obj objeto,int Xmove, int Ymove, map mapa){
    if (!(objeto.x + Xmove >= sizex || objeto.x + Xmove < 0)){
        if (mapa.mapa[objeto.y][objeto.x + Xmove] == 0){
            objeto.x += Xmove;
        }
    }
    if (!(objeto.y + Ymove >= sizey || objeto.y + Ymove < 0)){
        if(mapa.mapa[objeto.y+Ymove][objeto.x] == 0){
            objeto.y += Ymove;
        }
    }
    return objeto;
}