#include "NatAllocator.h"
#include <iostream>
#include <chrono>

using namespace std::chrono;

void assert_(bool expr){
    if (!expr){
        std::cout << "ASSERT: " << "\n";
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

milliseconds test_case_addr_alloc(NatAllocator& nat_allocator){
    std::cout << "test_case_addr_alloc(): " << nat_allocator.max_abonents() << " abonents\n";
    auto start = high_resolution_clock::now();
    for (int i = 0; i < nat_allocator.max_abonents(); ++i){
        ip public_ip;
        ip abonent_ip =  nat_allocator.max_public_ip() + 1 + i;
        bool result = nat_allocator.addr_alloc(public_ip, abonent_ip);
        assert_(result);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return duration;
}

milliseconds test_case_addr_free(NatAllocator& nat_allocator){
    std::cout << "test_case_addr_free(): " << nat_allocator.max_abonents() << " abonents\n";
    auto start = high_resolution_clock::now();
    for (int i = 0; i < nat_allocator.max_abonents(); ++i){
        ip abonent_ip =  nat_allocator.max_public_ip() + 1 + i;
        bool result = nat_allocator.addr_free(abonent_ip);
        assert_(result);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return duration;
}

struct TestData{
    TestData(const size_t max_public_ip_addresses,  const size_t max_abonents_per_public_ip) 
        :   max_public_ip_addresses(max_public_ip_addresses),
            max_abonents_per_public_ip(max_abonents_per_public_ip)
    {

    }
    const size_t max_public_ip_addresses;
    const size_t max_abonents_per_public_ip;
};

struct TestResult{
    TestResult(size_t max_abonents, int64_t execution_time_ms, size_t abonents_per_ms)
        :   max_abonents(max_abonents), execution_time_ms(execution_time_ms), abonents_per_ms(abonents_per_ms)
    {

    }
    size_t max_abonents;
    int64_t execution_time_ms;
    size_t abonents_per_ms;
};

int main(){
    std::vector<TestData> test_data{
        {10, 10},
        {10, 100},
        {100, 1000},
        {1000, 10000},
        {2000, 100000},
        {3000, 100000},
        {4000, 100000},
        {5000, 100000},
        //{1000, 1000000}
    };
    std::vector<TestResult> addr_alloc_test_results;
    std::vector<TestResult> addr_free_test_results;
    for (const auto& entry : test_data){
        NatAllocator nat_allocator(entry.max_public_ip_addresses, entry.max_abonents_per_public_ip);
        auto duration = test_case_addr_alloc(nat_allocator);
        std::cout << "Nat allocator has " << nat_allocator.get_current_abonents_number() << " abonents right now\n";
        int64_t max_abonents = entry.max_abonents_per_public_ip * entry.max_public_ip_addresses;
        int64_t duration_ms_int = duration.count();
        duration_ms_int = (duration_ms_int > 0) ? duration_ms_int : (duration_ms_int + 1);
        addr_alloc_test_results.push_back({max_abonents, duration_ms_int, (max_abonents / duration_ms_int)});
        duration = test_case_addr_free(nat_allocator);
        std::cout << "Nat allocator has " << nat_allocator.get_current_abonents_number() << " abonents right now\n";
        duration_ms_int = duration.count();
        duration_ms_int = (duration_ms_int > 0) ? duration_ms_int : (duration_ms_int + 1);
        addr_free_test_results.push_back({max_abonents, duration_ms_int, (max_abonents / duration_ms_int)});
    }


    std::cout << "\naddr_alloc()\n";
    for (auto& entry : addr_alloc_test_results){
        std::cout << "max abonents: " << entry.max_abonents << " | duration: " <<  entry.execution_time_ms << "ms | addresses per ms: " <<  entry.abonents_per_ms << "\n";
    }
    std::cout << "\naddr_free()\n";
    for (auto& entry : addr_free_test_results){
        std::cout << "max abonents: " << entry.max_abonents << " | duration: " <<  entry.execution_time_ms << "ms | addresses per ms: " <<  entry.abonents_per_ms << "\n";
    }
}