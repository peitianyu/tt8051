#include <stdio.h>
#include <string.h>
#include "common/c_test.h"
#include "common/logger.h"

// 解析函数声明
int parseMovStatement(const char* statement);

// 处理函数声明
void processMovStatement(const char* operand, int immediateValue);


JUST_RUN_TEST(mov, test)
TEST(mov, test){
    const char* statement = "MOV A, #42";
    int result = parseMovStatement(statement);

    if (result == 0) {
        printf("语句解析成功！\n");
    } else {
        printf("语句解析失败。\n");
    }
}

int parseMovStatement(const char* statement) {
    const char* keyword = "MOV";
    char operand[10];
    int immediateValue;

    // 解析操作符
    if (sscanf(statement, "%*s %[^,], #%d", operand, &immediateValue) == 2) {
        // 处理操作
        processMovStatement(operand, immediateValue);
        return 0; // 成功解析并处理语句
    }

    return -1; // 语句解析失败
}

void processMovStatement(const char* operand, int immediateValue) {
    printf("执行 MOV 操作：%s, #%d\n", operand, immediateValue);

    
}