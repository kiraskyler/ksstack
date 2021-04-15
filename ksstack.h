/**
 *  Kiraskyler@qq.com
 *  
 * 
 * */
 
#ifndef __KSSTACK_H_
#define __KSSTACK_H_


#define     KSSTACK_ALLOC(size)                 (ksstack_alloc(size))
#define     KSSTACK_FREE(point)                 (ksstack_free(point))

typedef enum
{
    KSSTACK_TYPE_FIFO,
    KSSTACK_TYPE_FILO,
}e_ksstack_fi_type;

typedef enum
{
    KSSTACK_MEM_TYPE_STATIC,
    KSSTACK_MEM_TYPE_DYNAMIC,
}e_ksstack_mem_type;

typedef enum
{
    KSSTACK_ERRNO_OK,               // 木有错
    KSSTACK_ERRNO_PARAM,            // 传参错误
    KSSTACK_ERRNO_NOMEM,            // 木有内存
    KSSTACK_ERRNO_SIZE_MORE,        // 栈深度过深
    KSSTACK_ERRNO_SIZE_LESS,        // 栈深度不足
    KSSTACK_ERRNO_ERRO,             // 我不管,反正就是有错
}e_ksstack_errno;

typedef struct s_ksstack_attr
{
    unsigned long int           ele_size;            // 栈中每一元素的大小
    e_ksstack_fi_type           type;
    unsigned long int           deep_size;           // 栈深度
    unsigned long int           stack_len;           // 栈长度
}s_ksstack_attr;

typedef struct s_ksstack_static
{
    void*                       mem;                // 用户给栈分配的内存
    unsigned long int           mem_size;           // 用户给栈分配的内存size
    s_ksstack_attr              attr;               // 通用属性

    unsigned long int           stack_top;          // 按数组的方式,环形遍历,优点内存效率最高(相比链表),缺点麻烦(我来!)
    unsigned long int           stack_button;       
}s_ksstack_static;

typedef struct s_ksstack_dynamic_list               // 动态内存时,双向链表
{
    struct s_ksstack_dynamic_list*  top;
    struct s_ksstack_dynamic_list*  button;
}s_ksstack_dynamic_list;

typedef struct s_ksstack_dynamic
{
    s_ksstack_attr              attr;           // 通用属性

    s_ksstack_dynamic_list*     stack_top;           // 按数组的方式,环形遍历,优点内存效率最高(相比链表),缺点麻烦(我来!)
    s_ksstack_dynamic_list*     stack_button;       
}s_ksstack_dynamic;

typedef e_ksstack_errno     (*f_ksstack_pop)(void* st, void* ele);
typedef e_ksstack_errno     (*f_ksstack_push)(void* st, void* ele);
typedef e_ksstack_errno     (*f_ksstack_clean)(void* st);
typedef unsigned long int   (*f_ksstack_get_size)(void* st);

typedef struct s_ksstack
{
    e_ksstack_mem_type type;                         // 内存管理类型

    union u_ksstack
    {
        s_ksstack_static st_static;
        s_ksstack_dynamic st_dynamic;
    }u_ksstack;

    f_ksstack_pop       pop;
    f_ksstack_push      push;
    f_ksstack_clean     clean;
    f_ksstack_get_size  get_size;

}s_ksstack;

/******************************* 静态内存栈 **********************************/
extern e_ksstack_errno      ksstack_static_init(s_ksstack_static* st, void* mem, unsigned long int mem_size, \
    unsigned long int ele_size, e_ksstack_fi_type type);
extern e_ksstack_errno      ksstack_static_pop(s_ksstack_static* st, void* ele);
extern e_ksstack_errno      ksstack_static_push(s_ksstack_static* st, void* ele);
extern e_ksstack_errno      ksstack_static_clean(s_ksstack_static* st);
extern unsigned long int    ksstack_static_get_size(s_ksstack_static* st);

/******************************** 动态内存栈 ****************************************/
extern e_ksstack_errno ksstack_dynamic_init(s_ksstack_dynamic* st, unsigned long int deep_size, \
    unsigned long int ele_size, e_ksstack_fi_type type);
extern e_ksstack_errno      ksstack_dynamic_pop(s_ksstack_dynamic* st, void* ele);
extern e_ksstack_errno      ksstack_dynamic_push(s_ksstack_dynamic* st, void* ele);
extern e_ksstack_errno      ksstack_dynamic_clean(s_ksstack_dynamic* st);
extern unsigned long int    ksstack_dynamic_get_size(s_ksstack_dynamic* st);

/******************************** 通用糅合 ****************************************/
extern e_ksstack_errno ksstack_init(s_ksstack* st, e_ksstack_mem_type mem_type, \
    void* mem, unsigned long int mem_size, \
    unsigned long int deep_size, \
    unsigned long int ele_size, e_ksstack_fi_type type);
#endif
