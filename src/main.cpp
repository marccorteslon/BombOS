/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "rlgl.h"       // Required to define vertex data (immediate-mode style)

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
map<char, Texture2D> textures2D;
int level_w = 0;
int level_h = 0;

char** floor_map;
char** map_map;
char** collisions_map;
char** objects_map;

void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color); // Draw cube textured
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube with a region of a texture

int start_raylib(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - cubicmap rendering");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = { 0.0f, 12.0f, 0.0f };     // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 0.0f, -1.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type


    for (auto const& it : textures_files) {
        string image_file = "resources/" + it.second;

        Image image_tmp = LoadImage(image_file.c_str());
        Texture2D texture_tmp = LoadTextureFromImage(image_tmp);       // Convert image to texture to display (VRAM)
        UnloadImage(image_tmp);

        textures2D.insert({ it.first, texture_tmp });
    }

    

    bool pause = false;     // Pause camera orbital rotation (and zoom)

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    float offset_x = -level_w / 2;

    if (level_w % 2 == 0) {
        offset_x += 0.5;
    }

    float offset_z = -level_h / 2;

    if (level_h % 2 == 0) {
        offset_z += 0.5;
    }

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_P)) pause = !pause;

        //if (!pause) UpdateCamera(&camera, CAMERA_ORBITAL);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        
        BeginMode3D(camera);

        // Draw cube with an applied texture
        // DrawCubeTexture(textures2D['a'], { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, WHITE);

        // For para ir dibujando los cubos

        for (int i = 0; i < level_h; i++) {
            for (int j = 0; j < level_w; j++) {

                char textureLetter = floor_map[i][j];

                DrawCubeTexture(
                    textures2D[textureLetter],
                    { j + offset_x, 0.0f, i + offset_z },
                    1.0f, 1.0f, 1.0f, WHITE
                );
            }
        }

        for (int i = 0; i < level_h; i++) {
            for (int j = 0; j < level_w; j++) {

                char textureLetter = map_map[i][j];

                if (textureLetter != '0') {
                    DrawCubeTexture(
                        textures2D[textureLetter],
                        { j + offset_x, 1.0f, i + offset_z },
                        1.0f, 1.0f, 1.0f, WHITE
                    );
                }
            }
        }

        DrawGrid(10, 1.0f);        // Draw a grid

        EndMode3D();




        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------


    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


int main()
{
    ifstream level("level_1.bos");

    if (!level.is_open()) {
        cout << "Error 1: No se pudo abrir el archivo" << endl;
        level.close();
        return 1;
    }


    getline(level, header, ';');

    if (header != "BOS") {
        cout << "Error 2: Formato incorrecto.";
        level.close();
        return 2;
    }

    getline(level, version, ';');

    cout << "Version: " << version << endl;

    string tmp;

    getline(level, tmp);

    getline(level, tmp, ';');

    if (tmp != "TITLE") {
        cout << "Error 3: Formato de título incorrecto.";
        level.close();
        return 3;
    }

    getline(level, title, ';');

    getline(level, tmp);


    getline(level, tmp, ';');

    if (tmp != "TEXTURES") {
        cout << "Error 4: Formato de texturas incorrecto.";
        level.close();
        return 4;
    }

    getline(level, tmp, ';');

    textures_num = stoi(tmp);

    getline(level, tmp);


    for (int i = 0; i < textures_num; i++) {
        getline(level, tmp, ';');

        char tmp_char = tmp[0];

        getline(level, tmp, ';');

        textures_files.insert({ tmp_char, tmp });

        getline(level, tmp);
    }

    for (auto texture_file : textures_files) {
        cout << texture_file.first << "=>" << texture_file.second << endl;
    }


    getline(level, tmp, ';');

    if (tmp != "SIZE") {
        cout << "Error 5: Formato de tamaño incorrecto.";
        level.close();
        return 5;
    }

    getline(level, tmp, ';');
    level_w = stoi(tmp);

    getline(level, tmp, ';');
    level_h = stoi(tmp);

    getline(level, tmp);

    // FLOOR
    getline(level, tmp, ';');

    if (tmp != "FLOOR") {
        cout << "Error 6: Formato de suelo incorrecto.";
        level.close();
        return 6;
    }

    getline(level, tmp);


    cout << "FLOOR" << endl;

    floor_map = new char* [level_h];

    for (int i = 0; i < level_h; i++) {
        floor_map[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level, tmp, ';');

            floor_map[i][j] = tmp[0];
        }
        getline(level, tmp);
    }


    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << floor_map[i][j];
        }

        cout << endl;
    }

    getline(level, tmp, ';');

    if (tmp != "MAP") {
        cout << "Error 7: Formato de mapa incorrecto.";
        level.close();
        return 7;
    }

    getline(level, tmp);


    cout << "MAP" << endl;

    map_map = new char* [level_h];

    for (int i = 0; i < level_h; i++) {
        map_map[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level, tmp, ';');

            map_map[i][j] = tmp[0];
        }
        getline(level, tmp);
    }


    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << map_map[i][j];
        }

        cout << endl;
    }


    getline(level, tmp, ';');

    if (tmp != "COLLISIONS") {
        cout << "Error 8: Formato de mapa incorrecto.";
        level.close();
        return 8;
    }

    getline(level, tmp);


    cout << "COLLISIONS" << endl;


    collisions_map = new char* [level_h];

    for (int i = 0; i < level_h; i++) {
        collisions_map[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level, tmp, ';');

            collisions_map[i][j] = tmp[0];
        }
        getline(level, tmp);
    }


    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << collisions_map[i][j];
        }

        cout << endl;
    }



    getline(level, tmp, ';');

    if (tmp != "OBJECTS") {
        cout << "Error 9: Formato de mapa incorrecto.";
        level.close();
        return 9;
    }

    getline(level, tmp);


    cout << "OBJECTS" << endl;


    objects_map = new char* [level_h];

    for (int i = 0; i < level_h; i++) {
        objects_map[i] = new char[level_w];
    }

    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            getline(level, tmp, ';');

            objects_map[i][j] = tmp[0];
        }
        getline(level, tmp);
    }


    for (int i = 0; i < level_h; i++) {
        for (int j = 0; j < level_w; j++) {
            cout << objects_map[i][j];
        }

        cout << endl;
    }

    level.close();

    start_raylib();

    return 0;
}


//------------------------------------------------------------------------------------
// Custom Functions Definition
//------------------------------------------------------------------------------------
// Draw cube textured
// NOTE: Cube position is the center position
void DrawCubeTexture(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // Vertex data transformation can be defined with the commented lines,
    // but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
    //rlPushMatrix();
        // NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
        //rlTranslatef(2.0f, 0.0f, 0.0f);
        //rlRotatef(45, 0, 1, 0);
        //rlScalef(2.0f, 2.0f, 2.0f);

    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);
    // Front Face
    rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    // Back Face
    rlNormal3f(0.0f, 0.0f, -1.0f);     // Normal Pointing Away From Viewer
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    // Top Face
    rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    // Bottom Face
    rlNormal3f(0.0f, -1.0f, 0.0f);     // Normal Pointing Down
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    // Right face
    rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z - length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z - length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width / 2, y + height / 2, z + length / 2);  // Top Left Of The Texture and Quad
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width / 2, y - height / 2, z + length / 2);  // Bottom Left Of The Texture and Quad
    // Left Face
    rlNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Pointing Left
    rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z - length / 2);  // Bottom Left Of The Texture and Quad
    rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width / 2, y - height / 2, z + length / 2);  // Bottom Right Of The Texture and Quad
    rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z + length / 2);  // Top Right Of The Texture and Quad
    rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width / 2, y + height / 2, z - length / 2);  // Top Left Of The Texture and Quad
    rlEnd();
    //rlPopMatrix();

    rlSetTexture(0);
}

// Draw cube with texture piece applied to all faces
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float texWidth = (float)texture.width;
    float texHeight = (float)texture.height;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // We calculate the normalized texture coordinates for the desired texture-source-rectangle
    // It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent
    rlBegin(RL_QUADS);
    rlColor4ub(color.r, color.g, color.b, color.a);

    // Front face
    rlNormal3f(0.0f, 0.0f, 1.0f);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);

    // Back face
    rlNormal3f(0.0f, 0.0f, -1.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);

    // Top face
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);

    // Bottom face
    rlNormal3f(0.0f, -1.0f, 0.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);

    // Right face
    rlNormal3f(1.0f, 0.0f, 0.0f);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z - length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x + width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x + width / 2, y - height / 2, z + length / 2);

    // Left face
    rlNormal3f(-1.0f, 0.0f, 0.0f);
    rlTexCoord2f(source.x / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z - length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, (source.y + source.height) / texHeight);
    rlVertex3f(x - width / 2, y - height / 2, z + length / 2);
    rlTexCoord2f((source.x + source.width) / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z + length / 2);
    rlTexCoord2f(source.x / texWidth, source.y / texHeight);
    rlVertex3f(x - width / 2, y + height / 2, z - length / 2);

    rlEnd();

    rlSetTexture(0);
}