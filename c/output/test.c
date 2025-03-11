#include "../leetcode.h"
int findMaxConsecutiveOnes(int* nums, int numsSize)
{
    int consecutive_ones = 0, max_consecutive = 0, i = 0;
    
    while(i < numsSize)
    {
        if (*(nums + i) == 1)
        {
            while (*(nums + i) == 1 && i < numsSize) 
            {
                ++consecutive_ones;
                ++i;
            }
            if (consecutive_ones > max_consecutive) max_consecutive = consecutive_ones;
            consecutive_ones = 0;
        }
        else
        {
            ++i;
        }
    }
    return max_consecutive;
}

int main(void)
{
    // Same input as LeetCode
    int *ones_arr = calloc(6, sizeof(int));

    ones_arr[0] = 1;
    ones_arr[1] = 1;
    ones_arr[3] = 1;
    ones_arr[4] = 1;
    ones_arr[5] = 1;

    int count = findMaxConsecutiveOnes(ones_arr, 6);
    free(ones_arr);

    printf("%d\n", count);

    return 0;
}
