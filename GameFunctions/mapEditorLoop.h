#include "menuLoops.h"

void editPlayerControlsLoop(int &etapa, char &botao)
{
    
    if (_kbhit())
    {
        char tecla = getch();
        if (tecla != 27)
            botao = tecla;
        etapa++;
        system("cls");
        if(_kbhit())etapa--;
        return;
    }
}

void editPlayerloop(int indexPlayer, int &tela)
{

    static int etapa = 0;
    char tecla;
    switch (etapa)
    {
    case 0:
        cout << "Selecione o tamanho padrao da bomba \n";
        int newBombSize;
        cin >> newBombSize;
        currentGame.players[indexPlayer].bomba.resizeRaio(newBombSize);
        system("cls");
        etapa++;
        break;
    case 1:
        cout << "Digite uma tecla para acao cima";
        editPlayerControlsLoop(etapa, currentGame.players[indexPlayer].controles.up);
        return;
        break;
    case 2:
        cout << "Digite uma tecla para acao baixo";
        editPlayerControlsLoop(etapa, currentGame.players[indexPlayer].controles.down);
        return;
        break;
    case 3:
        cout << "Digite uma tecla para acao esquerda";
        editPlayerControlsLoop(etapa, currentGame.players[indexPlayer].controles.left);
        return;
        break;
    case 4:
        cout << "Digite uma tecla para acao direita";
        editPlayerControlsLoop(etapa, currentGame.players[indexPlayer].controles.right);
        return;
        break;
    case 5:
        cout << "Digite uma tecla para acao bomba";
        editPlayerControlsLoop(etapa, currentGame.players[indexPlayer].controles.bomb);
        return;
        break;
    case 6:
        tela = 0;
        etapa = 0;
        system("cls");
        break;
    }
}

void mapEditorLoop()
{
    static int Iditemselecionado = 0; // Id do item selecionado no editor de mapas
    static int IndexCenarioSelecionado = 0;
    currentGame.preencherObjetos();
    currentframe.copyMap(currentGame.mapa);
    /// executa os movimentos
    if (_kbhit())
    {
        char tecla = getch();
        switch (tecla)
        {
        case 72:
        case 'w': 
            if (ghost.isInsideMap(0, -1, currentframe))
                ghost.y--;
            break; /// cima
        case 80:
        case 's':
            if (ghost.isInsideMap(0, 1, currentframe))
                ghost.y++;
            break; /// baixo
        case 75:
        case 'a':
            if (ghost.isInsideMap(-1, 0, currentframe))
                ghost.x--;
            break; /// esquerda
        case 77:
        case 'd':
            if (ghost.isInsideMap(1, 0, currentframe))
                ghost.x++;
            break; /// direita

        // A array objetos possui QtdInimigos+1 itens, logo QtdInimigos é o último ID da array
        case '1': // seleciona o proximo item
            rotateItem(Iditemselecionado, currentGame.QtdInimigos + currentGame.QtdPlayers, -1);
            break;
        case '3': // seleciona o item anterior
            rotateItem(Iditemselecionado, currentGame.QtdInimigos + currentGame.QtdPlayers, 1);
            break;
        case 'z': // Bara de espaço, coloca o item selecionado
            currentGame.objetos[Iditemselecionado]->x = ghost.x;
            currentGame.objetos[Iditemselecionado]->y = ghost.y;
            break;
        case 'q':
            rotateItem(IndexCenarioSelecionado,QtdIdsFixos,-1);
            break;
        case 'e':
            rotateItem(IndexCenarioSelecionado,QtdIdsFixos,1);
            break;
        case ' ':
            currentGame.mapa.mapa[ghost.y][ghost.x] = idsCenario[IndexCenarioSelecionado];
            break;
        case 27: // ESC
            currentGame.Restart(defaultGame, true);
            system("cls");
            gameStatus = InMenu;
            return;
            break;
        case 13: // Enter
            // Define as variaveis padrao para as variaveis editadas
            defaultGame.Restart(currentGame, true);
            system("cls");
            gameStatus = InMenu;
            return;
            break;
        case 't':
            gameStatus = InMapEditorConfig;
            system("cls");
            return;
            break;
        }
    }
    //------------------
    currentGame.AddItensToMap(currentframe);
    draw_hud(currentGame.objetos[Iditemselecionado]->id, Iditemselecionado);
    draw_hud(idsCenario[IndexCenarioSelecionado],IndexCenarioSelecionado);
    currentframe.SumItens(ghost.toCore());
    currentframe.draw_map();
}

void mapEditorConfigLoop()
{
    static int tela = 0;
    static int selecPlayer = 0;
    static int selecInimigo = 0;
    int sizeX, sizeY, qtd;
    char confirm;
    cout << "\033[37m";
    switch (tela)
    {
    case 0:

        cout << "Configuracoes do game \n";
        cout << "1. Tamanho do Mapa \n";
        cout << "2. Quantidade de players \n";
        cout << "3. Quantidade de inimigos \n";
        cout << "w. Editar Player P" << selecPlayer << "\n";
        cout << "s. Editar Inimigo N" << selecInimigo << "\n";
        cout << "Esc. Voltar ao editor \n";
        if (_kbhit())
        {
            char tecla = getch();
            switch (tecla)
            {
            case '1':
                tela = 1;
                system("cls");
                break;
            case '2':
                tela = 2;
                system("cls");
                break;
            case '3':
                tela = 3;
                system("cls");
                break;
            case 'q':
                rotateItem(selecPlayer, currentGame.QtdPlayers, -1);
                break;
            case 'e':
                rotateItem(selecPlayer, currentGame.QtdPlayers, 1);
                break;
            case 'w':
                tela = 4;
                system("cls");
                break;
            case 'a':
                rotateItem(selecInimigo, currentGame.QtdInimigos, -1);
                break;
            case 's':
                tela = 5;
                system("cls");
                break;
            case 'd':
                rotateItem(selecInimigo, currentGame.QtdInimigos, 1);
                break;

            case 27:
                gameStatus = InMapEditor;
                ghost.x = 0;
                ghost.y = 0;
                system("cls");
                break;
            }
        }
        break;

    case 1:
        cout << "Definir novo tamanho de mapa: \n";
        cout << "Altura do Mapa: ";
        cin >> sizeY;
        cout << "Largura do Maoa:";
        cin >> sizeX;
        cout << "Confirmar alteracoes? (digite Y para confirmar) \n";
        cout << "Altura: " << sizeY << ", Largura: " << sizeX << "\n";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            currentGame.resizeMap(sizeX, sizeY);
            currentframe.copyMap(currentGame.mapa, true);
        }
        tela = 0;
        system("cls");
        break;
    case 2:
        cout << "Definir Quantidade de players: \n";
        cout << "Quantidade: ";
        cin >> qtd;
        cout << "Confirmar alteracoes? (digite Y para confirmar) \n";
        cout << "Quantidade de players: " << qtd << "\n";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            currentGame.resizePlayers(qtd);
            currentGame.preencherObjetos();
        }
        tela = 0;
        system("cls");
        break;
    case 3:
        cout << "Definir Quantidade de inimigos: \n";
        cout << "Quantidade: ";
        cin >> qtd;
        cout << "Confirmar alteracoes? (digite Y para confirmar) \n";
        cout << "Quantidade de inimigos: " << qtd << "\n";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y')
        {
            currentGame.resizeInimigos(qtd);
            currentGame.preencherObjetos();
        }
        tela = 0;
        system("cls");
        break;
    case 4:
        editPlayerloop(selecPlayer, tela);
        break;
    case 5:
        break;
    }
}
