#include "gtest/gtest.h"
#include "thread_pool.h"

TEST(ThreadPool, post) {
    ThreadPool pool;
    future<bool> result = pool.post([] {
        return true;
    });
    EXPECT_TRUE(result.get());
}
