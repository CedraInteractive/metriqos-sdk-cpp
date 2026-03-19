#include "metriqos/metriqos.h"
#include "raylib.h"

int main() {
    InitWindow(800, 600, "Metriqos SDK Demo");
    SetTargetFPS(60);

    Metriqos::Config config;
    config.apiKey = "mq_live_your_key_here";
    config.appVersion = "0.1.0";
    config.endpoint = "https://api.metriqos.io";
    config.batchIntervalMs = 10000;
    config.debug = true;
    Metriqos::init(config);

    Metriqos::identify("player_42");
    Metriqos::setUserProperty("tier", "gold");

    int score = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            score += 10;
            Metriqos::EventData scoreEvent;
            scoreEvent.nums = {{"score", (double)score}};
            scoreEvent.strs = {{"trigger", "spacebar"}};
            Metriqos::track("score_increase", scoreEvent);
        }

        if (IsKeyPressed(KEY_B)) {
            Metriqos::EventData bossEvent;
            bossEvent.nums = {{"health", 100.0}, {"damage", 25.5}};
            bossEvent.strs = {{"boss_id", "dragon_01"}};
            bossEvent.pos = {120.0f, 45.0f, 0.0f};
            bossEvent.hasPos = true;
            bossEvent.mapId = "world_1";
            bossEvent.category = "gameplay";
            Metriqos::track("boss_fight", bossEvent);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(TextFormat("Score: %d", score), 10, 10, 20, DARKGRAY);
        DrawText("SPACE = score | B = boss fight | ESC = quit", 10, 40, 16, GRAY);

        Metriqos::Status status = Metriqos::getStatus();
        DrawText(TextFormat("Queued: %d | Offline: %d | Quota: %lld",
            status.queuedEvents, status.offlineEvents, status.quotaRemaining),
            10, 70, 14, BLUE);
        EndDrawing();
    }

    Metriqos::shutdown();
    CloseWindow();
    return 0;
}
