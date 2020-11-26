#define REDPIN 9
#define GREENPIN 10
#define BLUEPIN 11
#define FADESPEED *(vals + 1)

void setup() {
  Serial.begin(9600);
  Serial.flush();
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}

void loop() {
  int input[8];

  getInstr(input);
  change_led(input);
}

void change_led(int* vals) {
  switch (vals[0]) {
    case 0:
      analogWrite(REDPIN, vals[1]);
      analogWrite(GREENPIN, vals[2]);
      analogWrite(BLUEPIN, vals[3]);
      break;

    case 1:
      analogWrite(REDPIN, vals[2]);
      analogWrite(GREENPIN, vals[3]);
      analogWrite(BLUEPIN, vals[4]);
      delay(FADESPEED);
      analogWrite(REDPIN, vals[5]);
      analogWrite(GREENPIN, vals[6]);
      analogWrite(BLUEPIN, vals[7]);
      delay(FADESPEED);
      break;

    case 2:
      int r, g, b;

      // Fade from blue to violet
      if (getInstr(vals)) return;
      for (r = 0; r < 256; r++) {
        analogWrite(REDPIN, r);
        delay(FADESPEED);
      }

      if (getInstr(vals)) return;
      // Fade from violet to red
      for (b = 255; b > 0; b--) {
        analogWrite(BLUEPIN, b);
        delay(FADESPEED);
      }

      if (getInstr(vals)) return;
      // Fade from red to yellow
      for (g = 0; g < 256; g++) {
        analogWrite(GREENPIN, g);
        delay(FADESPEED);
      }

      if (getInstr(vals)) return;
      // Fade from yellow to green
      for (r = 255; r > 0; r--) {
        analogWrite(REDPIN, r);
        delay(FADESPEED);
      }

      if (getInstr(vals)) return;
      // Fade from green to teal
      for (b = 0; b < 256; b++) {
        analogWrite(BLUEPIN, b);
        delay(FADESPEED);
      }

      if (getInstr(vals)) return;
      // Fade from teal to blue
      for (g = 255; g > 0; g--) {
        analogWrite(GREENPIN, g);
        delay(FADESPEED);
      }
      break;
  }
}

int getInstr(int* input) {
  if (Serial.available() == 0) return 0;

  char buffer[4];
  for (uint8_t i = 0; i < 8; i++) {
    char peek_val = Serial.peek();
    Serial.print("Peeked: ");
    Serial.println(peek_val);
    uint8_t num_read = Serial.readBytesUntil(',', buffer, 4);

    Serial.print("Read ");
    Serial.print(num_read);
    Serial.print(", Buffer: ");
    Serial.println(buffer);

    num_read = Serial.readBytes(buffer, 5);
    Serial.print("Read again ");
    Serial.println(num_read);
  }

  return 1;

  Serial.print("Input given: ");
  for (int n = 0; n < 8; n++) {
    input[n] = Serial.parseInt();
    Serial.print(", ");
    Serial.print(input[n]);
  }
  Serial.println();

  /*
      if (Serial.read() == '\n') {
          Serial.print("1 ");
          Serial.println(num_avail);
          return 1;
      }
      */
}
