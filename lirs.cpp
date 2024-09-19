#include <list>
#include <unordered_map>

class cash
{
    enum state
    {
        LIR,
        resident_HIR,
        non_resident_HIR,
    }

    struct element {
        int name;
        enum state state_element;
        std::list<int>::iterator pos_lirs;
    };

    long unsigned int Lhirs;
    long unsigned int count_lirs;
    long unsigned int Llirs;
    std::list<int> lirs_stack;
    std::unordered_map<int, struct Hash_map_node> table;

/*
нужно ли реализовать и как???

struct dlinked_list_element {
    struct element element;
    struct dlinked_list_element *next;
    struct dlinked_list_element *previous;


struct stack {
    struct dlinked_list_element **upper_element;
    struct dlinked_list_element **down_element;
};

struct list {
    struct dlinked_list_element **upper_element;
    struct dlinked_list_element **down_element;
};

struct element_hash {
    int name;
    struct dlinked_list_element *stack;
    struct dlinked_list_element *list;
    struct element_hash *next;
};

struct hash_list {
    struct element_hash *top;
    float size;
};

struct hash {
    struct element_hash **array;
    int capacity;
    float size;
    struct hash_list list;
};*/

}
