#include <iostream>
#include <fstream>
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

u64 pow(u64 a, u64 b){
	// Find the power of a number
	u64 result = 1;
	for (int i=0; i<b; i++){
		result = result*a;
	}
	return result;
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

u64 lcm(u64 a, u64 b){
	// Find the least common multiple of two numbers
	return (a*b)/gcd(a,b);
}

uint64_t modexp(uint64_t k, int m, uint64_t n){
	// Find the modular exponentiation of k^m mod n
	uint64_t result = 1;
	for (int i=0; i<m; i++){
		result = (result*k) % n;
	}
	return result;
}

struct prime_dec
{
	u64 number;
	u64 power;
};

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

u64 carmichael(uint64_t n) {
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

key_pair key_pair_generation(){
	std::cout << "-> Prime number generation :"<<std::endl;
	u64 p,q;
	srand(time(NULL));
	u64 max = 256000;
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

u64 RSA_encrypt(u64 m, public_key k){
	return modexp(m,k.e,k.n);
}

u64 RSA_decrypt(u64 c, key_pair k){
	return modexp(c,k.private_k,k.public_k.n);
}

int main(){
	std::cout << "KEY GENERATION"<<std::endl;
	key_pair keys = key_pair_generation();
	
	
	std::cout << "ENCRYPTION"<<std::endl;
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	std::cout << "-> Message :"<<std::endl;
	u64 m = rand()%(4096);
	std::cout << "m = " << m << std::endl;
	std::cout << "-> Encrypted message :"<<std::endl;
	u64 c = modexp(m,keys.public_k.e,keys.public_k.n);
	std::cout << "c = " << c << std::endl;
	
	duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - duration;
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

	std::cout << "DECRYPTION"<<std::endl;
	std::cout << "-> Decrypted message :"<<std::endl;
	u64 m2 = modexp(c,keys.private_k,keys.public_k.n);
	std::cout << "m2 = " << m2 << std::endl;
	duration2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - duration2;

	bool success = (m==m2);
	int bytes = log2(keys.public_k.n);
	std::cout << "Key lenght : " << bytes << std::endl;
	std::cout << "Success : " << success << std::endl;
	std::cout << "Encryption : " << duration << " microseconds" << std::endl;
	std::cout << "Decryption : " << duration2 << " microseconds" << std::endl;
	
	mean_time_encrypt += duration;
	mean_time_decrypt += duration2;
	success_number += success;
	return 0;
}
