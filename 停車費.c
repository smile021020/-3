#include <stdio.h>

int main()
{
	int start1=0,start2=0,end1=0,end2=0;
	scanf("%d%d",&start1,&start2);
	//Ū�J�}�l�ɶ�
	scanf("%d%d",&end1,&end2);
	//Ū�J�����ɶ�
	int time=(end1*60+end2)-(start1*60+start2);//�p���`�ɼ�
	if(time<=120&&time>=0)
        //2�p�ɤ����O��
        printf("%d\n",time/30*30);
	else if(time>120&&time<=240)
        //4�p�ɤ����O��+2�p�ɪ��O��
        printf("%d\n",(time-120)/30*40+120);
	else
        //4�p�ɥ~���O��+4�p�ɪ��O��+2�p�ɪ��O��
        printf("%d\n",(time-240)/30*60+120+160);
	return 0;
}
