
#include "controlDevice.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int cameraInit()
{
	printf("do not move,camera will be open ......\n");
	sleep(5);
	system("raspistill -o wch2.jpg");
	printf("wait -----\n");

}


struct Device camera =
{

	.deviceName = "camera", 
	.cameraStatus = cameraInit, 
	.next = NULL
};


//添加摄像头结构体到链表 	 （头插法）
struct Device * addCameraToDeviceLinks(struct Device * phead)
{
	if (phead == NULL)
		{
		return & camera;
		}
	else 
		{
		camera.next 		= phead;
		phead				= &camera;

		return phead;

		}

}

