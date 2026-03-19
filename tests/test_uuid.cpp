#include <iostream>
#include <cassert>
#include <set>
#include <string>
#include "internal.h"

using Metriqos::Internal::generateUUID;
using Metriqos::Internal::nowISO8601;

void test_uuid_uniqueness() {
    std::cout << "  uuid_uniqueness... ";
    std::set<std::string> uuids;
    for (int i = 0; i < 1000; ++i) {
        auto id = generateUUID();
        assert(uuids.find(id) == uuids.end());
        uuids.insert(id);
    }
    assert(uuids.size() == 1000);
    std::cout << "OK\n";
}

void test_uuid_format() {
    std::cout << "  uuid_format... ";
    for (int i = 0; i < 100; ++i) {
        auto id = generateUUID();
        assert(id.size() == 36);
        assert(id[8] == '-');
        assert(id[13] == '-');
        assert(id[18] == '-');
        assert(id[23] == '-');
    }
    std::cout << "OK\n";
}

void test_uuid_version4() {
    std::cout << "  uuid_version4... ";
    for (int i = 0; i < 100; ++i) {
        auto id = generateUUID();
        assert(id[14] == '4');
    }
    std::cout << "OK\n";
}

void test_now_iso8601_length() {
    std::cout << "  now_iso8601_length... ";
    auto ts = nowISO8601();
    assert(ts.size() == 24);
    std::cout << "OK\n";
}

void test_now_iso8601_ends_with_z() {
    std::cout << "  now_iso8601_ends_with_z... ";
    auto ts = nowISO8601();
    assert(ts.back() == 'Z');
    std::cout << "OK\n";
}

void test_now_iso8601_has_t_separator() {
    std::cout << "  now_iso8601_has_t_separator... ";
    auto ts = nowISO8601();
    assert(ts.find('T') != std::string::npos);
    std::cout << "OK\n";
}

int main() {
    std::cout << "test_uuid\n";
    test_uuid_uniqueness();
    test_uuid_format();
    test_uuid_version4();
    test_now_iso8601_length();
    test_now_iso8601_ends_with_z();
    test_now_iso8601_has_t_separator();
    std::cout << "All uuid tests passed.\n";
    return 0;
}
