#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cstdint>
#include <string>
#include <chrono>
#include <thread>

#define u64 uint64_t

struct public_key{
	u64 n;
	u64 e;
};

struct key_pair{
	public_key public_k;
	u64 private_k;
};

void progressbar(u64 n, u64 nmax){
	// Print the progress bar
	std::cout << "[" << n << "/" << nmax << "] " << 100*n/nmax << "%" << std::endl;
}

u64 gcd(u64 a, u64 b){
	// Find the greatest common divisor of two numbers
	int r = 0;
	while (b != 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

uint64_t modexp(uint64_t k, int m, uint64_t n){
	// Find the modular exponentiation of k^m mod n
	uint64_t result = 1;
	for (int i=0; i<m; i++){
		result = (result*k) % n;
	}
	return result;
}

u64 carmichael(uint64_t n) {
  uint64_t k = 1;
  for( ;; ) {
	progressbar(k,n);
    int done = 1;
    for( long x=1; x<n; x++ ) {
      if( gcd(x,n)==1 ) {
		if (modexp(x,k,n) != 1){
        	done = 0;
        	k++;
			break;
		}
		else{

		}
      }
    }
    if(done) break;
  }
  return k;
}

int random_primary(int n){
	//Find the first primary number greater than n
	bool is_primary = false;
	int number = n;
	while (!(is_primary)){
		number++;
		is_primary = true;
		for (int k=2; k<number; k++){
			if (number % k == 0){
				is_primary = false;
				break;
			}
		}
	}
	return number;
}

key_pair key_pair_generation(){
		std::cout << "-> Prime number generation :"<<std::endl;
	u64 p,q;
	srand(time(NULL));
	p = random_primary(rand()/2);
	q = random_primary(rand()/2);
	std::cout << "p = " << p << std::endl;
	std::cout << "q = " << q << std::endl;
	u64 n = p*q;
	std::cout << "n = " << n << std::endl;
	
	std::cout << "-> Carmichael function :"<<std::endl;
	u64 lambda = carmichael(n);
	std::cout << "lambda = " << lambda << std::endl;

	std::cout << "-> Public key generation :"<<std::endl;

	u64 e = 0;
	while (gcd(e,lambda) != 1){
		e = rand()%lambda;
	}
	std::cout << "e = " << e << std::endl;

	std::cout << "-> Private key generation :"<<std::endl;
	u64 d = 0;
	while ((e*d)%lambda != 1){
		d++;
	}
	std::cout << "d = " << d << std::endl;

	key_pair keys;
	keys.public_k.e = e;
	keys.public_k.n = n;
	keys.private_k = d;

	return keys;

}

int main(){
	std::cout << "PROGRESS BAR"<<std::endl;
	
	std::cout << "KEY GENERATION"<<std::endl;
	key_pair keys = key_pair_generation();
	
	std::cout << "ENCRYPTION"<<std::endl;
	std::cout << "-> Message :"<<std::endl;
	u64 m;
	std::cin >> m;
	std::cout << "m = " << m << std::endl;
	std::cout << "-> Encrypted message :"<<std::endl;
	u64 c = modexp(m,keys.public_k.e,keys.public_k.n);
	std::cout << "c = " << c << std::endl;

	std::cout << "DECRYPTION"<<std::endl;
	std::cout << "-> Decrypted message :"<<std::endl;
	u64 m2 = modexp(c,keys.private_k,keys.public_k.n);
	std::cout << "m2 = " << m2 << std::endl;

}
