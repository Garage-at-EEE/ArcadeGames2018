/*
  NTU Garage@EEE
  Button.h - Arduino Library for button.
  Created by Jian Xian and Cong Sheng Dec 2018.
*/

#ifndef Button_h //if lib is not defined, define button_h
#define Button_h 

#include <Arduino.h>



class Button{
	private:
		int btnState = 0, pin; 
		unsigned long instance_Pressed, instance_Released, current_Time, time_Press, time_Leftopen;

	public:
		
		void attach(int pin);
		void set_State(int value);	
		bool get_State();
		bool isPressed(int pin, int sample_Size);
		bool isHeld(int pin, int sample_Time);
		bool isReleased(int pin, int sample_Size);
		bool isLeftopen(int pin, int sample_Time);
};

#endif
