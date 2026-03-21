#include "teliqos/teliqos.h"
#include "raylib.h"

int main() {
    InitWindow(800, 600, "Teliqos SDK Demo");
    SetTargetFPS(60);

    Teliqos::Config config;
    config.apiKey = "mq_live_your_key_here";
    config.appVersion = "0.1.0";
    config.endpoint = "https://api.teliqos.io";
    config.batchIntervalMs = 10000;
    config.debug = true;
    Teliqos::init(config);

    Teliqos::identify("player_42");
    Teliqos::setUserProperty("tier", "gold");

    int score = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            score += 10;
            Teliqos::EventData scoreEvent;
            scoreEvent.nums = {{"score", (double)score}};
            scoreEvent.strs = {{"trigger", "spacebar"}};
            Teliqos::track("score_increase", scoreEvent);
        }

        if (IsKeyPressed(KEY_B)) {
            Teliqos::EventData bossEvent;
            bossEvent.nums = {{"health", 100.0}, {"damage", 25.5}};
            bossEvent.strs = {{"boss_id", "dragon_01"}};
            bossEvent.pos = {120.0f, 45.0f, 0.0f};
            bossEvent.hasPos = true;
            bossEvent.mapId = "world_1";
            bossEvent.category = "gameplay";
            Teliqos::track("boss_fight", bossEvent);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, DARKGRAY);
        DrawText("SPACE = score | B = boss fight | ESC = quit", 10, 40, 16, GRAY);

        Teliqos::Status status = Teliqos::getStatus();
        DrawText(TextFormat("Queued: %d | Offline: %d | Quota: %lld",
            status.queuedEvents, status.offlineEvents, status.quotaRemaining),
            10, 70, 14, BLUE);
        EndDrawing();
    }

    Teliqos::shutdown();
    CloseWindow();
    return 0;
}
