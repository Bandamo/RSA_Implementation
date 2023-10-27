#include "main.h"

std::vector<u64> load_prime(){
	// Load prime number in prime.txt
	std::vector<u64> primes;
	std::ifstream f;
	f.open("prime.txt");
	if (f.is_open()){
		std::string line;
		while (getline(f,line)){
			//remove the last character
			u64 number = std::stoull(line);
			primes.push_back(number);
		}
	}
	return primes;
}

struct prime_dec
{
	u64 number;
	u64 power;
};

RSA::RSA(){
    // Constructor
}

RSA::~RSA(){
    // Destructor
}


u64 RSA::lcm(u64 a, u64 b){
    // Find the least common multiple of two numbers
    return (a*b)/gcd(a,b);
}

u64 RSA::gcd(u64 a, u64 b){
	// Find the greatest common divisor of two numbers
	int r = 0;
	while (b != 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

uint64_t RSA::modexp(uint64_t k, int m, uint64_t n){
	// Find the modular exponentiation of k^m mod n
	uint64_t result = 1;
	for (int i=0; i<m; i++){
		result = (result*k) % n;
	}
	return result;
}

u64 RSA::carmichael(uint64_t n) {
	// Get prime decomposition
	//std::cout << "-> Prime decomposition :"<<std::endl;
	std::vector<u64> primes = load_prime();

	std::vector<prime_dec> decomposition;

	bool error = false;

	while (n != 1 && !error){
		for (int i=0; i<primes.size(); i++){
			if (n % primes[i] == 0){
				prime_dec p;
				p.number = primes[i];
				p.power = 0;
				while (n % primes[i] == 0){
					p.power++;
					n = n/primes[i];
				}
				decomposition.push_back(p);
			}
		}
		if (n != 1){
			error = true;
			std::cout << "Error : " << n << " is not a prime number" << std::endl;
		}
	}

	// Compute the carmichael function
	//std::cout << "-> Carmichael function :"<<std::endl;
	u64 lambda = 1;
	int size = decomposition.size();
	for (int i=0; i<size; i++){
		u64 p = decomposition[i].number;
		u64 k = decomposition[i].power;
		u64 l = 1;
		if (p==2){
			if (k==1){
				l = 1;
			}
			else if (k==2){
				l = 2;
			}
			else{
				l = pow(2,k-2);
			}
		}
		else{
			l = pow(p,k-1)*(p-1);
		}
		lambda = lcm(lambda,l);
		
	}

	return lambda;
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

key_pair RSA::key_pair_generation(){
	std::cout << "-> Prime number generation :"<<std::endl;
	u64 p,q;
	srand(time(NULL));
	u64 max = 429490;
	p = random_primary(rand()%max);
	q = random_primary(rand()%max);
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
	while ((d*e)%lambda != 1){
		d++;
	}
	
	std::cout << "d = " << d << std::endl;

	key_pair keys;
	keys.public_k.e = e;
	keys.public_k.n = n;
	keys.private_k = d;

	return keys;

}

u64 RSA::RSA_encrypt(u64 m, public_key k){
	return modexp(m,k.e,k.n);
}

u64 RSA::RSA_decrypt(u64 c, key_pair k){
	return modexp(c,k.private_k,k.public_k.n);
}