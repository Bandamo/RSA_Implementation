from math import gcd

f = open("primes.txt","r")
primes = [int(p) for p in f.read().split("\n") if p!=""]
f.close()
k = primes[-1]+1
saved = primes[-1]
l_saved = len(primes)
while True:
    if all(gcd(k,p)==1 for p in primes):
        primes.append(k)
        
    k+=1

    # append to file every 1000 numbers 
    if k-saved>=1000:
        f = open("primes.txt","a")
        f.write("\n"+"\n".join(str(p) for p in primes[l_saved:]))
        f.close()
        saved = k
        l_saved = len(primes)
        print("Saved",saved,"primes")
    
