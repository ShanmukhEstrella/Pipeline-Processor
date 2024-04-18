//K Gireesh CS22B031
//Malireddi Sri Sai Shanmukh raj CS22B029
#define WB 4
#define MEM 3
#define EXE 2
#define ID 1
#define IF 0
#include<bits/stdc++.h>
using namespace std;
int main()
{
  auto fptr = fopen("input/ICache.txt","r");
  unsigned short int ICache[128];
  signed short int DCache[128];
  unsigned int a,b;
  int i=0;
  while(fscanf(fptr,"%x",&a) == 1)
  {
    // if(i>128)break;
    fscanf(fptr,"%x",&b);
    a = a<<8;
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
  int stage = 0;
  int LMD;
  int A,B;
  int halt_signal;
  int source1,source2;
  int instruction_fetched;
  int ALUOutput[5];
  int opcode[5];
  int dest[5];
  int imm[5];
  while(1)
  {
    if(stage >= WB)
    {
      if(opcode[WB] < 10)
      {
        registers[dest[WB]] = ALUOutput[WB];  // answer of arthmetic operations or bitwise operations.
      }
      if(opcode[WB]==10)
      {
        registers[dest[WB]]  = imm[WB];
      }
      if(opcode[WB] == 11)
      {
        if(imm[EXE] < 128)
        {
              ALUOutput[EXE] = A + imm[EXE];
        }
        else
        {
              ALUOutput[EXE] = A - (128 - imm[EXE]);
        }  // immediate values to be stored.
      }
      if(opcode[WB] == 12)
      {
        registers[dest[WB]] = LMD;    // data fetched from memory stage is stored in LMD.
      }
    }
    if(stage >= MEM)
    {
      if(opcode[MEM] == 11)
      {
        LMD = DCache[ALUOutput[MEM]];
      }
      if(opcode[MEM] == 12)
      {
        DCache[ALUOutput[MEM]] = A;     // source register A
      }
    }
    if(stage >= EXE)
    {
      switch (opcode[EXE])
      {
          case 0:
            /*ADD*/
            ALUOutput[EXE] = A + B;
            break;
          case 1:
            /*SUB*/
            ALUOutput[EXE] = A - B;
            break;
          case 2:
            /*MUL*/
            ALUOutput[EXE] = A * B;
            break;
          case 3:
            /*INC*/
            ALUOutput[EXE] = A + 1;
            break;
          case 4:
            /*AND*/
            ALUOutput[EXE] = A & B;
            break;
          case 5:
            /*OR*/
            ALUOutput[EXE] = A | B;
            break;
          case 6:
            /*XOR*/
            ALUOutput[EXE] = A ^ B;
            break;
          case 7:
            /*NOT*/
            ALUOutput[EXE] = ~A;    // to be checked
            break;
          case 8:
            /*SLLI*/
            if(imm[EXE] < 8)
            {
              ALUOutput[EXE] = A << imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = A >> (16 - imm[EXE]);
            }
            break;
          case 9:
          /*SRLI*/
            if(imm[EXE] < 8)
            {
              ALUOutput[EXE] = A >> imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = A << (16 - imm[EXE]);
            }
            break;
          case 11:
          /*LD*/
            if(imm[EXE] < 8)
            {
              ALUOutput[EXE] = A + imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = A - (16 - imm[EXE]);
            }
            break;
          case 12:
            /*ST*/
            if(imm[EXE] < 8)
            {
              ALUOutput[EXE] = A + imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = A - (16 - imm[EXE]);
            }
            break;
          case 14:
            /*BEQZ*/
            if(A == 0)
            {
                    // to be checked.
            }
            break;
          case 15:
            halt_signal = 0;
          default:
            break;
      }
    }
    if(stage >= ID){
      opcode[ID] = instruction_fetched >> 12;
      cout<<"Instr in stage ID"<<" "<<instruction_fetched<<endl;
      dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
      source1 = (instruction_fetched >> 4) - ((instruction_fetched >> 8) << 4);
      source2 =  (instruction_fetched) - ((instruction_fetched >> 4) << 4);
      switch (opcode[ID])
      {
          case 0:
          case 1:
          case 2:
          case 4:
          case 5:
          case 6:
            /*XOR*/
            A = registers[source1];
            B = registers[source2];
            break;
          case 3:
            A = registers[dest[ID]];
            break;
          case 7:
            /*NOT*/  // to be checked
            break;
          case 8:
            /*SLLI*/
          case 9:
          /*SRLI*/
            A = registers[source1];
            imm[ID] = source2;
            break;
          case 10:
            imm[ID] = (source1 << 4)+source2;
            cout<<"value of imm"<<imm[ID]<<endl;
            break;
          case 11:
          /*LD*/
          case 12:
            /*ST*/
            A = registers[source1];
            imm[ID] = source2;
            break;
          case 13:
            imm[ID] = (dest[ID] << 4)+source1;
          case 14:
            /*BEQZ*/
            A = registers[dest[ID]];
            break;
          case 15:
                      // to be checked
          default:
            break;
      }
    }
    if(stage >= IF)
    {
      instruction_fetched = ICache[pc];
      cout<<"ins ib stage IF"<<" "<<instruction_fetched<<endl;
      pc += 1;
    }
    stage++;
    for(int i = 4;i > 0;i--)
    {
      opcode[i] = opcode[i-1]; 
      dest[i] = dest[i-1]; 
      if(dest[i] == 5)cout << "pc: " << pc << "reg5: " << registers[5] << "\n";
      imm[i] = imm[i-1];
      ALUOutput[i] = ALUOutput[i-1];
    }
    cout<<"Opcode:"<<opcode[2]<<endl;
    if(pc>=25) break;
  }
  for(int i = 0;i < 16;i++)
  {
    cout << i << ": " << registers[i] << endl;
  }
}
