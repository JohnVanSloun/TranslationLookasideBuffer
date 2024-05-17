//Implement the API modeling the translation of virtual page address to a
//physical frame address. We assume a 32 bit virtual memory and physical memory.
//Access to the page table is only via the CR3 register.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vmemory.h"

#define OUT_TLB "../bin/tlb_out.txt"

bool FIFO_policy = true;
int **cr3;
//
// You can declare global variables necessary for your TLB implementation here
// TODO 1: Add global variables here as necessary
// TODO 2: Define the TLB with correct size and dimension
// [Hint]: You can use static variables like hit_count, miss_count, front or back for FIFO
//

// tlb holds 8 virtual addresses and their corresponding physical frame base addresses, and how often an item is used (LFU mode only)
static int tlb[8][3];
static int write = 0; // store the next index to write to the TLB buffer

static int hit_count = 0;
static int miss_count = 0;

// Initializing the tlb to not hold any valid addresses initially (to be populated later)
static void initialize_tlb()
{
	for (int i = 0; i < 8; i++) {
		tlb[i][0] = -1;
		tlb[i][1] = -1;
		tlb[i][2] = 0;
	}
}
//
// More static helper functions can be implemented here
//

// The implementation of get_vpage_cr3 is provided in 
// an object file, so no need to re-implement it
void initialize_vmanager(int policy) 
{
	// Set LFU policy when passed as a parameter
	if (policy)
		FIFO_policy = false;
	// Set base pointer to page table
	cr3 = get_vpage_cr3();
	
	
	// You can add other initialization here as necessary
	initialize_tlb();
}

// Translates the virtual address provided to its corresponding physical address in the page table
int translate_virtual_address(unsigned int v_addr)
{
	// Store the page table indexes and offset separately
	int l1_index = (v_addr >> 22) & 0x3ff;
	int l2_index = (v_addr >> 12) & 0x3ff;
	int offset = v_addr & 0xfff;

	// Check if physical address in page table is valid
	if ((cr3[l1_index] == NULL) || (cr3[l1_index][l2_index] == -1)) {
		return -1;
	}

	// Create physical address from page table and offset
	int p_addr = (cr3[l1_index][l2_index] << 12) | offset;

	return p_addr;
}

// Prints out the physical address as a hexadecimal number when provided the physical address frame and offset
void print_physical_address(int frame, int offset)
{

	// Create full physical address from the frame and offset numbers
	int full_addr = (frame << 12) | offset;

	// Check if full address is valid
	if (full_addr == -1) {
		printf("-1\n");
		return;
	}

	// Print the physical address formatted as a hexadecimal number
	printf("0x%08x\n", full_addr);

	return;
}

int get_tlb_entry(int n)
{
	int index = 0; // linear search for location of virtual address in TLB
	while( (index < 8) && (tlb[index][0] != n)) {
		index++;
	}

	if(index == 8) { // TLB miss
		miss_count++;
		return -1;
	}
	hit_count++;
	tlb[index][2]++; // LFU usage iteration
	return tlb[index][1]; // TLB hit

}

void populate_tlb(int v_addr, int p_addr) 
{
	if (get_tlb_entry(v_addr) == -1) { // TLB miss. Populate TLB
		if(FIFO_policy) { // FIFO policy
			tlb[write][0] = v_addr;
			tlb[write][1] = p_addr;
			write++;
			if(write > 7) {
				write = 0;
			}
		}
		else { // LFU policy
			if(write < 7) { // TLB not full yet
				tlb[write][0] = v_addr;
				tlb[write][1] = p_addr;
				write++;
			}
			else { // evict element from TLB which is less frequently used
				int smallest_index = 0; // index corresponding to tlb element which was read the least.
				int i = 0;
				while(i < 8) { // linear search for least read element in tlb
					if(tlb[smallest_index][2] > tlb[i][2]) {
						smallest_index = i;
					}
					i++;
				}
				tlb[smallest_index][0] = v_addr;
				tlb[smallest_index][1] = p_addr;
				tlb[smallest_index][2] = 0;
			}
		}
	}
	return;
}


// hit ratio = #hits / (#hits + #misses), # = total number of
float get_hit_ratio()
{
	if (hit_count + miss_count != 0) {
		return hit_count / (float) (hit_count + miss_count);
	}
	else {
		return 0.0;
	}
}

// Write to the file denoted by OUT_TLB
void print_tlb()
{
	// Open the file in append mode
	FILE *file = fopen(OUT_TLB, "a");
	for (int i = 0; i < 8; i++) {
		// Write the TLB to the file in the correct format
		if (tlb[i][0] != -1) {
			unsigned int v_page = tlb[i][0];
			unsigned int p_frame = tlb[i][1];
			fprintf(file, "0x%05x 0x%05x\n", v_page, p_frame);
		}
		// TLB entry not yet set
		else {
			fprintf(file, "-1 -1\n");
		}
	}
	fprintf(file, "\n");
	fclose(file);
	return;
}
