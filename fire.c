
#include "controlDevice.h"
#include <stdlib.h>


int fireInit(int pin)
{
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);

	//pinMode(fpin,OUTPUT);
	//digitalWrite(fpin,HIGH);
}


int fireStatusRead(int pin)
{
	return digitalRead(pin);

}


struct Device fireOn =
{													//定义火灾结构体

	.deviceName = "fire", 
	.pin = 25, 
	.initDevice = fireInit, 
	.readStatus = fireStatusRead, 
	.next = NULL
};


//添加火灾模块结构体到链表
struct Device * addFireToDeviceLinks(struct Device * phead)
{
	if (phead == NULL)
		{
		return & fireOn;
		}
	else 
		{
		fireOn.next = phead;
		phead = &fireOn;

		return phead;

		}

}

