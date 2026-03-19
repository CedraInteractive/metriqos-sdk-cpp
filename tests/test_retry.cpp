#include <iostream>
#include <cassert>
#include "internal.h"

using Metriqos::Internal::calculateBackoffMs;

void test_backoff_attempt_0() {
    std::cout << "  backoff_attempt_0... ";
    // Base delay is 1s (1000ms), jitter +/-25% → [750, 1250]
    for (int i = 0; i < 50; ++i) {
        int ms = calculateBackoffMs(0, 0);
        assert(ms >= 750 && ms <= 1250);
    }
    std::cout << "OK\n";
}

void test_backoff_attempt_4() {
    std::cout << "  backoff_attempt_4... ";
    // 1 << 4 = 16s → 16000ms, jitter +/-25% → [12000, 20000]
    for (int i = 0; i < 50; ++i) {
        int ms = calculateBackoffMs(4, 0);
        assert(ms >= 12000 && ms <= 20000);
    }
    std::cout << "OK\n";
}

void test_backoff_gives_up() {
    std::cout << "  backoff_gives_up... ";
    assert(calculateBackoffMs(5, 0) == -1);
    assert(calculateBackoffMs(6, 0) == -1);
    assert(calculateBackoffMs(100, 0) == -1);
    std::cout << "OK\n";
}

void test_backoff_retry_after() {
    std::cout << "  backoff_retry_after... ";
    // retryAfterSec=5 → 5000ms, jitter +/-25% → [3750, 6250]
    for (int i = 0; i < 50; ++i) {
        int ms = calculateBackoffMs(0, 5);
        assert(ms >= 3750 && ms <= 6250);
    }
    std::cout << "OK\n";
}

int main() {
    std::cout << "test_retry\n";
    test_backoff_attempt_0();
    test_backoff_attempt_4();
    test_backoff_gives_up();
    test_backoff_retry_after();
    std::cout << "All retry tests passed.\n";
    return 0;
}
