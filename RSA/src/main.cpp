#include "main.h"


int main(){
    uintn a,b,c,m;
    a.set("FFFFFFF");
    b.set("111111111");
    m.set("11111111");
    
    c=a.modexp(b,m);

    std::cout << "Result :" << c << std::endl;
    return 0;
}
