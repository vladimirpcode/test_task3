#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using ip = uint32_t;

class NatAllocator{
public:
    NatAllocator(const size_t number_of_public_ip, const size_t max_abonents_per_address);
    bool addr_alloc(ip& public_ip_address, const ip abonent_ip);
    bool addr_free(const ip abonent_ip);
    size_t max_abonents() const;
    ~NatAllocator();
private:
    std::unordered_set<ip> public_ip_addresses_;
    std::unordered_map<ip, ip> abonent_with_public_;
    const size_t max_abonents_number_;
    size_t abonents_count_;
    std::list<ip> loading_queues_[3];
    std::unordered_map<ip, size_t> public_with_queue_index;
    std::unordered_map<ip, std::list<ip>::iterator> public_with_iter_in_queue;
};