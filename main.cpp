#include "arquivos.hpp"
#include "algoritmos.hpp"
#include "grafosempeso.hpp"
#include "grafocompeso.hpp"

int main(){
    int tipo = 0;


    gera_pastas();
    do{

        do{
        system("cls");
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
        cout << "{|                 M E N U                   |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 1 - Grafos SEM Peso                       |}" << "\n";
        cout << "{| 2 - Grafos COM Peso                       |}" << "\n";
        cout << "{|-------------------------------------------|}" << "\n";
        cout << "{| 0 - Sair                                  |}" << "\n";
        cout << "{|+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+|}" << "\n";
            cin>>tipo;
        }while (tipo < 0 || tipo > 2);

        if (tipo != 0){

            switch (tipo){
            case 1: gspMenu(); break;

            case 2: gcpMenu(); break;
            }

            cout << "\n=============================================";
            cout << "\nPressione ENTER para voltar ao menu...";
            cin.ignore();
            cin.get();
        }

    } while (tipo != 0);

    return 0;
}