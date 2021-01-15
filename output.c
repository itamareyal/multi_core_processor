/*
output.c
*/


/*------------------------------------------------------------------------------------
										INCLUDE
------------------------------------------------------------------------------------*/
#include "output.h"

/*------------------------------------------------------------------------------------
										DEFINES
------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------
										DECLARATION
------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------
										IMPLEMENTATION
------------------------------------------------------------------------------------*/
void write_output_files(char** args, int* regs_0, int* regs_1, int* regs_2, int* regs_3,
	int* dsram_0, int* dsram_1,int* dsram_2, int* dsram_3,
	int* tsram_0, int* tsram_1, int* tsram_2, int* tsram_3
	, int* mem,STAT stat_0, STAT stat_1, STAT stat_2, STAT stat_3)
{
	create_memout(mem, args[6]);
	create_regout(regs_0, args[7]);
	create_regout(regs_1, args[8]);
	create_regout(regs_2, args[9]);
	create_regout(regs_3, args[10]);
	create_dsram_output(dsram_0, args[16]);
	create_dsram_output(dsram_1, args[17]);
	create_dsram_output(dsram_2, args[18]);
	create_dsram_output(dsram_3, args[19]);
	create_tsram_output(tsram_0, args[20]);
	create_tsram_output(tsram_1, args[21]);
	create_tsram_output(tsram_2, args[22]);
	create_tsram_output(tsram_3, args[23]);
	create_stat_output(stat_0, args[24]);
	create_stat_output(stat_1, args[25]);
	create_stat_output(stat_2, args[26]);
	create_stat_output(stat_3, args[27]);
}

void create_stat_output(STAT stat, char file_name[]) {
	
	//initinlize parameters
	FILE* fp_statout = NULL;

	fopen_s(&fp_statout, file_name, "w"); // open new file
	if (fp_statout == NULL) // handle error
	{
		printf("error opening file");
		exit(1);
	}

	fprintf(fp_statout, "cycles %d\n", stat.cycles);
	fprintf(fp_statout, "instructions %d\n", stat.instructions);
	if (stat.read_hit < 0)//special case
		stat.read_hit = 0;
	fprintf(fp_statout, "read_hit %d\n", stat.read_hit);
	if (stat.write_hit>0)
		stat.write_hit += 1;
	fprintf(fp_statout, "write_hit %d\n", stat.write_hit);
	fprintf(fp_statout, "read_miss %d\n", stat.read_miss);
	fprintf(fp_statout, "write_miss %d\n", stat.write_miss);
	fprintf(fp_statout, "decode_stall %d\n", stat.decode_stall);
	fprintf(fp_statout, "mem_stall %d\n", stat.mem_stall);

	fclose(fp_statout); // close file
}

void create_dsram_output(int* dsram, char file_name[]) {
	FILE* fp_memout = NULL;

	fopen_s(&fp_memout, file_name, "w"); // open new file
	if (fp_memout == NULL) // handle error
	{
		printf("error opening file");
		exit(1);
	}
	for (int i = 0; i < DSRAM_SIZE; i++) // print memory to file
	{
		fprintf(fp_memout, "%08X\n", *dsram);
		dsram++;
	}
	fclose(fp_memout); // close file
}

void create_tsram_output(int* tsram, char file_name[]) {
	FILE* fp_memout = NULL;
	fopen_s(&fp_memout, file_name, "w"); // open new file
	if (fp_memout == NULL) // handle error
	{
		printf("error opening file");
		exit(1);
	}
	for (int i = 0; i < TSRAM_SIZE; i++) // print memory to file
	{
		fprintf(fp_memout, "%08X\n", *tsram);
		tsram++;
	}
	fclose(fp_memout); // close file
}

// this function creates regout file
void create_regout(int regs[], char file_name[]) {
	FILE* fp_regout=NULL;

	fopen_s(&fp_regout, file_name, "w"); // open new file
	if (fp_regout == NULL) // handle error
	{
		printf("error opening file");
		exit(1);
	}
	for (int i = 2; i <= 15; i++) // print registers to file
	{
		fprintf(fp_regout, "%08X\n", regs[i]);
	}
	fclose(fp_regout); // close file
}

void create_memout(int* mem, char file_name[]) {
	FILE* fp_memout = NULL;
	fopen_s(&fp_memout, file_name, "w"); // open new file
	if (fp_memout == NULL) // handle error
	{
		printf("error opening file");
		exit(1);
	}
	
	for (int i = 0; i < MAIN_MEM_SIZE ; i++) // print memory to file
	{
		fprintf(fp_memout, "%08X\n", *mem);
		mem++;
	}
	fclose(fp_memout); // close file
}

//A function that converts a negative number to positive in 2's compliment
int neg_to_pos(signed int num)
{
	num = abs(num);
	signed int mask = 0xffffffff;
	num = num ^ mask; // invert all bits
	num++; // add 1 as in 2's comp
	return num;
}
