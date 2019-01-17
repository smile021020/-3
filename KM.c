#include <stdio.h>
#include <stdlib.h>
int main()
{
    float a;
    float x;
    while(scanf("%f",&a)==1)
    {
        x=a*1.6;
            printf("%.1f\n",x);
    }
    return 0;
}
