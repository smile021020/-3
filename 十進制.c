#include <stdio.h>
int main()
{
    int i;
	int input=0,Bin[8];

	scanf("%d",&input);
	if(input<0)
        input+=256;                 //�t��shift�쥿��

	for(i=7;i>=0;i--)               //�]���̫ᨺ���O�ڭ̤G�i�쪺�Ĥ@��
    {
    	Bin[i]=input%2;             //�C�C�s�J0��1
    	input/=2;                   //�ݭn�L�����~�򩹤U��
    }
    for(i=0;i<8;i++)
    {
    	if(i==7)
            printf("%d\n",Bin[i]);
            //cout<<Bin[i]<<endl;     //�Y���̫�@��h����
    	else
            printf("%d",Bin[i]);
            //cout<<Bin[i];//�_�h�򱵦b���X
    }
	return 0;
}
