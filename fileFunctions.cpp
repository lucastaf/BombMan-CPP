#include "gameFunctions.cpp"
#include <fstream>

void importFile(string name){
    ifstream arquivo;
    arquivo.open(name);
    int inimigoindex = 0;

    if (!arquivo.is_open()){
        cout << "Nao foi possivel abrir o arquivo";
        cin >> name; //descartar variavel name
        return;
    }
    getline(arquivo,name); //Ignora a primeira linha e descarta na variavel name
    for (int i = 0; i < sizey; i ++){
        for (int j = 0;j < sizex; j ++){
            int digito = arquivo.get() - 48;
            cout << digito;
            switch (digito)
            {
            case playerid:
                playerInicial.x = j;
                playerInicial.y = i;
                MapaInicial.mapa[i][j] = vazioid;
            break;
            case inimigoid:
                inimigsoIniciais[inimigoindex].inimigo.x = j;
                inimigsoIniciais[inimigoindex].inimigo.y = i;
                MapaInicial.mapa[i][j] = vazioid;
                inimigoindex++;
            break;
            default:
                MapaInicial.mapa[i][j] = digito;
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

void exportFile(string name){
    map mapaeditado = MapaInicial;
    mapaeditado.mapa[playerInicial.y][playerInicial.x] = playerid;
    for (int i = 0; i < QtdInimigos; i++){
        obj inimigoatual = inimigsoIniciais[i].inimigo;
        mapaeditado.mapa[inimigoatual.y][inimigoatual.x] = inimigoid;
    }

    remove(name.c_str());
    ofstream arquivo;
    arquivo.open(name);
    

    arquivo << "sep=, \n";
    for (int i = 0; i < sizey; i++){
        for (int j = 0; j < sizex; j++){
            arquivo << mapaeditado.mapa[i][j];
            arquivo << ",";
        }
        arquivo << "\n";
    }

    arquivo.close();
}