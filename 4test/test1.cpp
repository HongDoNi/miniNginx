// 测试全全局变量能否用于进程间通信
#include <iostream>
#include <unistd.h>

using namespace std;


int *pi = new int{20};

int main() {
    pid_t pid  = fork();
    if(pid > 0) {
        // parent
        *pi = 100;
        
        while(true) {
            sleep(1);
        }
    } 
    else if(pid == 0) {
        // child
        *pi = 200;
        while(true) {
            sleep(1);
        }
    }
    else {
        cout << "error" << endl;
        return -1;
    }
    return 0;
}
