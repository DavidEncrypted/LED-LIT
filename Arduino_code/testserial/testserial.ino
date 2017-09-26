// Serial test script

int setPoint = 55;
String command;

void setup()
{

  Serial.begin(9600);  // initialize serial communications at 9600 bps

}

void loop()
{
  while(!Serial.available()) {}
  // serial read section
  while (Serial.available())
  {
    if (Serial.available() >0)
    {
      char c = Serial.read();  //gets one byte from serial buffer
      command += c; //makes the string readString
    }
  }

  if (command.length() >0)
  {
    Serial.print("Arduino received: ");  
    Serial.println(command); //see what was received
  }

  delay(500);

  // serial write section

  char ard_sends = '1';
  Serial.print("Arduino sends: ");
  Serial.println(ard_sends);
  Serial.print("\n");
  Serial.flush();
}
