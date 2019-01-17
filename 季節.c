#include <stdio.h>

int main()
{
	int input=0;

	scanf("%d",&input);
	//(input/3)用商數分季節
	{
	    if(input/3==1)
            printf("Spring\n");
	    else if(input/3==2)
            printf("Summer\n");
	    else if(input/3==3)
            printf("Autumn\n");
        else
            printf("Winter\n");
	}
	return 0;
}
