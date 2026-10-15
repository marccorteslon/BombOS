#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string header;
string version;
string title;
string lvl;
string textures;

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
        cout << "Error 3: Version incorrecta";
        return 3;
    }

    cout << "Version: " << version;

    getline(level, title, ';');
    getline(level, title, ';');


    if (title != "TITLE") {
        cout << "Error 4: No título";
        return 4;
    }

    getline(level, lvl, ';');

    if (lvl != "Nivel 1") {
        cout << "Error 5: Nivel erróneo";
        return 5;
    }

    cout << "Level: " << lvl;

    getline(level, textures, ';');
    getline(level, textures, ';');


    if (textures != "TEXTURES") {
        cout << "Error 6: No textures";
        return 6;
    }
    
    string tmp;

    getline(level, tmp);

    return 0;
}