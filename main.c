
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "controlDevice.h"
#include "InputCommand.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define fpin					1				//蜂鸣器端口

int 			c_fd;

struct Device * pdeviceHead = NULL;
struct Device * pdeviceHead1 = NULL;
struct Device * pdeviceHead2 = NULL;
struct Device * pdeviceHead3 = NULL;
struct Device * pdeviceHead4 = NULL;
struct Device * pdeviceHead5 = NULL;
struct Device * pdeviceHead6 = NULL;
struct InputCommander * pcommanderHead = NULL;
struct InputCommander * socketHandler = NULL;

//找设备名
struct Device * findDeviceByName(struct Device * phead, char * name)
{
	struct Device * tmp = phead;

	if (phead == NULL)
		{
		return NULL;
		}


	else 
		{
		while (tmp != NULL)
			{
			if (strcmp(tmp->deviceName, name) == 0)
				{
				return tmp;
				}

			tmp 				= tmp->next;
			}

		return NULL;
		}


} //找指令	 语音or网络


struct InputCommander * findCommandByName(struct InputCommander * phead, char * name)
{
	struct InputCommander * tmp = phead;

	if (phead == NULL)
		{
		return NULL;
		}


	else 
		{
		while (tmp != NULL)
			{
			if (strcmp(tmp->commandName, name) == 0)
				{
				return tmp;
				}

			tmp 				= tmp->next;
			}

		return NULL;
		}


} void * camera_Pthread(void * arg)
{

	FILE *			p;
	int 			i;

	//struct Device *tmpc = findDeviceByName(pdeviceHead,"camera");  
	//struct Device *tmpl = findDeviceByName(pdeviceHead,"lock");  
	for (i = 0; i < 3; i++)
		{
		char			cmd[1] =
			{
			'\0'
			};
		pdeviceHead6->initDevice(pdeviceHead6->pin);
		pdeviceHead5->cameraStatus();

		//调用人脸识别程序
		p					= popen("python sheixian.py", "r");

		if (p == NULL)
			{
			perror("reason");
			exit(-1);
			}

		printf("you face data is will be read.....\n");
		fread(cmd, 1, 1, p);

		//读取结果进行开关锁
		if (strstr(cmd, "1") != NULL)
			{
			printf("face recognize ok\n");

			//digitalWrite(SWITCH,LOW);
			pdeviceHead6->open(pdeviceHead6->pin);
			sleep(5);

			//digitalWrite(SWITCH,HIGH);
			pdeviceHead6->close(pdeviceHead6->pin);
			pthread_exit(NULL);
			}
		else if (strstr(cmd, "0") != NULL)
			{
			printf("wrong!!!!!!!!");
			}
		else 
			{
			perror("reason");
			}

		}
}


//对设备(灯)的具体操作 			关 or 开
void operateDevice(char handlerDevice[32])
{
	//pthread_t cameraPthread;
	//handlerDevice = (char*)malloc(32);
	//memmset(handlerDevice,'\0',32);
	if (strstr(handlerDevice, "oys") != NULL)
		{
		pdeviceHead->initDevice(pdeviceHead->pin);
		pdeviceHead->open(pdeviceHead->pin);
		}
	else if (strstr(handlerDevice, "cys") != NULL)
		{
		pdeviceHead->initDevice(pdeviceHead->pin);
		pdeviceHead->close(pdeviceHead->pin);
		}

	if (strstr(handlerDevice, "okt") != NULL)
		{
		pdeviceHead1->initDevice(pdeviceHead1->pin);
		pdeviceHead1->open(pdeviceHead1->pin);
		}
	else if (strstr(handlerDevice, "ckt") != NULL)
		{
		pdeviceHead1->initDevice(pdeviceHead1->pin);
		pdeviceHead1->close(pdeviceHead1->pin);
		}

	if (strstr(handlerDevice, "oct") != NULL)
		{
		pdeviceHead3->initDevice(pdeviceHead3->pin);
		pdeviceHead3->open(pdeviceHead3->pin);
		}
	else if (strstr(handlerDevice, "cct") != NULL)
		{
		pdeviceHead3->initDevice(pdeviceHead3->pin);
		pdeviceHead3->close(pdeviceHead3->pin);
		}

	if (strstr(handlerDevice, "ows") != NULL)
		{
		pdeviceHead2->initDevice(pdeviceHead2->pin);
		pdeviceHead2->open(pdeviceHead2->pin);
		}
	else if (strstr(handlerDevice, "cws") != NULL)
		{
		pdeviceHead2->initDevice(pdeviceHead2->pin);
		pdeviceHead2->close(pdeviceHead2->pin);
		}

	/*if(strstr(handlerDevice,"osxt") != NULL){
												//创建摄像头线程
		 pthread_create(&cameraPthread,NULL,camera_Pthread,NULL);
		 pthread_join(cameraPthread,NULL);
	} */
	// free(handlerDevice);
	memset(handlerDevice, '\0', 32);
}


//语音线程
void * voice_Pthread(void * arg)
{
	//pthread_t cameraPthread;
	struct InputCommander * voiceHandler = findCommandByName(pcommanderHead, "voice");

	if (voiceHandler == NULL)
		{
		printf("find command error\n");
		pthread_exit(NULL);
		}
	else 
		{
		printf("voice serial init!\n");
		voiceHandler->Init(voiceHandler, NULL, NULL, NULL);

		if (voiceHandler->fd == -1)
			{
			printf("serial error\n");
			pthread_exit(NULL);
			}
		else 
			{
			//printf("voice serial ok!\n");
			//读取串口中的数据
			while (1)
				{
				int 			nread = voiceHandler->getCommand(voiceHandler);

				if (nread == 0)
					{
					printf("no data from usart\n");
					}
				else 
					{
					printf("read %d byte,content is %s\n", nread, voiceHandler->command);

					/* if(strstr(voiceHandler->command,"open") != NULL){
						 pdeviceHead->initDevice(pdeviceHead->pin);
						 pdeviceHead->open(pdeviceHead->pin);  
					 }else if(strstr(voiceHandler->command,"close") != NULL){
						 pdeviceHead->initDevice(pdeviceHead->pin);
						 pdeviceHead->close(pdeviceHead->pin);

					 } */
					operateDevice(voiceHandler->command);

					}
				}
			}
		}
}


void * read_Pthread(void * arg)
{

	while (1)
		{
		pthread_t		cameraPthread;

		memset(&socketHandler->command, '\0', sizeof(socketHandler->command));
		int 			n_read = read(c_fd, socketHandler->command, sizeof(socketHandler->command));

		if (n_read == -1)
			{
			perror("read");
			}
		else 
			{
			printf("get message:%d,%s\n", n_read, socketHandler->command);

			if (strstr(socketHandler->command, "qc") != NULL)
				{
				printf("socket is lost connect.....\n");
				pthread_exit(NULL);

				}

			if (strstr(socketHandler->command, "osxt") != NULL)
				{
				//创建摄像头线程
				pthread_create(&cameraPthread, NULL, camera_Pthread, NULL);
				pthread_join(cameraPthread, NULL);
				}
			else 
				{
				operateDevice(socketHandler->command);
				}
			}
		}
}


//网络 socket线程
void * socket_Pthread(void * arg)
{
	struct sockaddr_in c_addr;
	pthread_t		readPthread;

	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	int 			clen = sizeof(struct sockaddr_in);

	socketHandler		= findCommandByName(pcommanderHead, "socket");

	if (socketHandler == NULL)
		{
		printf("find command error\n");
		pthread_exit(NULL);
		}
	else 
		{
		printf("socket init!\n");
		socketHandler->Init(socketHandler, NULL, NULL, NULL);

		if (socketHandler->fd == -1)
			{
			printf("socket error\n");
			pthread_exit(NULL);
			}
		else 
			{
			//printf("socket ok!\n");
			socketHandler->getCommand(socketHandler);

			//和服务端连接
			while (1)
				{
				c_fd				= accept(socketHandler->fd, (struct sockaddr *) &c_addr, &clen);

				if (c_fd == -1)
					{
					perror("accept");
					}
				else 
					{
					printf("get connect:%s\n", inet_ntoa(c_addr.sin_addr));

					//创建读取网络的指令
					pthread_create(&readPthread, NULL, read_Pthread, NULL);
					}
				}
			}
		}
}


//火灾模块线程
void * fire_Pthread(void * arg)
{
	pdeviceHead4->initDevice(pdeviceHead4->pin);
	pinMode(fpin, OUTPUT);
	digitalWrite(fpin, HIGH);

	while (1)
		{
		pdeviceHead4->readStatus(pdeviceHead4->pin);

		if (digitalRead(pdeviceHead4->pin) == 0)
			{
			printf("md!!!have fire!!!!\n");
			digitalWrite(fpin, LOW);
			sleep(3);
			digitalWrite(fpin, HIGH);
			}
		else 
			{
			digitalWrite(fpin, HIGH);
			}

		}

}


int main()
{
	if (wiringPiSetup() == -1)
		{
		printf("wringPi error\n");

		}

	char *			name = (char *)

	malloc(128);

	//1.指令工厂初始化
	pcommanderHead		= addVoiceControlToCommandLinks(pcommanderHead);
	pcommanderHead		= addSocketControlToCommandLinks(pcommanderHead);

	//2.设备控制工厂初始化
	pdeviceHead 		= addBathroomLightToDeviceLinks(pdeviceHead);
	pdeviceHead1		= addLivingroomLightToDeviceLinks(pdeviceHead);
	pdeviceHead2		= addUpstairLightToDeviceLinks(pdeviceHead);
	pdeviceHead3		= addRestaurantLightToDeviceLinks(pdeviceHead);
	pdeviceHead4		= addFireToDeviceLinks(pdeviceHead);

	pdeviceHead5		= addCameraToDeviceLinks(pdeviceHead);
	pdeviceHead6		= addLockToDeviceLinks(pdeviceHead);

	while (1)
		{
		/*printf("input you commmand of light\n");
		scanf("%s",name);
		struct Device *tmp = findDeviceByName(pdeviceHead,name);

		if(tmp != NULL){
			 tmp->initDevice(tmp->pin);
			 tmp->open(tmp->pin);
			 }
			*/
		//3.线程池建立
		pthread_t		voicePthread;
		pthread_t		socketPthread;
		pthread_t		firePthread;

		//3.1语音线程
		pthread_create(&voicePthread, NULL, voice_Pthread, NULL);

		//3.2 Socket线程
		pthread_create(&socketPthread, NULL, socket_Pthread, NULL);

		//3.3 摄像头线程
		//3.4 火灾模块线程
		pthread_create(&firePthread, NULL, fire_Pthread, NULL);

		pthread_join(voicePthread, NULL);
		pthread_join(socketPthread, NULL);
		pthread_join(firePthread, NULL);

		//free(name);
		}


	return 0;
}

