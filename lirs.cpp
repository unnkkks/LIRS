#include <stdio.h>
#include <list>
#include <stack>
#include <unordered_map>
#include <vector>

void visit_LIR(struct elem)
{
    element front_elem;
    front_elem = lirs_stack.back();
    while(front_elem->element_state = lir)
    {
        elem_hit_the_top(elem);
        front_elem = lirs_stack.back();
    }
    while(front_elem->element_state = resident_hir)
    {
        remove_data_blocks(elem);
        front_elem = lirs_stack.back();
    }
}

void elem_hit_the_top(struct elem)
{
    lirs_stack.remove(elem);
    lirs_stack.push_front(elem);
    //return???
}
void remove_data_blocks(struct elem)
{
    front_elem = lirs_stack.back();
    lirs_stack.pop_back();
    front_elem->element_state = out;
    //return??
}
