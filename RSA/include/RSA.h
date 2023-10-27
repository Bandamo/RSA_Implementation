#ifndef RSA_INCLUDED
#define RSA_INCLUDED

#include "main.h"

struct public_key{
	u64 n;
	u64 e;
};

struct key_pair{
	public_key public_k;
	u64 private_k;
};

class RSA {
    public:
        RSA();
        ~RSA();
        key_pair key_pair_generation();
        u64 RSA_encrypt(u64 m, public_key k);
        u64 RSA_decrypt(u64 c, key_pair k);
    
    private:
        u64 carmichael(u64 n);
        u64 gcd(u64 a, u64 b);
        u64 modexp(u64 k, int m, u64 n);
        u64 lcm(u64 a, u64 b);
};

#endif // RSA_INCLUDED