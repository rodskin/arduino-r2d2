// Inclusion des librairies
#include <Servo.h>

/**********************************
 * PARTIE LED
 **********************************/
int redLed = 5;    // Red led pin
int blueLed = 6;   // Blue led pin
int greenLed = 7;  // Green led pin

int songLed[] = {
  blueLed,
  redLed,
  blueLed
};

/**********************************
 * PARTIE SERVO
 **********************************/
Servo myServo;
int servoPin = 8;
int val;
int oldVal = 0;
long randAngle;
int tmpRandAngle = 0;
int angle;
int servoMove = 1;
int nbTimesToMove = 5;
int isMoving = 0;
int angleSpeed = 5; // speed for moving head

/**********************************
 * PARTIE MUSIQUE
 **********************************/
//Tone tone1;
int buzzerPin = 9;
int randSong;
int note_num = 0;

#define OCTAVE_OFFSET 0

int notes[] = { 0,
  262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
  523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

char *song[] = {
  "i_love_lei:d=4,o=5,b=120:13p,25a7,28p,34a7,32c#7,240p,17c#7,15p.,34a.7,24p,18f6,240p,30d.8,48p,18d6,3p,33d#.7,96p,34f.7,17p,18g7,14a#7", 
  "merci_sema:d=4,o=5,b=120:25p,17d#7,13p,17d8,9p,9d8,40p,34g#.,48a,40p,18a#,28b,30a#.,48p,25b,7p,48b6,240p,40e6,40p,25c8,160p,30f#7,28p,40g7,15p,40a7,32p,17g7,15a#7,p.,40a#6", 
  "no_i_am_yo:d=4,o=5,b=120:7p,25f6,p,17a7,60p,34a7,34d7,160p,17d7,15p.,18g7,14a#7,2p,33f.6,15p,28a7,p,40a6,17g7,18a#7,3p,48d6,7p,25f6,240p,32a6"
};

/**********************************
 * PARTIE BOUTON
 **********************************/
int buttonPin = 10;     // the number of the pushbutton pin
int buttonState;

// Tests
int testNoServo = 0; // 1 => on utilise pas le servo :)
 
/**********************************
 * SETUP
 **********************************/
void setup() 
{ 
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  myServo.attach(servoPin);
  //tone1.begin(buzzerPin);
  Serial.begin(9600);
  randomSeed(analogRead(5));
} 
 
/**********************************
 * PARTIE LOOP
 **********************************/
void loop() 
{ 
  myServo.write(90);
  buttonState = digitalRead(buttonPin);
  Serial.print("buttonState hors while : ");
  Serial.println(buttonState);
  
  while (buttonState == HIGH || isMoving == 1) {
    Serial.print("buttonState in while : ");
    Serial.println(buttonState);
    
    //loop musique et led
    randSong = random(1, 3);
    // Allumage Led en fonction de la musique
    play_rtttl(song[randSong]);
    digitalWrite(songLed[0], HIGH);
    
    // Code pour tourner le servo
    while (servoMove <= nbTimesToMove) {
      Serial.print("mouvement numero : ");
      Serial.println(servoMove);
      
      randAngle = random(1, 180);  //Angle du servo en degres (de 1 a 180)
      while (randAngle == oldVal) {
        randAngle = random(1, 180);
      }
      //Serial.print("oldVal : ");
      //Serial.println(oldVal);
      tmpRandAngle = randAngle;
      //Serial.print("tmpRandAngle : ");
      //Serial.println(tmpRandAngle);
      if (tmpRandAngle < oldVal) {
        //Serial.println("boucle tmpRandAngle < oldVal");
        for (angle = oldVal; angle >= tmpRandAngle; angle -= 1) {
          //Serial.println(angle);
          myServo.write(angle);
          delay(angleSpeed);
        }
      } else {
        //Serial.println("boucle tmpRandAngle > oldVal");
        for (angle = oldVal; angle <= tmpRandAngle; angle += 1) {
          //Serial.println(angle);
          myServo.write(angle);
          delay(angleSpeed);
        }
      }
      Serial.println("-------------");
      delay(1000);
      oldVal = randAngle;
      servoMove += 1;
      isMoving = 1;
    }
    servoMove = 1;
    isMoving = 0;
    buttonState = digitalRead(buttonPin); // on reteste l'etat du bouton :)
    digitalWrite(songLed[0], LOW);
    Serial.println("Done.");
    //while(1);
  }
}

#define isdigit(n) (n >= '0' && n <= '9')

void play_rtttl(char *p)
{
  // Absolutely no error checking in here

  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(duration, 10);
      note_num = (scale - 4) * 12 + note;
      if (note_num >= 25) {
        digitalWrite(songLed[2], HIGH);
      } else {
        digitalWrite(songLed[1], HIGH);
      }
      tone(buzzerPin, notes[note_num]);
      delay(duration);
      noTone(buzzerPin);
      if (note_num >= 25) {
        digitalWrite(songLed[2], LOW);
      } else {
        digitalWrite(songLed[1], LOW);
      }
    }
    else
    {
      Serial.print("Pausing: ");
      Serial.println(duration, 10);
      delay(duration);
    }
  }
}
