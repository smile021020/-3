#include <stdio.h>
#include <stdlib.h>
int main()
{
	int input=0,i=0;

	scanf("%d",&input);
	for(i=input;i>=input-2;i--)//��X�̪�3������
	{
    	if(i%3)
            continue;
    	else
            break;
	}
	printf("%d\n",i/3*(3+i)/2);
	//cout<<i/3*(3+i)/2<<endl;//�o���`�M(���t�żƩM����)
	system("pause");
	return 0;
}
