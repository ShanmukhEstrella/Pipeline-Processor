//K Gireesh CS22B031
//Malireddi Sri Sai Shanmukh raj CS22B029
//Scalar Pipeline Processor Design(Project CS2610)
#define WB 4       //Write Back stage hashdefined with 4
#define MEM 3      //Mem operation stage hashdefined with 3
#define EXE 2      //Executed stage hashdefined with 2
#define ID 1       //Instruction decode stage hash defined with 1
#define IF 0       //Instruction fetch stage hash defined with 0
#include<bits/stdc++.h>
using namespace std;
int main()
{
  auto fptr = fopen("input/ICache.txt","r");   //Opening the ICcahe.txt file in input directory in read only mode.
  unsigned short int ICache[128];
  int8_t DCache[256];
  unsigned int a,b;
  char argv[2];
  int i=0;
  while(fscanf(fptr,"%x",&a) == 1)
  {
    fscanf(fptr,"%x",&b);
    a = a<<8;
    a+=b;
    ICache[i] = a;
    i++;
  }
  auto fptr2 = fopen("input/DCache.txt","r");
  i=0;
  while(fscanf(fptr2,"%s",argv) == 1)
  {
    string c(argv);
    DCache[i] = static_cast<int8_t>(stoi(c, 0, 16));
    i++;
  }
  int8_t registers[16];
  auto fptr3 = fopen("input/RF.txt","r");
  i=0;
  while(fscanf(fptr3,"%x",&a) == 1)
  {
    registers[i]=a;
    i++;
  }
  int pc = 0;
  int stage = 0;
  int LMD;
  int8_t A,B,C;
  int halt_condition = 45000;
  int halt_signal;
  int source1,source2;
  int data_hazard_stalls = 0;
  int total_stalls = 0;
  int arith_inst = 0;
  int logis_inst = 0;
  int shift_ins=0;
  int mem_ins=0;
  int load_imm=0;
  int ctrl_inst=0;
  int hlt_inst=0;
  unsigned short int instruction_fetched;
  int temp;
  int8_t ALUOutput[5];
  unsigned short int opcode[5];
  int dest[5];
  int imm[5];
  bool status[5];
  int dirty[16];
  bool data_hazard = false;
  memset(status,true,sizeof(status));
  memset(dirty,0,sizeof(dirty));
  while(1){

      // START OF PIPELINE.

      /********
       * 
      WRITE BACK STAGE.
       *
      ********/
    if(stage >= WB && status[WB] && halt_condition >= 2)
    {
      if(opcode[WB] < 11)
      {
        registers[dest[WB]] = ALUOutput[WB];  // answer of arthmetic operations or bitwise operations
        dirty[dest[WB]]--;
        //cout << "dest[WB]: " << dest[WB] << " ALUOUTPUT: "<<(int)ALUOutput[WB] << endl;
      }
      if(opcode[WB] == 11)
      {
        registers[dest[WB]] = LMD;    // data fetched from memory stage is stored in LMD.
        dirty[dest[WB]]--;
      }
    }
    else
    {
      if(halt_condition < 2)break;
    }


    /********
     * 
    MEMORY OPERATIONS STAGE.
      *
    ********/
    if(stage >= MEM && status[MEM] && halt_condition >= 3)
    {
      if(opcode[MEM] == 11)
      {
        LMD = DCache[ALUOutput[MEM]];
      }
      if(opcode[MEM] == 12)
      {
        DCache[ALUOutput[MEM]] = C;     // source register A
      }
    }


    /******* 
     * 
    EXECUTION OR ALU OPERATIONS STAGE
      *
    ********/
    if(stage >= EXE && status[EXE] && halt_condition >= 4){
      switch (opcode[EXE])
      {
          case 0:
            /*ADD*/
            ALUOutput[EXE] = A + B;
            arith_inst++;
            break;
          case 1:
            /*SUB*/
            ALUOutput[EXE] = A - B;
            arith_inst++;
            break;
          case 2:
            /*MUL*/
            ALUOutput[EXE] = A * B;
            arith_inst++;
            break;
          case 3:
            /*INC*/
            ALUOutput[EXE] = A + 1;
            arith_inst++;
            break;
          case 4:
            /*AND*/
            ALUOutput[EXE] = A & B;
            logis_inst++;
            break;
          case 5:
            /*OR*/
            ALUOutput[EXE] = A | B;
            logis_inst++;
            break;
          case 6:
            /*XOR*/
            ALUOutput[EXE] = A ^ B;
            logis_inst++;
            break;
          case 7:
            /*NOT*/
            ALUOutput[EXE] = ~A;    // to be checked
            logis_inst++;
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
            shift_ins++;
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
            shift_ins++;
            break;
          case 10:
            if(imm[EXE] < 128)
            {
              ALUOutput[EXE] = imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = -(256 - imm[EXE]);
            }
            load_imm++;
            break;  // immediate values to be stored.
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
            mem_ins++;
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
            C = B;
            mem_ins++;
            break;
          case 13:
            /*JMP*/
            status[IF] = false;
            //cout << "JUMP in execute: " << imm[EXE] << endl;
            if(imm[EXE] < 128)
            {
              ALUOutput[EXE] = imm[EXE];
            }
            else
            {
              ALUOutput[EXE] = -(256 - imm[EXE]);
            }  // immediate values to be stored.
            pc += ALUOutput[EXE];
            ctrl_inst++;
            break;
          case 14:
            /*BEQZ*/
            status[IF] = false;
            //cout << "branch in execute: " << imm[EXE] << endl;
            //cout << "A: " << (int)A << endl;
            // //cout << registers[6] << endl;
            if(A == 0)
            {
                if(imm[EXE] < 128)
                {
                    ALUOutput[EXE] = imm[EXE];
                }
                else
                {
                    ALUOutput[EXE] = -(256 - imm[EXE]);
                }  // immediate values to be stored.
                pc += ALUOutput[EXE];
            }
            ctrl_inst++;
            break;
          case 15:
            hlt_inst++;
            halt_signal = 0;
          default:
            break;
      }
    }


    /******* 
     * 
    INSTRUCTION DECODE STAGE.
      *
    ********/
    if(stage >= ID && status[ID] && halt_condition > 5)
    {
        opcode[ID] = instruction_fetched >> 12;
        //cout<<"Instr in stage ID"<<" "<<(int)instruction_fetched<<endl;
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
              dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              if(dirty[source1] == 0 && dirty[source2] == 0)
              {
                A = registers[source1];
                B = registers[source2];
                dirty[dest[ID]]++;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 3:
            /*INC*/
              dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              if(dirty[dest[ID]] == 0)
              {
                A = registers[dest[ID]];
                dirty[dest[ID]]++;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 7:
              /*NOT*/  // to be checked
            case 8:
              /*SLLI*/
            case 9:
            /*SRLI*/
              dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              if(dirty[source1] == 0)
              {
                A = registers[source1];
                imm[ID] = source2;
                dirty[dest[ID]]++;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 10:
              /*LI*/
              dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              imm[ID] = (source1 << 4)+source2;
              dirty[dest[ID]]++;
              ////cout<<"value of imm"<<imm[ID]<<endl;
              break;
            case 11:
            /*LD*/
              dest[ID] = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              // A = registers[source1];
              // imm[ID] = source2;
              if(dirty[source1] == 0)
              {
                A = registers[source1];
                imm[ID] = source2;
                dirty[dest[ID]]++;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 12:
              /*ST*/
              temp  = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              // B = registers[temp];
              dest[ID] = -1;                     // to avoid false data dependencies.
              // A = registers[source1];
              // imm[ID] = source2;
              if(dirty[temp] == 0 && dirty[source1] == 0)
              {
                B = registers[temp];
                A = registers[source1];
                imm[ID] = source2;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 13:
              /*JMP*/
              temp  = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              imm[ID] = (temp << 4)+source1;
              dest[ID] = -1;                    // to avoid false data dependencies.
              status[IF] = false;
              //cout << "imm: in jump  " << imm[ID] << endl;
              //cout << temp << "  :temp,source1:  " << source1 << endl;
              break;
            case 14:
              /*BEQZ*/
              temp = (instruction_fetched >> 8) - ((instruction_fetched >> 12) << 4);
              if(dirty[temp] == 0)
              {
                imm[ID] = (source1 << 4)+source2;
                A = registers[temp];
                //cout << "temp: " << temp << endl;
                //cout << (int)registers[temp] << endl;
                dest[ID] = -1;                   // to avoid false data dependencies.
                status[IF] = false;
              }
              else
              {
                status[IF] = false;
                status[ID] = false;
                data_hazard = true;
              }
              break;
            case 15:
                  halt_condition = 5;  // to be checked
            default:
              break;
        }
        //cout << "opcode[ID]: " << opcode[ID] << " imm[ID]: " << imm[ID] <<  endl;
    }



    /******* 
     * 
    INSTRUCTION FETCH STAGE.
      *
    ********/
    if(stage >= IF && status[IF] && halt_condition > 5)
    {
      instruction_fetched = ICache[pc];
      //cout<<"instruction_fetched: "<<" "<<instruction_fetched<<endl;
      pc += 1;
    }
    else
    {
      if(data_hazard){
      status[IF] = true;
      data_hazard = false;
      data_hazard_stalls++;
      }
      if(halt_condition>5)
      {
        total_stalls++;
      }
    }

    stage++;
    halt_condition--;
    for(int i = 4;i > 0;i--)
    {
      opcode[i] = opcode[i-1]; 
      dest[i] = dest[i-1]; 
      //if(dest[i] == 5)////cout << "pc: " << pc << "reg5: " << registers[5] << "\n";
      imm[i] = imm[i-1];
      //cout << i << ": " << imm[i] << endl;
      status[i] = status[i-1];
      ALUOutput[i] = ALUOutput[i-1];
    }
    status[IF] = true;
    // for(int i =0;i < 16;i++)
    // {
    //   printf("%5d", dirty[i]);
    // }
  }
  auto ptrf = fopen("output/RF.txt","w");
  for(int i=0;i<16;i++)
  {
    int y = registers[i];
    int x = (y) - ((y>>8)<<8);
    if(x >= 16)fprintf(ptrf,"%x\n",x);
    else fprintf(ptrf,"%s%x\n","0",x);
  }
  auto shannu = fopen("output/DCache.txt","w");
  for(int i=0;i<256;i++)
  {
    int y = DCache[i];
    int x = (y) - ((y>>8)<<8);
    if(x >= 16)fprintf(shannu,"%x\n",x);
    else fprintf(shannu,"%s%x\n","0",x);
  }
  fclose(shannu);
  fclose(fptr);
  fclose(fptr2);
  fclose(fptr3);
  int total_inst = arith_inst + logis_inst + shift_ins + mem_ins + load_imm + ctrl_inst + hlt_inst;
  ofstream ofile("output/Output.txt",ios::out);
  ofile<<"Total number of instructions executed        : "<<total_inst<<endl;
  ofile<<"Number of instructions in each class     "<<endl;
  ofile<<"Arithmetic instructions                      : "<<arith_inst<<endl;
  ofile<<"Logical instructions                         : "<<logis_inst<<endl;
  ofile<<"Shift instructions                           : "<<shift_ins<<endl;
  ofile<<"Memory instructions                          : "<<mem_ins<<endl;
  ofile<<"Load immediate instructions                  : "<<load_imm<<endl;
  ofile<<"Control instructions                         : "<<ctrl_inst<<endl;
  ofile<<"Halt instructions                            : "<<hlt_inst<<endl;
  ofile<<"Cycles Per Instruction                       : "<<(stage)/(total_inst*1.0)<<endl;
  ofile<<"Total number of stalls                       : "<<total_stalls<<endl;
  ofile<<"Data stalls (RAW)                            : "<<data_hazard_stalls<<endl;
  ofile<<"Control stalls                               : "<<total_stalls-data_hazard_stalls<<endl;
}
