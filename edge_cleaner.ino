#include <SimpleTimer.h>

SimpleTimer timer;  // 타이머 레딧 셋 고!
unsigned long time; // 프로그램의 실행부터의 시간
unsigned long fir_time; 
unsigned long sec_time;

int relay_pump = 10; //릴레이에 5V 신호를 보낼 핀설정(펌프)
int relay_bel = 9; //릴레이에 5V 신호를 보낼 핀설정(벨브)
int relay_2 = 8;  //릴레이에 5V 신호를 보낼 핀설정(약품 벨브)

int relay_3 = 7;  //릴레이에 5V 신호를 보낼 핀설정(수도관 펌프)
int relay_4 = 6;  //건조기 모터 릴레이 220v
int relay_5 = 5;  //환풍기 팬 릴레이 12v 

int water_pin_1 = A5;      //수분수위센서 A5에 연결
int water_pin_2 = A4;      //센서 A4에 연결

//모터 핀모드
int moter_1 = 22;
int moter_2 = 23;
int moter_3 = 24;
int moter_4 = 25;
int moter_5 = 26;
int moter_6 = 27;

//스위치 핀설정
int sw1 = 11;
int sw2 = 12;
int sw3 = 13;
int sw4 = 4;      //원터치
int sw5 = 14;

int sw_state_1;           // 스위치1 상태
int sw_state_2;           // 스위치2 상태
int relay_state = 0;
int state_bel = 1;
int state_one = 0;
long debounce = 100;  // Debounce 타임 설정

void setup() {
  Serial.begin(9600); 

  pinMode (relay_pump, OUTPUT); // relay를 output으로 설정한다.
  pinMode (relay_bel, OUTPUT);
  pinMode (relay_2, OUTPUT);
  pinMode (relay_3, OUTPUT);
  pinMode (relay_4, OUTPUT);
  pinMode (relay_5, OUTPUT);

  pinMode (moter_1,OUTPUT); //모터 아웃풋으로 설정
  pinMode (moter_2,OUTPUT);
  pinMode (moter_4,OUTPUT);
  pinMode (moter_5,OUTPUT);
  
  //수위 센서 input으로 설정
  pinMode(water_pin_1 , INPUT);
  pinMode(water_pin_2 , INPUT);
  
  digitalWrite(relay_pump, HIGH);
  digitalWrite(relay_bel, HIGH);
  digitalWrite(relay_5, HIGH);
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_3, HIGH);
  digitalWrite(relay_2, HIGH);

  pinMode(sw1,INPUT_PULLUP);
  pinMode(sw2,INPUT_PULLUP);
  pinMode(sw3,INPUT_PULLUP);
  pinMode(sw4,INPUT_PULLUP);
  pinMode(sw5,INPUT_PULLUP);

}
 
void loop() {

  fir_time = millis();
  sec_time = fir_time;
  timer.run();
  
  int val_1 = analogRead(A5);   // 수분수위센서값을 'val'에 저장 윗
  int val_2 = analogRead(A4);   // 아래
  Serial.print("val 1 = ");
  Serial.print(val_1);
  Serial.print("   val 2 = ");
  Serial.println(val_2);

  
  if(val_1 > 100)
    state_bel = 1;
  else
    state_bel = 0;
  
  sw_state_1 = digitalRead(sw1);
  sw_state_2 = digitalRead(sw2);
  
  
  
  if( val_1 < 100 && val_2 < 500)// 물탱크에 물이 없을 경우 밸브 오픈 + 제2펌프 작동
  {
    JongGeun_Choi();
    JunBeom_Sim();
    state_bel = 0;
  }
  
  else if(val_1 < 100 && val_2 > 500 && state_bel == 0) // 밸브 오픈 후 val_1가 100이 넘을 때까지 물 공급
  {
    JongGeun_Choi();
    JunBeom_Sim();
  }

  else if(val_1 < 100 && val_2 > 500 && state_bel == 1) // 밸브 차단 후 val_2가 500이 이하가 될 때까지 물 차단
  {
    NaHyeon_Park();
    HyeBin_Kwon();
  }

  else if(val_1 > 100 && val_2 > 500) // 물탱크에 충분히 물이 차 있을 경우 밸브 차단 + 제2펌프 종료
  {
    NaHyeon_Park();
    HyeBin_Kwon();
    state_bel = 1;
  }
  
  
  if(digitalRead(sw2)== HIGH && digitalRead(sw1) == LOW && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce) {

    digitalWrite(relay_4, HIGH);

    delay(2000);
    digitalWrite(relay_pump, LOW);
    
    JinKi_Park(); //펌프 종료
    
    time = millis();
    
  }

  else if(digitalRead(sw2)== LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce) {
    
    JeongMin_Kim();
  } 
    
  else if(digitalRead(sw2)== HIGH && digitalRead(sw1) == HIGH && digitalRead(sw3) == LOW && digitalRead(sw4) == HIGH && millis() - time > debounce) {

    digitalWrite(relay_pump, HIGH);
    delay(2000);
    digitalWrite(relay_4, LOW);
    digitalWrite(relay_5, LOW);   //건조가 실행되면 환풍기 같이 작동
    
    ChungSin_Oh(); // 건조 종료
    
    time = millis();
  }

  else if(digitalRead(sw4) == LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw2) == HIGH && millis() - time > debounce){       //원터치

    digitalWrite(relay_4, HIGH);
    digitalWrite(relay_5, HIGH);
    delay(2000);
    digitalWrite(relay_pump, LOW);
    
    WonGyu_Sim();
    
    time = millis();
  }
}

void JinKi_Park()                 //펌프 꺼지는 함수
{
  NaHyeon_Park();
  HyeBin_Kwon();
  
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_5, HIGH);
  sec_time = fir_time;
  
  while(1){
    fir_time = millis();

    if(digitalRead(sw2)== LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce){
      
      JeongMin_Kim();
      break;
    }
    
    if(fir_time - sec_time >= 15000)
    {
      digitalWrite(relay_pump, HIGH);
      break;
    }

    if(digitalRead(sw2)== HIGH && digitalRead(sw1) == HIGH && digitalRead(sw3) == LOW && digitalRead(sw4) == HIGH && millis() - time > debounce) {

      digitalWrite(relay_pump, HIGH);
      delay(2000);
      digitalWrite(relay_4, LOW);
      digitalWrite(relay_5, LOW);
      
      ChungSin_Oh(); // 건조 종료
      
      time = millis();
      break;
    }

    else if(digitalRead(sw4) == LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw2) == HIGH && millis() - time > debounce){       //원터치

      digitalWrite(relay_4, HIGH);
      digitalWrite(relay_5, HIGH);

      delay(2000);
      
      digitalWrite(relay_pump, LOW);
      
      WonGyu_Sim();
      
      time = millis();
    }
  }
}

void ChungSin_Oh()                //모터 꺼지는 함수
{
  NaHyeon_Park();
  HyeBin_Kwon();
  digitalWrite(relay_pump, HIGH);
  sec_time = fir_time;
  
  while(1){
    fir_time = millis();

    if(digitalRead(sw2)== LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce){
      
      JeongMin_Kim();
      break;
    }
    
    if(fir_time - sec_time >= 10000)
    {
      digitalWrite(relay_4, HIGH);
    }

    if(fir_time - sec_time >= 20000)
    {
      digitalWrite(relay_5, HIGH);
      break;
    }
    
    if(digitalRead(sw2)== HIGH && digitalRead(sw1) == LOW && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce) {

      digitalWrite(relay_4, HIGH);
      delay(2000);
      digitalWrite(relay_pump, LOW);
      
      JinKi_Park();
      
      time = millis();
      break;
    }

    else if(digitalRead(sw4) == LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw2) == HIGH && millis() - time > debounce){       //원터치

      digitalWrite(relay_4, HIGH);
      digitalWrite(relay_5, HIGH);
      delay(2000);
      digitalWrite(relay_pump, LOW);
      
      WonGyu_Sim();
      
      time = millis();
    }
  }
}

void WonGyu_Sim()                 //원터치 함수
{
  NaHyeon_Park();
  HyeBin_Kwon();
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_5, HIGH);

  sec_time = fir_time;

  while(1)
  {
    fir_time = millis();

    if(fir_time - sec_time >= 15000 && state_one == 0)
    {
      digitalWrite(relay_pump, HIGH);
    }

    if(fir_time - sec_time >= 17000 && state_one == 0)
    {
      digitalWrite(relay_4, LOW);
      digitalWrite(relay_5, LOW);
      state_one = 1;
      sec_time = millis();
    }

    if(fir_time - sec_time >= 10000 && state_one == 1)
    {
      digitalWrite(relay_4, HIGH);
    }

    if(fir_time - sec_time >= 20000 && state_one == 1)
    {
      digitalWrite(relay_5, HIGH);
      state_one = 0;
      break;
    }
    
    if(digitalRead(sw2)== HIGH && digitalRead(sw1) == LOW && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce) {
        
      digitalWrite(relay_4, HIGH);
      delay(2000);
      digitalWrite(relay_pump, LOW);
      
      JinKi_Park();
      
      time = millis();
      break;
    }

    if(digitalRead(sw2)== HIGH && digitalRead(sw1) == HIGH && digitalRead(sw3) == LOW && digitalRead(sw4) == HIGH && millis() - time > debounce) {

      digitalWrite(relay_pump, HIGH);
      delay(2000);
      digitalWrite(relay_4, LOW);
      digitalWrite(relay_5, LOW);
      
      ChungSin_Oh(); // 건조 종료
      
      time = millis();
      break;
    }

    if(digitalRead(sw2)== LOW && digitalRead(sw1) == HIGH && digitalRead(sw3) == HIGH && digitalRead(sw4) == HIGH && millis() - time > debounce){
      
      JeongMin_Kim();
      break;
    }
  }
}

void JeongMin_Kim()               //종료버튼
{
  digitalWrite(relay_pump, HIGH);
  digitalWrite(relay_4, HIGH);
  digitalWrite(relay_5, HIGH);

  time = millis();
}

void JongGeun_Choi()              //벨브 오픈
{
  digitalWrite(relay_bel, LOW);
  //Serial.println("open");
}

void NaHyeon_Park()                //벨브 클로즈
{
  digitalWrite(relay_bel, HIGH);
  //Serial.println("close");
}

void JunBeom_Sim()                //제 2펌프 작동.
{
  digitalWrite(relay_3, LOW);
  relay_state = 1;
}

void HyeBin_Kwon()                //제2 펌프 종료
{
  digitalWrite(relay_3, HIGH);
  relay_state = 0;
}
