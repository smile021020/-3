#include <stdio.h>

int main()
{
	int start1=0,start2=0,end1=0,end2=0;
	scanf("%d%d",&start1,&start2);
	//讀入開始時間
	scanf("%d%d",&end1,&end2);
	//讀入結束時間
	int time=(end1*60+end2)-(start1*60+start2);//計算總時數
	if(time<=120&&time>=0)
        //2小時內的費用
        printf("%d\n",time/30*30);
	else if(time>120&&time<=240)
        //4小時內的費用+2小時的費用
        printf("%d\n",(time-120)/30*40+120);
	else
        //4小時外的費用+4小時的費用+2小時的費用
        printf("%d\n",(time-240)/30*60+120+160);
	return 0;
}
