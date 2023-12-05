#include "common/c_test.h"
#include "common/logger.h"


// JUST_RUN_TEST(hello, test)
TEST(hello, test)
{
    LOG("hello world %d %d %d", 1, 2, 3);
    LOG_INFO("hello world %d %d %d", 1, 2, 3);
    LOG_DEBUG("hello world %d %d %d", 1, 2, 3);
    LOG_WARN("hello world %d %d %d", 1, 2, 3);
    LOG_ERROR("hello world %d %d %d", 1, 2, 3);

    SET_LOG_FILE("../out/matrix_c1.log", 1024 * 1024 * 10);

    LOG_FILE("hello world %d %d %d", 1, 2, 3);
}