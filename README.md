/*test machine: CSEL-LINDL102-32
* date: 03/28/24
* name: Sharmarke Ahmed, Bradley Johnson, John Van Sloun
* x500: ahme0344, joh22202, vansl060
*/

--Extra Credit Has Been Attempted--

Contributions:
- Sharmarke Ahmed: get_tlb_entry(), populate_tlb(), LFU usage implementation, tlb initialization, fixed error in get_hit_ratio(), LFU vs FIFO comparison
- Bradley Johnson: get_hit_ratio(), print_tlb(), fixed issues in initialize_vmanager(), print_physical_address(), populate_tlb(), and main.c
- John Van Sloun: main.c, tlb initialization, translate_virtual_address(), print_physical_address()


Test Cases:
- virtual.txt

LFU vs FIFO Comparison:
Two additional test cases, comparisontest.txt and comparisontest2.txt, were used to compare the performance of LFU vs FIFO. Under comparisontest.txt, a few of the virtual addresses were re-listed multiple times, first to initially add the virtual address into the TLB, another soon after to get the TLB entry again, and then the TLB entry was requested again much later in the code. Under this test case, LFU performed better, with a hit rate of 37.5%, compared to a hit rate of 18.75% for FIFO. This test case illustrated the benefit of LFU in identifying more frequently used virtual addresses in the TLB and preserving them. Under comparisontest2.txt, the same 8 virtual addresses are repeated twice in the file, which means the main code would get the tlb entry of each virtual address twice. FIFO performed better, with a hit rate of 50%, versus a hit rate of 37.5% for the LFU. FIFO worked better in this case since the virtual addresses, haven been just recently added to the tlb, will be re-used again in the very near future. This highlights the limited benefit that LFU has, as for LFU to work more successfully than FIFO in caching memory requires virtual addresses to be used in the future to have a high probability of being chosen based on how frequently they were used in the past.

Assumptions:


Special Instructions:
