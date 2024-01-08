#include <iostream>

typedef unsigned long           uintptr_t;

typedef struct stu{
    const char* name;
    unsigned int age;
}STU;

int main() {
    int i = 0;
    printf("sizeof int : %lu\n", sizeof(i));
    printf("sizeof uintptr_t : %lu\n", sizeof(uintptr_t));
    STU *p = new STU;
    printf("%lu\n", sizeof(p));
    printf("%p\n", p);
    uintptr_t p2 = (uintptr_t) p | 1;
    // printf("%p\n", p2);
    // unsigned int p3 = (unsigned int)p & 1;
    p = (STU*)((uintptr_t)p | 1);
    uintptr_t a = (uintptr_t)~1;
    p = (STU*)((uintptr_t)p & (uintptr_t) ~1);
    return 0;

}