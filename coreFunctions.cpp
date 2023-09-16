#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

#define QtdInimigos 2
#define sizex 10
#define sizey 10
#define raio 1
//Ids de objetos
#define vazioid 0
#define explosaoid 1
#define inimigoid 2
#define playerid 3
#define paredeid 4
#define paredefragilid 5
#define bombaid 6 
#define ghostid 7

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
    obj bomba = {0,0,bombaid}; //Objeto bomba (centro)
    char status = 0; //0 nao existe, 1 existe, 2 explodiu
    clock_t set,trigger; //Momento de armar a bomba e de explosão
    obj explosao[4][raio]; //Array com as particulas de explosão nas 4 direções
};
typedef struct bomba bomba;

struct inimigo{
    obj inimigo = {0,0,inimigoid}; //Objeto do inimigo
    clock_t set = clock(), trigger; //Inicio da mudança de estado / final
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
                        case ghostid: cout<<"\033[36m"<<char(219); break;//fantasma - ciano
                        
                        default: cout<<"-"; //erro
                    } //fim switch
                }
                cout<<"\n";
            }
} //fim for mapa

void draw_hud(obj item, int Iditem){
    //A função de desenhar o hud serve para imprimir a primeira linha do editor de mapas
    cout << "\033[37m>> ";
    switch (item.id)
    {
        //Essa função desenha apenas se o item q está selecionado é um player ou um inimigo
    case playerid: cout<<"\033[33m"<<char(219); break;
    case inimigoid: cout<<"\033[31m"<<char(219); break;
    }
    cout << "\033[37m <<";
    cout << "Item Selecionado: " << Iditem <<"\n";
}

bool isInsideMap(obj objeto, int xMove, int yMove){
    //Inimide map testa se um objeto estara dentro do mapa após ele se mover
    int i = true;
    if (xMove != 0){
     if (!(objeto.x + xMove >= 0 && objeto.x + xMove < sizex)){
        i = false;
     }
    }

    if (yMove != 0){
     if (!(objeto.y + yMove >= 0 && objeto.y + yMove < sizey)){
        i = false;
     }    
    }
    return i;
}

obj moveObject(obj objeto,int xMove, int yMove, map mapa, bool isBomb = false){
    //A função MoveObject verifica se o local que está tentando se mover é vazio,
    //Se o local for vazio, ele se move, caso o contrario, retorna a instancia na direção original

    //Testa colisão em X
    if (isInsideMap(objeto,xMove,yMove)){ 
        //Testa colisões em X
        if (mapa.mapa[objeto.y][objeto.x + xMove] <= playerid){ //Qualquer objeto com ID <= PlayerID é um objeto sem colisão
            objeto.x += xMove; //Se não houve colisão em X, o objeto se move em X
        }else
        if(isBomb && mapa.mapa[objeto.y][objeto.x + xMove] == paredefragilid){ //Se o objeto for uma bomba, ele pode atravessar paredes frageis
            objeto.x += xMove; //Se colisão da bomba foi com uma parede fragil, ele se move
        }

        //Testa colisão em Y
        //O código é o mesmo de cima, só muda para Y
        if(mapa.mapa[objeto.y+yMove][objeto.x] <= playerid){
            objeto.y += yMove;
        }else
        if(isBomb && mapa.mapa[objeto.y+yMove][objeto.x] == paredefragilid){
            objeto.y += yMove;
        }
    }
    //----------
    return objeto; //Retorna o objeto, estando ele movido ou nao
}

bool CanMove(obj objeto, int xMove, int yMove,map mapa){
    //A função CanMove testa se um objeto pode se mover a partir da logica da moveObject:
    //A função MoveObject só move se for possível se mover, 
    //então se o objeto retornado for igual ao enviado, ele não se moveu (logo não pode se mover)

    if (xMove == 0 && yMove == 0){ //Movimentar em 0 em x e y ele sempre podera se mover
        return true;
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

bool isEven(int i){
    return !(i%2); //Função simples para saber se um número é par
};