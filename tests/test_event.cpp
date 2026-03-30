#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "internal.h"

namespace Teliqos::Internal {
    nlohmann::json eventToJson(const Event& e);
    nlohmann::json batchToJson(const std::vector<Event>& events);
}

using Teliqos::Internal::Event;
using Teliqos::Internal::eventToJson;
using Teliqos::Internal::batchToJson;

void test_event_serialization_full() {
    std::cout << "  event_serialization_full... ";
    Event e;
    e.eventName = "player_kill";
    e.category = "combat";
    e.sessionId = "sess-123";
    e.playerId = "player-456";
    e.deviceId = "hw-device-001";
    e.timestamp = "2025-01-15T10:30:00.000Z";
    e.nums["damage"] = 75.5;
    e.strs["weapon"] = "sword";
    e.tags.push_back("pvp");
    e.tags.push_back("ranked");
    e.pos[0] = 1.0f; e.pos[1] = 2.0f; e.pos[2] = 3.0f;
    e.hasPos = true;
    e.mapId = "arena_01";
    e.device["os"] = "windows";
    e.appVersion = "1.2.3";

    auto j = eventToJson(e);

    assert(j["event_name"] == "player_kill");
    assert(j["category"] == "combat");
    assert(j["session_id"] == "sess-123");
    assert(j["player_id"] == "player-456");
    assert(j["device_id"] == "hw-device-001");
    assert(j["timestamp"] == "2025-01-15T10:30:00.000Z");
    assert(j["nums"]["damage"] == 75.5);
    assert(j["strs"]["weapon"] == "sword");
    assert(j["tags"].size() == 2);
    assert(j["tags"][0] == "pvp");
    assert(j["tags"][1] == "ranked");
    assert(j["pos"].is_array());
    assert(j["pos"].size() == 3);
    assert(j["pos"][0] == 1.0f);
    assert(j["pos"][1] == 2.0f);
    assert(j["pos"][2] == 3.0f);
    assert(j["map_id"] == "arena_01");
    assert(j["device"]["os"] == "windows");
    assert(j["app_version"] == "1.2.3");
    std::cout << "OK\n";
}

void test_event_serialization_optional_omitted() {
    std::cout << "  event_serialization_optional_omitted... ";
    Event e;
    e.eventName = "page_view";
    e.sessionId = "sess-789";
    e.playerId = "player-000";
    e.deviceId = "hw-device-002";
    e.timestamp = "2025-01-15T10:30:00.000Z";
    // Leave category, mapId, device, appVersion empty
    // Leave hasPos = false

    auto j = eventToJson(e);

    assert(j.contains("event_name"));
    assert(j.contains("session_id"));
    assert(j.contains("player_id"));
    assert(j.contains("device_id"));
    assert(j.contains("timestamp"));
    assert(!j.contains("category"));
    assert(!j.contains("map_id"));
    assert(!j.contains("device"));
    assert(!j.contains("app_version"));
    assert(!j.contains("pos"));
    assert(!j.contains("nums"));
    assert(!j.contains("strs"));
    assert(!j.contains("tags"));
    std::cout << "OK\n";
}

void test_batch_serialization() {
    std::cout << "  batch_serialization... ";
    Event e1;
    e1.eventName = "event_a";
    e1.sessionId = "s1";
    e1.playerId = "p1";
    e1.deviceId = "hw-1";
    e1.timestamp = "2025-01-15T10:00:00.000Z";

    Event e2;
    e2.eventName = "event_b";
    e2.sessionId = "s2";
    e2.playerId = "p2";
    e2.deviceId = "hw-2";
    e2.timestamp = "2025-01-15T10:01:00.000Z";

    auto j = batchToJson({e1, e2});

    assert(j.contains("events"));
    assert(j["events"].is_array());
    assert(j["events"].size() == 2);
    assert(j["events"][0]["event_name"] == "event_a");
    assert(j["events"][1]["event_name"] == "event_b");
    std::cout << "OK\n";
}

int main() {
    std::cout << "test_event\n";
    test_event_serialization_full();
    test_event_serialization_optional_omitted();
    test_batch_serialization();
    std::cout << "All event tests passed.\n";
    return 0;
}
