#include <stdio.h>

int main()
{
		int in;
		float nSu=0,Su=0;
		scanf("%d",&in);
		if(in>700)
            {
                Su+=(in-700)*5.63;
                nSu+=(in-700)*4.5;
                in=700;
            }
        if(in>500)
            {
                Su+=(in-500)*4.97;
                nSu+=(in-500)*4.01;
                in=500;
            }
		if(in>330)
            {
                Su+=(in-330)*4.39;
                nSu+=(in-330)*3.61;
                in=330;
            }
		if(in>120)
            {
                Su+=(in-120)*3.02;
                nSu+=(in-120)*2.68;
                in=120;
            }
		if(in>0)
            {
                Su+=in*2.1;
                nSu+=in*2.1;
            }
		printf("Summer months:%.2f\n",Su);
		printf("Non-Summer months:%.2f\n",nSu);
	return 0;
}
