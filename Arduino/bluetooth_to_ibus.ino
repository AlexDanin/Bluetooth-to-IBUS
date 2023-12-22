#include "SoftwareSerial.h" 
 
#define BUFFER_SIZE 30 
char inData[BUFFER_SIZE + 1]; 
 
#define IBUS_FRAME_LENGTH 0x20 
#define IBUS_COMMAND40 0x40 
#define MAX_CHANNELS 6 
#define IBUS_MAXCHANNELS 14 
#define IBUS_DEFAULT_VALUE (uint16_t)1500 
  
SoftwareSerial HC06(10, 11); 
    
int r = 0, t = 0, buffer_index = 0; 
 
byte serial_buffer[IBUS_FRAME_LENGTH] = { 0 }; 
volatile uint16_t channel_data[14] = { 0 }; 
 
void setup()  {  
  pinMode(13, OUTPUT);   
  Serial.begin(115200);   
  HC06.begin(9600); 
} 
 
void loop()  {   
  readData(); 
  WriteSerial(); 
  delay(10); 
} 
 
void WriteSerial() { 
  uint16_t ibus_checksum = ((uint16_t)0xFFFF); 
   
  buffer_index = 0; 
  serial_buffer[buffer_index++] = (byte)IBUS_FRAME_LENGTH; 
  serial_buffer[buffer_index++] = (byte)IBUS_COMMAND40; 
  
  for (int i = 0; i < min(MAX_CHANNELS, IBUS_MAXCHANNELS); i++) { 
    serial_buffer[buffer_index++] = (byte)(channel_data[i] & 0xFF); 
    serial_buffer[buffer_index++] = (byte)((channel_data[i] >> 8) & 0xFF); 
  } 
 
  if (MAX_CHANNELS < IBUS_MAXCHANNELS) { 
    for (int i = 0; i < IBUS_MAXCHANNELS - MAX_CHANNELS; i++) { 
      serial_buffer[buffer_index++] = (byte)(IBUS_DEFAULT_VALUE & 0xFF); 
      serial_buffer[buffer_index++] = (byte)((IBUS_DEFAULT_VALUE >> 8) & 0xFF); 
    } 
  } 
 
  for (int i = 0; i < buffer_index; i++) { 
    ibus_checksum -= (uint16_t)serial_buffer[i]; 
  } 
  
  serial_buffer[buffer_index++] = (byte)(ibus_checksum & 0xFF); 
  serial_buffer[buffer_index++] = (byte)((ibus_checksum >> 8) & 0xFF); 
 
  Serial.write(serial_buffer, buffer_index); 
 
  buffer_index = 0; 
} 
 
void readData(){ 
  t = 0; 
  r = 0; 
  byte byte_count=HC06.available();   
  if(byte_count) 
  { 
    byte size =  HC06.readBytes(inData, BUFFER_SIZE); 
    inData[size] = 0;  
    String s = String(inData); 
    for (int i = 0; i < s.length(); i++)  
    {         
      if(s.charAt(i) == '/')   
      {             
        channel_data[t] = s.substring(r, i).toInt(); 
        // Serial.println(channel_data[t]); 
        r=(i+1);             
        t++;   
      }        
    }    
  } 
}
