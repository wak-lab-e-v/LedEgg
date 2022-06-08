#include "LedEgg.h"

class Ei {
	public:
		Ei(LedEgg egg);
		~Ei();
		bool knopfGedrueckt();
		void ledAn(int nummer);
		void ledAn(int nummer, int helligkeit);
		void ledAus(int nummer);
		void wechsleLed(int nummer);
		void sendeBefehl();
	private:
		LedEgg egg;
		bool buttonPressed;
};

