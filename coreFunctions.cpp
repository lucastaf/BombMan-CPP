#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

#define sizex 15
#define sizey 10
#define raio 1
//Ids
#define vazioid 0
#define explosaoid 1
#define inimigoid 2
#define playerid 3
#define paredeid 4
#define paredefragilid 5
#define bombaid 6 
//Cada objeto do cenario possui um ID proprio, esses IDs são categorizados e a programação do jogo gira em torno deles//
//Player e inimigo não possuem colisão, eles podem ser atravessado, isso garante q seja detectado quando o player ou inimigo colidem um com o outro
//Note que os objetos com colisão possuem um ID superior ao PlayerID, isso será útil no futuro para realizar testes de colisão

struct obj{
    //Struct de um objeto: possui um vetor x,y e um ID
    int x;
    int y;
    int id;
};
typedef struct obj obj;

struct map{
    //Struct Map: possui apenas uma array de mapa para ser utilizado como return
    int mapa[sizey][sizex];
};
typedef struct map map;

struct bomba{
    obj bomba; //Objeto bomba (centro)
    char status; //0 nao existe, 1 existe, 2 explodiu
    clock_t set,trigger; //Momento de armar a bomba e de explosão
    obj explosao[4][raio]; //Array com as particulas de explosão nas 4 direções
};
typedef struct bomba bomba;

struct inimigo{
    obj inimigo; //Objeto do inimigo
    clock_t set, trigger; //Inicio da mudança de estado / final
    char status = 1; //0 = morto, 1 = parado, 2 = andando
    int numeroPassos; //Passos a serem andados
    obj direcao; //Direção a ser andada (serve apenas como vetor, o ID é descartavel)
};
typedef struct inimigo inimigo;

void draw_map(map mapa){
        ///Imprime o jogo: mapa e personagem.
        for(int i=0;i<sizey;i++){ //Para cada linha
            for(int j=0;j<sizex;j++){ //Para cada coluna da linha
                    switch (mapa.mapa[i][j]){ //Desenha o id daquele objeto
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
    //A função MoveObject verifica se o local que está tentando se mover é vazio,
    //Se o local for vazio, ele se move, caso o contrario, retorna a instancia na direção original

    //Testa colisão em X
    if (objeto.x + Xmove >= 0 && objeto.x + Xmove < sizex){ //Verifica se o objeto estará dentro dos limite X do mapa
        if (mapa.mapa[objeto.y][objeto.x + Xmove] <= playerid){ //Qual objeto com ID <= PlayerID é um objeto sem colisão
            objeto.x += Xmove; //Se não houve colisão em X, o objeto se move em X
        }else
        if(isBomb && mapa.mapa[objeto.y][objeto.x + Xmove] == paredefragilid){ //Se o objeto for uma bomba, ele pode atravessar paredes frageis
            objeto.x += Xmove; //Se colisão da bomba foi com uma parede fragil, ele se move
        }
    }
    //----------

    //Testa colisão em Y
    //O código é o mesmo de cima, só muda para Y
    if (objeto.y + Ymove >= 0 && objeto.y + Ymove < sizey){
        if(mapa.mapa[objeto.y+Ymove][objeto.x] <= playerid){
            objeto.y += Ymove;
        }else
        if(isBomb && mapa.mapa[objeto.y+Ymove][objeto.x] == paredefragilid){
            objeto.y += Ymove;
        }
    }
    //----------
    return objeto; //Retorna o objeto, estando ele movido ou nao
}

bool CanMove(obj objeto, int xMove, int yMove,map mapa){
    //A função CanMove testa se um objeto pode se mover a partir da logica da moveObject
    //A função MoveObject só move se for possível se mover, 
    //então se o objeto retornado for igual ao enviado, ele não se moveu (logo não pode se mover)

    if (xMove == 0 && yMove == 0){ //Movimentar em 0 em x e y ele sempre podera se mover
        return 0;
    }
    obj objetoMovido = moveObject(objeto,xMove,yMove,mapa);
    if(objetoMovido.x == objeto.x && objetoMovido.y == objeto.y){
        return false;
    }else{
        return true;
    }
    
}

map SumMapItens(map &mapa, obj objeto){
    //Essa função adiciona um único objeto dentro de um mapa
    mapa.mapa[objeto.y][objeto.x] = objeto.id;
    return mapa; 
}

map SumMapExplosion(map &mapa, obj objeto[4][raio]){
    //Essa função adiciona todos as instancias de uma explosão,
    //as explosões são colocadas dentro de uma matriz dentro da struct bomba
    for (int i = 0; i < 4; i++){
        for(int j = 0;j < raio; j ++){
            mapa.mapa[objeto[i][j].y][objeto[i][j].x] = objeto[i][j].id;
        }
    }
    return mapa;
}

