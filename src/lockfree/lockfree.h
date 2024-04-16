#include <atomic>

struct node {
    int data;
    std::atomic<node*> next;
    node(int const& dta): data(dta), next(nullptr)
    { }    
};

class lock_free_queue {

    private:
        std::atomic<node*> head;
        std::atomic<node*> tail;
    public:
        
        void push(int const& dta) {
            std::atomic<node*> const new_node = new node(dta);
            node* old_tail = tail.load();
            while(!old_tail->next.compare_exchange_weak(nullptr, new_node)){
                node* old_tail = tail.load();
            }
        }
        
};

