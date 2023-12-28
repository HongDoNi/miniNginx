#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main() {
    int i = 0x12345678;
    char *p = (char*)&i;
    if(*p == 0x12) {
        cout << "big" << endl;
    }
    else cout << "small" << endl;


    int32_t ii = htonl(i);
    p = (char*) &ii;
    if(*p == 0x12) {
        cout << "big" << endl;
    }
    else cout << "small" << endl;
    return 0;
}