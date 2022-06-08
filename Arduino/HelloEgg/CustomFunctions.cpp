#include "CustomFunctions.h"

Ei::Ei(LedEgg egg)
{
	this->egg = egg;
	buttonPressed = false;
}

Ei::~Ei()
{
}

bool Ei::knopfGedrueckt()
{
	if (digitalRead(4) == 0 && !buttonPressed) {
		buttonPressed = true;
		return true;
	}
	if (digitalRead(4) != 0) {
		buttonPressed = false;
	}
	return false;
}

void Ei::ledAn(int nummer)
{
	if (nummer < NUM_LED) {
		egg.LEDarray[nummer] = 255;
	}
}

void Ei::ledAn(int nummer, int helligkeit)
{
	if (nummer < NUM_LED) {
		egg.LEDarray[nummer] = helligkeit;
	}
}

void Ei::ledAus(int nummer)
{
	if (nummer < NUM_LED) {
		egg.LEDarray[nummer] = 0;
	}
}

void Ei::wechsleLed(int nummer)
{
	if (nummer < NUM_LED) {
		if (egg.LEDarray[nummer] != 0) {
			egg.LEDarray[nummer] = 0;
		}
		else {
			egg.LEDarray[nummer] = 255;
		}
	}
}

void Ei::sendeBefehl()
{
	egg.do_LESs();
}
