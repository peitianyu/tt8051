#include <stdio.h>
#include "common/c_test.h"
#include "common/logger.h"

// JUST_RUN_TEST(putc, test)
TEST(putc, test)
{
    FILE *fp;
    int ch;

    fp = fopen("../out/file.txt", "w+");
    for( ch = 33 ; ch <= 100; ch++ ) {
        fputc(ch, fp);
    }
    fclose(fp);
}