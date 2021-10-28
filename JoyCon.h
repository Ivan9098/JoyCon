#pragma once

//拾ってきたヘッダーファイルです.
#include"hidapi.h"
#include <cstdint>


//かぶらないためのnamespace
namespace gyro{
	class JoyCon {
	private:
		//通信する上で必要な変数や関数
		hid_device_info* devices;
		hid_device* device;
		size_t size = 49;
		uint8_t data[0x01];
		uint8_t buff[0x40];
		uint8_t globalCount;
		
		void GetValue();
	public:
		
		enum class JoyConType
		{
			Invalid,

			JoyConL,

			JoyConR,
		};

		JoyConType joyconType = JoyConType::Invalid;
		bool
			button0,//Y or Down
			button1,//X or Up
			button2,//B or Right
			button3,//A or left
			buttonSL,
			buttonSR,
			buttonMinus,
			buttonPlus,
			buttonStick,
			buttonHome,
			buttonScreenshot,
			buttonLR,
			buttonZLZR
			= false;

		//単位はDegree/s
		double
			gyroX,
			gyroY,
			gyroZ,

		//単位はG
		//この値には重力が含まれている
			accelX,
			accelY,
			accelZ
			= 0;

		//とってくる値がx,yで上限下限が異なって少し厄介
		int
			stickX,
			stickY
			= 0;

		JoyCon(const size_t index);
		~JoyCon();
	};
}

