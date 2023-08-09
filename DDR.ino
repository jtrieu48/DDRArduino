#include <LedControl.h>
#include <time.h>
#include <LiquidCrystal.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>


//Global Variables

//Button Inputs
int l = A0;
int u = A1;
int d = A2;
int r = A3;
int s = A4;


//LED Arrow Positions
int Lrow1 = 7;
int Lcol1 = 0;
int Lrow2 = 7;
int Lcol2 = 1;
int Lrow3 = 6;
int Lcol3 = 1;

int Urow1 = 7;
int Ucol1 = 2;
int Urow2 = 6;
int Ucol2 = 2;
int Urow3 = 6;
int Ucol3 = 3;

int Drow1 = 6;
int Dcol1 = 4;
int Drow2 = 6;
int Dcol2 = 5;
int Drow3 = 7;
int Dcol3 = 5;

int Rrow1 = 6;
int Rcol1 = 6;
int Rrow2 = 7;
int Rcol2 = 6;
int Rrow3 = 7;
int Rcol3 = 7;

//Player Streak
int score;
int miss;
long randResult;
long randResult2;
int delays;

//MAX7219 Ports & Initialization 
int DIN = 12;
int CS = 11;
int CLK = 10;
LedControl lc = LedControl(DIN, CLK, CS, 0);

//int Led_array[8][56] = {
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//{0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
//};
//
//
//void DDRScrollLEFT(int speed)
//{
//  int row1 = 7;
//  int col1 = 0;
//
//  int row2 = 7;
//  int col2 = 1;
//
//  int row3 = 6;
//  int col3 = 1;
//
//  for(int i=0; i<9; i++)
//  {
//    lc.setLed(0,row1,col1,true);
//    lc.setLed(0,row2,col2,true);
//    lc.setLed(0,row3,col3,true);
//
//    delay(speed);
//
//    lc.setLed(0,row1,col1,false);
//    lc.setLed(0,row2,col2,false);
//    lc.setLed(0,row3,col3,false);
//
//    row1--;
//    row2--;
//    row3--;
//  }
//}


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
} task;

const unsigned short tasksNum = 2;
task tasks[tasksNum];

//Player
enum P_states {P_START, 
               P_LEFT1, P_LEFT2, P_LEFT3, P_LEFT4, P_LEFT5, P_LEFT6, P_LEFT7, P_LEFT8, 
               P_UP1, P_UP2, P_UP3, P_UP4, P_UP5, P_UP6, P_UP7, P_UP8,
               P_DOWN1, P_DOWN2, P_DOWN3, P_DOWN4, P_DOWN5, P_DOWN6, P_DOWN7, P_DOWN8,
               P_RIGHT1, P_RIGHT2, P_RIGHT3, P_RIGHT4, P_RIGHT5, P_RIGHT6, P_RIGHT7, P_RIGHT8,
               P_HIT, P_MISS, P_STOP} p_state = P_START;
int TickFct_Player(int state){


  //STATE TRANSITIONS
  switch(p_state){

      //GAME START
      case P_START:

      //Randomly generate an arrow direction 
      lc.clearDisplay(0);
      
      randResult = random(4);
      randResult2 = random(1,3);
      delays = randResult2 * 100;
      
      Serial.println(randResult);

      Serial.print("Score = ");
      Serial.print(score);
      Serial.println();

      Serial.print("Miss = ");
      Serial.print(miss);
      Serial.println();

      if (miss > 2)
        p_state = P_STOP;
      //Send to arrow state
      else if(randResult == 0)
        p_state = P_LEFT1;
      else if (randResult == 1)
        p_state = P_UP1;
      else if (randResult == 2)
        p_state = P_DOWN1;
      else if (randResult == 3)
        p_state = P_RIGHT1;
        break;


      //GAME END
      case P_STOP:
        if(digitalRead(s) == HIGH)
        {
          p_state = P_START;
        }
      break;
        

      //LEFT ARROW CYCLE
      case P_LEFT1:
        p_state = P_LEFT2;
        break;
      case P_LEFT2:
        p_state = P_LEFT3;
        break;
      case P_LEFT3:
        p_state = P_LEFT4;
        break;
      case P_LEFT4:
        p_state = P_LEFT5;
        break;
      case P_LEFT5:
        p_state = P_LEFT6;
        break;
      case P_LEFT6:
        p_state = P_LEFT7;
        break;
      case P_LEFT7:
        if(digitalRead(l) == HIGH)
        {
          score++;
          p_state = P_START;
        }
        else
          p_state = P_LEFT8;
        break;
      case P_LEFT8:
        p_state = P_START;
        break;

      //UP ARROW CYCLE
      case P_UP1:
        p_state = P_UP2;
        break;
      case P_UP2:
        p_state = P_UP3;
        break;
      case P_UP3:
        p_state = P_UP4;
        break;
      case P_UP4:
        p_state = P_UP5;
        break;
      case P_UP5:
        p_state = P_UP6;
        break;
      case P_UP6:
        p_state = P_UP7;
        break;
      case P_UP7:
        if(digitalRead(u) == HIGH)
        {
          score++;
          p_state = P_START;
        }
        else
          p_state = P_UP8;
        break;
      case P_UP8:
          p_state = P_START;
        break;

      //DOWN ARROW CYCLE
       case P_DOWN1:
        p_state = P_DOWN2;
        break;
      case P_DOWN2:
        p_state = P_DOWN3;
        break;
      case P_DOWN3:
        p_state = P_DOWN4;
        break;
      case P_DOWN4:
        p_state = P_DOWN5;
        break;
      case P_DOWN5:
        p_state = P_DOWN6;
        break;
      case P_DOWN6:
        p_state = P_DOWN7;
        break;
      case P_DOWN7:
        if(digitalRead(d) == HIGH)
        {
          score++;
          p_state = P_START;
        }
        else
          p_state = P_DOWN8;
        break;
      case P_DOWN8:
        p_state = P_START;
        break;

      //RIGHT ARROW CYCLE
       case P_RIGHT1:
        p_state = P_RIGHT2;
        break;
      case P_RIGHT2:
        p_state = P_RIGHT3;
        break;
      case P_RIGHT3:
        p_state = P_RIGHT4;
        break;
      case P_RIGHT4:
        p_state = P_RIGHT5;
        break;
      case P_RIGHT5:
        p_state = P_RIGHT6;
        break;
      case P_RIGHT6:
        p_state = P_RIGHT7;
        break;
      case P_RIGHT7:
        if(digitalRead(r) == HIGH)
          {
            score++;
            p_state = P_START;
          }
        else
          p_state = P_RIGHT8;
        break;
      case P_RIGHT8:
        p_state = P_START;
        //p_state = P_RIGHT1;
        break;

      
     
  }



  ///////////////////////////////STATE ACTIONS
  switch(p_state){
    case P_START:
      break;

    case P_STOP:
      //Reset Score & Miss Counter
      if (digitalRead(s) == HIGH)
      {
          miss = 0;
          score = 0;
      }

      //Left G
      lc.setLed(0,1,3,true);
      lc.setLed(0,1,2,true);
      lc.setLed(0,1,1,true);
      lc.setLed(0,1,0,true);
      lc.setLed(0,2,0,true);
      lc.setLed(0,3,0,true);
      lc.setLed(0,4,0,true);
      lc.setLed(0,5,0,true);
      lc.setLed(0,5,1,true);
      lc.setLed(0,5,2,true);
      lc.setLed(0,5,3,true);
      lc.setLed(0,4,3,true);
      lc.setLed(0,3,3,true);
      lc.setLed(0,3,2,true);

      //Right G
      lc.setLed(0,1,7,true);
      lc.setLed(0,1,6,true);
      lc.setLed(0,1,5,true);
      lc.setLed(0,1,4,true);
      lc.setLed(0,2,4,true);
      lc.setLed(0,3,4,true);
      lc.setLed(0,4,4,true);
      lc.setLed(0,5,4,true);
      lc.setLed(0,5,5,true);
      lc.setLed(0,5,6,true);
      lc.setLed(0,5,7,true);
      lc.setLed(0,4,7,true);
      lc.setLed(0,3,7,true);
      lc.setLed(0,3,6,true);      
      break;

    //LEFT ARROW CYCLE
    case P_LEFT1:
      lc.clearDisplay(0);
      lc.setLed(0,Lrow1-7,Lcol1,false);
      lc.setLed(0,Lrow2-7,Lcol2,false);
      lc.setLed(0,Lrow3-7,Lcol3,false);
    
      lc.setLed(0,Lrow1,Lcol1,true);
      lc.setLed(0,Lrow2,Lcol2,true);
      lc.setLed(0,Lrow3,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT2:
      lc.setLed(0,Lrow1,Lcol1,false);
      lc.setLed(0,Lrow2,Lcol2,false);
      lc.setLed(0,Lrow3,Lcol3,false);
    
      lc.setLed(0,Lrow1-1,Lcol1,true);
      lc.setLed(0,Lrow2-1,Lcol2,true);
      lc.setLed(0,Lrow3-1,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT3:
      lc.setLed(0,Lrow1-1,Lcol1,false);
      lc.setLed(0,Lrow2-1,Lcol2,false);
      lc.setLed(0,Lrow3-1,Lcol3,false);
    
      lc.setLed(0,Lrow1-2,Lcol1,true);
      lc.setLed(0,Lrow2-2,Lcol2,true);
      lc.setLed(0,Lrow3-2,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT4:
      lc.setLed(0,Lrow1-2,Lcol1,false);
      lc.setLed(0,Lrow2-2,Lcol2,false);
      lc.setLed(0,Lrow3-2,Lcol3,false);
    
      lc.setLed(0,Lrow1-3,Lcol1,true);
      lc.setLed(0,Lrow2-3,Lcol2,true);
      lc.setLed(0,Lrow3-3,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT5:
      lc.setLed(0,Lrow1-3,Lcol1,false);
      lc.setLed(0,Lrow2-3,Lcol2,false);
      lc.setLed(0,Lrow3-3,Lcol3,false);
    
      lc.setLed(0,Lrow1-4,Lcol1,true);
      lc.setLed(0,Lrow2-4,Lcol2,true);
      lc.setLed(0,Lrow3-4,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT6:
      lc.setLed(0,Lrow1-4,Lcol1,false);
      lc.setLed(0,Lrow2-4,Lcol2,false);
      lc.setLed(0,Lrow3-4,Lcol3,false);
    
      lc.setLed(0,Lrow1-5,Lcol1,true);
      lc.setLed(0,Lrow2-5,Lcol2,true);
      lc.setLed(0,Lrow3-5,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT7:
      lc.setLed(0,Lrow1-5,Lcol1,false);
      lc.setLed(0,Lrow2-5,Lcol2,false);
      lc.setLed(0,Lrow3-5,Lcol3,false);
    
      lc.setLed(0,Lrow1-6,Lcol1,true);
      lc.setLed(0,Lrow2-6,Lcol2,true);
      lc.setLed(0,Lrow3-6,Lcol3,true);
  
      delay(delays);
      break;
    case P_LEFT8:
      //Flash Columns on miss
      lc.clearDisplay(0);
      lc.setIntensity(0,1);
      lc.setColumn(0,1,B11111111);
      lc.setColumn(0,0,B11111111);
      lc.setIntensity(0,5);

      miss++;
  
      delay(200);
      break;


    //UP ARROW CYCLE
    case P_UP1:
      lc.clearDisplay(0);
      lc.setLed(0,Urow1-7,Ucol1,false);
      lc.setLed(0,Urow2-7,Ucol2,false);
      lc.setLed(0,Urow3-7,Ucol3,false);
    
      lc.setLed(0,Urow1,Ucol1,true);
      lc.setLed(0,Urow2,Ucol2,true);
      lc.setLed(0,Urow3,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP2:
      lc.setLed(0,Urow1,Ucol1,false);
      lc.setLed(0,Urow2,Ucol2,false);
      lc.setLed(0,Urow3,Ucol3,false);
    
      lc.setLed(0,Urow1-1,Ucol1,true);
      lc.setLed(0,Urow2-1,Ucol2,true);
      lc.setLed(0,Urow3-1,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP3:
      lc.setLed(0,Urow1-1,Ucol1,false);
      lc.setLed(0,Urow2-1,Ucol2,false);
      lc.setLed(0,Urow3-1,Ucol3,false);
    
      lc.setLed(0,Urow1-2,Ucol1,true);
      lc.setLed(0,Urow2-2,Ucol2,true);
      lc.setLed(0,Urow3-2,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP4:
      lc.setLed(0,Urow1-2,Ucol1,false);
      lc.setLed(0,Urow2-2,Ucol2,false);
      lc.setLed(0,Urow3-2,Ucol3,false);
    
      lc.setLed(0,Urow1-3,Ucol1,true);
      lc.setLed(0,Urow2-3,Ucol2,true);
      lc.setLed(0,Urow3-3,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP5:
      lc.setLed(0,Urow1-3,Ucol1,false);
      lc.setLed(0,Urow2-3,Ucol2,false);
      lc.setLed(0,Urow3-3,Ucol3,false);
    
      lc.setLed(0,Urow1-4,Ucol1,true);
      lc.setLed(0,Urow2-4,Ucol2,true);
      lc.setLed(0,Urow3-4,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP6:
      lc.setLed(0,Urow1-4,Ucol1,false);
      lc.setLed(0,Urow2-4,Ucol2,false);
      lc.setLed(0,Urow3-4,Ucol3,false);
    
      lc.setLed(0,Urow1-5,Ucol1,true);
      lc.setLed(0,Urow2-5,Ucol2,true);
      lc.setLed(0,Urow3-5,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP7:
      lc.setLed(0,Urow1-5,Ucol1,false);
      lc.setLed(0,Urow2-5,Ucol2,false);
      lc.setLed(0,Urow3-5,Ucol3,false);
    
      lc.setLed(0,Urow1-6,Ucol1,true);
      lc.setLed(0,Urow2-6,Ucol2,true);
      lc.setLed(0,Urow3-6,Ucol3,true);
  
      delay(delays);
      break;
    case P_UP8:
      lc.clearDisplay(0);
      lc.setIntensity(0,1);
      lc.setColumn(0,2,B11111111);
      lc.setColumn(0,3,B11111111);
      lc.setIntensity(0,5);

      miss++;
  
      delay(200);
      break;

      //DOWN ARROW CYCLE
    case P_DOWN1:
      lc.clearDisplay(0);
      lc.setLed(0,Drow1-7,Dcol1,false);
      lc.setLed(0,Drow2-7,Dcol2,false);
      lc.setLed(0,Drow3-7,Dcol3,false);
    
      lc.setLed(0,Drow1,Dcol1,true);
      lc.setLed(0,Drow2,Dcol2,true);
      lc.setLed(0,Drow3,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN2:
      lc.setLed(0,Drow1,Dcol1,false);
      lc.setLed(0,Drow2,Dcol2,false);
      lc.setLed(0,Drow3,Dcol3,false);
    
      lc.setLed(0,Drow1-1,Dcol1,true);
      lc.setLed(0,Drow2-1,Dcol2,true);
      lc.setLed(0,Drow3-1,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN3:
      lc.setLed(0,Drow1-1,Dcol1,false);
      lc.setLed(0,Drow2-1,Dcol2,false);
      lc.setLed(0,Drow3-1,Dcol3,false);
    
      lc.setLed(0,Drow1-2,Dcol1,true);
      lc.setLed(0,Drow2-2,Dcol2,true);
      lc.setLed(0,Drow3-2,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN4:
      lc.setLed(0,Drow1-2,Dcol1,false);
      lc.setLed(0,Drow2-2,Dcol2,false);
      lc.setLed(0,Drow3-2,Dcol3,false);
    
      lc.setLed(0,Drow1-3,Dcol1,true);
      lc.setLed(0,Drow2-3,Dcol2,true);
      lc.setLed(0,Drow3-3,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN5:
      lc.setLed(0,Drow1-3,Dcol1,false);
      lc.setLed(0,Drow2-3,Dcol2,false);
      lc.setLed(0,Drow3-3,Dcol3,false);
    
      lc.setLed(0,Drow1-4,Dcol1,true);
      lc.setLed(0,Drow2-4,Dcol2,true);
      lc.setLed(0,Drow3-4,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN6:
      lc.setLed(0,Drow1-4,Dcol1,false);
      lc.setLed(0,Drow2-4,Dcol2,false);
      lc.setLed(0,Drow3-4,Dcol3,false);
    
      lc.setLed(0,Drow1-5,Dcol1,true);
      lc.setLed(0,Drow2-5,Dcol2,true);
      lc.setLed(0,Drow3-5,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN7:
      lc.setLed(0,Drow1-5,Dcol1,false);
      lc.setLed(0,Drow2-5,Dcol2,false);
      lc.setLed(0,Drow3-5,Dcol3,false);
    
      lc.setLed(0,Drow1-6,Dcol1,true);
      lc.setLed(0,Drow2-6,Dcol2,true);
      lc.setLed(0,Drow3-6,Dcol3,true);
  
      delay(delays);
      break;
    case P_DOWN8:
      lc.clearDisplay(0);
      lc.setIntensity(0,1);
      lc.setColumn(0,4,B11111111);
      lc.setColumn(0,5,B11111111);
      lc.setIntensity(0,5);

      miss++;
  
      delay(delays);
      break;


      //RIGHT ARROW CYCLE
    case P_RIGHT1:
      lc.clearDisplay(0);
      lc.setLed(0,Rrow1-7,Rcol1,false);
      lc.setLed(0,Rrow2-7,Rcol2,false);
      lc.setLed(0,Rrow3-7,Rcol3,false);
    
      lc.setLed(0,Rrow1,Rcol1,true);
      lc.setLed(0,Rrow2,Rcol2,true);
      lc.setLed(0,Rrow3,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT2:
      lc.setLed(0,Rrow1,Rcol1,false);
      lc.setLed(0,Rrow2,Rcol2,false);
      lc.setLed(0,Rrow3,Rcol3,false);
    
      lc.setLed(0,Rrow1-1,Rcol1,true);
      lc.setLed(0,Rrow2-1,Rcol2,true);
      lc.setLed(0,Rrow3-1,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT3:
      lc.setLed(0,Rrow1-1,Rcol1,false);
      lc.setLed(0,Rrow2-1,Rcol2,false);
      lc.setLed(0,Rrow3-1,Rcol3,false);
    
      lc.setLed(0,Rrow1-2,Rcol1,true);
      lc.setLed(0,Rrow2-2,Rcol2,true);
      lc.setLed(0,Rrow3-2,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT4:
      lc.setLed(0,Rrow1-2,Rcol1,false);
      lc.setLed(0,Rrow2-2,Rcol2,false);
      lc.setLed(0,Rrow3-2,Rcol3,false);
    
      lc.setLed(0,Rrow1-3,Rcol1,true);
      lc.setLed(0,Rrow2-3,Rcol2,true);
      lc.setLed(0,Rrow3-3,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT5:
      lc.setLed(0,Rrow1-3,Rcol1,false);
      lc.setLed(0,Rrow2-3,Rcol2,false);
      lc.setLed(0,Rrow3-3,Rcol3,false);
    
      lc.setLed(0,Rrow1-4,Rcol1,true);
      lc.setLed(0,Rrow2-4,Rcol2,true);
      lc.setLed(0,Rrow3-4,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT6:
      lc.setLed(0,Rrow1-4,Rcol1,false);
      lc.setLed(0,Rrow2-4,Rcol2,false);
      lc.setLed(0,Rrow3-4,Rcol3,false);
    
      lc.setLed(0,Rrow1-5,Rcol1,true);
      lc.setLed(0,Rrow2-5,Rcol2,true);
      lc.setLed(0,Rrow3-5,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT7:
      lc.setLed(0,Rrow1-5,Rcol1,false);
      lc.setLed(0,Rrow2-5,Rcol2,false);
      lc.setLed(0,Rrow3-5,Rcol3,false);
    
      lc.setLed(0,Rrow1-6,Rcol1,true);
      lc.setLed(0,Rrow2-6,Rcol2,true);
      lc.setLed(0,Rrow3-6,Rcol3,true);
  
      delay(delays);
      break;
    case P_RIGHT8:
      lc.clearDisplay(0);
      lc.setIntensity(0,1);
      lc.setColumn(0,6,B11111111);
      lc.setColumn(0,7,B11111111);
      lc.setIntensity(0,5);

      miss++;
  
      delay(200);
      break;
  }
  return p_state;
}






//Arrows
enum A_states {A_START, A_PLAY, A_HIT, A_MISS, A_STOP} a_state = A_START;
int TickFct_Arrows(int state){

  switch(a_state){
    case A_START:
      a_state = A_PLAY;
      break;

    case A_PLAY:
      a_state = A_PLAY;
      break;
  }

  switch(a_state){
    case A_START:
      break;

    case A_PLAY:
     if (digitalRead(l) == HIGH)
        Serial.print("Left");
     else if (digitalRead(u) == HIGH)
         Serial.print("UP");
     else if (digitalRead(d) == HIGH)
         Serial.print("DOWN");
     else if (digitalRead(r) == HIGH)
         Serial.print("RIGHT");
      break;
  }
  return a_state;
}









void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,5);
  lc.clearDisplay(0);
  
  unsigned char i = 0;
  tasks[i].state = P_START;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Player;
  i++;
  tasks[i].state = A_START;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Arrows;

  Serial.begin(9600);
  randomSeed(analogRead(A5));

  pinMode(l, INPUT);
}









void loop(){
  unsigned char i;
  for (int i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) 
    {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  delay(100);
}