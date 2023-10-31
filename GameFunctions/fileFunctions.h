#include "gameFunctions.h"
#include <fstream>
/*
clock atual - clock armado = x (tempo restante)
clock armado(set) = clock atual(clock()) - x(na save)

quando carregado
clock atual - x = clock armado
*/
void readWord(ifstream &arquivo, bool &out)
{
    string temp;
    arquivo >> temp;
    if (temp == "1")
    {
        out = true;
    }
    else
    {
        out = false;
    }
}
void readWord(ifstream &arquivo, long &out)
{
    string temp;
    arquivo >> temp;
    out = stol(temp);
}
void readWord(ifstream &arquivo, char &out)
{
    string temp;
    arquivo >> temp;
    out = temp[0];
}
void readWord(ifstream &arquivo, int &out)
{
    string temp;
    arquivo >> temp;
    out = stoi(temp);
}

bool importFile(string name, gameState &game)
{
    ifstream arquivo;
    string temp;
    arquivo.open(name);


    if (!arquivo.is_open())
    {
        cout << "Nao foi possivel abrir o arquivo\n";
        cin >> name; // descartar variavel name
        return false;
    }
    arquivo >> temp;
    if (temp != "bombermansavedata")
    {
        cout << "arquivo invalido\n";
        cin >> name;
        return false;
    }
    int newMapX, newMapY;
    int newPlayersQtd, newInimigosQtd;
    readWord(arquivo, newMapX);
    readWord(arquivo, newMapY);
    readWord(arquivo, newPlayersQtd);
    readWord(arquivo, game.contPlayers);
    readWord(arquivo, newInimigosQtd);
    readWord(arquivo, game.contInimigos);
    game.resizePlayers(newPlayersQtd);
    game.resizeInimigos(newInimigosQtd);
    game.mapa.resize(newMapX, newMapY);
    for (int i = 0; i < game.QtdPlayers; i++)
    {
        player *tempPlayer = &game.players[i];
        readWord(arquivo, tempPlayer->objeto.x);
        readWord(arquivo, tempPlayer->objeto.y);
        readWord(arquivo, tempPlayer->bomba.objeto.x);
        readWord(arquivo, tempPlayer->bomba.objeto.y);
        readWord(arquivo, tempPlayer->bomba.set);
        readWord(arquivo, tempPlayer->bomba.status);
        readWord(arquivo, tempPlayer->bomba.raio);
        tempPlayer->bomba.resizeRaio(tempPlayer->bomba.raio);
        readWord(arquivo, tempPlayer->bomba.atravessaParede);
        readWord(arquivo, tempPlayer->status);
        readWord(arquivo, tempPlayer->ghostPowerup);
        readWord(arquivo, tempPlayer->controles.up);
        readWord(arquivo, tempPlayer->controles.down);
        readWord(arquivo, tempPlayer->controles.left);
        readWord(arquivo, tempPlayer->controles.right);
        readWord(arquivo, tempPlayer->controles.bomb);
        game.players[i].controles.lerEspacos();
    }
    for(int i = 0; i < game.QtdInimigos; i++){
        readWord(arquivo, game.inimigos[i].objeto.x);
        readWord(arquivo, game.inimigos[i].objeto.y);
        readWord(arquivo, game.inimigos[i].status);
    }

    for(int i = 0; i < game.mapa.sizeY; i++){
        for(int j = 0; j < game.mapa.sizeX; j++){
            readWord(arquivo, game.mapa.mapa[i][j]);
        }
    }
    game.preencherObjetos();
    arquivo.close();
    


    cout << "Mapa carregado com sucesso\n";
    cin >> name;
    return true;
}

void exportFile(string name, gameState game)
{
    remove(name.c_str());
    ofstream arquivo;
    arquivo.open(name);
    arquivo << "bombermansavedata ";
    arquivo << game.mapa.sizeX << " " << game.mapa.sizeY << " " << game.QtdPlayers << " " << game.contPlayers << " " << game.QtdInimigos << " " << game.contInimigos << "\n";

    for (int i = 0; i < game.QtdPlayers; i++)
    {
        player tempPlayer = game.players[i];
        tempPlayer.controles.deletarEspacos();
        arquivo << tempPlayer.objeto.x << " " << tempPlayer.objeto.y << " ";
        arquivo << tempPlayer.bomba.objeto.x << " " << tempPlayer.bomba.objeto.y << " " << tempPlayer.bomba.set << " " << tempPlayer.bomba.status << " " << tempPlayer.bomba.raio << " ";
        arquivo << tempPlayer.bomba.atravessaParede << " " << tempPlayer.status << " " << tempPlayer.ghostPowerup << " ";
        arquivo << tempPlayer.controles.up << " " << tempPlayer.controles.down << " " << tempPlayer.controles.left << " " << tempPlayer.controles.right << " " << tempPlayer.controles.bomb << " ";
        arquivo << "\n";
    }
    for (int i = 0; i < game.QtdInimigos; i++)
    {
        inimigo tempInimigo = game.inimigos[i];
        arquivo << tempInimigo.objeto.x << " " << tempInimigo.objeto.y << " " << tempInimigo.status;
        arquivo << "\n";
    }

    for (int i = 0; i < game.mapa.sizeY; i++)
    {
        for (int j = 0; j < game.mapa.sizeX; j++)
        {
            arquivo << game.mapa.mapa[i][j];
            arquivo << " ";
        }
        arquivo << "\n";
    }

    arquivo.close();
}