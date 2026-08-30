#include "raylib.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int TILE_SIZE = 30;
const int ROWS = 20;
const int COLS = 20;

// Картата: 0 = Път, 1 = Стена, 2 = Зелен бонус
int mapGrid[ROWS][COLS] = {
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0}
};

const float MAX_HEALTH = 10.0f;

struct Player {
    int x = 18;
    int y = 0;
    int score = 3;
    float health = 1.0f;
};

struct Enemy {
    int x;
    int y;
    int spawnX;
    int spawnY;
    float respawnTimer = 0.0f; // Таймер за 3-те секунди в сиво
    float health = 1.0f;
    int points = 10;
};

int LoadHighScore() {
    ifstream file("highscore.txt");
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void SaveHighScore(int score) {
    int currentHigh = LoadHighScore();
    if (score > currentHigh) {
        ofstream file("highscore.txt");
        if (file.is_open()) {
            file << score;
            file.close();
        }
    }
}

// Поставя ново зелено бонус квадратче (2) на случайна свободна клетка
void SpawnRandomBonus() {
    int randR, randC;
    do {
        randR = GetRandomValue(0, ROWS - 1);
        randC = GetRandomValue(0, COLS - 1);
    } while (mapGrid[randR][randC] != 0);

    mapGrid[randR][randC] = 2;
}

void TryMove(Player &player, int dx, int dy, float &powerUpTimer) {
    if (player.health <= 0.0f) return;

    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS) {
        if (mapGrid[newY][newX] != 1) { // Не е стена
            player.x = newX;
            player.y = newY;

            if (mapGrid[newY][newX] == 2) {
                powerUpTimer = 5.0f;
                mapGrid[newY][newX] = 0;
                SpawnRandomBonus();
            }
        }
    }
}

bool IsEnemyFrozen(const Enemy &enemy) {
    return enemy.respawnTimer > 0.0f;
}

bool IsTileOccupiedByEnemy(const vector<Enemy> &enemies, int x, int y, int ignoreIndex = -1) {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if ((int)i == ignoreIndex) continue;
        if (enemies[i].x == x && enemies[i].y == y) {
            return true;
        }
    }
    return false;
}

int CountValidNeighbors(int x, int y, const vector<Enemy> &enemies, int ignoreIndex = -1) {
    int count = 0;
    vector<Vector2> dirs = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
    for (const auto &dir : dirs) {
        int nx = x + (int)dir.x;
        int ny = y + (int)dir.y;
        if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS && mapGrid[ny][nx] != 1) {
            if (!IsTileOccupiedByEnemy(enemies, nx, ny, ignoreIndex)) {
                count++;
            }
        }
    }
    return count;
}

void ResetEnemyToSpawn(Enemy &enemy) {
    enemy.x = enemy.spawnX;
    enemy.y = enemy.spawnY;
    enemy.health = 1.0f;
    enemy.points = 10;
    enemy.respawnTimer = 0.0f;
}

void MoveEnemy(Enemy &enemy, const Player &player, const vector<Enemy> &enemies, int enemyIndex, float powerUpTimer) {
    if (player.health <= 0.0f || enemy.health <= 0.0f) return;

    vector<Vector2> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    int bestDx = 0;
    int bestDy = 0;
    float bestScore = (powerUpTimer <= 0.0f) ? 99999.0f : -99999.0f;
    bool foundMove = false;

    for (const auto &dir : directions) {
        int targetX = enemy.x + (int)dir.x;
        int targetY = enemy.y + (int)dir.y;

        if (targetX >= 0 && targetX < COLS && targetY >= 0 && targetY < ROWS) {
            if (mapGrid[targetY][targetX] == 1) continue;
            if (IsTileOccupiedByEnemy(enemies, targetX, targetY, enemyIndex)) continue;

            int dist = abs(player.x - targetX) + abs(player.y - targetY);
            float score = 0.0f;

            // Брой валидни съседи на целевата позиция (предпочитаме по-отворени области)
            int neighborCount = CountValidNeighbors(targetX, targetY, enemies, enemyIndex);
            float neighborBonus = neighborCount * 0.5f;

            if (powerUpTimer <= 0.0f) {
                // ЧЕРНИ: гонят играча.
                score = (float)dist;
                if (dist <= 2) score -= 3.0f;
                if (dist == 1) score -= 5.0f;
                if (abs(player.x - targetX) <= 1 && abs(player.y - targetY) <= 1) score -= 2.0f;
                score += neighborBonus; // Предпочитаме по-отворени пътища
            } else {
                // СИНИ: бягат от героя. Винаги се избира ход, който увеличава дистанцията.
                score = (float)dist;
                if (abs(player.x - targetX) <= 1 && abs(player.y - targetY) <= 1) score -= 50.0f;

                // Ако ходът е в посока към героя, го наказваме силно.
                int dxFromPlayer = targetX - player.x;
                int dyFromPlayer = targetY - player.y;
                int currentDx = enemy.x - player.x;
                int currentDy = enemy.y - player.y;
                if ((dxFromPlayer * currentDx + dyFromPlayer * currentDy) < 0) {
                    score -= 15.0f;
                }

                if (targetX <= 0 || targetX >= COLS - 1 || targetY <= 0 || targetY >= ROWS - 1) {
                    score -= 1000.0f;
                }
                
                score += neighborBonus; // Сините също предпочитат по-отворени пътища
            }

            if (powerUpTimer <= 0.0f) {
                if (score < bestScore) {
                    bestScore = score;
                    bestDx = (int)dir.x;
                    bestDy = (int)dir.y;
                    foundMove = true;
                }
            } else {
                if (score > bestScore) {
                    bestScore = score;
                    bestDx = (int)dir.x;
                    bestDy = (int)dir.y;
                    foundMove = true;
                }
            }
        }
    }

    if (powerUpTimer > 0.0f && !foundMove) {
        // Ако няма свободен ход в бягство, избягваме ръба чрез обратна посока.
        if (enemy.x <= 0) bestDx = 1;
        else if (enemy.x >= COLS - 1) bestDx = -1;
        else if (enemy.y <= 0) bestDy = 1;
        else if (enemy.y >= ROWS - 1) bestDy = -1;
        else {
            bestDx = (enemy.x < player.x) ? 1 : -1;
            bestDy = (enemy.y < player.y) ? 1 : -1;
        }
    }

    if (foundMove || (bestDx != 0 || bestDy != 0)) {
        int nextX = enemy.x + bestDx;
        int nextY = enemy.y + bestDy;

        if (nextX >= 0 && nextX < COLS && nextY >= 0 && nextY < ROWS && mapGrid[nextY][nextX] != 1) {
            enemy.x = nextX;
            enemy.y = nextY;
        }
    }
}

int main() {
    const int screenWidth = COLS * TILE_SIZE;
    const int screenHeight = (ROWS * TILE_SIZE) + 180;

    InitWindow(screenWidth, screenHeight, "Raylib - PacMan Labirint");
    SetTargetFPS(60);

    Player player;
    
    // Коригирани начални позиции на враговете (на свободни клетки '0')
    vector<Enemy> enemies = {
        {2, 0, 2, 0, 0.0f, 1.0f, 10},
        {19, 18, 19, 18, 0.0f, 1.0f, 15},
        {0, 18, 0, 18, 0.0f, 1.0f, 20}
    };

    float powerUpTimer = 0.0f;
    float enemyMoveTimer = 0.0f;
    int highScore = LoadHighScore();

    float btnSize = 45.0f;
    float centerX = (screenWidth / 2.0f) - (btnSize / 2.0f);
    float startY = (ROWS * TILE_SIZE) + 15.0f;

    Rectangle btnUp    = { centerX, startY, btnSize, btnSize };
    Rectangle btnDown  = { centerX, startY + 95.0f, btnSize, btnSize };
    Rectangle btnLeft  = { centerX - 50.0f, startY + 47.0f, btnSize, btnSize };
    Rectangle btnRight = { centerX + 50.0f, startY + 47.0f, btnSize, btnSize };

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (powerUpTimer > 0) {
            powerUpTimer -= deltaTime;
        }

        for (auto &e : enemies) {
            if (IsEnemyFrozen(e)) {
                e.respawnTimer -= deltaTime;
            }
        }

        // --- 1. УПРАВЛЕНИЕ ---
        if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) TryMove(player, 0, -1, powerUpTimer);
        if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) TryMove(player, 0, 1, powerUpTimer);
        if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) TryMove(player, -1, 0, powerUpTimer);
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) TryMove(player, 1, 0, powerUpTimer);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 clickPos = GetMousePosition();
            if (CheckCollisionPointRec(clickPos, btnUp))    TryMove(player, 0, -1, powerUpTimer);
            if (CheckCollisionPointRec(clickPos, btnDown))  TryMove(player, 0, 1, powerUpTimer);
            if (CheckCollisionPointRec(clickPos, btnLeft))  TryMove(player, -1, 0, powerUpTimer);
            if (CheckCollisionPointRec(clickPos, btnRight)) TryMove(player, 1, 0, powerUpTimer);
        }

        // --- 2. ДВИЖЕНИЕ НА ВРАГОВЕТЕ ---
        enemyMoveTimer += deltaTime;
        float enemyStep = (powerUpTimer > 0.0f) ? 0.18f : 0.30f;
        if (enemyMoveTimer >= enemyStep) {
            for (size_t i = 0; i < enemies.size(); ++i) {
                MoveEnemy(enemies[i], player, enemies, (int)i, powerUpTimer);
            }
            enemyMoveTimer = 0.0f;
        }

        // --- 3. СБЛЪСЪЦИ (без да спират — просто прехвърлят точки, докато нападателят седи върху жертвата) ---
        if (player.health > 0.0f) {
            for (auto &e : enemies) {
                if (e.health <= 0.0f) {
                    ResetEnemyToSpawn(e);
                    continue;
                }

                if (e.x == player.x && e.y == player.y) {
                    if (powerUpTimer > 0.0f) {
                        float transfer = 0.01f;
                        if (transfer > e.health) transfer = e.health;

                        e.health -= transfer;
                        player.health += transfer;

                        if (player.health > MAX_HEALTH) player.health = MAX_HEALTH;

                        if (e.health <= 0.0f) {
                            ResetEnemyToSpawn(e);
                            player.score += 10;
                            SaveHighScore(player.score);
                            highScore = LoadHighScore();
                        }
                    } else {
                        player.health -= 0.01f;
                        if (player.health < 0.0f) player.health = 0.0f;
                    }
                }
            }
        }

        // --- 4. РИСУВАНЕ ---
        BeginDrawing();
        ClearBackground(GetColor(0x0F172AFF));

        // Рисуване на картата
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                int posX = c * TILE_SIZE;
                int posY = r * TILE_SIZE;

                if (mapGrid[r][c] == 1) {
                    DrawRectangle(posX + 1, posY + 1, TILE_SIZE - 2, TILE_SIZE - 2, DARKGRAY);
                } else if (mapGrid[r][c] == 2) {
                    DrawRectangle(posX + 1, posY + 1, TILE_SIZE - 2, TILE_SIZE - 2, GREEN);
                } else {
                    DrawRectangle(posX + 1, posY + 1, TILE_SIZE - 2, TILE_SIZE - 2, GetColor(0x1E293BFF));
                }
            }
        }

        // Играч
        Color playerColor = (powerUpTimer > 0) ? GOLD : RED;
        DrawCircle(player.x * TILE_SIZE + TILE_SIZE / 2, player.y * TILE_SIZE + TILE_SIZE / 2, TILE_SIZE / 2 - 4, playerColor);
        DrawText(TextFormat("%.2f", player.health), player.x * TILE_SIZE + 5, player.y * TILE_SIZE - 12, 12, WHITE);

        // Врагове
        for (const auto &e : enemies) {
            Color enemyColor;
            if (IsEnemyFrozen(e)) {
                enemyColor = GRAY;
            } else if (powerUpTimer > 0.0f) {
                enemyColor = BLUE;
            } else {
                enemyColor = BLACK;
            }

            int drawX = e.x * TILE_SIZE + TILE_SIZE / 2;
            int drawY = e.y * TILE_SIZE + TILE_SIZE / 2;
            DrawCircle(drawX, drawY, TILE_SIZE / 2 - 4, enemyColor);
            DrawText(TextFormat("%.2f", e.health), drawX - 12, drawY - 22, 12, WHITE);

            if (IsEnemyFrozen(e)) {
                DrawText(TextFormat("%.1fs", e.respawnTimer), drawX - 10, drawY - 20, 10, WHITE);
            }
        }

        // Джойстик
        DrawRectangleRec(btnUp, SKYBLUE);   DrawText("^", btnUp.x + 18, btnUp.y + 10, 20, WHITE);
        DrawRectangleRec(btnDown, SKYBLUE); DrawText("v", btnDown.x + 18, btnDown.y + 10, 20, WHITE);
        DrawRectangleRec(btnLeft, SKYBLUE); DrawText("<", btnLeft.x + 16, btnLeft.y + 12, 20, WHITE);
        DrawRectangleRec(btnRight, SKYBLUE);DrawText(">", btnRight.x + 16, btnRight.y + 12, 20, WHITE);

        // UI
        DrawText(TextFormat("SCORE: %d | HIGH: %d", player.score, highScore), 10, 10, 18, YELLOW);
        DrawText(TextFormat("HEALTH: %.2f / %.2f", player.health, MAX_HEALTH), 10, 32, 18, GREEN);

        if (powerUpTimer > 0) {
            DrawText(TextFormat("POWER-UP: %.1fs", powerUpTimer), 10, 54, 18, GREEN);
        }

        if (player.health <= 0.0f) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.85f));
            DrawText("GAME OVER!", screenWidth / 2 - 90, screenHeight / 2 - 20, 30, RED);
            DrawText("Health reached 0", screenWidth / 2 - 80, screenHeight / 2 + 20, 18, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}