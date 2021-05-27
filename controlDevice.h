
#include <wiringPi.h>


struct Device
{
	char			deviceName[128];
	int 			status;
	int 			pin;

	//函数指针
	int(*open) (int pin);
	int(*close) (int pin);
	int(*initDevice) (int pin);

	int(*readStatus) (int pin);
	int(*changeStatus) (int status);

	int(*cameraStatus) ();

	struct Device * next;
};


struct Device * addBathroomLightToDeviceLinks(struct Device * phead);
struct Device * addLivingroomLightToDeviceLinks(struct Device * phead);
struct Device * addUpstairLightToDeviceLinks(struct Device * phead);
struct Device * addRestaurantLightToDeviceLinks(struct Device * phead);
struct Device * addFireToDeviceLinks(struct Device * phead);
struct Device * addCameraToDeviceLinks(struct Device * phead);
struct Device * addLockToDeviceLinks(struct Device * phead);
