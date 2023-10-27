#ifndef UINTN_INCLUDED
#define UINTN_INCLUDED

#include "main.h"

class uintn{
    public:
        uintn();
        uintn(u64);
        ~uintn();
        void set(std::string);
        void reset();
        void autocomplete();
        uintn operator+(uintn);
        bool operator>(uintn);
        uintn operator*(uintn);
        uintn operator-(uintn);
        uintn operator/(uintn);
        uintn operator%(uintn);
        friend std::ostream& operator<<(std::ostream&, uintn&);
        u64 size;
        std::vector<u32> data;
        void set_pow2(u64); 
        uintn modexp(uintn,uintn);
};

#endif // UINTN_INCLUDED
