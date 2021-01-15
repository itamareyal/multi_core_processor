#pragma once
/*
core.h
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

// WATCH: struct to identify writes for ll & sc
typedef struct _watch { 
	int address[4];//the address we want to watch
	int lock[4];// if lock=0 its open, lock=1 core watch it,lock=2 someone perform SC before and we need to fail
}Watch,*Watch_ptr;

// BUS: holds transactions of data between main mem & cores
typedef struct _bus {
	int bus_origid; //3 bits. 0-3 cores; 4 mem
	int bus_cmd;	 //2 bits
	int bus_addr;	 //20 bits of address
	int bus_data;	 //32 bits of data. 1 word
	int prev_cmd;
	int flush_cycle;
	int data_owner;
	int data_destination;
	int bus_busy;
	int creation_cycle;
}BUS, *BUS_ptr;

//PIPE: holds all the information about the commands inside the pipeline
typedef struct _pipe {
	int core_id;
	int IF;
	int ID;
	int EX;
	int MEM;
	int WB;
}PIPE, * PIPE_ptr;

//STAT: hold statistics for each core
typedef struct _stat {
	int cycles; 
	int instructions;	 
	int read_hit;	 
	int write_hit;	 
	int read_miss;
	int write_miss;
	int decode_stall;
	int mem_stall;

}STAT, * STAT_ptr;

//COMMAND: holds cmd fields for a cmd in the pipline 
typedef struct _command {
	int opcode;
	int rd;
	int rs;
	int rt;
	int immiediate;
	int core_id;
}Command;

/*------------------------------------------------------------------------------------
										DECLARATION
------------------------------------------------------------------------------------*/


//execution of one core.
int core_execution(int* cycle, int pc, int core_id, int* imem, int* regs, 
	PIPE_ptr pipe, FILE* fp_trace, BUS_ptr last_bus, int* dsram,
	int* tsram, STAT_ptr stat, Watch_ptr watch);

//function to check if there is data hazard
int data_hazard(Command id, Command exe, Command mem, Command wb);

//responsible on compare betweem 2 commands and check the hazards if exists
int hazard_from_command(Command id, Command older);

//comapre 2 buses, 0 for identical
int compare_bus(BUS_ptr prev_bus, BUS_ptr curr_bus);

//copy the important values for the bus trace
void copy_bus(BUS_ptr prev_bus, BUS_ptr curr_bus);

//function for the snoop on the bus, executed by every core
void snoop_bus(BUS_ptr last_bus, int* tsram, int* cycle, int core_id, int* dsram);

// execution of all the transaction made by main memory
void execution_bus(BUS_ptr last_bus, int* cycle, int mem[]);

//initilize one pipeline
void init_pipe(int core_id, PIPE_ptr pipe);

//initiilize all the pipelines of all the cores
void  initilize_pipelines(PIPE_ptr pipe_0, PIPE_ptr pipe_1, PIPE_ptr pipe_2, PIPE_ptr pipe_3);

//each cycle update the pipline with the PC
void update_pipeline(PIPE_ptr pipe, int pc, STAT_ptr stat, BUS_ptr bus);

//function for initilize the bus for all the cores.
BUS_ptr initilize_bus(BUS_ptr Bus);

//get the tag from the address in memory
int get_tag(int address);

//get the tag from the tag in memory
int get_index(int address);

//responsible to open all trace files safety
void create_line_for_trace(char line_for_trace[], int regs[], int pc, int cycle, PIPE_ptr pipe);

//function for initilize the STAT structure for all the cores.
STAT_ptr initilize_STAT(STAT_ptr core_stat);

//init all stat of all cores
void  initilize_all_stats(STAT_ptr stat_0, STAT_ptr stat_1, STAT_ptr stat_2, STAT_ptr stat_3);

//initialize watch for all cores
void initilize_watch(Watch_ptr watch);

//create one line for the trace of the bus
void create_line_for_bus(char line_for_bus[], int cycle, BUS_ptr last_bus);

//responsible on the sign extention of the immidiate
int sign_extend(int imm);

//get the tag with bit manipulation
int get_tag_from_tsram(int line);

//get the msi with bit manipulation
int get_msi_from_tsram(int line);

//put new value for tsram
void put_msi_in_tsram(int* tsram, int index, int msi);

//put new value for tsram
void put_tag_in_tsram(int* tsram, int index, int tag);

//get us one byte from 32 bit
int get_byte(int num, int pos);

//get line in hex and convert it to type command
Command line_to_command(int inst, int core_id);

//execution of a cmd
int execution(int regs[], int pc, Command cmd, int* mem, BUS_ptr last_bus,
	int* dsram, int* tsram[], STAT_ptr stat,
	PIPE_ptr pipe, int* cycle, Watch_ptr watch);

//all the commands in order of the opcodes
void add(int* regs, Command cmd);
void sub(int* regs, Command cmd);
void and (int* regs, Command cmd);
void or (int* regs, Command cmd);
void xor (int* regs, Command cmd);
void mul(int* regs, Command cmd);
void sll(int* regs, Command cmd);
void sra(int* regs, Command cmd);
void srl(int* regs, Command cmd);
int beq(int* regs, Command cmd, int pc);
int bne(int* regs, Command cmd, int pc);
int blt(int* regs, Command cmd, int pc);
int bgt(int* regs, Command cmd, int pc);
int ble(int* regs, Command cmd, int pc);
int bge(int* regs, Command cmd, int pc);
int jal(int* regs, Command cmd, int pc);
void lw(int* regs, Command cmd, int* dsram);
void sw(int* regs, Command cmd, int* dsram, int* tsram);
void ll(int* regs, Command cmd, int* dsram, Watch_ptr watch, int core_id);
void sc(int* regs, Command cmd, int* dsram, Watch_ptr watch, int* tsram);
