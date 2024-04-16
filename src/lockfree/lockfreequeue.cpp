#include <iostream>
#include <atomic>
#include <thread>
#define MAX_ENQUEUE 10000000

struct data_node {
    int index;
    data_node(int const& indx) : index(indx)
    {}
};

class lock_free_queue {
    
    public :
                std::atomic<int> num_elements;
    private:
    struct node
        {
            data_node* data;
            std::atomic<node*> next;
            node(data_node* data_):
                data(data_), next(nullptr)
            {}
        };
        std::atomic<node*> head;
        std::atomic<node*> tail;

    public:
        lock_free_queue() {
            this->head = nullptr;
            this->tail = nullptr;
            this->num_elements = 0;
            this->head = new node(new data_node(INT32_MAX));
            this->tail = this->head.load();
            // std::cout << " Created lock-free queue" << std::endl;
            // std::cout << "HEAD : " << head << std::endl;
            // std::cout << "TAIL : " << tail << std::endl;
        }
        void push( data_node* dta) {
            std::atomic<node*> const new_node = new node(dta);
            node* old_tail = tail.load();
            while(!tail.compare_exchange_weak(old_tail, new_node));
            num_elements++;
            old_tail->next = new_node.load();
        }

        data_node* pop(){
            node* old_head = head.load();
            /*
             * lock cmpxchg operation decides which thread gets to pop
             * the head, and return the element. 
             * 
             */
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next));
            num_elements--;
            return old_head ? old_head->data : nullptr;
        }

        void walk_queue() {
            // std::cout << num_elements << std::endl;
            node* node_ptr = head.load();
            while(node_ptr) {

                std::cout << node_ptr->data->index << " " ;
                node_ptr = node_ptr->next;
            }
            std::cout << std::endl;
        }        
};

lock_free_queue lq ;



void* worker_push(void) {

    data_node *iptr = nullptr;
    for (int i = 0; i<MAX_ENQUEUE; i++){
        iptr = new data_node(i);
        // std::cout << iptr << " " << std::endl;
        lq.push(iptr);
    }
}

std::atomic<int> pop_count = 0;
void* worker_pop() {
    data_node *iptr = nullptr;
    for (int i = 0; i<MAX_ENQUEUE; i++){
        iptr = lq.pop();
        delete iptr;
        pop_count++;
    }
} 

int main () {
    


    std::thread t1(worker_push);
    std::thread t2(worker_push);
    t1.join();
    t2.join();
    std::cout<< "Num Elements after concurrent push :" << lq.num_elements << std::endl;
    // lq.walk_queue();

    std::thread pop_thread1(worker_pop);
    std::thread pop_thread2(worker_pop);
    pop_thread1.join();
    pop_thread2.join();
    std::cout << "Num Elements aftewr concurrent pop :" << lq.num_elements << std::endl;
    // lq.walk_queue();

    std::cout << "Elements after concurrent pop :";
    lq.walk_queue();
}