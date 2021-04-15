#include "ksstack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************** 基础函数 *********************/
void ksstack_free(void* p)
{
    free(p);
}

void* ksstack_alloc(unsigned long size)
{
    void* p = malloc(size);
    return p;
}

/************************** 静态内存的栈 *****************/
/** 
 * 静态栈初始化
 * 参数:静态栈结构体(内容无需初始化), 用户静态分配的内存,内存size,栈中元素大小,栈类型
*/
e_ksstack_errno ksstack_static_init(s_ksstack_static* st, void* mem, unsigned long int mem_size, \
    unsigned long int ele_size, e_ksstack_fi_type type)
{   
    if (st == NULL || mem == NULL || mem_size < ele_size)
        return KSSTACK_ERRNO_PARAM;

    st->mem = mem;
    st->mem_size = mem_size;
    st->attr.ele_size = ele_size;
    st->attr.type = type;
    st->attr.stack_len = 0;

    st->stack_top = 0;
    st->stack_button = 0;
    st->attr.deep_size = st->mem_size / st->attr.ele_size;

    return KSSTACK_ERRNO_OK;
}

/**
 *  弹一弹,弹走鱼尾纹
*/
e_ksstack_errno ksstack_static_pop(s_ksstack_static* st, void* ele)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    if (st->attr.stack_len == 0)      // 空栈
        return KSSTACK_ERRNO_SIZE_MORE;

    if(st->attr.type == KSSTACK_TYPE_FIFO)        // 先进先出,把栈底弹出去
    {
        memcpy(ele, (void* )((unsigned long long int)st->mem + (unsigned long long int)(st->stack_button * st->attr.ele_size)), st->attr.ele_size);
        if (st->stack_button == st->attr.deep_size - 1)
            st->stack_button = 0;
        else
            st->stack_button++; 
            
        st->attr.stack_len--;
    }
    else if(st->attr.type == KSSTACK_TYPE_FILO)   // 先进后出,把栈顶弹出去 
    {
        memcpy(ele, (void* )((unsigned long long int)st->mem + (unsigned long long int)(st->stack_top * st->attr.ele_size)), st->attr.ele_size);
        if (st->stack_top == 0)
            st->stack_top = st->attr.deep_size - 1;
        else
            st->stack_top--; 

        st->attr.stack_len--;
    }

    return KSSTACK_ERRNO_OK;
}

/**
 * 快到碗里来
*/
e_ksstack_errno ksstack_static_push(s_ksstack_static* st, void* ele)
{

    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    if (st->attr.stack_len == st->attr.deep_size)       // 满栈
        return KSSTACK_ERRNO_SIZE_MORE;

    if (st->stack_top == st->attr.deep_size - 1)
    {
        st->stack_top = 0;
    }
    else
        st->stack_top++; 
    
    memcpy((void* )((unsigned long long int)st->mem + (unsigned long long int)(st->stack_top * st->attr.ele_size)), ele, st->attr.ele_size);
    st->attr.stack_len++;

    return KSSTACK_ERRNO_OK;
}

e_ksstack_errno ksstack_static_clean(s_ksstack_static* st)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    st->stack_top = 0;
    st->stack_button = 0;
    st->attr.stack_len = 0;

    return KSSTACK_ERRNO_OK;
}

unsigned long int ksstack_static_get_size(s_ksstack_static* st)
{
    if (st == NULL)
        return 0;

    return st->attr.stack_len;
}

/******************************** 动态内存的栈 ***************************************/
e_ksstack_errno ksstack_dynamic_init(s_ksstack_dynamic* st, unsigned long int deep_size, \
    unsigned long int ele_size, e_ksstack_fi_type type)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    st->attr.deep_size = deep_size;
    st->attr.ele_size = ele_size;
    st->attr.type = type;
    st->attr.stack_len = 0;

    st->stack_top = NULL;
    st->stack_button = NULL;

    return KSSTACK_ERRNO_OK;
}

e_ksstack_errno ksstack_dynamic_pop(s_ksstack_dynamic* st, void* ele)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    if (st->attr.stack_len == 0)      // 空栈
        return KSSTACK_ERRNO_SIZE_LESS;

    if(st->attr.type == KSSTACK_TYPE_FIFO)        // 先进先出,把栈底弹出去
    {
        memcpy(ele, (void* )((unsigned long long int)st->stack_button + sizeof(s_ksstack_dynamic_list)), st->attr.ele_size);
        if (st->attr.stack_len == 1)                     // 栈顶也没了
        {
            KSSTACK_FREE(st->stack_top);
            st->stack_top = NULL;
            st->stack_button = NULL;
        }
        else
        {
            s_ksstack_dynamic_list* buff = st->stack_button->top;
            KSSTACK_FREE(st->stack_button);
            st->stack_button = buff;
            st->stack_button->button = NULL;
        }
        st->attr.stack_len--;
    }
    else if(st->attr.type == KSSTACK_TYPE_FILO)   // 先进后出,把栈顶弹出去 
    {
        memcpy(ele, (void* )((unsigned long long int)st->stack_top + sizeof(s_ksstack_dynamic_list)), st->attr.ele_size);
        if (st->attr.stack_len == 1)                     // 栈也没了
        {
            KSSTACK_FREE(st->stack_button);
            st->stack_button = NULL;
            st->stack_top = NULL;
        }
        else
        {
            s_ksstack_dynamic_list* buff = st->stack_top->button;
            KSSTACK_FREE(st->stack_top);
            st->stack_top = buff;
            st->stack_top->top = NULL;
        }
        st->attr.stack_len--;
    }

    return KSSTACK_ERRNO_OK;
}

e_ksstack_errno ksstack_dynamic_push(s_ksstack_dynamic* st, void* ele)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    if (st->attr.stack_len == st->attr.deep_size)       // 满栈
        return KSSTACK_ERRNO_SIZE_MORE;

    s_ksstack_dynamic_list* new = KSSTACK_ALLOC(sizeof(s_ksstack_dynamic_list) + st->attr.ele_size);
    if (new == NULL)
        return KSSTACK_ERRNO_NOMEM;

    new->top = NULL;
    new->button = st->stack_top;
    memcpy((void* )((unsigned long long int)new + (unsigned long long int)(sizeof(s_ksstack_dynamic_list))), ele, st->attr.ele_size);
    if (st->stack_top != NULL)
         st->stack_top->top = new;
    else                                    // 空栈
        st->stack_button = new;
    st->stack_top = new;
    st->attr.stack_len++;

    return KSSTACK_ERRNO_OK;
}

e_ksstack_errno ksstack_dynamic_clean(s_ksstack_dynamic* st)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    if (st->attr.stack_len == 0)
        return KSSTACK_ERRNO_OK;

    while (st->stack_top != NULL)
    {
        s_ksstack_dynamic_list* buff = st->stack_top->button;
        KSSTACK_FREE(st->stack_top);
        st->stack_top = buff;
    }
    st->attr.stack_len = 0;
    return KSSTACK_ERRNO_OK;
}

unsigned long int ksstack_dynamic_get_size(s_ksstack_dynamic* st)
{
    if (st == NULL)
        return 0;

    return st->attr.stack_len;
}

/********************************* 通用糅合 ****************************************/
e_ksstack_errno ksstack_init(s_ksstack* st, e_ksstack_mem_type mem_type, \
    void* mem, unsigned long int mem_size, \
    unsigned long int deep_size, \
    unsigned long int ele_size, e_ksstack_fi_type type)
{
    if (st == NULL)
        return KSSTACK_ERRNO_PARAM;

    st->type = mem_type;
    
    e_ksstack_errno ksstack_errno = KSSTACK_ERRNO_ERRO;
    switch (mem_type)
    {
    case KSSTACK_MEM_TYPE_STATIC:
        ksstack_errno = ksstack_static_init(&(st->u_ksstack.st_static), mem, mem_size, ele_size, type);
        st->pop = (f_ksstack_pop)ksstack_static_pop;
        st->push = (f_ksstack_push)ksstack_static_push;
        st->clean = (f_ksstack_clean)ksstack_static_clean;
        st->get_size = (f_ksstack_get_size)ksstack_static_get_size;
        break;
    
    case KSSTACK_MEM_TYPE_DYNAMIC:
        ksstack_errno = ksstack_dynamic_init(&(st->u_ksstack.st_dynamic), deep_size, ele_size, type);
        st->pop = (f_ksstack_pop)ksstack_dynamic_pop;
        st->push = (f_ksstack_push)ksstack_dynamic_push;
        st->clean = (f_ksstack_clean)ksstack_dynamic_clean;
        st->get_size = (f_ksstack_get_size)ksstack_dynamic_get_size;
        break;
    
    default:
        break;
    }


    return ksstack_errno;
}

