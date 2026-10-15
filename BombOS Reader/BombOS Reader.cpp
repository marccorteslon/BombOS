#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

string header;
string version;
string title;

int textures_num = 0;
map<char, string> textures_files;

int height;
int width;


//string tmp; // Variable temporal para la ultima ;

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

    cout << "Version: " << version << endl;

    /*  HECHO POR MI CUENTA PERO ESTA MAL

    getline(level, title, ';');
    getline(level, title, ';');


    if (title != "TITLE") {
        cout << "Error 4: No titulo";
        return 4;
    }

    getline(level, lvl, ';');

    if (lvl != "Nivel 1") {
        cout << "Error 5: Nivel erroneo";
        return 5;
    }

    cout << "Level: " << lvl;

    getline(level, textures, ';');
    getline(level, textures, ';');


    if (textures != "TEXTURES") {
        cout << "Error 6: No textures";
        return 6;
    }

    getline(level, texNum, ';');


    if (texNum != "5") {
        cout << "Error 7: No textures";
        return 6;
    }*/

    string tmp;

    getline(level, tmp);

    getline(level, tmp, ';');

    if (tmp != "TITLE") {
        cout << "Error 3: Title incorrecto";
        level.close();
        return 3;
    }

    getline(level, title, ';');

    getline(level, tmp);

    getline(level, tmp, ';');


    if (tmp != "TEXTURES") {
        cout << "Error 4: Formato incorrecto";
        level.close();
        return 4;
    }

    getline(level, tmp, ';');

    // Leer el numero del nivel

    textures_num = stoi(tmp);

    getline(level, tmp);

    for (int i = 0; i < textures_num; i++) {
        getline(level, tmp, ';');

        char tmp_char = tmp[0];

        getline(level, tmp, ';'); // Leemos textura

        textures_files.insert({ tmp_char, tmp });

        getline(level, tmp);
    }

    for (auto texture_file : textures_files) {
        cout << texture_file.first << "=>" << texture_file.second << endl;
    }

    // Leer SIZE

    getline(level, tmp, ';');

    if (tmp != "SIZE") {
        cout << "Error 5: Formato incorrecto";
        level.close();
        return 5;
    }

    // Leer width y height

    getline(level, tmp, ';');
    width = stoi(tmp);

    getline(level, tmp, ';');
    height = stoi(tmp);

    cout << "\nWidth: " << width;

    cout << " Height: " << height << endl;


    /*for (int i = 0; i < textures_num; i++) {

        level.close();

        return 0;
    }*/

    //getline(level, tmp, ';');

    // Leer FLOOR

    getline(level, tmp, ';');

    cout << tmp;

    if (tmp != "FLOOR") {
        cout << "Error 6: Formato incorrecto";
        level.close();
        return 6;
    }

    getline(level, tmp);

    char** floor_map = new char* [height];

    for (int i = 0; i < height; i++) {
        floor_map[i] = new char[width];
    }

    // Leer el mapa

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            getline(level, tmp, ';');

            floor_map[i][j] = tmp[0];
        }
        getline(level, tmp);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << floor_map[i][j];
        }
        cout << endl;
    }





    level.close();

    return 0;

}

    // Width primero, height después