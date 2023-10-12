#include "menuLoops.h"

void mapEditorLoop()
{
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
            if (ghost.isInsideMap(0, -1,currentframe))
                ghost.y--;
            break; /// cima
        case 80:
        case 's':
            if (ghost.isInsideMap(0, 1,currentframe))
                ghost.y++;
            break; /// baixo
        case 75:
        case 'a':
            if (ghost.isInsideMap(-1, 0,currentframe))
                ghost.x--;
            break; /// esquerda
        case 77:
        case 'd':
            if (ghost.isInsideMap(1, 0,currentframe))
                ghost.x++;
            break; /// direita

        // A array objetos possui QtdInimigos+1 itens, logo QtdInimigos é o último ID da array
        case 'q': // seleciona o proximo item
            if (Iditemselecionado > 0)
            {
                Iditemselecionado-= 1;
            }
            else
            {
                Iditemselecionado = currentGame.QtdInimigos + currentGame.QtdPlayers - 1; // Ultimo ID
            }
            break;
        case 'e': // seleciona o item anterior
            if (Iditemselecionado < currentGame.QtdInimigos + currentGame.QtdPlayers - 1)
            { // Id selecionado menor q o ultimo ID
                Iditemselecionado+= 1;
            }
            else
            {
                Iditemselecionado = 0;
            }
            break;
        case ' ': // Bara de espaço, coloca o item selecionado
            currentGame.objetos[Iditemselecionado]->x = ghost.x;
            currentGame.objetos[Iditemselecionado]->y = ghost.y;
            break;
        case 'z':
            currentGame.mapa.mapa[ghost.y][ghost.x] = vazioid;
            break;
        case 'x':
            currentGame.mapa.mapa[ghost.y][ghost.x] = paredeid;
            break;
        case 'c':
            currentGame.mapa.mapa[ghost.y][ghost.x] = paredefragilid;
            break;
        case 27: // ESC
            currentGame.Restart(defaultGame);
            system("cls");
            gameStatus = InMenu;
            return;
            break;
        case 13: // Enter
            // Define as variaveis padrao para as variaveis editadas
            defaultGame.Restart(currentGame);
            system("cls");
            gameStatus = InMenu;
            return;
            break;
        }
    }
    //------------------
    currentGame.AddItensToMap(currentframe);
    draw_hud(currentGame.objetos[Iditemselecionado]->id, Iditemselecionado);
    currentframe.SumItens(ghost.toCore());
    currentframe.draw_map();
}