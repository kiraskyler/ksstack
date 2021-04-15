#include "ksstack_static.h"

#include "stdlib.h"
#include "time.h"

#include "ksstack.h"
#include "config.h"

#define STACK_DEEP          10
#define STACK_TEST_0_LOOP   100
#define STACK_TEST_0_SIZE   (STACK_DEEP + 10)
#define STACK_TYPE          KSSTACK_TYPE_FIFO    

static void stack_test_0_judge(s_ksstack_static* st, int pop_index, int test_deep, int test_num[])
{
    int num0, num1 = 0;
    ksstack_static_pop(st, &num1);

    if (test_deep <= STACK_DEEP && st->type == KSSTACK_TYPE_FILO)
        num0 = test_num[test_deep - 1 - pop_index];
    else if (test_deep > STACK_DEEP && st->type == KSSTACK_TYPE_FILO)
        num0 = test_num[STACK_DEEP - 1 - pop_index];
    else if (st->type == KSSTACK_TYPE_FIFO)
        num0 = test_num[pop_index];
    
    if (num0 != num1)
        dbg_printf("Erro, %d, %d, %d, %d", pop_index, st->type, num0, num1);
}

/**
 * 测试一下
 * 
 * 随鸡填满一个栈,再读出来,试一哈
*/
static void stack_test_0(s_ksstack_static* st)
{
    int test_num[STACK_TEST_0_SIZE];

    srand(time(NULL));
    
    for (int i = 0; i < STACK_TEST_0_LOOP; i++)
    {
        int test_deep = rand() % (STACK_TEST_0_SIZE - 1) + 1;
        
        for (int j = 0; j < test_deep; j++)
        {
            test_num[j] = rand();
            ksstack_static_push(st, &test_num[j]); 
            // dbg_printf("%d, test_num[%d] = %d", i, j, test_num[j]); // 调试用
        }

        for (int pop_index = 0; pop_index < test_deep; pop_index++)
        {
            if (pop_index >= STACK_DEEP)
                break;

            stack_test_0_judge(st, pop_index, test_deep, test_num);
        }

        ksstack_static_clean(st);
    }
}

int main(int argc, char* argv[])
{
    s_ksstack_static st;
    int     stack_mem[STACK_DEEP];

    ksstack_static_init(&st, &stack_mem, sizeof(stack_mem), sizeof(int), STACK_TYPE);

    stack_test_0(&st);
    return 0;
}
