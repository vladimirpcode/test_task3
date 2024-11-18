#include "NatAllocator.h"

#include <stdexcept>


NatAllocator::NatAllocator(const size_t number_of_public_ip, const size_t max_abonents_per_address)
    :   max_abonents_number_(max_abonents_per_address * number_of_public_ip),
        abonents_count_(0)
{
    if (max_abonents_per_address == 0){
        throw std::runtime_error("NatAllocator::NatAllocator()  максимальное количество абонентов не может быть равно нулю");
    }
    for (int i = 0; i < number_of_public_ip; ++i){
        ip new_public_ip = i + 1;
        public_ip_addresses_.insert(new_public_ip);
        loading_queues_[0].insert(loading_queues_->end(), new_public_ip);
        public_with_queue_index[new_public_ip] = 0;
        public_with_iter_in_queue[new_public_ip] = loading_queues_[0].begin();
    }
}

bool NatAllocator::addr_alloc(uint32_t& public_ip_address, const uint32_t abonent_ip){
    if (public_ip_addresses_.find(abonent_ip) != public_ip_addresses_.end()){
        return false;
    }
    if (max_abonents_number_ == abonents_count_){
        return false;
    }
    size_t queue_index = 0;
    for (; queue_index < 3; ++queue_index){
        if (loading_queues_[queue_index].size() != 0){
            break;
        }
    }
    ip public_ip_for_bind = loading_queues_[queue_index].front();
    loading_queues_[queue_index].pop_front();
    size_t new_queue_index = queue_index + 1;
    loading_queues_[new_queue_index].push_back(public_ip_for_bind);
    public_with_queue_index[public_ip_for_bind] = new_queue_index;
    public_with_iter_in_queue[public_ip_for_bind] = --loading_queues_[new_queue_index].end();
    abonents_count_++;
    public_ip_address = public_ip_for_bind;
    return true;
}

bool NatAllocator::addr_free(const uint32_t abonent_ip){
    if (abonent_with_public_.find(abonent_ip) == abonent_with_public_.end()){
        return false;
    }
    ip public_ip = abonent_with_public_[abonent_ip];
    abonent_with_public_.erase(abonent_ip);
    size_t queue_index = public_with_queue_index[public_ip];
    auto iter = public_with_iter_in_queue[public_ip];
    loading_queues_[queue_index].erase(iter);
    size_t new_queue_index = queue_index - 1;
    public_with_queue_index[public_ip] = new_queue_index;
    loading_queues_[new_queue_index].push_back(public_ip);
    public_with_iter_in_queue[public_ip] = --loading_queues_[new_queue_index].end();
    abonents_count_--;
    return true;
}

size_t NatAllocator::max_abonents() const{
    return max_abonents_number_;
}
 
NatAllocator::~NatAllocator(){

}
