#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <algorithm>

#define u64 uint64_t

void find_prime_in(u64 min_val, u64 max_val, std::vector<u64> prev_prime, std::string filename){
    //std::cout << "Start finding prime in range " << min_val << " to " << max_val << "\n";
    std::vector<u64> primes;
    u64 size = prev_prime.size();
    for (u64 i = min_val; i<max_val; i++){
        for (u64 j = 0; j<size; j++){
            if (i%prev_prime[j] == 0){
                break;
            }
            if (j == prev_prime.size()-1){
                primes.push_back(i);
            }
        }
    }

    std::ofstream file;
    file.open(filename);
    for (u64 i = 0; i<primes.size(); i++){
        file << primes[i] << "\n";
    }
    file.close();
}

void dispatch(std::vector<u64> prev_prime, int j){
    std::vector<u64> primes;
    int border[j+1];
    u64 begin = prev_prime[prev_prime.size()-1];
    int max_interv = begin;
    if (max_interv > 100000){
        max_interv = 100000;
    }
    for (int i = 0; i<10; i++){
        border[i] = (begin+1)+max_interv*i/j;
    }
    border[j] = begin+max_interv;
    std::thread processes[j];
    for (int i = 0; i<j; i++){
        processes[i] = std::thread(find_prime_in, border[i], border[i+1], prev_prime, "share/"+std::to_string(i));
    }
    for (int i = 0; i<j; i++){
        processes[i].join();
    }
}

std::vector<u64> recover_primes(int j, std::vector<u64> prev_prime){
    std::vector<u64> primes = prev_prime;
    for (int i = 0; i<j; i++){
        std::ifstream file;
        file.open("share/"+std::to_string(i));
        std::string line;
        while (std::getline(file, line)){
            primes.push_back(std::stoull(line));
        }
        file.close();
    }
    return primes;
}

void write_primes(std::vector<u64> primes){
    std::ofstream file;
    file.open("primes.txt");
    for (u64 i = 0; i<primes.size(); i++){
        file << primes[i] << "\n";
    }
    file.close();
}

std::vector<u64> read_primes(){
    std::vector<u64> primes;
    std::ifstream file;
    file.open("primes.txt");
    std::string line;
    while (std::getline(file, line)){
        primes.push_back(std::stoull(line));
    }
    file.close();
    std::cout << "Loaded " << primes.size() << " primes\n";
    return primes;
}

int main(int argc, char** argv){
    int j = 10;

    std::vector<u64> primes = read_primes();
    for (int i = 0; i<1000; i++){
        std::cout << "Max prime: " << primes[primes.size()-1] << "\n";
        dispatch(primes, j);
        primes = recover_primes(j, primes);
        write_primes(primes);
    }
    

}