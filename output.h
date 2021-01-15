#pragma once
/*
output.h

responsible on all the outputs of the program
*/


/*------------------------------------------------------------------------------------
										INCLUDE
------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core.h"
#include "HardCodedData.h"

/*------------------------------------------------------------------------------------
										DEFINES
------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------
										DECLARATION
------------------------------------------------------------------------------------*/
//create all the output files that not have to update during the execution.
void write_output_files(char **args,int *regs_0,int *regs_1,int *regs_2,int *regs_3, 
	int *dsram_0, int *dsram_1, int* dsram_2, int *dsram_3, 
	int* tsram_0, int* tsram_1, int* tsram_2, int* tsram_3
	, int *mem, STAT stat_0, STAT stat_1, STAT stat_2, STAT stat_3);

//create stat output file
void create_stat_output(STAT stat, char file_name[]);

//create the dsram output file.
void create_dsram_output( int* dsram, char file_name[]);

//create the tsram output file.
void create_tsram_output(int* tsram, char file_name[]);

//function to create the regout file
void create_regout(int regs[], char file_name[]);

//function to create the memout file
void create_memout(int* mem, char file_name[]);

//A function that converts a negative number to positive in 2's compliment
int neg_to_pos(signed int num);

//responsible to close all the trace files
void close_trace_files(FILE* core_0_trace, FILE* core_1_trace, FILE* core_2_trace, FILE* core_3_trace, FILE* bus_trace);





