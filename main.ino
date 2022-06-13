volatile char *TWC_R = (char*)0xBC;
volatile char *TWS_R = (char*)0xB9;
volatile char *TWD_R = (char*)0xBB;
volatile char *TWB_R = (char*)0xB8;
unsigned char status,sec,minutes,hours,day,date,month,year;
char days [7][20]= {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void setup() {
  Serial.begin(115200);
  i2c_init();
  i2c_start();
  init_timer();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0xD0);//slave address of RTC 
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x07);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x00);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_stop();
  delay1();
//  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

//  set_date_time();

}


void init_timer(){
  volatile char *TCCR1_A = 0x80;
  volatile char *TCCR1_B = 0x81;
  volatile short *TCNT_1 = 0x84;
  volatile short *OCR1_A = 0x88;
  volatile char *TIMSK_1 = 0x6F;

  *TCCR1_A = 0;
  *TCCR1_B = 0x0C;
  *TCNT_1  = 0;
   
   *OCR1_A  = 60000;
   *TIMSK_1 = 0x03;
}

ISR (TIMER1_COMPA_vect){
   get_date_time();
   show_date_time();
}

void show_date_time(){
  char hours1 = (hours>>4)&0x0F;
  char hours2 = (hours)&0x0F;
  lcd_write(hours1+'0');
  lcd_write(hours2+'0');
  lcd_write(':');

  char minutes1 = (minutes>>4)&0x0F;
  char minutes2 = (minutes)&0x0F;
  lcd_write(minutes1+'0');
  lcd_write(minutes2+'0');
  lcd_write(':');

  char sec1 = (sec>>4)&0x0F;
  char sec2 = (sec)&0x0F;
  lcd_write(sec1+'0');
  lcd_write(sec2+'0');

  second_line();

  char date1 = (date>>4)&0x0F;
  char date2 = (date)&0x0F;
  lcd_write(date1+'0');
  lcd_write(date2+'0');
  lcd_write('/');

  char month1 = (month>>4)&0x0F;
  char month2 = (month)&0x0F;
  lcd_write(month1+'0');
  lcd_write(month2+'0');
  lcd_write('/');

  char year1 = (year>>4)&0x0F;
  char year2 = (year)&0x0F;
  lcd_write('2');
  lcd_write('0');
  lcd_write(year1+'0');
  lcd_write(year2+'0');
  
  lcd_write(' ');
  lcd_write('(');
  for(char i=0;i<3;i++){
    lcd_write(days[day-1][i]);
  }
  lcd_write(')');
}

void set_date_time(){
  i2c_start();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0xD0);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x00); //Initial address
  delay1();
  status = *TWS_R & 0xF8;
  Serial.println(status,HEX);

  i2c_write(0x00);//seconds
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x37);//minutes
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x13);//hours
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x05);//day
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x24);//date
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x03);//month
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x22);//year
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_stop();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

}

void get_date_time(){
  i2c_start();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0xD0);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0x00);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_stop();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_start();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  i2c_write(0xD1);
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  sec = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
  // Serial.println(status,HEX);

  set_ack();
  minutes = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  hours = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  day = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  date = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);

  set_ack();
  month = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
//  Serial.println(status,HEX);
  

  not_ack();
  year = *TWD_R;
  delay1();
  status = *TWS_R & 0xF8;
  // Serial.println(status,HEX);

  i2c_stop();
  delay1();
  status = *TWS_R & 0xF8;
  // Serial.println(status,HEX);
}

void i2c_init(){
  *TWB_R = 0x48; //100khz 
  *TWS_R = 0x00;
  *TWC_R = 0x04; //TWI enable
}

void i2c_start(){
  *TWC_R = 0b10100100;
  while((*TWC_R>>7)&0x01 != 0);
}

void i2c_stop(){
  *TWC_R = 0b10010100;
}

void i2c_write(unsigned data){
  *TWD_R = data;
  *TWC_R = 0b10000100;
  while((*TWC_R>>7)&0x01==0);
}

void set_ack(){
  *TWC_R = 0b11000100;
  while((*TWC_R>>7)&0x01==0);
}

void not_ack(){
 *TWC_R = 0b10000100;
  while((*TWC_R>>7)&0x01==0);
}

void delay1(){
  volatile long i;
  for(i=0;i<100;i++);
}


void loop() {
}
