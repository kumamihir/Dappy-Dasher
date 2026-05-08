#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updatetime;
    float runningtime;
};

int main() {
    const int window_width = 600;
    const int window_height = 600;
    InitWindow(window_width, window_height, "Game");

    const int gravity = 1000;
    const int nebCount = 10;
    const int jumpVal = -610;

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec = {0, 0, (float)scarfy.width / 6, (float)scarfy.height};
    scarfyData.pos = {(float)window_width/2 - scarfyData.rec.width/2, 
                      (float)window_height - scarfyData.rec.height};
    scarfyData.frame = 0;
    scarfyData.runningtime = 0;
    scarfyData.updatetime = 1.0/12.0f;

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");

    float bgx = 0, mgx = 0, fgx = 0;
    bool isInAir = false;
    bool collision = false;
    int velocity = 0;

    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebulae[nebCount];
    for(int i=0;i<nebCount;i++){
        nebulae[i].rec = {0,0,(float)nebula.width/8,(float)nebula.height/8};
        nebulae[i].pos = {(float)(500 + i*400), (float)(window_height-nebula.height/8)};
        nebulae[i].frame = 0;
        nebulae[i].runningtime = 0;
        nebulae[i].updatetime = 1.0/16.0f;
    }

    float finishLine = nebulae[nebCount-1].pos.x;
    int nebVel = -150;
    int gameState = 0; 

    SetTargetFPS(60);

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        if (gameState == 0) {
            bgx -= 20 * dt;
            if(bgx <= -background.width*2) bgx = 0;

            mgx -= 40 * dt;
            if(mgx <= -midground.width*2) mgx = 0;

            fgx -= 80 * dt;
            if(fgx <= -foreground.width*2) fgx = 0;

            DrawTextureEx(background, {bgx,0},0,3.5,WHITE);
            DrawTextureEx(background, {bgx+background.width*2,0},0,3.5,WHITE);
            DrawTextureEx(midground, {mgx,0},0,3.4,WHITE);
            DrawTextureEx(midground, {mgx+midground.width*2,0},0,3.4,WHITE);
            DrawTextureEx(foreground, {fgx,0},0,3.4,WHITE);
            DrawTextureEx(foreground, {fgx+foreground.width*2,0},0,3.4,WHITE);

            if(scarfyData.pos.y >= window_height - scarfyData.rec.height){
                velocity = 0;
                isInAir = false;
            } else {
                velocity += gravity * dt;
                isInAir = true;
            }

            if(!isInAir && IsKeyPressed(KEY_SPACE)) velocity += jumpVal;

            for(int i=0;i<nebCount;i++) nebulae[i].pos.x += nebVel * dt;
            finishLine += nebVel * dt;

            scarfyData.pos.y += velocity * dt;

            if(!isInAir){
                scarfyData.runningtime += dt;
                if(scarfyData.runningtime >= scarfyData.updatetime){
                    scarfyData.runningtime = 0;
                    scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                    scarfyData.frame++;
                    if(scarfyData.frame > 5) scarfyData.frame = 0;
                }
            }

            for(int i=0;i<nebCount;i++){
                nebulae[i].runningtime += dt;
                if(nebulae[i].runningtime >= nebulae[i].updatetime){
                    nebulae[i].runningtime = 0;
                    nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                    nebulae[i].frame++;
                    if(nebulae[i].frame > 7) nebulae[i].frame = 0;
                }
            }

            collision = false;
            for(auto neb : nebulae){
                float pad = 59;
                Rectangle nebRec = {neb.pos.x+pad, neb.pos.y+pad, neb.rec.width-pad*2, neb.rec.height-pad*2};
                Rectangle scarfRec = {scarfyData.pos.x, scarfyData.pos.y, scarfyData.rec.width, scarfyData.rec.height};
                if(CheckCollisionRecs(nebRec, scarfRec)) collision = true;
            }

            if(collision){
                gameState = 1;
            } 
            else if(scarfyData.pos.x >= finishLine){
                gameState = 2;
            } 
            else {
                for(int i=0;i<nebCount;i++) DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            }
        }
g++ -I"C:/raylib/raylib/src" -L"C:/raylib/raylib/lib" dasher.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o output/dasher.exeg++ -I"C:/raylib/raylib/src" -L"C:/raylib/raylib/lib" dasher.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o output/dasher.exe
        else if(gameState == 1){
            DrawText("GAME OVER!", 180, 260, 50, ORANGE);
            DrawText("Press R To Retry", 180, 320, 30, BLACK);
            if(IsKeyPressed(KEY_R)){
                scarfyData.pos = {(float)window_width/2-scarfyData.rec.width/2, 
                                  (float)window_height - scarfyData.rec.height};
                scarfyData.frame = 0;
                scarfyData.runningtime = 0;
                velocity = 0;
                for(int i=0;i<nebCount;i++){
                    nebulae[i].pos.x = 500 + i * 400;
                    nebulae[i].frame = 0;
                    nebulae[i].runningtime = 0;
                }
                finishLine = nebulae[nebCount-1].pos.x;
                gameState = 0;
            }
        }

        else if(gameState == 2){
            DrawText("YOU WIN! 🎉", 200, 200, 50, GREEN);

            Rectangle replayBtn = {200, 300, 200, 50};
            Rectangle exitBtn = {200, 360, 200, 50};
            Vector2 mp = GetMousePosition();

            Color replayColor = CheckCollisionPointRec(mp, replayBtn) ? LIGHTGRAY : GRAY;
            Color exitColor = CheckCollisionPointRec(mp, exitBtn) ? LIGHTGRAY : GRAY;

            DrawRectangleRec(replayBtn, replayColor);
            DrawRectangleRec(exitBtn, exitColor);

            DrawText("Replay", replayBtn.x + 45, replayBtn.y + 10, 30, BLACK);
            DrawText("Exit", exitBtn.x + 70, exitBtn.y + 10, 30, BLACK);

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(CheckCollisionPointRec(mp, replayBtn)){
                    scarfyData.pos = {(float)window_width/2-scarfyData.rec.width/2, 
                                      (float)window_height - scarfyData.rec.height};
                    scarfyData.frame = 0;
                    scarfyData.runningtime = 0;
                    velocity = 0;
                    for(int i=0;i<nebCount;i++){
                        nebulae[i].pos.x = 500 + i * 400;
                        nebulae[i].frame = 0;
                        nebulae[i].runningtime = 0;
                    }
                    finishLine = nebulae[nebCount-1].pos.x;
                    gameState = 0;
                }
                if(CheckCollisionPointRec(mp, exitBtn)){
                    CloseWindow();
                    return 0;
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
