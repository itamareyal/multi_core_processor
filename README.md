# multi_core_processor
This project implements a simulation of a multi-core processor in C.
the processor has 4 cores, each has it’s own cache in write-back, write allocate.
All cores connected to the same main memory via one bus, handling one transaction at a given cycle.
data is flushed between cores and mem using the MSI system, stored in the tsram of each core.
Cores are able to share data and coordinate writing with the bus and the ll & sc commands.
