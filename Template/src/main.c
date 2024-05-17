//main.c program to translate the virtual addresses from the virtual.txt
//into the physical frame address. Project 3 - CSci 4061

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "vmemory.h"

#define INPUT_FILE "../bin/virtual.txt"

int main(int argc, char* argv[])
{
	// Declare variables as necessary

	if (argc > 2) {
		printf("Too many arguments, enter up to one argument\n");
		exit(-1);
	}

	// "?" is a conditional operator that returns the second value if the first value is true
	int policy = (argc == 2) ? 1:0;
	initialize_vmanager(policy);

	int buf_len = 128;
	char buf[buf_len];
	int v_addr;
	int p_addr;
	int offset;

	// open input file and check for errors
	FILE* file = fopen(INPUT_FILE, "r");

	if (file == NULL) {
		perror("Error opening input file");
		return 1;
	}

	// Print the TLB to an output file
	print_tlb();

	// Read the input file line by line and populate the tlb with the entries and print translated addresses
	while (fgets(buf, buf_len, file)) {
		v_addr = strtol(buf, NULL, 16);
		p_addr = translate_virtual_address(v_addr);

		// Split the addresses in to frames and offsets
		v_addr = (v_addr >> 12) & 0xfffff;
		offset = (p_addr & 0xfff);
		p_addr = (p_addr >> 12) & 0xffffffff;

		// Print out the physical address
		print_physical_address(p_addr, offset);

		// add address to the TLB
		populate_tlb(v_addr, p_addr);
	}

	// Close the file
	fclose(file);

	// Print the hit ratio of the TLB
	printf("Hit rate of the cache is %f\n", get_hit_ratio());
	
	// Print the TLB to an output file
	print_tlb();

	//Free the page table
	free_resources(); // you don't need to worry about this function
	return 0;
}
