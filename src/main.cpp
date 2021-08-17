#include <Arduino.h>

#include "FlashIAPBlockDevice.h"
#include "LittleFileSystem.h"
#include <mbed.h>

#define FS_BASE "fs"

// 1MB partition, starting 1MB into the flash
FlashIAPBlockDevice bd(XIP_BASE + 0x100000, 0x100000);
mbed::LittleFileSystem fs(FS_BASE);

char filename[] = FS_BASE "/ksdm.dat";
char logname[] = FS_BASE "/ksdm.log";

typedef char kbyte;

long fileSize(const char * path){
  FILE *file = fopen(path, "r");
  long sz=0;
  while(true){
    fgetc(file);
    if (feof(file)) 
    { 
      break;
    }
    sz++;
  }
  fclose(file);
  return sz;
}

void dumpLog() 
{
  Serial1.print("Dump: "); Serial1.println(logname);

  FILE *file = fopen(logname, "r");
  
  if (!file) 
    return;

  char c;

  while (true) 
  {
    c = fgetc(file);
    
    if ( feof(file) ) 
    { 
      break;
    }
    else   
      Serial1.print(c);
  }
  Serial1.println(" #eof");
  fclose(file);
  
  Serial1.print("Dump: "); Serial1.println(filename);
  file = fopen(filename, "r");
  if (!file) 
    return;

  char x[20];
  while (true) 
  {
    c = fgetc(file);
    
    if ( feof(file) ) 
    { 
      break;
    }
    else {
      sprintf(x, "HEX Value: 0x%X", c);
      Serial1.print(x);
    }
  }
  Serial1.println(" #eof");
  fclose(file);
}
void deleteFile(const char * path){
  if (remove(path) == 0)
    return;
}


void writeFile(const char * path, const char * message, size_t messageSize) 
{
  FILE *file = fopen(path, "w");
  
  if (!file) 
    return;
  
  fwrite((uint8_t *) message, 1, messageSize, file);
  
  fclose(file);
}

void appendFile(const char * path, const char * message, size_t messageSize){
  if (fileSize(path) > 1024 * 1024){
    deleteFile(path);
    writeFile(path, "\n", 1);
  }
  FILE *file = fopen(path, "a");
  
  if (!file) 
    return;
  
  fwrite((uint8_t *) message, 1, messageSize, file);
  
  fclose(file);
}

kbyte readFile(const char * path) 
{
  FILE *file = fopen(path, "r");
  
  if (!file) 
    return 0;

  kbyte c;
  fread((uint8_t *) &c, sizeof(c), 1, file);

  fclose(file);
  return c;
}

bool fileExists(const char * path){
    FILE *file;
    if((file = fopen(path,"r"))!=NULL){
        fclose(file);
        return true;
    } else {
        return false;
    }
}



int i = 0;
void setup() {
  const char* x;
  bool clean = true;
  Serial1.begin(115200);
  while (!Serial1){}

  Serial1.println("hello fs.");
  int err = fs.mount(&bd);
  if (err) {
    fs.reformat(&bd);
  }
  
  if (clean){
    Serial1.println("Cleaning files...");
    if (fs.reformat(&bd) == 0){
      Serial1.println("Cleaned files...");
    }
  }
  
  if (!fileExists(filename)){
    kbyte v = 0xFF;
    x = &v;
    writeFile(filename, x, 1);
    Serial1.println("File didn't exists sadge.");
  } else {
    Serial1.println("File exists! lets read it!");
    readFile(filename);
  }
  delay(4000);
  
  kbyte c = readFile(filename);
  c &= 0xF0;
  char y[25];
  sprintf(y, "this should be 240: %d\n", c);
  Serial1.println(y);
}

void loop() {
  if (Serial1.available())
  {
    String s = Serial1.readString();
    s.remove(2);
    if (s == "id")
    {
      Serial1.println("ksdm3-rp2040-3");
    } else if (s == "dl"){
      Serial1.println("Size of file: ");
      Serial1.println(fileSize(logname));
      dumpLog();
    }
  }
  if (i < 100){
    delay(250);
    char x[16];
    sprintf(x, "Hello World! %d\n", i);
    appendFile(logname, x, strlen(x));
    i++;
  } else {
    
  }
}