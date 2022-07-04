#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/UInt16MultiArray.h>

//루프문에 불켜는걸 콜백에다가 넣어보자 안되면
//문자열 데이터 
#include <std_msgs/String.h>
//#include <std_msgs/Byte.h>


#define rePublishTime 1000 // (ms)
//초기 스위치 값

//버튼핀은 define으로 핀 번호를 설정한 뒤에  boolean newState2 = digitalRead(BUTTON_PIN2);으로 값을 받아오자
//ㄴㄴ 위에처럼 하지 말자
//수동 스위치 개념 버리기 버튼 핀을 실제 버튼으로 입력받을 필요가 없음

#define LED_PIN6    6
#define LED_PIN7    7

// WS2812B 의 연결된 개수
#define NUMPIXELS  144 // 576 //461

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 5 // Set BRIGHTNESS to about 1/5 (max = 255)


#ifdef AVR
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


Adafruit_NeoPixel strip6(NUMPIXELS, LED_PIN6, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip7(NUMPIXELS, LED_PIN7, NEO_GRBW + NEO_KHZ800);
// pixels.Color(a, b, c, d) = pixels.Color(Red, Green, Blue, White)
// Red(빨강), Green(초록), Blue(파랑), White(하양) 의 색상을 각 255단계로 제어

  const int init_switch = 0;
  boolean newState2 = init_switch;
  boolean newState3 = init_switch;
  boolean newState4 = init_switch;
//  boolean newState5 = init_switch; //를 쓰려면 아마 uint32를 써야할거 같은 느낌

//  int ledPin = 13; 굳이 정의 안해도 아두이노에서 정해져 있는듯.
  
  //인풋 스위치 변수 지정
  boolean oldState2 = HIGH; //풀업 인풋은 HIGH가 끊겨있는거임
  boolean oldState3 = HIGH;
  boolean oldState4 = HIGH;
//  boolean oldState5 = HIGH;


    uint32_t b = strip6.Color(0,0,0,255);
  uint32_t c = strip6.Color(255,0,0,0);
  uint32_t d = strip6.Color(0,0,0,0);
  uint32_t e = strip6.Color(0,255,0,0);
  uint32_t f = strip6.Color(0,0,255,0);
//  int inputcmd[3] = { newState2, newState3, newState4 };
  int sum = 0;


//ros 노드 핸들 
ros::NodeHandle  nh;

/*
 * ROS 콜백 함수 
 * 스위치 데이터 받기 / 이게 문자열로 명령을 못 받을 수도 있음
 * 그리고 서보 블로그 참고 예제에서는 servo함수 자체에 포함돼서 그런지 
 * 위에서 미리 선언하지 않은 변수를 바로 콜백함수에서 사용함
 * cmd를 switch로 바꿔주자
 */

//a_led방식으로 트라이
//void switch_cb( const std_msgs::Byte& switch_msg){
//  int i;
//
//  for (i=0; i<3; i++)
//  {
//    inputcmd[i]   = switch_msg.data[i];
//    digitalWrite(13, HIGH-digitalRead(13)); 
//  } 
//}

void switch_cb( const std_msgs::UInt16MultiArray& switch_msg){

    newState2   = switch_msg.data[0];
    newState3   = switch_msg.data[1];
    newState4   = switch_msg.data[2];
//  newState5   = switch_msg.data[3];
    digitalWrite(13, HIGH-digitalRead(13));
    
    sum = newState2 + 2 * newState3 + 4 * newState4;



}



// Subscriber switch 메시지 입력과 콜백 함수와 연결 
ros::Subscriber<std_msgs::UInt16MultiArray> sub("switch", switch_cb);
//ros::Subscriber<std_msgs::Byte> sub("switch", switch_cb);

// Publisher echo_hello  생성 
std_msgs::String str_msg;
ros::Publisher pubTester("echo_hello", &str_msg);




/*
 * 초기 설정 
 */
void setup() {
  strip6.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip6.show();            // Turn OFF all pixels ASAP
  strip6.setBrightness(BRIGHTNESS);
  strip7.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip7.show();            // Turn OFF all pixels ASAP
  strip7.setBrightness(BRIGHTNESS);
//  Serial.begin(9600);


  // LED 핀 설정
  pinMode(13, OUTPUT);
  
  // 노드 설정
  nh.initNode();
  
  //Subscriber 설정 
  nh.subscribe(sub);

  //Publisher 설정   
  nh.advertise(pubTester);

  char str[]= "setup finish";
  nh.loginfo(str);

}

void loop() {
  char str[]= "in loop";
//  reload:
  nh.loginfo(str);
  //이전 시간 기억(ms)
  static unsigned long prevTime = 0;  
  //현재 시간 기억(ms)
  unsigned long currentTime;
  //ROS 콜백 함수 newstate에 명령이 입력된다.
//  nh.spinOnce();
  delay(1);
//Serial.println(sum);
  
  currentTime = millis();
  //publish 시간 체크
  if((currentTime - prevTime) >= rePublishTime)
  {
    str_msg.data = "Hello, This is Testing for ROS Publisher";
    //'hello' 전송 
    pubTester.publish(&str_msg);

    //현재 시간 이전 시간으로 기억
    prevTime = currentTime;
  }

  // STOP red 001
  if((newState2 == LOW) && (newState3 == LOW) && (newState4 == HIGH) ) {

    delay(20);
    strip6.fill(c,0,NUMPIXELS);
    strip6.show();
    strip7.fill(c,0,NUMPIXELS);
    strip7.show();
    }
    else if((newState2 == LOW) && (oldState2 == HIGH)){

    delay(20);

//    else if(newState2 == LOW){
//    pixels.fill(d,0,NUMPIXELS);
//    pixels.show(); 
//    }
  }

//  // 검정
//  if((newState2 == LOW) && (newState3 == LOW)&&(newState4 == LOW) && (newState5 == HIGH) ) {
//    
//
//    delay(20);
//
//    
//    pixels.fill(d,0,NUMPIXELS);
//    pixels.show(); 
//
//  }
//    else if((newState5 == LOW) && (oldState5 == HIGH)){
//    //GND(-극)로 연결이 돼있다가 연결이 끊기면 LED가 꺼진다
//    delay(20);
//    if(newState5 == LOW){
//    pixels.fill(d,0,NUMPIXELS);
//
//    }
//  }
  
  // move green 100
  if((newState2 == HIGH) && (newState3 == LOW)&&(newState4 == LOW) ) {
    

    

    
    whiteOverRainbow1(1, 72);
    delay(500);
//    goto reload;

  }
    else if((newState2 == LOW) && (oldState2 == HIGH)){
    //GND(-극)로 연결이 돼있다가 연결이 끊기면 LED가 꺼진다
    delay(20);
    strip6.fill(b,0,NUMPIXELS);
    strip6.show();
    strip7.fill(b,0,NUMPIXELS);
    strip7.show();

    }
  
  //초록 후진 010
  if((newState2 == HIGH)&& (newState3 == HIGH)&&(newState4 == LOW)) {
    // Short delay to debounce button. HZ to GND
    delay(20);
    whiteOverRainbow2(1, 72);

  }
    else if((newState2 == LOW) && (oldState2 == HIGH)){
    //GND(-극)로 연결이 돼있다가 연결이 끊기면 LED가 꺼진다
    delay(20);
    strip6.fill(b,0,NUMPIXELS);
    strip6.show();
    strip7.fill(b,0,NUMPIXELS);
    strip7.show();

    }
  
  
  //end to end 000 
  if((newState4 == LOW)&& (newState2 == LOW)&&(newState3 == LOW) ) {
    
  
    delay(20);
    

  }
    else if((newState2 == LOW) && (oldState2 == HIGH)){
    //GND(-극)로 연결이 돼있다가 연결이 끊기면 LED가 꺼진다
    delay(20);
    strip6.fill(b,0,NUMPIXELS);
    strip6.show();
    strip7.fill(b,0,NUMPIXELS);
    strip7.show();

    }
  
  
  // right turn signal lights 011
  if((newState2 == LOW) &&(newState3 == HIGH) &&(newState4 == HIGH) )
  {

    delay(20);
    strip6.fill(e,0,NUMPIXELS);
    strip7.fill(d,0,NUMPIXELS);
    strip6.show();
    strip7.show();
    delay(500);
    strip6.fill(d,0,NUMPIXELS);
    strip7.fill(d,0,NUMPIXELS);
    strip6.show();
    strip7.show();

    }
    
  // PIN2와 PIN4 동시에 ON 빨강 왔따갔다
  if((newState2 == HIGH) &&(newState3 == LOW) &&(newState4 == HIGH) )
  {
    // Short delay to debounce button. HZ to GND
    delay(20);


    whiteOverRainbow4(1, 72);

  }

    


  // left turn signal lights 010
  if((newState2 == LOW) &&(newState3 == HIGH) &&(newState4 == LOW) )
  {

    delay(20);
    strip6.fill(d,0,NUMPIXELS);
    strip7.fill(e,0,NUMPIXELS);
    strip6.show();
    strip7.show();
    delay(500);
    strip6.fill(d,0,NUMPIXELS);
    strip7.fill(d,0,NUMPIXELS);
    strip6.show();
    strip7.show();

    }

  
    
  // PIN2와 PIN3 PIN4 동시에 ON
  if((newState2 == HIGH) &&(newState3 == HIGH) &&(newState4 == HIGH) )
  {
    // Short delay to debounce button. HZ to GND
    delay(20);

    rainbowFade2White(0.0001, 60, 0); //(wait, rainbow period, white period) 


    }

  
  // Set the last-read button state to the old state.
  oldState2 = newState2;
  oldState3 = newState3;
  oldState4 = newState4;  
//  oldState5 = newState5; 

  delay(300); // 깜빡이는 주기
  char str1[]= "end loop";
  nh.loginfo(str1);
//
//  sum_msg.data = sum;
//  pub_sum.publish(&sum_msg);
  nh.spinOnce();
//  pre_time = millis();

}


void whiteOverRainbow1(int whiteSpeed, int whiteLength) {
  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 10000;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;
  
//
//char str3[]= "in function";
//  nh.loginfo(str3);

//예상되기론 이 부분에서 많이 호출 당한다
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;

//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  while (sum==1) { // Repeat forever (or until a 'break' or 'return')
    absss();
    char str4[]= "in while1";
    nh.loginfo(str4);
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

//    
//    reload:
    for(int i=0; i<strip6.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        loops = absss1();
absss();absss();absss();

        //알맹이 색깔 설정
        strip6.setPixelColor(i, strip6.Color(0, 255, 0, 0)); // Set white

        strip7.setPixelColor(i, strip6.Color(0, 255, 0, 0)); // Set white

      } else {   // else set rainbow

      absss();absss();absss();
        //알맹이 이외 부분 색깔 설정
        int pixelHue = firstPixelHue + (i * 65536L / strip6.numPixels());
        strip6.setPixelColor(i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        strip7.setPixelColor(i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        
//        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
      }
    }
//    
////이 부분이 생각처럼 동작하지 않음
//    nh.spinOnce();
//      if (sum != 1){
//    return;
//  }
absss();absss();absss();
    strip6.show(); // Update strip with new contents

    strip7.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.
//    loops = absss1();

//sum이 1이 아니면 멈춘다
if (sum !=1){return;}
    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
        head = 0;
        absss();absss();absss();

        if(--loopNum >= loops) return; //원래대로 ++로 하면 루프 넘버만큼 돌다가 리턴한다
        
      }
      if(++tail >= strip6.numPixels()) {      // Advance tail, wrap around
        tail = 0;
        absss();absss();absss();
      }
      lastTime = millis;// Save time of last movement
      absss();absss();absss();
    }
  }


}

void whiteOverRainbow2(int whiteSpeed, int whiteLength) {
  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 10000;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;
  
//
//char str3[]= "in function";
//  nh.loginfo(str3);

//예상되기론 이 부분에서 많이 호출 당한다
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();




  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;

//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

  while (sum==2) { // Repeat forever (or until a 'break' or 'return')
    absss();
    char str8[]= "in backing";
    nh.loginfo(str8);
    absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();


    for(int i=strip6.numPixels(); i>-1; i--) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        loops = absss1();
absss();absss();absss();

        //알맹이 색깔 설정
        strip6.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white

        strip7.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white

      } else {   // else set rainbow

      absss();absss();absss();
        //알맹이 이외 부분 색깔 설정
        int pixelHue = firstPixelHue + ((144-i) * 65536L / strip6.numPixels());
        strip6.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        strip7.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        
//        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
      }
    }

absss();absss();absss();
    strip6.show(); // Update strip with new contents

    strip7.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.
//    loops = absss1();

//sum이 2이 아니면 멈춘다
if (sum !=2){return;}
    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
        head = 0;
        absss();absss();absss();

        if(--loopNum >= loops) return;
        
      }
      if(++tail >= strip6.numPixels()) {      // Advance tail, wrap around
        tail = 0;
        absss();absss();absss();
      }
      lastTime = millis();// Save time of last movement
      absss();absss();absss();
    }
  }


}

//void whiteOverRainbow2(int whiteSpeed, int whiteLength) {
//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 100;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
//
//  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;
//
////  int      head          = whiteLength - 1;
////  int      tail          = 0;
////  int      loops         = 3;
////  int      loopNum       = 0;
////  uint32_t lastTime      = millis();
////  uint32_t firstPixelHue = 0;
//
//  while (sum==2) { // Repeat forever (or until a 'break' or 'return')
//    char str7[]= "in while2";
//    nh.loginfo(str7);
//    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    
//    for(int i=strip6.numPixels(); i>-1; i--) {  // For each pixel in strip...
//      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
//         ((tail > head) && ((i >= tail) || (i <= head)))) {
//
//        
//        //알맹이 색깔 설정
//        strip6.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white
//      } else {                                             // else set rainbow
//
//        
//        //알맹이 이외 부분 색깔 설정
//        int pixelHue = firstPixelHue + ((144-i) * 65536L / strip6.numPixels());
//        strip6.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
////        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
////        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//      }
//    }
//
//    strip6.show(); // Update strip with new contents
//    strip7.show(); // Update strip with new contents
//    // There's no delay here, it just runs full-tilt until the timer and
//    // counter combination below runs out.
//  
//    firstPixelHue += 40; // Advance just a little along the color wheel
//
//    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
//      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
//        head = 0;
//        
//        if(--loopNum >= loops) return; //이거 ++ 바꾸니까 역방향이 자연스러워짐
//      }
//      if(++tail >= strip6.numPixels()) {      // Advance tail, wrap around
//        tail = 0;
//      }
//      lastTime = millis();                   // Save time of last movement
//    }
//  }
//}
//드르륵
void whiteOverRainbow3(int whiteSpeed, int whiteLength) {
  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 10000;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;
  
//
//char str3[]= "in function";
//  nh.loginfo(str3);

//예상되기론 이 부분에서 많이 호출 당한다
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();





  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;

//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
absss();

  while (sum==4) { // Repeat forever (or until a 'break' or 'return')
    absss();
    char str8[]= "in while4";
    nh.loginfo(str8);
    absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();


//    
//    reload:
    for(int i=0; i<strip6.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        loops = absss1();
absss();

        //알맹이 색깔 설정
        strip6.setPixelColor(i, strip6.Color(0, 255, 0, 0)); // Set white

        strip7.setPixelColor(i, strip6.Color(0, 255, 0, 0)); // Set white

      } else {   // else set rainbow

      absss();
        //알맹이 이외 부분 색깔 설정
        int pixelHue = firstPixelHue + (i * 65536L / strip6.numPixels());
        strip6.setPixelColor(i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();
        strip7.setPixelColor(i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();
        
//        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
      }
    }
//    
////이 부분이 생각처럼 동작하지 않음
//    nh.spinOnce();
//      if (sum != 1){
//    return;
//  }
absss();
    strip6.show(); // Update strip with new contents

    strip7.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.
//    loops = absss1();

//sum이 1이 아니면 멈춘다
if (sum !=4){return;}
    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
        head = 0;
        absss();

        if(--loopNum >= loops) return; //원래대로 ++로 하면 루프 넘버만큼 돌다가 리턴한다
        
      }
      if(--tail >= strip6.numPixels()) {      // Advance tail, wrap around
        tail = 0;
        absss();
      }
      lastTime = millis();// Save time of last movement
      absss();
    }
  }


}

//void whiteOverRainbow3(int whiteSpeed, int whiteLength) {
//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
//
//  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;
//
////  int      head          = whiteLength - 1;
////  int      tail          = 0;
////  int      loops         = 3;
////  int      loopNum       = 0;
////  uint32_t lastTime      = millis();
////  uint32_t firstPixelHue = 0;
//
//  for(;;) { // Repeat forever (or until a 'break' or 'return')
//    for(int i=strip6.numPixels(); i>-1; i--) {  // For each pixel in strip...
//      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
//         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        
//        //알맹이 색깔 설정
//        strip6.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(0, 255, 0, 0)); // Set white
//      } else {                                             // else set rainbow
//
//        //알맹이 이외 부분 색깔 설정
//        int pixelHue = firstPixelHue + ((144-i) * 65536L / strip6.numPixels());
//        strip6.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
////        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
////        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//      }
//    }
//
//    strip6.show(); // Update strip with new contents
//    strip7.show(); // Update strip with new contents
//    // There's no delay here, it just runs full-tilt until the timer and
//    // counter combination below runs out.
//
//    firstPixelHue += 40; // Advance just a little along the color wheel
//
//    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
//      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
//        head = 0;
//        if(--loopNum >= loops) return;
//      }
//      if(--tail >= strip6.numPixels()) {      // Advance tail, wrap around
//        tail = 0;
//      }
//      lastTime = millis();                   // Save time of last movement
//    }
//  }
//}
// 101
void whiteOverRainbow4(int whiteSpeed, int whiteLength) {
  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 10000;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;
  
//
//char str3[]= "in function";
//  nh.loginfo(str3);

//예상되기론 이 부분에서 많이 호출 당한다
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();





  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;

//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

  while (sum==5) { // Repeat forever (or until a 'break' or 'return')
    absss();
    char str9[]= "wari";
    nh.loginfo(str9);
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
//    loops = absss1();

  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();

    for(int i=strip6.numPixels(); i>-1; i--) {  // For each pixel in strip...
      if(((144-i >= tail) && (144-i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        loops = absss1();
absss();absss();absss();

        //알맹이 색깔 설정
        strip6.setPixelColor(144-i, strip6.Color(255, 255, 0, 0)); // Set white

        strip7.setPixelColor(144-i, strip6.Color(255, 255, 0, 0)); // Set white

      } else {   // else set rainbow

      absss();absss();absss();
        //알맹이 이외 부분 색깔 설정
        int pixelHue = firstPixelHue + (144-i * 65536L / strip6.numPixels());
        strip6.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        strip7.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
        absss();absss();absss();
        
//        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
      }
    }

absss();absss();absss();
    strip6.show(); // Update strip with new contents

    strip7.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.
//    loops = absss1();

//sum이 1이 아니면 멈춘다
if (sum !=5){return;}
    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
        head = 0;
        absss();absss();absss();

        if(--loopNum >= loops) return; //원래대로 ++로 하면 루프 넘버만큼 돌다가 리턴한다
        
      }
      if(++tail >= strip6.numPixels()) {      // Advance tail, wrap around
        tail = 0;
        absss();absss();absss();
      }
      lastTime = millis(); // Save time of last movement
      absss();absss();absss();
    }
  }


}

//void whiteOverRainbow4(int whiteSpeed, int whiteLength) {
//  int      head          = whiteLength - 1;
//  int      tail          = 0;
//  int      loops         = 3;
//  int      loopNum       = 0;
//  uint32_t lastTime      = millis();
//  uint32_t firstPixelHue = 0;
//
//  if(whiteLength >= strip6.numPixels()) whiteLength = strip6.numPixels() - 1;
//
////  int      head          = whiteLength - 1;
////  int      tail          = 0;
////  int      loops         = 3;
////  int      loopNum       = 0;
////  uint32_t lastTime      = millis();
////  uint32_t firstPixelHue = 0;
//
//  for(;;) { // Repeat forever (or until a 'break' or 'return')
//    for(int i=strip6.numPixels(); i>-1; i--) {  // For each pixel in strip...
//      if(((144-i >= tail) && (144-i <= head)) ||      //  If between head & tail...
//         ((tail > head) && ((i >= tail) || (i <= head)))) {
//        
//        //알맹이 색깔 설정
//        strip6.setPixelColor(144-i, strip6.Color(255, 0, 0, 0)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(255, 0, 0, 0)); // Set white
//      } else {                                             // else set rainbow
//
//        //알맹이 이외 부분 색깔 설정
//        int pixelHue = firstPixelHue + ((144-i) * 65536L / strip6.numPixels());
//        strip6.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
//        strip7.setPixelColor(144-i, strip6.Color(0, 0, 0, 255)); // Set white
////        strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
////        strip7.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue)));
//      }
//    }
//
//    strip6.show(); // Update strip with new contents
//    strip7.show(); // Update strip with new contents
//    // There's no delay here, it just runs full-tilt until the timer and
//    // counter combination below runs out.
//
//    firstPixelHue += 40; // Advance just a little along the color wheel
//
//    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
//      if(++head >= strip6.numPixels()) {      // Advance head, wrap around
//        head = 0;
//        if(--loopNum >= loops) return;
//      }
//      if(++tail >= strip6.numPixels()) {      // Advance tail, wrap around
//        tail = 0;
//      }
//      lastTime = millis();                   // Save time of last movement
//    }
//  }
//}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=50, fadeMax=100;

absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {
absss();absss();
    for(int i=0; i<strip6.numPixels(); i++) { // For each pixel in strip...
absss();absss();
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip6.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.

      absss();absss();
      strip6.setPixelColor(i, strip6.gamma32(strip6.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
      strip7.setPixelColor(i, strip7.gamma32(strip7.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));

    }

    char str6[]= "in rainbow";
    nh.loginfo(str6);
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    
    if (sum != 7){
      return;
    }
    absss();absss();absss();absss();
    strip6.show();
    strip7.show();
//    delay(wait);

    if(firstPixelHue < 65536) { absss();absss();                             // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;  absss();absss();                           // fade out
    } else {
      fadeVal = fadeMax; absss();absss();// Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip6.fill(strip6.Color(0, 0, 0, strip6.gamma8(j)));
      strip7.fill(strip7.Color(0, 0, 0, strip7.gamma8(j)));
      absss();absss();absss();
      strip6.show();
      strip7.show();
      absss();absss();absss();
    }
    delay(10); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip6.fill(strip6.Color(0, 0, 0, strip6.gamma8(j)));
      strip7.fill(strip7.Color(0, 0, 0, strip6.gamma8(j)));
      absss();absss();absss();
      strip6.show();
      strip7.show();
      absss();absss();absss();
    }
  }
absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
    delay(10); // Pause 1/2 second
    absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();absss();
}

//확인결과 return값을 이용하지 않고 void만 사용해도 spinOnce를 잘 호출하며 sum값의 변화도 반영이 된다
//또한 return으로 처리했을 때 있었던 지연문제가 사라진다
//-> 얘를 많이 넣으면 될듯
void absss()
{nh.spinOnce();}

//
//int absss1()
//{
//  char str6[]= "in absss";
//    nh.loginfo(str6);
//  nh.spinOnce();
//  
//  if(sum!=1 )
//  
//  {
//
//return(0);
//
//    }
//    else{
//      return(10000);
//    }
//
//}
//
//
//
//int absss2()
//{
//  char str6[]= "in absss2";
//    nh.loginfo(str6);
//  nh.spinOnce();
//  
//  if(sum!=2 )
//  
//  {
//
//return(0);
//
//    }
//    else{
//      return(10000);
//    }
//
//}
//
//int absss7()
//{
//  char str6[]= "in absss7";
//    nh.loginfo(str6);
//  nh.spinOnce();
//  
//  if(sum!=7 )
//  
//  {
//
//return(0);
//
//    }
//    else{
//      return(10000);
//    }
//
//}
//
//int absss1()
//{
//  
//
//  return 0;
//}


//void theaterChaseRainbow(int wait) {
//  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
//  for(int a=0; a<30; a++) {  // Repeat 30 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      pixels.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in increments of 3...
//      for(int c=b; c<NUMPIXELS; c += 3) {
//        // hue of pixel 'c' is offset by an amount to make one full
//        // revolution of the color wheel (range 65536) along the length
//        // of the strip (strip.numPixels() steps):
//        int      hue   = firstPixelHue + c * 65536L / NUMPIXELS;
//        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
//        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      pixels.show();                // Update strip with new contents
//      delay(wait);                 // Pause for a moment
//      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
//    }
//  }
//}
