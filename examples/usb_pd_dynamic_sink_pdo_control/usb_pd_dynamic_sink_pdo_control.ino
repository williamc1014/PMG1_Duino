byte pdoIdx;
unsigned char str[8] = {0};

void outputSelPdo()
{
    //pdo.val = usbpd0.ctx->dpmStat.snkSelPdo.val;
    //int2str(pdo.val);
    //Serial.println(String((const char *)&str[0]));
    //delay(100);
    Serial.print(usbpd0.getCurrentSnkPdo());
}

void setup() {
  Serial.begin(9600);
  usbpd0.begin(); // Initiate the USB PD port 0
  usbpd0.registerEvent(APP_EVT_PD_CONTRACT_NEGOTIATION_COMPLETE, outputSelPdo);
  pinMode(D15, INPUT_PULLUP); // set D15 pin as input pull-up mode
  digitalWrite(LED1, 1);
}

byte changePdo = 5; // New PDO change count
byte enabledPdo = 5; // Old PDO change count


void loop() {

  // If the new PDO count is not equal to old PDO count
  // proceed the PDO change
  if (changePdo != enabledPdo)
  {
    setSnkPdoList(changePdo); // Change the sink PDO list
    enabledPdo = changePdo; // Update the old PDO change count
  }

  if (digitalRead(D15) == 0)
  {
    // If button is pressed (active low), de-bounce it
    // And the increase the new PDO change count (maximum is 3)
    digitalWrite(LED_BUILTIN, 0);
    delay(200);
    if (digitalRead(D15) == 0)
    {
      ++ changePdo;
      if (changePdo >= 6)
        changePdo = 0;

      // Wait the button to be relased
      while(digitalRead(D15) == 0) {}
    }
    digitalWrite(LED_BUILTIN, 1);
  }
}

unsigned char hex_char[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};

void int2str(unsigned int val)
{
  unsigned char nibble;
  str[0] = hex_char[(val >> 28) & 0x0F];
  str[1] = hex_char[(val >> 24) & 0x0F];
  str[2] = hex_char[(val >> 20) & 0x0F];
  str[3] = hex_char[(val >> 16) & 0x0F];
  str[4] = hex_char[(val >> 12) & 0x0F];
  str[5] = hex_char[(val >> 8) & 0x0F];
  str[6] = hex_char[(val >> 4) & 0x0F];
  str[7] = hex_char[(val >> 0) & 0x0F];
}

// Input pdos is in range of 0-3
// pdos = 0, only 5V/3A is enabled
// pdos = 0, 5V/3A and 9V/3A are enabled
// pdos = 0, 5V/3A, 9V/3A and 15V/3A are enabled
// pdos = 0, 5V/3A, 9V/3A, 15V/3A and 20V/3A are enabled
void setSnkPdoList(byte pdos)
{
  // Get the current number of available sink PDO
  byte cnt = usbpd0.getSnkPdoNum();

  // Remove them from the sink PDO list
  for (byte i = 1; i <= cnt; i++)
    usbpd0.removeSnkPdo(i);
 
  // Enable the sink PDOs depended on variable pdos
  switch(pdos)
  {
    case 5:
      usbpd0.enableSnkEprPdo(true);
      break;
    case 4:
      usbpd0.addFixedSnkPdo(4, 20000, 3000);  
    case 3:
      usbpd0.addFixedSnkPdo(3, 15000, 3000);
    case 2:
      usbpd0.addFixedSnkPdo(2, 12000, 3000);
    case 1:
      usbpd0.addFixedSnkPdo(1, 9000, 3000);
    case 0:
      usbpd0.enableSnkEprPdo(false);
      break;
  }
  // Update the sink PDO list to USB PD engine
  usbpd0.updateSnkPdo();
  delay(500);
  //for (byte i=0; i<13; i++)
  //{
  //  pdo.val = usbpd0.ctx->dpmStat.curSnkPdo[i].val;
  //  int2str(pdo.val);
  //  Serial.println(String((const char *)&str[0]));
  //  delay(50);
  //}
}
