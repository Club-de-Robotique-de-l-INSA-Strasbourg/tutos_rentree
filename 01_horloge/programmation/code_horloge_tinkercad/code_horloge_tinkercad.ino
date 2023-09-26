//a,b,c,d,e,f,g 상태값
byte segValue[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, //0
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4
  {1, 0, 1, 1, 0, 1, 1}, //5
  {1, 0, 1, 1, 1, 1, 1}, //6
  {1, 1, 1, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 0, 1, 1} //9
};

byte segPin[8] = {3, 4, 5, 6, 7, 8, 9, 10}; //사용핀{a,b,c,d,e,f,g,dp} 순서대로임
byte digitPin[4] = {A0, A1, A2, A3}; //segment 위치 핀

const byte interruptPin = 2;//인터럽트핀

boolean state = false;//시간 출력형식 지정
extern volatile unsigned long timer0_millis; //타이머변수
unsigned long readTime; //현재타이머시간
int hour, min, sec;

void setup() {
  Serial.begin(9600);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), switchFn, FALLING);

  for (int i = 0; i < 8; i++) {
    pinMode(segPin[i], OUTPUT);
  }
  for (int j = 0; j < 4; j++) {
    pinMode(digitPin[j], OUTPUT);
    digitalWrite(digitPin[j], HIGH);
  }
}

void loop() {
  if (Serial.available()) {
    String inString = Serial.readStringUntil('\n');
    int index1 = inString.indexOf(':');
    int index2 = inString.indexOf(':', index1 + 1);
    int index3 = inString.length();

    hour = inString.substring(0, index1).toInt();
    min = inString.substring(index1 + 1, index2).toInt();
    sec = inString.substring(index2 + 1, index3).toInt();

    timer0_millis = ((long)hour * 3600 + min * 60 + sec) * 1000;
  }
  readTime = millis() / 1000;
  if (millis() >= 86400000) {
    timer0_millis = 0;
  }
  sec = readTime % 60;
  min = (readTime / 60) % 60;
  hour = (readTime / (60 * 60)) % 24;

  if (state == true) { //12시 or 24시 출력모드
    hour = hour % 12;
  }
  segOutput(3, min % 10, 0); //min 1의 자리
  segOutput(2, min / 10, 0); //min 10의 자리
  segOutput(1, hour % 10, 1); //hour 1의 자리
  segOutput(0, hour / 10, 0); //hour 10의 자리
}

//12시 or 24시 출력 변경
void switchFn() {
  state = !state;
}

//LED 초기화
void segClear() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(segPin[i], LOW);
  }
}
//LED 출력
void segOutput(int d, int Number, int dp) {
  segClear();
  digitalWrite(digitPin[d], LOW);
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPin[i], segValue[Number][i]);
  }
  digitalWrite(segPin[7], dp);
  delayMicroseconds(1000);
  digitalWrite(digitPin[d], HIGH);
}
