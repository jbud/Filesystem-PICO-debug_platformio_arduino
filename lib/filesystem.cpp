#include "filesystem.h"


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
  
  Serial1.print("Dump: "); Serial1.println(logname);
  file = fopen(filename, "r");
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
}

void appendFile(const char * path, const char * message, size_t messageSize){
  FILE *file = fopen(path, "a");
  
  if (!file) 
    return;
  
  fwrite((uint8_t *) message, 1, messageSize, file);
  
  fclose(file);
}

void writeFile(const char * path, const char * message, size_t messageSize) 
{
  FILE *file = fopen(path, "w");
  
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