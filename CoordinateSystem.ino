//Motors
const int leftMotorForwards = 5;
const int leftMotorBackwards = 3;
const int rightMotorForwards = 9;
const int rightMotorBackwards = 6;

//Photoresistors
const int leftPhoto = A2;
const int centrePhoto = A1;
const int rightPhoto = A0;

//important constants
const int buffer = 60;
const int timebuffer = 1000;
const int desiredy = 4;

int leftnat, centnat, rightnat;
int linecount = 0;

unsigned long lastline = 0;

boolean first = false;


void setup(){
  
  pinMode(leftMotorForwards, OUTPUT);
  pinMode(leftMotorBackwards, OUTPUT);
  pinMode(rightMotorForwards, OUTPUT);
  pinMode(rightMotorBackwards, OUTPUT);
  pinMode(leftPhoto, INPUT);
  pinMode(centrePhoto, INPUT);
  pinMode(rightPhoto, INPUT);
  Serial.begin(9600);

}

void loop(){

  analogWrite(rightMotorForwards, 230);
  analogWrite(leftMotorForwards, 230);
  
  if(!first){
  
    leftnat = calibrate(leftPhoto);
    centnat = calibrate(centrePhoto);
    rightnat = calibrate(rightPhoto);
    first = true;  
  }
  
   if(linecount == desiredy)
  {
    digitalWrite(leftMotorForwards, LOW);
    digitalWrite(rightMotorForwards, LOW); 
    delay(1000);
    analogWrite(leftMotorBackwards,140);
    analogWrite(rightMotorBackwards,140);
    while (!((averageRead(rightPhoto) < (rightnat - buffer)) && averageRead(centrePhoto) < (centnat - buffer)) || (averageRead(centrePhoto) < (centnat - buffer) && averageRead(leftPhoto) < (leftnat - buffer))){ }
    digitalWrite(leftMotorBackwards, LOW);
    digitalWrite(rightMotorBackwards, LOW);
    delay(1000);
    turn(true);
    exit(0);  
  }
  if(averageRead(leftPhoto) < (leftnat - buffer) && averageRead(rightPhoto) < (rightnat - buffer) && averageRead(centrePhoto) < (centnat - buffer))
  {
     /* for(int i = 0; i < 5; i++)
      {
         if(!(averageRead(leftPhoto) < (leftnat - buffer) && averageRead(rightPhoto) < (rightnat - buffer) && averageRead(centrePhoto) < (centnat - buffer)))
         {
            goto justadrill;
         } 
      }*/
      if(millis() - lastline > timebuffer){
        while(averageRead(leftPhoto) < (leftnat - buffer) && averageRead(rightPhoto) < (rightnat - buffer)){} 
        linecount++;
        Serial.print("Were crossing line number: ");
        Serial.println(linecount);
        lastline = millis();
      }   
  }
  else if(averageRead(leftPhoto) < (leftnat - buffer) && averageRead(centrePhoto) > (centnat - buffer))//left but not centre
  {
      Serial.println("Time to turn left"); 
      adjust(true);      
  } 
  else if(averageRead(rightPhoto) < (rightnat - buffer) && averageRead(centrePhoto) > (centnat - buffer))//right but not centre
  {
     Serial.println("Time to turn right");
     adjust(false);
  } 
/*  else//everythings normal
  {
     analogWrite(rightMotorForwards, 230);
     analogWrite(leftMotorForwards, 230);
  }
  */
 
  
  
 /* 
  Serial.print("Right: ");
  Serial.print(averageRead(rightPhoto));
  Serial.print(" centre: ");
  Serial.print(averageRead(centrePhoto));
  Serial.print(" Left: ");
  Serial.println(averageRead(leftPhoto));
  delay(500);
  */
  

}



void adjust(boolean left){

  unsigned long timer = millis();
  if (left){
     //curve left
     analogWrite(rightMotorForwards, 230);
     analogWrite(leftMotorForwards, 170);
     
     //in case we cross line while curving left
     encounteredlineright:
     
     //wait till centre sees black again or right sees black
     while(averageRead(centrePhoto) > (centnat - buffer) && averageRead(rightPhoto) > (rightnat - buffer))
     {

     }
        //if centre became black (i.e. crossed line before halfway of curve or just normal case)
        if(averageRead(centrePhoto) < (centnat - buffer))
        { 
          //start a timer
         // for(int i = 0; i < 10; i++)
          //{
              // if we're crossing a grid line
              if (averageRead(leftPhoto) < (leftnat - buffer))
              {
               //double check you're not seeing the same line twice
               if(millis() - lastline > timebuffer)
               {
                 //increment line count
                 linecount++;
                 if(linecount == desiredy){return;}
                 Serial.print("(in first half adjust)Were crossing line number: ");
                 Serial.println(linecount);
                 lastline = millis();
                 while(averageRead(centrePhoto) < (centnat - buffer) && averageRead(leftPhoto) < (leftnat - buffer)){}
                 //continue curving
                 goto encounteredlineright; 
               }
              } 
           //}
         }
         
         //we cross line with right going black first
        else if(averageRead(rightPhoto) < (rightnat - buffer) && millis() - lastline > timebuffer && averageRead(centrePhoto) > (centnat - buffer))
        { 
           linecount++;
           if(linecount == desiredy){return;}
           Serial.print("(in second half adjust)Were crossing line number: ");
           Serial.println(linecount);
           lastline = millis();
           timer = timer - 10;
           delay(100);              
        } 

     //for adjustment factor
     timer = millis() - timer;
     analogWrite(rightMotorForwards, 170);
     
     analogWrite(leftMotorForwards, 230);
     for(unsigned long i = 0; i < timer/2; i++)
     {
        if (averageRead(leftPhoto) < (leftnat - buffer) && millis() - lastline > timebuffer)
        {
           linecount++; 
           lastline = millis();
        }
        delay(1);
     }
     digitalWrite(leftMotorForwards, LOW);
     digitalWrite(rightMotorForwards, LOW);
  }
  
  else{
    analogWrite(leftMotorForwards, 230);
    analogWrite(rightMotorForwards, 170);
    
    encounteredlineleft:
    while(averageRead(centrePhoto) > (centnat - buffer) && averageRead(leftPhoto) > (leftnat - buffer))
    {
      
    }
    
    if(averageRead(centrePhoto) < (centnat - buffer))
      { 
        //start a timer
       // for(int i = 0; i < 10; i++)
        //{
            // if we're crossing a grid line
            if (averageRead(rightPhoto) < (rightnat - buffer))
            {
              if(millis() - lastline > 500)
              {
               //increment line count
               linecount++;
               if(linecount == desiredy){return;}
               Serial.print("(in first half adjust)Were crossing line number: ");
               Serial.println(linecount);
               lastline = millis();
               while(averageRead(centrePhoto) < (centnat - buffer) && averageRead(rightPhoto) < (rightnat - buffer)){}
               //continue curving
               goto encounteredlineleft; 
              }
            } 
         //}
       }
         
     else if(averageRead(leftPhoto) < (leftnat - buffer) && millis() - lastline > timebuffer)
      { 
         linecount++;
         if(linecount == desiredy){return;}
         Serial.print("(in second half adjust)Were crossing line number: ");
         Serial.println(linecount);
         lastline = millis();
         timer -= 10;
         delay(100);              
      } 
        
     timer = millis() - timer;
     analogWrite(leftMotorForwards, 170);
 
     analogWrite(rightMotorForwards, 230);
     for(unsigned long i = 0; i < timer/2; i++)
     {
        if (averageRead(rightPhoto) < (rightnat - buffer) && millis() - lastline > timebuffer)
        {
           linecount++; 
           lastline = millis();
        }
        delay(1);
     }
     digitalWrite(rightMotorForwards, LOW);
     digitalWrite(leftMotorForwards, LOW);
   }
}

int averageRead(int pin){

  int sum = 0;
  for (int i = 0; i < 3; i++){
    sum += analogRead(pin);  
  }
  return (int)(sum / 3);
}


int calibrate(int photo){

  int count = 0;
  
  for(int i = 0; i < 5; i++)
  {  
    count += analogRead(photo);  
  }
  return (int)(count / 5);
}

void turn(boolean left){
  //check if right/left went from white to black
  boolean rightCheck = false;
  boolean leftCheck = false;
  
  analogWrite(rightMotorForwards, 100);
  analogWrite(leftMotorBackwards, 100);

  delay(700);
//    if (averageRead(rightPhoto) < (rightnat - buffer))
//      rightCheck = true;
//    
//    if (averageRead(leftPhoto) < (leftnat - buffer))
//      leftCheck = true;

  digitalWrite(leftMotorBackwards, LOW);
  digitalWrite(rightMotorForwards, LOW);
}
      
