#include "timer.h"

int ChangeMinute(int Minute, bool Mode) {
	switch (Mode) {
	case true:
		if (Minute < 99) {
			Minute++;
		}
		break;
	case false:
		if (Minute > 0) {
			Minute--;
		}
		break;
	}
	return Minute;
}
