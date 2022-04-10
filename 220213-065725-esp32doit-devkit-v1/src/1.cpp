/*
 Name:    Test1.ino
 Created: 11/21/2018 5:40:57 PM
 Author:  Yordan
*/

#include <ADS1256.h>

ADS1256 ads;
// pH meter Analog output to Arduino Analog Input 0
#define Offset 21.25 // deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40    // times of collection
int pHArray[ArrayLenth]; // Store the average value of the sensor feedback
int pHArrayIndex = 5;
// the setup function runs once when you press reset or power the board

boolean LEDflag = false;
// переменные времени
uint32_t myTimer, myTimer1, myTimer2;
uint32_t myTimer3;

void setup()
{
  Serial.begin(115200);
  ads.init(5, 14, 25, 1700000);
  Serial.println(ads.speedSPI);
}

// the loop function runs over and over again until power down or reset
double avergearray(int *arr, int number)
{
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0)
  {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5)
  { // less than 5, calculated directly statistics
    for (i = 0; i < number; i++)
    {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  }
  else
  {
    if (arr[0] < arr[1])
    {
      min = arr[0];
      max = arr[1];
    }
    else
    {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++)
    {
      if (arr[i] < min)
      {
        amount += min; // arr<min
        min = arr[i];
      }
      else
      {
        if (arr[i] > max)
        {
          amount += max; // arr>max
          max = arr[i];
        }
        else
        {
          amount += arr[i]; // min<=arr<=max
        }
      } // if
    }   // for
    avg = (double)amount / (number - 2);
  } // if
  return avg;
}

void loop()
{

  // 3 раза в секунду
  if (millis() - myTimer1 >= 333)
  {
    myTimer1 = millis(); // сбросить таймер
    Serial.println("timer 1");
  }
  // каждые 2 секунды
  if (millis() - myTimer2 >= 2000)
  {
    myTimer2 = millis(); // сбросить таймер
    Serial.println("timer 2");
  }
  // каждые 5 секунд
  if (millis() - myTimer3 >= 5000)
  {
    myTimer3 = millis(); // сбросить таймер
    Serial.println("timer 3");
  }

  ads.readInputToAdcValuesArray();
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = ads.adcValues[0];
    if (pHArrayIndex == ArrayLenth)
      pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * (5 / 8.388608) / 1000000;
    pHValue = -5.7 * (voltage) + Offset;
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval) // Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 4);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    digitalWrite(LED, digitalRead(LED) ^ 1);
    printTime = millis();
  }

  // каждую секунду
  if (millis() - myTimer >= 1000)
  {
    myTimer = millis();        // сбросить таймер
    digitalWrite(13, LEDflag); // вкл/выкл
    LEDflag = !LEDflag;        // инвертировать флаг

    //  for( int i = 0; i <= 7; i++ )
    //  {
    // Serial.print( ads.adcValues[ i ] );   // Raw ADC integer value +/- 23 bits
    //   Serial.print( ads.adcValues[ i ]*(5.00/8.388608) );   // Raw ADC integer value +/- 23 bits
    //   Serial.print( "      " );
    // }
    //  Serial.println();
    // Serial.println( ads.adcValues[ 2 ] * (5.00 / 8.388608) );   // Raw ADC integer value +/- 23 bits
    Serial.println(ads.adcValues[7] * (5 / 8.388608) / 1000000); // Raw ADC integer value +/- 23 bits
                                                                 // Serial.println( ads.adcValues[ 2 ]);   // Raw ADC integer value +/- 23 bits
    //  delay( 500 );
  }
}

