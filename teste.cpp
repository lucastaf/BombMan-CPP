#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    ifstream arquivo;

    arquivo.open("jogobosta.txt");
    if (arquivo.is_open())
    {
        int i;
        string texto;
        while (arquivo >> texto)
        {
            i = stoi(texto);
            cout << i;
            cout << '\n';
        }
        arquivo.close();
    }
    else
    {
        cout << "Não abriu";
    }
    return 0;
}