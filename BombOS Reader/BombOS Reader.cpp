#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string header;
string version;

int main()
{
    ifstream level("level_1.bos");

    if (!level.is_open()) {
        cout << "Error 1: No se pudo abrir el archivo";
        return 1;
    }

    getline(level, header, ';');

    if (header != "BOS") {
        cout << "Error 2: Formato incorrecto";
        return 2;

    }

    getline(level, version, ';');

    if (version != "0.5") {
        cout << "Error 2: Version incorrecta";
        return 2;
    }

    cout << "Version: " << version;
    
    string tmp;

    getline(level, tmp);

    return 0;
}