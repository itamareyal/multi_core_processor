/*
main.c
*/

/*------------------------------------------------------------------------------------
										INCLUDE
------------------------------------------------------------------------------------*/
#include "input.h"
#include "output.h"
#include "core.h"
#include "HardCodedData.h"

/*------------------------------------------------------------------------------------
										IMPLEMENTATION
------------------------------------------------------------------------------------*/
// initialize main memory
int mem[MAIN_MEM_SIZE] = { 0 };

// initiali registers for each core
int regs_0[NUMBER_REGISTER_SIZE] = { 0 };
int regs_1[NUMBER_REGISTER_SIZE] = { 0 };
int regs_2[NUMBER_REGISTER_SIZE] = { 0 };
int regs_3[NUMBER_REGISTER_SIZE] = { 0 };

// initialize instruction memory for each core
int imem_0[I_MEM_SIZE] = { 0 };
int imem_1[I_MEM_SIZE] = { 0 };
int imem_2[I_MEM_SIZE] = { 0 };
int imem_3[I_MEM_SIZE] = { 0 };

// initialize DSRAM for each core
int dsram_0[DSRAM_SIZE] = { 0 };
int dsram_1[DSRAM_SIZE] = { 0 };
int dsram_2[DSRAM_SIZE] = { 0 };
int dsram_3[DSRAM_SIZE] = { 0 };

// initialize TSRAM for each core
static int tsram_0[TSRAM_SIZE] = { 0 };
static int tsram_1[TSRAM_SIZE] = { 0 };
static int tsram_2[TSRAM_SIZE] = { 0 };
static int tsram_3[TSRAM_SIZE] = { 0 };

int main(int argc, char* argv[]) {

	BUS last_bus; // holds last trans on the bus for snooping at next iteration
	BUS prev_bus;//keep the previus bus before the change
	int* bus_busy = 0; // 0-free for trans; 1-busy, wait for flush

	// initialize cycle counter and 4 pc for each of the 4 cores.
	int* cycle=0; 
	int pc_0 = 0;
	int pc_1 = 0;
	int pc_2 = 0;
	int pc_3 = 0;

	// initialize pipeline for each of the cores
	PIPE pipe_0;
	PIPE pipe_1;
	PIPE pipe_2;
	PIPE pipe_3;

	//initialize watch for all cores
	Watch watch;

	// initialize stats for each of the cores
	STAT stat_0;
	STAT stat_1;
	STAT stat_2;
	STAT stat_3;
	
	// initialize trace file for each of the cores and also one bus trace
	FILE* core_0_trace = NULL;
	FILE* core_1_trace = NULL;
	FILE* core_2_trace = NULL;
	FILE* core_3_trace = NULL;
	FILE* bus_trace = NULL;

	// read mem & imem files to arrays
	if (0 != open_mem_files(argc, argv, imem_0, imem_1, imem_2, imem_3, mem)){
		printf("Error opening mem files.\n");
		exit(1);
	}

	// open live trace files for write
	if (0 != open_trace_files(argv, &core_0_trace, &core_1_trace, &core_2_trace, &core_3_trace, &bus_trace)) {
		printf("Error opening trace files.\n");
		exit(1);
	}

	initilize_watch(&watch);

	initilize_pipelines(&pipe_0, &pipe_1, &pipe_2, &pipe_3);

	initilize_all_stats(&stat_0, &stat_1, &stat_2, &stat_3);

	initilize_bus(&last_bus);
	initilize_bus(&prev_bus);

	// multi core execution loop. exits when all cores are done. 
	while (pc_0 != -1 || pc_1 != -1 || pc_2 != -1 || pc_3 != -1) {
		// execute for each core
		pc_0 = core_execution(cycle, pc_0, 0, imem_0, regs_0,&pipe_0, core_0_trace, &last_bus,dsram_0, tsram_0, &stat_0, &watch);
		pc_1 = core_execution(cycle, pc_1, 1, imem_1, regs_1, &pipe_1,core_1_trace, &last_bus, dsram_1, tsram_1, &stat_1, &watch);
		pc_2 = core_execution(cycle, pc_2, 2, imem_2, regs_2, &pipe_2, core_2_trace, &last_bus, dsram_2, tsram_2, &stat_2, &watch);
		pc_3 = core_execution(cycle, pc_3, 3, imem_3, regs_3, &pipe_3, core_3_trace, &last_bus, dsram_3, tsram_3, &stat_3, &watch);
	
		execution_bus(&last_bus, cycle, mem );

		char line_for_bus[MAX_LINE_TRACE] = { 0 }; //create line for bus trace file
		if (last_bus.bus_cmd != 0 && compare_bus(&prev_bus,&last_bus)==0) {
			create_line_for_bus(line_for_bus, cycle, &last_bus);
			fprintf_s(bus_trace, "%s\n", line_for_bus);
		}

		// if the prev is also a flush then we already wrote it, not feasable to get 2 flushes in a row.
		if(last_bus.bus_cmd==3 && prev_bus.bus_cmd==3)
			initilize_bus(&last_bus);

		copy_bus(&prev_bus, &last_bus);
		
		cycle+=1;
		
	}
	// write memout, regout x4, dsram x4, tsram x4, stats x4
	write_output_files(argv, regs_0, regs_1, regs_2, regs_3,
		dsram_0, dsram_1, dsram_2, dsram_3,
		tsram_0, tsram_1, tsram_2, tsram_3
		,mem,stat_0,stat_1,stat_2,stat_3);

	//close all files
	close_trace_files(core_0_trace, core_1_trace, core_2_trace, core_3_trace, bus_trace);

	return 0;
}

