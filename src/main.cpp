#include <raylib.h>

#include <iostream>
#include <limits>
#include <string>
#include <sstream>
#include <string_view>
#include <functional>

#include "Random.h"

struct Monster
{
    int health{};
    int gold{};
    int damage{};
};

struct Player
{
    int health{};
    int gold{};
    int damage{};
    int level{};
};

//--------------------------------------------------------------------------
// Global constants
//--------------------------------------------------------------------------
constexpr int screenWidth{1026};
constexpr int screenHeight{768};
const char* screenTitle{"Monster battle"};

int FPS{144};

enum GameScreen{TITLE, MAIN};
enum GameState{FIGHT, WIN, LOSE, PAUSE};

std::string imagesPath{std::string(GetWorkingDirectory()) + "\\src\\images"};
std::string iconPath{imagesPath + "\\icon.png"};
Image icon{LoadImage(iconPath.c_str())};

std::string bg1Path{imagesPath + "\\backgrounds\\environment_forestbackground1-1.png"};
std::string bg2Path{imagesPath + "\\backgrounds\\environment_forest_evening.png"};

std::string knightIdlePath{imagesPath + "\\sprites\\knight-idle.png"};
Vector2 knightIdlePos{10.f, 447.f};

std::string dragonIdlePath{imagesPath + "\\sprites\\dragon-idle.png"};
Vector2 dragonIdlePos{600.f, 447.f};

std::string slimeIdlePath{imagesPath + "\\sprites\\slime-idle.png"};
Vector2 slimeIdlePos{650.f, 480.f};

std::string orcIdlePath{imagesPath + "\\sprites\\orc-idle.png"};
Vector2 orcIdlePos{560.f, 420.f};

std::string knightDiePath{imagesPath + "\\sprites\\sprite_sheet.png"};
Vector2 knightDiePos{250.f, 300.f};

Player player{12, 0, 1, 1};

Monster dragon{20, 100, 4};
Monster orc{4, 25, 2};
Monster slime{1, 10, 1};

std::reference_wrapper<Monster> monsters[] {
    dragon, orc, slime
};
int monsterIndex{Random::get(0, 2)};

// -------------------------------------------------------------------------
// Helper functions
// -------------------------------------------------------------------------
void playKnightIdle(Texture2D& knightIdle, Rectangle& knightIdleRec)
{
    static int currentFrameX{0};
    static int framesCounter{0};
    int framesSpeed{12};

    ++framesCounter;

    if (framesCounter >= (FPS / framesSpeed))
    {
        framesCounter = 0;
        ++currentFrameX;
        if (currentFrameX > 12)
        {
            currentFrameX = 0;
        }

        knightIdleRec.x = static_cast<float>(currentFrameX) * static_cast<float>(knightIdle.width / 12);
    }
    DrawTextureRec(knightIdle, knightIdleRec, knightIdlePos, WHITE);
}

void playDragonIdle(Texture2D& dragonIdle, Rectangle& dragonIdleRec)
{
    static int currentFrameX{0};
    static int framesCounter{0};
    int framesSpeed{6};

    ++framesCounter;

    if (framesCounter >= (FPS / framesSpeed))
    {
        framesCounter = 0;
        ++currentFrameX;
        if (currentFrameX > 3)
        {
            currentFrameX = 0;
        }

        dragonIdleRec.x = static_cast<float>(currentFrameX) * static_cast<float>(dragonIdle.width / 3);
    }
    DrawTextureRec(dragonIdle, dragonIdleRec, dragonIdlePos, WHITE);
}

void playSlimeIdle(Texture2D& slimeIdle, Rectangle& slimeIdleRec)
{
    static int currentFrameX{0};
    static int framesCounter{0};
    int framesSpeed{8};

    ++framesCounter;

    if (framesCounter >= (FPS / framesSpeed))
    {
        framesCounter = 0;
        ++currentFrameX;
        if (currentFrameX > 4)
        {
            currentFrameX = 0;
        }

        slimeIdleRec.x = static_cast<float>(currentFrameX) * static_cast<float>(slimeIdle.width / 4);
    }
    DrawTextureRec(slimeIdle, slimeIdleRec, slimeIdlePos, WHITE);
}

void playOrcIdle(Texture2D& orcIdle, Rectangle& orcIdleRec)
{
    static int currentFrameX{0};
    static int framesCounter{0};
    int framesSpeed{9};

    ++framesCounter;

    if (framesCounter >= (FPS / framesSpeed))
    {
        framesCounter = 0;
        ++currentFrameX;
        if (currentFrameX > 7)
        {
            currentFrameX = 0;
        }

        orcIdleRec.x = static_cast<float>(currentFrameX) * static_cast<float>(orcIdle.width / 7);
    }
    DrawTextureRec(orcIdle, orcIdleRec, orcIdlePos, WHITE);
}

void playKnightDie(Texture2D& knightDie, Rectangle& knightDieRec)
{
    static int currentFrameX{0};
    static int currentFrameY{0};
    static int framesCounter{0};
    
    static bool finished{false};

    int framesSpeed{12};

    ++framesCounter;

    if (framesCounter >= (FPS / framesSpeed) && !finished)
    {
        framesCounter = 0;
        ++currentFrameX;
        if (currentFrameX > 5)
        {
            currentFrameX = 0;
            ++currentFrameY;
            if (currentFrameY > 4)
            {
                finished = true;
                currentFrameX = 5;
                currentFrameY = 4;
            }
        }

        knightDieRec.x = static_cast<float>(currentFrameX) * static_cast<float>(knightDie.width / 6);
        knightDieRec.y = static_cast<float>(currentFrameY) * static_cast<float>(knightDie.height / 5);
    }
    
    DrawTextureRec(knightDie, knightDieRec, knightDiePos, WHITE);
}

//--------------------------------------------------------------------------
// Entry point
//--------------------------------------------------------------------------
int main()
{
    //----------------------------------------------------------------------
    // Initializing window
    //----------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, screenTitle);
    SetWindowIcon(icon);
    SetExitKey(KEY_Q);
    SetTargetFPS(FPS);

    //----------------------------------------------------------------------
    // Sprites and textures
    //----------------------------------------------------------------------
    Texture2D bg1{LoadTexture(bg1Path.c_str())};
    Texture2D bg2{LoadTexture(bg2Path.c_str())};
    
    Texture2D knightIdle{LoadTexture(knightIdlePath.c_str())};
    Rectangle knightIdleRec{0.f, 0.f, static_cast<float>(knightIdle.width / 12), static_cast<float>(knightIdle.height)};

    Texture2D dragonIdle{LoadTexture(dragonIdlePath.c_str())};
    Rectangle dragonIdleRec{0.f, 0.f, static_cast<float>(dragonIdle.width / 3), static_cast<float>(dragonIdle.height)};

    Texture2D slimeIdle{LoadTexture(slimeIdlePath.c_str())};
    Rectangle slimeIdleRec{0.f, 0.f, static_cast<float>(slimeIdle.width / 4), static_cast<float>(slimeIdle.height)};
    
    Texture2D orcIdle{LoadTexture(orcIdlePath.c_str())};
    Rectangle orcIdleRec{0.f, 0.f, static_cast<float>(orcIdle.width / 7), static_cast<float>(orcIdle.height)};

    Texture2D knightDie{LoadTexture(knightDiePath.c_str())};
    Rectangle knightDieRec{0.f, 0.f, static_cast<float>(knightDie.width / 6), static_cast<float>(knightDie.height / 5)};
    
    // ---------------------------------------------------------------------
    // Misc
    // ---------------------------------------------------------------------
    GameScreen currentScreen{TITLE};
    GameState currentState{FIGHT};
    Monster currentMonster = monsters[monsterIndex];
    double pauseTime{2};

    while (!WindowShouldClose())
    {
        // -----------------------------------------------------------------
        // Update
        // -----------------------------------------------------------------
        switch(currentScreen)
        {
            case TITLE:
            {
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                    currentScreen = MAIN;
            }
            break;
        }

        // -----------------------------------------------------------------
        // Draw
        // -----------------------------------------------------------------
        BeginDrawing();

        switch(currentScreen)
        {
        case TITLE:
        {
            DrawTexture(bg1, 0, 0, WHITE);
            DrawText("Monster Battle", screenWidth / 4, screenHeight / 3.5, 60, ORANGE);
            DrawText("Press 'SPACE/ENTER' to play\n\nPress 'Q' to quit", screenWidth / 4, screenHeight / 2, 30, ORANGE);
            DrawText("Controls: 'R' to escape/run, 'F' to fight", screenWidth / 4, screenHeight / 1.5, 30, ORANGE);
        }
        break;

        case MAIN:
        {
            DrawTexture(bg2, 0, 0, WHITE);
            switch(currentState)
            {
            case FIGHT:
            {
                DrawText(
                    TextFormat("Health: %i", player.health),
                     50, 50, 45, ORANGE
                );
                DrawText(
                    TextFormat("Level: %i", player.level),
                     50, 150, 45, ORANGE
                );
                DrawText(
                    TextFormat("Health: %i", currentMonster.health),
                     700, 50, 45, ORANGE
                );
                if (player.health > 0 && player.level < 20)
                {
                    playKnightIdle(knightIdle, knightIdleRec);
                    
                    switch(monsterIndex)
                    {
                    case 0: playDragonIdle(dragonIdle, dragonIdleRec); break;
                    case 1: playOrcIdle(orcIdle, orcIdleRec); break;
                    case 2: playSlimeIdle(slimeIdle, slimeIdleRec); break;
                    }
                    
                    if (IsKeyPressed(KEY_R))
                    {
                        bool escaped{static_cast<bool>(Random::get(0, 1))};
                        if (escaped)
                        {
                            monsterIndex = Random::get(0, 2);
                            currentMonster = monsters[monsterIndex];
                            break;
                        }
                        else
                        {
                            player.health -= currentMonster.damage;
                            currentState = PAUSE;
                        }
                    }
                    else if (IsKeyPressed(KEY_F))
                    {
                        currentMonster.health -= player.damage;
                        if (currentMonster.health <= 0)
                        {
                            player.gold += currentMonster.gold;
                            ++player.level;
                            ++ player.damage;
                            monsterIndex = Random::get(0, 2);
                            currentMonster = monsters[monsterIndex];
                            break;
                        }

                        player.health -= currentMonster.damage;
                    }
                }
                else if (player.health <= 0)
                {
                    currentState = LOSE;
                }
                else if (player.level >= 20)
                {
                    currentState = WIN;
                }
            }
            break;

            case PAUSE:
            {
                pauseTime -= static_cast<double>(GetFrameTime());
                playKnightIdle(knightIdle, knightIdleRec);
                DrawText("You failed to escape!", screenWidth / 3, screenHeight / 3.5, 60, ORANGE);
                if (pauseTime <= 0)
                {
                    pauseTime = 2;
                    currentState = FIGHT;
                }
            }
            break;

            case LOSE:
            {
                playKnightDie(knightDie, knightDieRec);
                DrawText("You lost!", screenWidth / 3 + 100, screenHeight / 3.5, 60, ORANGE);
            }
            break;

            case WIN:
            {
                playKnightIdle(knightIdle, knightIdleRec);
                DrawText("You won!", screenWidth / 3 + 100, screenHeight / 3.5, 60, ORANGE);
            }
            }
            break;
        }
        break;
        default: break;
        }

        EndDrawing();
    }

    // ---------------------------------------------------------------------
    // Deinitialization
    // ---------------------------------------------------------------------
    UnloadImage(icon);
    UnloadTexture(bg1);
    UnloadTexture(bg2);
    UnloadTexture(knightIdle);
    UnloadTexture(dragonIdle);
    UnloadTexture(slimeIdle);
    UnloadTexture(orcIdle);
    UnloadTexture(knightDie);

    CloseWindow();

    return 0;
}
