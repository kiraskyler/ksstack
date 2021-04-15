/**
 * 使用静态内存或动态内存
*/

#include "ksstack_user.h"

#include <time.h>
#include <stdlib.h>

#include "ksstack.h"
#include "config.h"

#define     STACK_MEM_TYPE          KSSTACK_MEM_TYPE_DYNAMIC
#define     STACK_FI_TYPE           KSSTACK_TYPE_FILO
#define     STACK_DEEP              100

#define     STACK_TEST_0_LOOP       1
#define     STACK_TEST_0_DEEP       105

typedef     int                     data;          

static void stack_test_0_judge(s_ksstack* st, data num1, int pop_index, int test_deep, int test_num[])
{
    data num0 = 0;

    if (test_deep <= STACK_DEEP && STACK_FI_TYPE == KSSTACK_TYPE_FILO)
        num0 = test_num[test_deep - 1 - pop_index];
    else if (test_deep > STACK_DEEP && STACK_FI_TYPE == KSSTACK_TYPE_FILO)
        num0 = test_num[STACK_DEEP - 1 - pop_index];
    else if (STACK_FI_TYPE == KSSTACK_TYPE_FIFO)
        num0 = test_num[pop_index];
    
    if (num0 != num1)
        dbg_printf("Erro, %d, %d, %d, %d", pop_index, STACK_FI_TYPE, num0, num1);
}

/**
 * 测试入栈再出栈
*/
static void stack_test_0(s_ksstack* st)
{
    data test_num[STACK_TEST_0_DEEP];

    srand(time(NULL));
    
    for (int i = 0; i < STACK_TEST_0_LOOP; i++)
    {
        data test_deep = rand() % (STACK_TEST_0_DEEP - 1) + 1;         // 去除0用
        
        for (int j = 0; j < test_deep; j++)
        {
            test_num[j] = rand();
            e_ksstack_errno errno_code;
            if ((errno_code = st->push((void* )&(st->u_ksstack), &test_num[j])) != KSSTACK_ERRNO_OK)
                dbg_printf("%d", errno_code);
            // dbg_printf("%d, test_num[%d] = %d", i, j, test_num[j]); // 调试用
        }

        for (int pop_index = 0; pop_index < test_deep; pop_index++)
        {
            if (pop_index >= STACK_DEEP)
                break;

            int num1;
            e_ksstack_errno errno_code;
            if ((errno_code = st->pop((void *)&(st->u_ksstack), &num1)) != KSSTACK_ERRNO_OK)
                dbg_printf("%d", errno_code);
            stack_test_0_judge(st, num1, pop_index, test_deep, test_num);
        }

        st->clean((void *)&(st->u_ksstack));
    }
}

int main (int argc, char* argv[])
{
    s_ksstack st;
    data stack_mem[STACK_DEEP];

    ksstack_init(&st, STACK_MEM_TYPE, &stack_mem, sizeof(stack_mem), STACK_DEEP, sizeof(data), STACK_FI_TYPE);
    stack_test_0(&st);

    return 0;
}
