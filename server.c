#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>

#define MAXLINE 512
#define MAXMEM 10
#define NAMELEN 20
#define SERV_PORT 8080
#define LISTENQ 5

int listenfd,connfd[MAXMEM];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char user[MAXMEM][NAMELEN];
void Quit();
void rcv_snd(int n);

typedef enum TicTacToeState{

	TTTS_Default = 0,
	TTTS_O = 1,
	TTTS_X = 2,
}TTTS;

TTTS array[3][3] = {TTTS_Default};

void Print(TTTS array[][3])
{
	printf("\n");
	for(int i = 0;i < 3;i++)
	{

		for(int j = 0;j < 3;j++)
		{

			if(array[i][j] == TTTS_Default)
				printf("-");		
			else if(array[i][j] == TTTS_O)
				printf("O");
			else
				printf("X");
		}
		printf("\n");
	}
	printf("\n");
}

TTTS CheckWin(TTTS array[][3])
{

	for(int i = 0;i < 3;i++)
		if(array[i][0] == array[i][1] && 
				array[i][0] == array[i][2] &&
				array[i][0] != TTTS_Default)
			return array[i][0];
	for(int j = 0;j < 3;j++)
		if(array[0][j] == array[1][j] &&
				array[0][j]== array[2][j] && 
				array[0][j] != TTTS_Default)
			return array[0][j];
	if(array[0][0] == array[1][1] &&
			array[0][0] == array[2][2] &&
			array[0][0] != TTTS_Default)
		return array[0][0];
	if(array[0][2] == array[1][1] &&
			array[0][2] == array[2][0] &&
			array[0][2] != TTTS_Default)
		return array[0][2];

	return TTTS_Default;
}

int main()
{
	pthread_t thread;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t length;
	char buff[MAXLINE];

	//用socket建server的fd
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	if(listenfd < 0) {
		printf("Socket created failed.\n");
		return -1;
	}
	//網路連線設定
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);	//port80
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//用bind開監聽器
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Bind failed.\n");
		return -1;
	}
	//用listen開始監聽
	printf("listening...\n");
	listen(listenfd, LISTENQ);

	//建立thread管理server
	pthread_create(&thread, NULL, (void*)(&Quit), NULL);

	//紀錄閒置的client(-1)
	//initialize
	int i=0;
	for(i=0; i<MAXMEM; i++) {
		connfd[i]=-1;
	}
	memset(user, '\0', sizeof(user));
	printf("initialize...\n");

	while(1) {
		length = sizeof(cli_addr);
		for(i=0; i<MAXMEM; i++) {
			if(connfd[i]==-1) {
				break;
			}
		}
		//等待client端連線
		printf("receiving...\n");
		connfd[i] = accept(listenfd, (struct sockaddr*)&cli_addr, &length);

		//對新client建thread，以開啟訊息處理
		pthread_create(malloc(sizeof(pthread_t)), NULL, (void*)(&rcv_snd), (void*)i);
	}

	return 0;
}
//關閉server
void Quit()
{
	char msg[10];
	while(1) {
		scanf("%s", msg);
		if(strcmp("/quit",msg)==0) {
			printf("Bye~\n");
			close(listenfd);
			exit(0);
		}
	}
}

void rcv_snd(int n)
{
	char msg_notify[MAXLINE];
	char msg_recv[MAXLINE];
	char msg_send[MAXLINE];
	char who[MAXLINE];
	char name[NAMELEN];
	char message[MAXLINE];

	char msg1[]="<SERVER> Who do you want to send? ";
	char msg2[]="<SERVER> Complete.\n";
	char msg3[]="<SERVER> Refuse to receive.";
	char msg4[]="<SERVER> Download...\n";
	char msg5[]="<SERVER> Confirm?";
	char msg6[]="ok";
	char msg7[]="<server> Please type the name of the player you want to against with";
	char check[MAXLINE];
	char ok[3];

	int i=0,b_cnt=0;
	int retval;

	//獲得client的名字
	int length;
	length = recv(connfd[n], name, NAMELEN, 0);
	if(length>0) {
		name[length] = 0;
		strcpy(user[n], name);
	}
	//告知所有人有新client加入
	memset(msg_notify, '\0', sizeof(msg_notify));
	strcpy(msg_notify, name);
	strcat(msg_notify, " join\n");
	for(i=0; i<MAXMEM; i++) {
		if(connfd[i]!=-1) {
			send(connfd[i], msg_notify, strlen(msg_notify), 0);
		}
	}
	//接收某client的訊息並轉發
	while(1) {
		memset(msg_recv, '\0', sizeof(msg_recv));
		memset(msg_send, '\0', sizeof(msg_send));
		memset(message,'\0',sizeof(message));
		memset(check,'\0',sizeof(check));

		if((length=recv(connfd[n], msg_recv, MAXLINE, 0))>0) {
			msg_recv[length]=0;
			//輸入quit離開
			if(strcmp("/quit\n", msg_recv)==0) {
				connfd[n]=-1;
				close(connfd[n]);
				pthread_exit(&retval);
			}
			//輸入chat傳給特定人
			else if(strncmp("/chat", msg_recv, 5)==0) {
				printf("private message...\n");
				send(connfd[n], msg1, strlen(msg1), 0);
				length = recv(connfd[n], who, MAXLINE, 0);
				who[length]=0;
				strcpy(msg_send, who);
				strcat(msg_send, ">");
				msg_send[strlen(who)-1]='>';
				send(connfd[n], msg_send, strlen(msg_send), 0);
				length = recv(connfd[n], message, MAXLINE, 0);
				message[length]=0;

				strcpy(msg_send, name);
				strcat(msg_send, ": ");
				strcat(msg_send, message);

				for(i=0; i<MAXMEM; i++) {
					if(connfd[i]!=-1) {
						if(strncmp(who, user[i], strlen(who)-1)==0) {
							send(connfd[i], msg_send, strlen(msg_send), 0);
						}
					}
				}
			}
			else if(strncmp("/game", msg_recv, 5)==0){
				int p1=0,p2=1;
				send(connfd[n], msg7, strlen(msg7), 0);
				length = recv(connfd[n], who, MAXLINE, 0);//got a name to fight
				who[length]=0;
				strcpy(msg_send,"Would you like to play a game with ");
				strcat(msg_send,name);
				strcat(msg_send,"(y/n)");
				for(i=0; i<MAXMEM; i++) {
					if(connfd[i]!=-1) {
						if(strncmp(who, user[i], strlen(who)-1)==0) {
							send(connfd[i], msg_send, strlen(msg_send), 0);
							length = recv(connfd[i], message, MAXLINE, 0);
						}
					}
				}
				printf("a:%s\n",message);
				if(strncmp(message, "Y", 1)==0 || strncmp(message, "y", 1)==0) {
					printf("yes\n");
 
					printf("Let's start the game:\n");
				//	printf("please enter the position with the type of(x,y),ex:0 0\n");
					strcpy(msg_send,"Let's start the game:\n");
					for(i=0; i<MAXMEM; i++) {
						if(connfd[i]!=-1) {
							if(strncmp(who, user[i], strlen(who)-1)==0) {
								send(connfd[i], msg_send, strlen(msg_send), 0);
								p2=i;
							}
							else if(strncmp(name, user[i], strlen(name)-1)==0) {
								send(connfd[i], msg_send, strlen(msg_send), 0);
								p1=i;
							}

						}
					}
					strcpy(msg_send,"please enter the position with the type of(x,y),ex:0 0\n");
					send(connfd[p1], msg_send, strlen(msg_send), 0);
					send(connfd[p2], msg_send, strlen(msg_send), 0);
					// TTTS array[3][3] = {TTTS_Default};
					int index_x,index_y;
					int counter = 0;
					Print(array);
					int flag=1,num1,num2,player1=p1,player2=p2;
					while(1){
						message[0]=0;
						stop:
						if(flag==1){
							p1=0,p2=1;///
							strcpy(msg_send,"It's your turn:'\n");
							send(connfd[p1], msg_send, strlen(msg_send), 0);
							length = recv(connfd[p1], message, MAXLINE, 0);
							message[length]=0;
							strcpy(msg_send, user[p1]);
							strcat(msg_send, ": ");
							strcat(msg_send, message);
							send(connfd[p2], msg_send, strlen(msg_send), 0);
							num1=atoi(message);
							num2=atoi(message+2);
							//printf("flag:%d num1,num2 p1 p2:%d %d %d %d\n",flag,num1,num2,p1,p2);
							printf("----------------------\n");
							flag=2;
							//no exception solve
						}
						else {//if(flag==2)
							p1=0,p2=1;///
							//printf("flag:%d num1,num2 p1 p2:%d %d %d %d\n",flag,num1,num2,p1,p2);
							printf("----------------------\n");
							strcpy(msg_send,"It's your turn:\n");
							send(connfd[p2], msg_send, strlen(msg_send), 0);
							length = recv(connfd[p2], message, MAXLINE, 0);
							message[length]=0;
							strcpy(msg_send, user[p2]);
							strcat(msg_send, ": ");
							strcat(msg_send, message);
							send(connfd[p1], msg_send, strlen(msg_send), 0);
							num1=atoi(message);
							num2=atoi(message+2);
							// printf("flag:%d num1,num2 p1 p2:%d %d %d %d\n",flag,num1,num2,p1,p2);
							flag=1;

						}
						/*	strcpy(msg_send,"It's your turn:\n");
							send(connfd[player2], msg_send, strlen(msg_send), 0);
							length = recv(connfd[player2], message, MAXLINE, 0);
							message[length]=0;
							strcpy(msg_send, user[player2]);
							strcat(msg_send, ": ");
							strcat(msg_send, message);
							send(connfd[player1], msg_send, strlen(msg_send), 0);
							num1=atoi(message);
							num2=atoi(message+2);
							printf("player:%d num1,num2:%d %d\n",player2,num1,num2);
						//	player2=1,player1=0;
						//	flag=1;
							if(player2==0){
								player1=0;
								player2=1;
							}						
							else {
								player1=1;
								player2=0;
							}*/
						array[num1][num2] = counter % 2 ? TTTS_O : TTTS_X; 
						Print(array);
						if(TTTS_O == CheckWin(array))
						{

							strcpy(msg_send,user[p2]);
							strcat(msg_send," win the game\n");
							send(connfd[0], msg_send, strlen(msg_send), 0);
							send(connfd[1], msg_send, strlen(msg_send), 0);
							printf("game over,O win!!!!!\n");
							break;
						}
						else if(TTTS_X == CheckWin(array))
						{
							strcpy(msg_send,user[p1]);
							strcat(msg_send," win the game\n");
							send(connfd[0], msg_send, strlen(msg_send), 0);
							send(connfd[1], msg_send, strlen(msg_send), 0);
							printf("game over,X win!!!!!\n");
							break;
						}	
						counter++;//识别O和X
						if(counter >= 9 )
						{
							strcpy(msg_send,"game over,draw game!\n");
							send(connfd[p1], msg_send, strlen(msg_send), 0);
							send(connfd[p2], msg_send, strlen(msg_send), 0);
							printf("game over,draw game!\n");
							break;
						}

					}
				}
				//No取消傳送
				else if(strncmp(message, "N", 1)==0 || strncmp(message, "n", 1)==0) {
					printf("no\n");

					send(connfd[n], msg3, strlen(msg3), 0);
					memset(message, '\0', sizeof(message));
					
				}
			
			}

			//顯示目前在線
			else if(strncmp("/list", msg_recv, 5)==0) {
				strcpy(msg_send, "<SERVER> Online:");
				for(i=0; i<MAXMEM; i++) {
					if(connfd[i]!=-1) {
						//	printf("onlineID:%d\n",i);
						strcat(msg_send, user[i]);
						strcat(msg_send, " ");
					}
				}
				strcat(msg_send, "\n");
				send(connfd[n], msg_send, strlen(msg_send), 0);
			}
			//直接傳給每個人
			else {
				b_cnt++;
				printf("...please type again\n");
				if(b_cnt>=0){
					if(strcmp(msg_recv,"y\n")!=0&&strcmp(msg_recv,"n\n")!=0){
						goto stop;
					}
					 
				}
			//
				strcpy(msg_send, name);
				strcat(msg_send,">>> ");
				strcat(msg_send, msg_recv);

				for(i=0;i<MAXMEM;i++) {
					if(connfd[i]!=-1) {
						if(strcmp(name, user[i])==0) {
							continue;
						}else {
							send(connfd[i], msg_send, strlen(msg_send), 0);
						}
					}
				}
			}
		}
	}
}
