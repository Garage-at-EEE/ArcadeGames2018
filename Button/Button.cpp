/*
  NTU Garage@EEE
  Button.cpp - Arduino Library for button.
  Created by Jian Xian and Cong Sheng Dec 2018.
*/

#include "Button.h"

void Button::attach(int pin){
	pinMode(pin, INPUT_PULLUP);
}

void Button::set_State(int value) {
	btnState = value;
}

bool Button::get_State() {
	return btnState; 
}

bool Button::isPressed(int pin, int sample_Size) {
	int sum = 0;
	for (int j = 0; j < sample_Size ; j++) {
	   sum += digitalRead(pin); //looping through the given sample size, add the value of digitalRead to sum
	}
	if (sum == 0 && btnState == 0) { 
	   btnState = 1;//resetting the btnState so that it doesn not double count
	   instance_Pressed = millis(); // record time for instance_Pressed
	return true;
	}
	else if (sum == sample_Size && btnState == 1) {  
	   btnState = 0; //resetting the btnState so that it doesn not double count
	  return false;
	}
	else {
	  return false;
	}
}

bool Button::isReleased(int pin, int sample_Size) {
	int sum = 0;
	for (int j = 0; j < sample_Size; j++) {
		sum += digitalRead(pin); //looping through the given sample size, add the value of digitalRead to sum
	}
	if(sum == sample_Size && btnState == 1) { 
		btnState = 0; //resetting the btnState so that it doesn not double count
		instance_Released = millis(); // record time for instance_Pressed
	  return true;
	}
	else if (sum == 0 && btnState == 0) { 
		btnState = 1; //resetting the btnState so that it doesn not double count
	  return false; 
	}
	else {
	  return false;
	}
}

bool Button::isHeld(int pin, int sample_Time){
	int sum = 0;
	for (int j = 0; j < sample_Time ; j++) {
	   isPressed(pin, sample_Time); //checking if the button is pressed and returning true
	   sum += digitalRead(pin); //looping through the given sample time, add the value of digitalRead to sum
	}
	if (sum == 0 && btnState == 1 ) {
	   current_Time = millis(); //getting current time and 
	   time_Press = current_Time - instance_Pressed; //substracting it with the instance it is pressed to give time the button is held
	  return true;
	}
	else if (sum > 0) { 
	  return false;
	}
	else {
	  return false;
	}  
}

bool Button::isLeftopen(int pin, int sample_Time){
	int sum = 0;
	for (int j = 0; j < sample_Time ; j++) {
	   isReleased(pin, sample_Time); //checking if the button is released and returning true
	   sum += digitalRead(pin); //looping through the given sample time, add the value of digitalRead to sum
	}
	if (sum == sample_Time && btnState == 0 ) {
	   current_Time = millis(); //getting current time and 
	   time_Leftopen = current_Time - instance_Released;//substracting it with the instance it is pressed to give time the button is left open
	  return true;
	}
	else if (sum < sample_Time) { 
	  return false;
	}
	else {
	  return false;
	}
}



