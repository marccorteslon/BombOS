/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

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
map<char, Texture2D> textures_textures2D;

int height;
int width;

int start_raylib(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - cubicmap rendering");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = { 16.0f, 14.0f, 16.0f };     // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type

    for (auto const& it : textures_files) {
        string image_file = "resources/" + it.second;
    }

    Image image_tmp = LoadImage("resources/cubicmap.png");      // Load cubicmap image (RAM)
    Texture2D texture_tmp = LoadTextureFromImage(image_tmp);       // Convert image to texture to display (VRAM)
    UnloadImage(image_tmp);


    Vector3 mapPosition = { -16.0f, 0.0f, -8.0f };          // Set model position

    UnloadImage(image_tmp);     // Unload cubesmap image from RAM, already uploaded to VRAM

    bool pause = false;     // Pause camera orbital rotation (and zoom)

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_P)) pause = !pause;

        if (!pause) UpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
       

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //0UnloadTexture(cubicmap);    // Unload cubicmap texture
    //UnloadTexture(texture);     // Unload map texture
    //UnloadModel(model);         // Unload map model

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

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

    getline(level, tmp, ';');

    // Leer FLOOR

    start_raylib();

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

    //return 0;

    //start_raylib();

}
