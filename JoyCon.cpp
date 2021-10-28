#include "JoyCon.h"
#include"hidapi.h"
#define JOYCON_L_PRODUCT_ID 8198
#define JOYCON_R_PRODUCT_ID 8199
#include<Windows.h>
#include<thread>
using namespace std;

//
void SendSubcommand(hid_device* dev, uint8_t command, uint8_t data[], int len, uint8_t* globalCount) {
	if (dev != nullptr) {
		uint8_t buf[0x40]; memset(buf, 0x0, size_t(0x40));

		buf[0] = 1; // 0x10 for rumble only
		buf[1] = *globalCount; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.

		if (*globalCount == 0xf0) {
			*globalCount = 0x00;
		}
		else {
			*globalCount++;
		}

		buf[10] = command;
		memcpy(buf + 11, data, len);

		hid_write(dev, buf, 0x40);

	}
}

void ValueToKey(uint8_t& buttonValue, uint8_t liminialValue, bool& button) {
	if (buttonValue >= liminialValue) {
		buttonValue -= liminialValue;
		button = true;
	}
	else button = false;
}


void gyro::JoyCon::GetValue() {
	if (joyconType != JoyConType::Invalid) {
		hid_device* d = device;
		while (d == device) {
			int a = hid_read(device, buff, size);
			if (joyconType == JoyConType::JoyConL) {
				uint8_t keytype_normal = buff[5];
				uint8_t keytype_special = buff[4];

				ValueToKey(keytype_normal, 128, buttonZLZR);
				ValueToKey(keytype_normal, 64, buttonLR);
				ValueToKey(keytype_normal, 32, buttonSL);
				ValueToKey(keytype_normal, 16, buttonSR);
				ValueToKey(keytype_normal, 8, button3);
				ValueToKey(keytype_normal, 4, button2);
				ValueToKey(keytype_normal, 2, button1);
				ValueToKey(keytype_normal, 1, button0);
				ValueToKey(keytype_special, 32, buttonScreenshot);
				ValueToKey(keytype_special, 8, buttonStick);
				ValueToKey(keytype_special, 1, buttonMinus);

				stickX = (buff[7] >> 4) | (buff[8] << 4);
				stickY= buff[6] | ((buff[7] & 0xF) << 8);

			}
			else {
				uint8_t keytype_normal = buff[3];
				uint8_t keytype_special = buff[4];

				ValueToKey(keytype_normal, 128, buttonZLZR);
				ValueToKey(keytype_normal, 64, buttonLR);
				ValueToKey(keytype_normal, 32, buttonSL);
				ValueToKey(keytype_normal, 16, buttonSR);
				ValueToKey(keytype_normal, 8, button3);
				ValueToKey(keytype_normal, 4, button2);
				ValueToKey(keytype_normal, 2, button1);
				ValueToKey(keytype_normal, 1, button0);
				ValueToKey(keytype_special, 16, buttonHome);
				ValueToKey(keytype_special, 4, buttonStick);
				ValueToKey(keytype_special, 2, buttonPlus);

				stickX = (buff[10] >> 4) | (buff[11] << 4);
				stickY = buff[9] | ((buff[10] & 0xF) << 8);
			}
			uint16_t byteValue = buff[13] | buff[14] << 8;
			accelX = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.000244;
			byteValue = buff[15] | buff[16] << 8;
			accelY = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.000244;
			byteValue = buff[17] | buff[18] << 8;
			accelZ = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.000244;
			byteValue = buff[19] | buff[20] << 8;
			gyroX = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.06103;
			byteValue = buff[21] | buff[22] << 8;
			gyroY = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.06103;
			byteValue = buff[23] | buff[24] << 8;
			gyroZ = (byteValue > 32768 ? byteValue - 65536 : byteValue) * 0.06103;


		}
		
	}
	
	
}

gyro::JoyCon::JoyCon(const size_t index) {
	int count = 0;
	globalCount = 0;
	devices = hid_enumerate(0, 0);
	while (devices) {
		if (devices->product_id == JOYCON_L_PRODUCT_ID) {
			if (index == count) {
				device = hid_open(devices->vendor_id, devices->product_id, devices->serial_number);
				joyconType = JoyConType::JoyConL;
				break;
			}
			else count++;
		}
		else if (devices->product_id == JOYCON_R_PRODUCT_ID) {
			if (index == count) {
				device = hid_open(devices->vendor_id, devices->product_id, devices->serial_number);
				joyconType = JoyConType::JoyConR;
				break;
			}
			else count++;
		}
		devices = devices->next;
	}


	//プレイヤーライト光らせる
	data[0] = 0x01<<index;
	SendSubcommand(device,0x30, data, 1,&globalCount);

	Sleep(30);

	//ジャイロや加速度センサーを使えるようにする
	data[0] = 0x01;
	SendSubcommand(device,0x40, data, 1,&globalCount);

	Sleep(30);

	//測定できるようにする
	data[0] = 0x30;
	SendSubcommand(device,0x03, data, 1,&globalCount);

	thread joy(&gyro::JoyCon::GetValue, this);
	joy.detach();
}

gyro::JoyCon::~JoyCon() {
	hid_free_enumeration(devices);
}
