#include "common/c_test.h"
#include "common/logger.h"

#include <stdio.h>
#include <string.h>

int next_token(char** src)
{
    
}

JUST_RUN_TEST(demo, test)
TEST(demo, test)
{
    int fd;
    char* src_file = "../data/code_file.S";
    if ((fd = open(src_file, 0)) < 0) {
        printf("could not open(%s)\n", src_file);
        return -1;
    }

    int src_size = 256 * 1024;/*src file size*/
    char* src = malloc(src_size);/*read src file string*/
    if(read(fd, src, src_size)){
        
    }
}