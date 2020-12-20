#include <iostream>
#include <fstream>

using namespace std;

class IF
{
private:	
	int PC0;
	unsigned int ins0;
	unsigned int type0;
	unsigned int rs0;
	unsigned int rt0;
	void decode0()
	{
		type0 = ins >> 26;
		rs0 = (ins << 6) >> 27;
		rt0 = (ins << 11) >> 27;
	}

public:
	int PC;
	unsigned int ins;
	int utilized;
	int check(int write, int write1, unsigned int type, unsigned int type1)
	{
		int i = 0;
		decode0();
		int a1 = (rs0 == write) && write != 0 && type != 0x2b;
		int a2 = (rt0 == write) && write != 0 && type != 0x2b;
		int b1 = (rs0 == write1) && write1 != 0 && type1 != 0x2b;
		int b2 = (rt0 == write1) && write1 != 0 && type1 != 0x2b; 
		if (type0 == 0x00 || type0 == 0x2b)
		{
			if (a1 || a2 || b1 || b2)
			{
				ins0 = ins;
				i = 1;
				cout << "NOP of data hazard |";
			}
		}
		else if (a1 || b1)
		{
			ins0 = ins;
			cout << "NOP of data hazard |";
			i = 1;
		}
		return i;
	}
	void calculate(int* ProgramC, int write, int write1, unsigned int type, unsigned int type1, int *IMem)
	{
		if (!check(write, write1, type, type1))
		{
			if(type0 == 0x04)
			{
				ins0 = 0x00000020;
				cout << "NOP of beq         |";
			}
			else
			{
				ins0 = (unsigned int)IMem[*ProgramC/4];
				PC0 = *ProgramC + 4;
				*ProgramC = *ProgramC + 4;
				printf("ins%2d: %08x    |", utilized + 1, ins0);
				utilized++;	 
			}
		}
	}
	void transfer()
	{
		PC = PC0;
		ins = ins0;
	}
};
  
class ID
{
private:
	int PC0;
	unsigned int ins0;
	unsigned int type0;
	unsigned int rs0;
	unsigned int rt0;
	unsigned int rd0;
	unsigned int shamt0;
	unsigned int function0;
	unsigned int limmedi0;
	int aimmedi0;
	unsigned int ALUcontrol0;
	int read10;
	int read20;
	int write0;
	void decode0()
	{
		type0 = ins0 >> 26;
		rs0 = (ins0 << 6) >> 27;
		rt0 = (ins0 << 11) >> 27;
	}

public:
	int PC;
	unsigned int ins;
	unsigned int type;
	unsigned int rs;
	unsigned int rt;
	unsigned int rd;
	unsigned int shamt;
	unsigned int function;
	unsigned int limmedi;
	int aimmedi;
	unsigned int ALUcontrol;
	int read1;
	int read2;
	int write;
	int utilized;
	void calculate(IF fetch, int* Reg, int write1, unsigned int type1)
	{
		PC0 = fetch.PC;
		ins0 = fetch.ins;
		decode0();//try to decode first
		//check if it needs to NOP
		int a1 = (rs0 == write) && write != 0 && type != 0x2b;
		int a2 = (rt0 == write) && write != 0 && type != 0x2b;
		int b1 = (rs0 == write1) && write1 != 0 && type1 != 0x2b;
		int b2 = (rt0 == write1) && write1 != 0 && type1 != 0x2b; 
		if (type0 == 0x00 || type0 == 0x2b)
		{
			if (a1 || a2 || b1 || b2)
			{
				ins0 = 0x00000020;
				decode0();
			}
		}
		else if (a1 || b1)
		{
			ins0 = 0x00000020;
			decode0();
		}
		if (type == 0x04)
		{
			ins0 = 0x00000020;
			decode0();
		}
		if (ins0 != 0x00000020)
		{
			if (utilized >= 1)
				printf(" decode%2d  |", utilized);
			else
				printf("    NOP    |");
			utilized++;
		}
		else
			printf("    NOP    |");
		rd0 = (ins0 << 16) >> 27;
		shamt0 = (ins0 << 21) >> 27;
		function0 = ins0 << 26 >> 26;
		limmedi0 = (ins0 << 16 >> 16);
		aimmedi0 = (int)(ins0 << 16) >> 16;

		if (type0 == 0)
		{
			switch (function0)
			{
				case 0x20: ALUcontrol0 = 2;//add
				break;
				case 0x22: ALUcontrol0 = 6;//sub
				break;
				case 0x18: ALUcontrol0 = 7;//mul
				break;
				case 0x25: ALUcontrol0 = 3;//or
				break;
				case 0x24: ALUcontrol0 = 5;//and
				break;
				case 0x00: ALUcontrol0 = 9;//sll
				break;
				case 0x02: ALUcontrol0 = 10;//srl
				break;
			}
		}
		else
		{
			switch (type0)
			{
				case 0x0f: ALUcontrol0 = 8;//lui
				break;
				case 0x0d: ALUcontrol0 = 3;//ori
				break;
				case 0x23: ALUcontrol0 = 2;//lw
				break;
				case 0x2b: ALUcontrol0 = 2;//sw
				break;
				case 0x0c: ALUcontrol0 = 5;//andi
				break;
				case 0x04: ALUcontrol0 = 6;//beq
				break;
				case 0x08: ALUcontrol0 = 2;//addi
				break;
				case 0x0a: ALUcontrol0 = 1;//slti
				break;
				case 0x0b: ALUcontrol0 = 1;//sltiu
				break;
			}
		}

		read10 = Reg[rs0];
		if (type0 == 0x00 || type0 == 0x04)
		{
			read20 = Reg[rt0];
			write0 = rd0;
		}
		else if (type0 == 0x0f || type0 == 0x0d || type0 == 0x0c || type0 == 0x0b)//type is lui/ori/andi
		{
			read20 = limmedi0;
			write0 = rt0;
		}
		else 
		{
			read20 = aimmedi0;
			write0 = rt0;
		}
	}
	void transfer()
	{
		PC = PC0;
		ins = ins0;
		type = type0;
		rs = rs0;
		rt = rt0;
		rd = rd0;
		shamt = shamt0;
		function = function0;
		limmedi = limmedi0;
		aimmedi = aimmedi0;
		ALUcontrol = ALUcontrol0;
		read1 = read10;
		read2 = read20;
		write = write0;
	}
};

class EX
{
private:
	int PC0;
	int write0;
	int ALUresult0;
	unsigned int type0;
	int read20;
	long long int mulresult0;
	unsigned int ins0;
public:
	int write;
	int ALUresult;
	unsigned int type;
	int read2;
	long long int mulresult;
	unsigned int ins;
	int utilized;
	void calculate(ID id, int* IMem, int* ProgramC)
	{
		PC0 = id.PC;
		type0 = id.type;
		switch(id.ALUcontrol)
		{
			case 2: ALUresult0 = id.read1 + id.read2;//add
			break;
			case 6: ALUresult0 = id.read1 - id.read2;//sub
			break;
			case 8: ALUresult0 = id.limmedi << 16;//lui
			break;
			case 3: ALUresult0 = id.read1 | ((unsigned int)id.read2);//ori
			break;
			case 5: ALUresult0 = id.read1 & ((unsigned int)id.read2);//andi
			break;
			case 7: mulresult0 = (long long int)id.read1 * (long long int)id.read2;
			type0 = 0xff;
			break;
			case 9: ALUresult0 = id.read1 << (unsigned int)id.read2;
			break;
			case 10: ALUresult0 = id.read1 >> (unsigned int)id.read2;
			break;
			case 1: if (id.read1 - id.read2 < 0) ALUresult0 = 1;
					else ALUresult0 = 0;
			break;
		}

		read20 = id.read2;
		write0 = id.write;
		if (ALUresult0 == 0 && type0 == 0x04)
		{
			*ProgramC = PC0 + (id.aimmedi << 2);
		}
		ins0 = id.ins;
	}
	void transfer()
	{
		write = write0;
		ALUresult = ALUresult0;
		type = type0;
		read2 = read20;
		mulresult = mulresult0;
		ins = ins0;
	}
	void display()
	{
		if (ins0 != 0x00000020)
		{
			if (utilized >= 2)
				printf(" execute%2d |", utilized - 1);
			else
				printf("    NOP    |");
			utilized++;
		}
		else
			printf("    NOP    |");
	}
};

class MEM
{
private:
	int result0;
	int write0;
	unsigned int type0;
	int ALUresult0;
	long long int mulresult0;
	unsigned int ins0;
public:
	int result;
	int write;
	int ALUresult;
	long long int mulresult;
	unsigned int type;
	unsigned int ins;
	int utilized;
	void calculate(EX ex, int* DMem, int * Reg)
	{
		if(ex.type == 0x23)//lw
		{
			result0 = DMem[ex.ALUresult/4];

		}
		else if(ex.type == 0x2b)//sw
		{
			DMem[ex.ALUresult/4] = Reg[ex.write];
		}
		write0 = ex.write;
		type0 = ex.type;
		ALUresult0 = ex.ALUresult;
		mulresult0 = ex.mulresult;
		ins0 = ex.ins;
		if (ins0 != 0x00000020)
		{
			if (utilized >= 3)
				printf("   mem%2d    |", utilized - 2);
			else
				printf("    NOP     |");
			utilized++;
		}
		else
			printf("    NOP     |");
	}
	void transfer()
	{
		result = result0;
		write = write0;
		type = type0;
		ALUresult = ALUresult0;
		mulresult = mulresult0;
		ins = ins0;
	}
};

class WB
{
public:
	int ins0;
	int utilized;
	int cycles;
	void calculate(MEM mem, int* Reg)
	{
		ins0 = mem.ins;
		cycles++;
		if(mem.write < 32 && mem.write > 0)
		{
			if(mem.type == 0x23)//lw
			{
				Reg[mem.write] = mem.result;
			}
			else if(mem.type == 0xff)
			{
				Reg[mem.write] =((unsigned long long int)mem.mulresult) << 32 >> 32;
				Reg[mem.write + 1] = ((unsigned long long int)mem.mulresult) >> 32;
			}
			else if(mem.type == 0x00 || mem.type == 0x0f || mem.type == 0x0c || mem.type == 0x0d)//R
			{
				Reg[mem.write] = mem.ALUresult;
			}
		}
	}
	void transfer()
	{
		
	}
	void display()
	{
		 if (ins0 != 0x00000020)
		{
			if (utilized >= 4)
				printf("   wb%2d  |", utilized - 3);
			else
				printf("   NOP   |");
			utilized++;
		}
		else
			printf("   NOP   |");
	}
};

int stringtoint(char *c)
{
	int value = 0;
	for(int i = 0; i < 8; i++)
	{
		if(c[i] >= '0' && c[i] <= '9')
		{
			value += (c[i] - '0') << (4 * (7 - i));
		}
		else if (c[i] >= 'A' && c[i] <= 'F')
		{
			value += (c[i] - 'A' + 10) << (4 * (7 - i));
		}
		else if (c[i] >= 'a' && c[i] <= 'f')
		{
			value += (c[i] - 'a' + 10) << (4 * (7 - i));
		}
	}
	return value;
}

void readByLine(int* IMem, string file)
{
    char buffer[9];
    fstream outFile;
    outFile.open(file,ios::in);
    for (int i = 0; !outFile.eof(); i++)
    {
        outFile.getline(buffer,9,'\n');
        IMem[i] = stringtoint(buffer);
    }
    outFile.close();
}

int adjusttozero(int a)
{
	 if(a >= 0)
	 	return a;
	 else 
	 	return 0;
}

void display(int* Reg, int* DMem)
{
	printf(" %8x |", Reg[8]);
	printf(" %8x |", Reg[9]);
	printf(" %8x |", Reg[10]);
	printf(" %8x |", Reg[11]);
	printf(" %8x |", Reg[12]);
    printf(" %8x |", Reg[13]);
	printf(" %8x |", Reg[14]);
	printf(" %8x |", Reg[15]);
	printf(" %8x |", DMem[0]);
	printf(" %8x |", DMem[1]);
	printf(" %8x |", DMem[2]);
	printf(" %8x \n", DMem[3]);
}

int main(int argc, char const *argv[])
{
	int IMem[256] = {0};
	int DMem[256] = {0};
	int Reg[32] = {0};
	int ProgramC = 0;
	cout << "Please choose the mode by printing 1 or 2: 1-instruction mode 2-cycle mode" << endl;
	int mode;
	cin >> mode;
	int number = 0;
	int ifcontinue = 1;
	int totalnumber = 0;
	readByLine(IMem, "mips.txt");
	IF fetch; fetch.utilized = 0;
	ID id;    id.utilized = 0;
	EX ex;	  ex.utilized = 0;
	MEM mem;  mem.utilized = 0;
	WB wb;    wb.utilized = 0; 
	wb.cycles = 0;

	if (mode == 1)
	{
		while(ifcontinue)
		{
			cout << "Please print the instructions you'd like to run" << endl;
			cin >> number;
			cout << "clk|      IF stage      |  ID stage |  EX stage |  MEM stage | WB stage|   $t0    |   $t1    |   $t2    |   $t3    |   $t4    |   $t5    |   $t6    |   $t7    |   M[0]   |   M[1]   |   M[2]   |   M[3]   "<< endl;
			totalnumber += number;
			for (int i = 0; (wb.utilized - 4) != totalnumber; i++)
			{
				printf("%2d | ", wb.cycles+1);
				ex.calculate(id, IMem, &ProgramC);
				fetch.calculate(&ProgramC, id.write, ex.write, id.type, ex.type, IMem);
				wb.calculate(mem, Reg);// implementing WB before reading, RAW
				id.calculate(fetch, Reg, ex.write, ex.type);
				ex.display();
				mem.calculate(ex, DMem, Reg);
				wb.display();

				fetch.transfer();
				id.transfer();
				ex.transfer(); 
				mem.transfer();
				wb.transfer();
				display(Reg, DMem);
			}	
			cout << number << " instructions have been run, 1 - continue, 0 - quit." << endl;
			cin >> ifcontinue;
		}
		cout << "TOTAL TIME: " << wb.cycles << " cycles" << endl;
		cout << "Utilization of IF  stage: " << fetch.utilized << "/" << wb.cycles << "," << endl;
		cout << "Utilization of ID  stage: " << adjusttozero(id.utilized - 1) << "/" << wb.cycles << "," << endl;
		cout << "Utilization of EX  stage: " << adjusttozero(ex.utilized - 2) << "/" << wb.cycles << "," << endl;
		cout << "Utilization of MEM stage: " << adjusttozero(mem.utilized - 3) << "/" << wb.cycles << "," << endl;
		cout << "Utilization of WB  stage: " << totalnumber << "/" << wb.cycles << "." << endl;
	}
	else if (mode == 2)
	{
		while(ifcontinue)
		{
			cout << "Please print the cycles you'd like to run" << endl;
			cin >> number;
			cout << "clk|      IF stage      |  ID stage |  EX stage |  MEM stage | WB stage|   $t0    |   $t1    |   $t2    |   $t3    |   $t4    |   $t5    |   $t6    |   $t7    |   M[0]   |   M[1]   |   M[2]   |   M[3]   "<< endl;
			totalnumber += number;
			for(int i = 0; i < number; i++)
			{
				printf("%2d | ", wb.cycles + 1);
				ex.calculate(id, IMem, &ProgramC);//adjust for beq stall only once
				fetch.calculate(&ProgramC, id.write, ex.write, id.type, ex.type, IMem);
				wb.calculate(mem, Reg);// implementing WB before reading, RAW
				id.calculate(fetch, Reg, ex.write, ex.type);
				ex.display();//for output in order
				mem.calculate(ex, DMem, Reg);
				wb.display();//for output in order

				fetch.transfer();
				id.transfer();
				ex.transfer(); 
				mem.transfer();
				wb.transfer();
				display(Reg, DMem);
			}
			cout << number << " cycles have been run, 1 - continue, 0 - quit." << endl;
			cin >> ifcontinue;
		}
		cout << "TOTAL TIME: " << totalnumber << " cycles" << endl;
		cout << "Utilization of IF  stage: " << fetch.utilized << "/" << totalnumber << ","<< endl;
		cout << "Utilization of ID  stage: " << adjusttozero(id.utilized - 1) << "/" << totalnumber << "," << endl;
		cout << "Utilization of EX  stage: " << adjusttozero(ex.utilized - 2)<< "/" << totalnumber << "," << endl;
		cout << "Utilization of MEM stage: " << adjusttozero(mem.utilized - 3) << "/" << totalnumber << "," << endl;
		cout << "Utilization of WB  stage: " << adjusttozero(wb.utilized - 4)<< "/" << totalnumber << "." << endl;
	}
	return 0;
}


