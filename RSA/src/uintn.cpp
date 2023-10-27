#include "main.h"

void uintn::set_pow2(u64 n){
    this->size = n/32+1;
    this->reset();
    u32 tmp = 1;
    tmp = tmp >> n%32;
    this->data[this->size-1] = tmp;
}

uintn::uintn(){
    this->size = 0;
}

uintn::uintn(u64 size){
    this->size = size;
    for (int i = 0; i<this->size; i++){
        this->data.push_back(0);
    }
}

uintn::~uintn(){
    this->data.clear();
}

void uintn::autocomplete(){
    while (this->data.size() > this->size){
        this->data.pop_back();
    }
    while (this->data.size() < this->size){
        this->data.push_back(0);
    }
}

void uintn::reset(){
    this->autocomplete();
    for (u64 i=0; i<this->data.size(); i++){
        this->data[i]=0;
    }
}

void uintn::set(std::string s){
    // From string of hex character assign value
    // One hex is 4 bits
    this->size = (s.length()/8)+1;
    u64 multiplier = 1;
    u64 c;
    u64 index = 0;

    this->autocomplete();

    for (int i=0; i<s.length(); i++){
        index = i/8;
        multiplier = pow(16,i%8);

        c = s[s.length()-i-1];
        c = (c>='A')?(c-'A'+10):(c-'0');
        this->data[index] += c * multiplier;
    }
}



bool uintn::operator>(uintn b){
    if (this->size > b.size){
        return true;
    }
    else if (b.size > this->size){
        return false;
    }
    else{
        for (u64 i = b.size-1; i>=0; i--){
            if (b.data[i]>this->data[i]){
                return false;
            }
            else if (b.data[i]<this->data[i]){
                return true;
            }
        }
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, uintn& n){
    // Print number in Hexa
    char temp;
    for (int i=n.size-1; i>=0; i--){
        for (int j = 7; j >=0; j--){
            temp = (n.data[i] >> (j*4)) & 0xF;
            temp = (temp>=10)?('A'+temp-10):('0'+temp); 
            os<<temp;
        }
        os << " ";
    }
    return os;    
}

uintn uintn::operator+(uintn b){
    u64 temp;
    uintn c = uintn(std::max(this->size,b.size));
    u64 carry = 0;

    while(this->size<b.size){
        this->data.push_back(0);
        this->size++;
    }
    while(this->size>b.size){
        (b.data).push_back(0);
        b.size++;
    }

    for (int i=0; i<std::min(this->size,b.size); i++){
        temp = static_cast<u64>(this->data[i]) + static_cast<u64>(b.data[i]) + carry;
        carry = 0;
        if (temp > 0xFFFFFFFF){
            carry = temp>>32; 
            temp = temp & 0xFFFFFFFF;
        }

        c.data[i] = temp;
    }
    if (carry){
        size++;
        c.data[size-1] = 1;
    }
    
    return c;
}

uintn uintn::operator-(uintn b){
    if (b>(*this)){
        uintn result(1);
        return result;
    }
    uintn result(std::max(this->size,b.size));
    
    bool carry = 0;
    u64 tmp;
    for (u64 i = 0; i < result.size ; i++){
        if (static_cast<u64>(this->data[i]) < static_cast<u64>(b.data[i]) + carry){
            result.data[i] = b.data[i] + carry - this->data[i];
            carry = 1;
        }
        else{
            result.data[i] = this->data[i] - b.data[i] - carry;
            carry = 0;
        }
    }

    return result;
}

uintn uintn::operator*(uintn b){
    // Compute this*b=c
    uintn c(this->size+b.size+1);
    u64 tmp;
    uintn tmp_n;
    u64 a_i,b_i;
    for (a_i = 0; a_i<this->size; a_i++){
        for (b_i = 0; b_i<b.size; b_i++){
            tmp = static_cast<u64>(this->data[a_i]) * static_cast<u64>(b.data[b_i]);
            tmp_n.size = a_i+b_i+2;
            tmp_n.reset();
            tmp_n.data[a_i+b_i] = tmp & 0xFFFFFFFF;
            tmp_n.data[a_i+b_i+1] = tmp >> 32;

            c = c + tmp_n;
        }
    }

    while (c.data[c.size-1] == 0){
        c.data.pop_back();
        c.size-=1;
    }

    return c;
}

uintn uintn::modexp(uintn e, uintn m){
    // Compute this^e mod m
    uintn incr;
    incr.set("1");
    
    uintn ep;
    ep.set("0");
    
    uintn test;
    test.set("2");

    uintn res;
    res.size = this->size;
    res.data = this->data;

    for (ep.set("0"); e>(ep+incr); ep = ep+incr){
        res = res * incr;//(*this);
        //res = res % m;
        std::cout << "Res : " << res.size << " status : "<<ep<<"/"<<e << std::endl;
    }
    return res;
}
