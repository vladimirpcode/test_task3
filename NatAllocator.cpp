#include "NatAllocator.h"

#include <stdexcept>


NatAllocator::NatAllocator(const size_t number_of_public_ip, const size_t max_abonents_per_address)
    :   max_abonents_number_(max_abonents_per_address * number_of_public_ip),
        abonents_count_(0),
        least_loaded_queue_index_(1),
        most_loaded_queue_index_(2)
{
    if (max_abonents_per_address == 0){
        throw std::runtime_error("NatAllocator::NatAllocator()  максимальное количество абонентов не может быть равно нулю");
    }
    for (int i = 0; i < 3; ++i){
        loading_queues_.push_back(std::list<ip>());
    }
    for (int i = 0; i < number_of_public_ip; ++i){
        ip new_public_ip = i + 1;
        public_ip_addresses_.insert(new_public_ip);
        loading_queues_.at(0).push_front(new_public_ip);
        public_with_queue_index[new_public_ip] = 0;
        public_with_iter_in_queue[new_public_ip] = loading_queues_.at(0).begin();
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
    if (loading_queues_.at(0).size() == 0){
        queue_index = least_loaded_queue_index_;
    }
    ip public_ip_for_bind = loading_queues_.at(queue_index).front();
    loading_queues_.at(queue_index).pop_front();
    size_t new_queue_index = most_loaded_queue_index_;
    loading_queues_.at(new_queue_index).push_back(public_ip_for_bind);
    public_with_queue_index[public_ip_for_bind] = new_queue_index;
    public_with_iter_in_queue[public_ip_for_bind] = --loading_queues_.at(new_queue_index).end();
    abonents_count_++;
    abonent_with_public_[abonent_ip] = public_ip_for_bind;
    public_ip_address = public_ip_for_bind;
    if (loading_queues_[most_loaded_queue_index_].size() == public_ip_addresses_.size()){
        std::swap(least_loaded_queue_index_, most_loaded_queue_index_);
    }
    return true;
}

bool NatAllocator::addr_free(const uint32_t abonent_ip){
    if (abonent_with_public_.find(abonent_ip) == abonent_with_public_.end()){
        return false;
    }
    ip public_ip = abonent_with_public_.at(abonent_ip);
    abonent_with_public_.erase(abonent_ip);
    size_t queue_index = public_with_queue_index.at(public_ip);
    auto iter = public_with_iter_in_queue.at(public_ip);
    loading_queues_.at(queue_index).erase(iter);
    size_t new_queue_index = least_loaded_queue_index_;
    if (queue_index == least_loaded_queue_index_){
        new_queue_index = most_loaded_queue_index_;
    }
    public_with_queue_index[public_ip] = new_queue_index;
    loading_queues_.at(new_queue_index).push_back(public_ip);
    public_with_iter_in_queue[public_ip] = --loading_queues_.at(new_queue_index).end();
    abonents_count_--;
    return true;
}

size_t NatAllocator::max_abonents() const{
    return max_abonents_number_;
}


size_t NatAllocator::max_public_ip() const{
    return public_ip_addresses_.size();
}

size_t NatAllocator::get_current_abonents_number() const{
    return abonents_count_;
}
 
NatAllocator::~NatAllocator(){

}
