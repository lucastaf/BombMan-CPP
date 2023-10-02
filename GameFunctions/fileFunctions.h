#include "gameFunctions.h"
#include <fstream>

void importFile(string name, gameState &game){
    ifstream arquivo;
    arquivo.open(name);
    int inimigoindex = 0;

    if (!arquivo.is_open()){
        cout << "Nao foi possivel abrir o arquivo";
        cin >> name; //descartar variavel name
        return;
    }
    getline(arquivo,name); //Ignora a primeira linha e descarta na variavel name
    for (int i = 0; i < game.mapa.sizeY; i ++){
        for (int j = 0;j < game.mapa.sizeX; j ++){
            int digito = arquivo.get() - 48;
            cout << digito;
            switch (digito)
            {
            case playerid:
                game.players[0].objeto.x = j;
                game.players[0].objeto.y = i;
                game.mapa.mapa[i][j] = vazioid;
            break;
            case inimigoid:
                game.inimigos[inimigoindex].objeto.x = j;
                game.inimigos[inimigoindex].objeto.y = i;
                game.mapa.mapa[i][j] = vazioid;
                inimigoindex++;
            break;
            default:
                game.mapa.mapa[i][j] = digito;
            break;
            }
            arquivo.get(); //Pula a virgula

        }
        arquivo.get(); //Pula o \n
        cout << "\n";
    }
    cout << "Mapa carregado com sucesso";
    cin >> name;



}

void exportFile(string name, gameState game){
    map mapaeditado = game.mapa;
    mapaeditado.mapa[game.players[0].objeto.y][game.players[0].objeto.x] = playerid;
    for (int i = 0; i < game.QtdInimigos; i++){
        obj inimigoatual = game.inimigos[i].objeto;
        mapaeditado.mapa[inimigoatual.y][inimigoatual.x] = inimigoid;
    }

    remove(name.c_str());
    ofstream arquivo;
    arquivo.open(name);
    

    arquivo << "sep=, \n";
    for (int i = 0; i < game.mapa.sizeY; i++){
        for (int j = 0; j < game.mapa.sizeX; j++){
            arquivo << mapaeditado.mapa[i][j];
            arquivo << ",";
        }
        arquivo << "\n";
    }

    arquivo.close();
}