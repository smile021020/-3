#include <stdio.h>
#include <stdlib.h>
int main()
{
	int input=0,i=0;

	scanf("%d",&input);
	for(i=input;i>=input-2;i--)//找出最近的3的倍數
	{
    	if(i%3)
            continue;
    	else
            break;
	}
	printf("%d\n",i/3*(3+i)/2);
	//得到總和(等差級數和公式)
	return 0;
}
