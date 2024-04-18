//K Gireesh CS22B031
//Malireddi Sri Sai Shanmukh raj CS22B029
#include<bits/stdc++.h>
using namespace std;
void ALUoperation(short int encode, short int register[], short int &temp)
{
  if(encode==0)
  {
    register
  }
}
int main()
{
  auto fptr = fopen("input/ICache.txt","r");
  unsigned short 
  int ICache[128];
  signed short int DCache[128];
  unsigned int a,b;
  int i=0;
  while(fscanf(fptr,"%x",&a) == 1)
  {
    // if(i>128)break;
    fscanf(fptr,"%x",&b);
    a<<8;
    a+=b;
    ICache[i] = a;
    i++;
  }
  auto fptr2 = fopen("input/DCache.txt","r");
  i=0;
  while(fscanf(fptr2,"%x",&a) == 1)
  {
    // if(i>128)break;
    fscanf(fptr2,"%x",&b);
    a = a<<8;
    a+=b;
    b=a%2;
    a = a>>1;
    DCache[i] = a;
    DCache[i] = DCache[i]<<1;
    DCache[i] = (DCache[i] | b);
    i++;
  }
  short int registers[16];
  memset(registers,0,sizeof(registers));
  int pc = 0;
  short int temp;
  short int ir;
  
}
