
#include "controlDevice.h"
#include <stdlib.h>


int bathroomLightOpen(int pin)
{
	digitalWrite(pin, LOW);

}


int bathroomLightClose(int pin)
{
	digitalWrite(pin, HIGH);

}


int bathroomLightCloseInit(int pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
}


/*int bathroomLightCloseStatus(int status)
{


} */
struct Device bathroomLight =
{													//定义浴室灯结构体

	.deviceName = "bathroomLight", 
	.pin = 22, 
	.open = bathroomLightOpen, 
	.close = bathroomLightClose, 
	.initDevice = bathroomLightCloseInit, 

	//	.changeStatus = bathrooomLightCloseStatus
	.next = NULL
};


//添加浴室灯结构体到链表 	 （头插法）
struct Device * addBathroomLightToDeviceLinks(struct Device * phead)
{
	if (phead == NULL)
		{
		return & bathroomLight;
		}
	else 
		{
		bathroomLight.next	= phead;
		phead				= &bathroomLight;

		return phead;

		}

}

