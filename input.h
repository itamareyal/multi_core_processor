#pragma once
/*
input.h
*/


/*------------------------------------------------------------------------------------
										INCLUDE
------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HardCodedData.h"

/*------------------------------------------------------------------------------------
										DEFINES
------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------
										DECLARATION
------------------------------------------------------------------------------------*/
char** check_args(int argc, char* argv[]);

FILE* open_file(char* file_name, char* file_open_type);

// open imem & mem files and read them to arrays
int open_mem_files(int argc, char* argv[], int imem_0[], int imem_1[], int imem_2[], int imem_3[], int mem[]);

int read_memin_imem(int* mem, char* address);

// open trace file in w mode
int open_trace_files(char* args[], FILE** core_0_trace, FILE** core_1_trace, FILE** core_2_trace, FILE** core_3_trace, FILE** bus_trace);

/*------------------------------------------------------------------------------------
										IMPLEMENTATION
------------------------------------------------------------------------------------*/
