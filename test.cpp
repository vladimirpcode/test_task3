#include "NatAllocator.h"
#include <iostream>

void assert_(bool expr, const std::string& msg){
    if (!expr){
        std::cout << "ASSERT: " << msg << "\n";
        exit(-1);
    }
}

std::string to_string(ip ip_address){
    uint8_t octets[4];
    octets[0] = ip_address >> 24;
    octets[1] = (ip_address << 8) >> 24;
    octets[2] = (ip_address << 16) >> 24;
    octets[3] = (ip_address << 24) >> 24;
    return std::to_string(octets[0]) + "." +
        std::to_string(octets[1]) + "." + 
        std::to_string(octets[2]) + "." + 
        std::to_string(octets[3]);
}

int main(){
    NatAllocator nat_allocator(1000, 100);
    std::unordered_set<ip> public_ip_addresses;
    for (int i = 0; nat_allocator.max_abonents(); ++i){
        ip public_ip;
        ip abonent_ip =  1000 + 1 + i;
        std::cout << (i + 1) << ") add abonent: " << to_string(abonent_ip) << "\n";
        assert_(nat_allocator.addr_alloc(public_ip, abonent_ip), "test 1");
        public_ip_addresses.insert(public_ip);
    }
    std::cout << "alocated public ip:\n";
    for (auto& addr : public_ip_addresses){
        std::cout << to_string(addr) << "\n";
    }

}