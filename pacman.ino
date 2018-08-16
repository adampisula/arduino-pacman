#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int x = 0;
int y = 0;

int rx = 0;
int ry = 0;

int state = 0;
int prevState = 0;

int score = 0;

byte pacmanright[8] = {
  B01110,
  B11101,
  B11111,
  B11110,
  B11100,
  B11110,
  B11111,
  B01110
};

byte pacmanleft[8] = {
  B01110,
  B10111,
  B11111,
  B01111,
  B00111,
  B01111,
  B11111,
  B01110
};

byte point[8] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B00000,
  B00000
};

int clickLCD()
{
  int x = analogRead(0);

  Serial.println(x);

  if(x > 1000) return 0; //brak
  if(x > 700) return 1; //select
  if(x > 400) return 2; //left
  if(x > 250) return 3; //down
  if(x > 100) return 4; //up
  if(x >= 0) return 5; //right

  return 0;
}

void createPoint() {
  do
  {
    rx = random(15);
    ry = random(2); 
  } while(rx == x || ry == y);
  
  lcd.setCursor(rx, ry);
  lcd.write(byte(2));
  lcd.setCursor(x, y);
}

void printScore(int score) {
  if(score < 10)
  {
    lcd.setCursor(15, 0);
    lcd.print("0");
    lcd.setCursor(15, 1);
    lcd.print(score);   
  }

  else
  { 
    lcd.setCursor(15, 0);
    lcd.print(score);
    lcd.setCursor(15, 1);
    lcd.print(score % 10);  
  }
  
  lcd.setCursor(x, y);
}

void setup() {
  Serial.begin(9600);
  
  lcd.createChar(0, pacmanright);
  lcd.createChar(1, pacmanleft);
  lcd.createChar(2, point);
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.write(byte(1));
  lcd.print("PAC-MAN");
  lcd.write(byte(0));
  lcd.setCursor(9, 1);
  lcd.print("*almost");
  while(analogRead(0) > 1000) {}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  createPoint();
  printScore(0);
}

void loop() {
  state = clickLCD();
  
  if(state != prevState)
  { 
    prevState = state;
    
    lcd.clear();

    lcd.setCursor(rx, ry);
    lcd.write(byte(2));
    lcd.setCursor(x, y);

    if(rx == x && ry == y)
    {
      score++;

      if(score == 100)
      {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Victory Royale!");
        delay(5000);

        //RESET ARDUINO
        asm volatile (" jmp 0");
      }
      
      createPoint(); 
    }

    printScore(score);
  
    switch(state)
    {
      case 5:
        lcd.createChar(0, pacmanright);
        x++;
        break;
      case 2:
        lcd.createChar(0, pacmanleft);
        x--;
        break;
      case 4:
        y--;
        
        if(y < 0)
          y = 0;
        break;
      case 3:
        y++;
        
        if(y > 1)
          y = 1;
        break;
    }
  
    lcd.setCursor(x, y);
    lcd.write(byte(0));
  
    if(x < 0)
      x = 14;
      
    if(x == 15)
      x = 0;
  }

  delay(75); 
}
