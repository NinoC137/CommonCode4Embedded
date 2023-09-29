#include "reformat_bystack.h"

void bufferReformat(char *buffer, int *result)
{
    stack_t *s = stack_create(sizeof(char));
    ReformatIndex_t index = {0, 0};

    for (int i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == startSymbol)
        {
            i += 1;
            index.index++;
            while (buffer[i] != endSymbol)
            {
                if (i > strlen(buffer) + 1)
                {
                    fprintf(stderr, "Error! your buffer without an end flag!\r\n");
                    break;
                }
                // buffer[i] = buffer[i] - '0';
                stack_push(s, &buffer[i]);
                index.length[index.index] += 1;
                i++;
            }
        }
    }

    int temp, multipiler = 1;
    while(index.index != 0){

        /***************运算结果******************/
        while(index.length[index.index] != 1){
            stack_pop(s, &temp);
            result[index.index - 1] += (temp - '0') * multipiler;
            index.length[index.index]--;
            multipiler *= 10;
        }
        /****************************************/

        /***************符号判断******************/
        stack_pop(s, &temp);
        if(temp == '-'){
            result[index.index - 1] *= -1;
        }else{
            result[index.index - 1] *= 1;
        }
        index.length[index.index]--;
        /****************************************/

        index.index--;
        multipiler = 1;
    }

    stack_free(s);
}

int main(void)
{
    static char *testBuffer = "a:-1109,b:+119,";

    bufferReformat(testBuffer, result);

    printf("result: %d, %d.\r\n", result[0], result[1]);

    return 0;
}