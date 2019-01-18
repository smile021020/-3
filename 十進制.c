#include <stdio.h>
int main()
{
    int i;
	int input=0,array[8];

	scanf("%d",&input);
	if(input<0)
        input+=256;                 //負數shift到正數

	for(i=7;i>=0;i--)               //因為最後那項是我們二進位的第一個
    {
    	array[i]=input%2;             //慢慢存入0或1
    	input/=2;                   //需要他的商繼續往下算
    }
    for(i=0;i<8;i++)
    {
    	if(i==7)
            printf("%d\n",array[i]);      //若為最後一位則換行
    	else
            printf("%d",array[i]);	//否則緊接在後輸出
    }
	return 0;
}
