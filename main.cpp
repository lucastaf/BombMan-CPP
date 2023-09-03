/**
    Estrutura inicial para um jogo de labirinto
    versão: 0.1 (Felski)
*/


#include "screenLoops.cpp"

int main()
{
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, A SEGUIR.
        //INICIO: COMANDOS PARA QUE O CURSOR NAO FIQUE PISCANDO NA TELA
        srand(time(NULL));
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
        //FIM: COMANDOS PARA QUE O CURSOR NÃO FIQUE PISCANDO NA TELA
        //INÍCIO: COMANDOS PARA REPOSICIONAR O CUSOR NO INÍCIO DA TELA
        COORD coord;
        coord.X = 0;
        coord.Y = 0;
        //FIM: COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA
    ///ALERTA: NAO MODIFICAR O TRECHO DE CODIGO, ACIMA.
    if(QtdInimigos > ((sizex+1)/2)*((sizey+1)/2)-1) return 0;

    gerarMapaInicial(MapaInicial);
    gerarInimigosIniciais(inimigsoIniciais,inimigos);
    m = MapaInicial;
    for(int x=0;x<=raio*4;x++){
        //estabelece todas as explosões da bomba com um ID de uma explosão
        bomba1.explosao[0][x].id = explosaoid;
    }
    

    while(true) //Loop principal do Jogo
    {
        ///Posiciona a escrita no início do console
        SetConsoleCursorPosition(out, coord);
        switch(gameStatus){
            case InMenu:
                mainMenuLoop();
            break;
            case InGame:
                MainGameLoop();
            break;
            case Defeat:
                GameOverLoop(0);
            break;
            case Vicotry:
                GameOverLoop(1);
            break;
            case InMapEditor:
                mapEditorLoop();
            break;
        }
        


    
    } //fim do loop
    return 0;
}



    
