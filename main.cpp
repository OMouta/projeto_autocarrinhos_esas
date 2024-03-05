#include <iostream>
#include <locale.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <conio.h>


using namespace std;

struct templateautomoveis
{
    char matricula[8], marca[30], modelo[20], cor[15], ob[100];
    int ano, mediacon;
    float km;
}c[4];

int main()
{
    int i, con;
    char op[8];

    setlocale(LC_ALL, "Portuguese");

    for(i = 0; i < 4; i++)
    {
        cout << "CARRO N. " << i + 1 << endl;
        cout << endl;
        cout << endl;

        cout << "Marca: ";
        fflush(stdin);
        gets(c[i].marca);
        cout << endl;

        cout << "Modelo: ";
        fflush(stdin);
        gets(c[i].modelo);
        cout << endl;

        cout << "Ano: ";
        cin >> c[i].ano;
        cout << endl;

        cout << "Matricula: ";
        fflush(stdin);
        gets(c[i].matricula);
        cout << endl;

        cout << "Cor: ";
        fflush(stdin);
        gets(c[i].cor);
        cout << endl;

        cout << "Kilometros: ";
        cin >> c[i].km;
        cout << endl;

        cout << "Media de consumo: ";
        cin >> c[i].mediacon;
        cout << endl;

        cout << "Observacoes (100 caracteres max): ";
        fflush(stdin);
        gets(c[i].ob);
        cout << endl;
        cout << endl;

        cout << "Para confirmar precione enter, para cancelar precione outro caracter!" << endl;
        con = getch();

        if(con =! 13)
        {
            i--;
        }

        system("CLS");
    }

    while (strcmp(op, "sair") != 0)
    {
        cout << "(Digite 'cmds' para a lista de commandos): " << endl;
        cout << ">> " << endl;

        fflush(stdin);
        gets(op);


        for(i = 0; i < 4; i++)
        {
            if(strcmp(c[i].matricula, op) == 0)
            {
                cout << "Marca: " << c[i].marca << endl;
            }
        }
    }
    
    system("Pause");

    return 0;
}