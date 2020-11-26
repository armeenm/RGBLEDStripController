#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11
#define FADESPEED *(vals + 1)

int input[8];

void setup() {
	Serial.begin(9600);
	Serial.flush();
	pinMode(REDPIN, OUTPUT);
	pinMode(GREENPIN, OUTPUT);
	pinMode(BLUEPIN, OUTPUT);
}

void loop() {
	getInstr();
	changeLED(input);
}

void changeLED(int* vals) {
	switch (*vals) {
		case 0:
			analogWrite(REDPIN, *(vals + 1));
			analogWrite(GREENPIN, *(vals + 2));
			analogWrite(BLUEPIN, *(vals + 3));
			break;
		case 1:
			analogWrite(REDPIN, *(vals + 2));
			analogWrite(GREENPIN, *(vals + 3));
			analogWrite(BLUEPIN, *(vals + 4));
			delay(FADESPEED);
			analogWrite(REDPIN, *(vals + 5));
			analogWrite(GREENPIN, *(vals + 6));
			analogWrite(BLUEPIN, *(vals + 7));
			delay(FADESPEED);
			break;
		case 2:
			int r, g, b;
			// fade from blue to violet
			for (r = 0; r < 256; r++) {
				analogWrite(REDPIN, r);
				delay(FADESPEED);
			}
			if (getInstr())
				return;
			// fade from violet to red
			for (b = 255; b > 0; b--) {
				analogWrite(BLUEPIN, b);
				delay(FADESPEED);
			}
			if (getInstr())
				return;
			// fade from red to yellow
			for (g = 0; g < 256; g++) {
				analogWrite(GREENPIN, g);
				delay(FADESPEED);
			}
			if (getInstr())
				return;
			// fade from yellow to green
			for (r = 255; r > 0; r--) {
				analogWrite(REDPIN, r);
				delay(FADESPEED);
			}
			if (getInstr())
				return;
			// fade from green to teal
			for (b = 0; b < 256; b++) {
				analogWrite(BLUEPIN, b);
				delay(FADESPEED);
			}
			if (getInstr())
				return;
			// fade from teal to blue
			for (g = 255; g > 0; g--) {
				analogWrite(GREENPIN, g);
				delay(FADESPEED);
			}
			break;
	}
}

int getInstr() {
	if (Serial.available() == 0)
		return 0;
  int num_avail = Serial.available();
	while (Serial.available() > 0) {
		for (int n = 0; n < 8; n++)
			input[n] = Serial.parseInt();

		if (Serial.read() == '\n') {
			Serial.print("1 ");
      Serial.println(num_avail);
			return 1;
		}
	}
}
