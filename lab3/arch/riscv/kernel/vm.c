#include "defs.h"

/* early_pgtbl: 用于 setup_vm 进行 1GB 的 映射。 */
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));

void setup_vm(void) {
    /* 
    1. 由于是进行 1GB 的映射 这里不需要使用多级页表 
    2. 将 va 的 64bit 作为如下划分： | high bit | 9 bit | 30 bit |
        high bit 可以忽略
        中间9 bit 作为 early_pgtbl 的 index
        低 30 bit 作为 页内偏移 这里注意到 30 = 9 + 9 + 12， 即我们只使用根页表， 根页表的每个 entry 都对应 1GB 的区域。 
    3. Page Table Entry 的权限 V | R | W | X 位设置为 1
    */
    memset(early_pgtbl,0x0,PGSIZE);

    int index = PHY_START >> 30 & 0x1ff;

    early_pgtbl[index] = (PHY_START >> 30 & 0x3fff) << 28 | 15;
    index = VM_START >> 30 & 0x1ff;
    early_pgtbl[index] = (PHY_START >> 30 & 0x3fff) << 28 | 15;

    return;
}

/* swapper_pg_dir: kernel pagetable 根目录， 在 setup_vm_final 进行映射。 */
// unsigned long  swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));

// void setup_vm_final(void) {
//     memset(swapper_pg_dir, 0x0, PGSIZE);

//     // No OpenSBI mapping required

//     // mapping kernel text X|-|R|V
//     create_mapping(,,,,11);

//     // mapping kernel rodata -|-|R|V
//     create_mapping(,,,,3);
    
//     // mapping other memory -|W|R|V
//     create_mapping(,,,,7);
    
//     // set satp with swapper_pg_dir

//     YOUR CODE HERE

//     // flush TLB
//     asm volatile("sfence.vma zero, zero");
  
//     // flush icache
//     asm volatile("fence.i")
//     return;
// }


// /* 创建多级页表映射关系 */
// /* 不要修改该接口的参数和返回值 */
// create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
//     /*
//     pgtbl 为根页表的基地址
//     va, pa 为需要映射的虚拟地址、物理地址
//     sz 为映射的大小
//     perm 为映射的读写权限

//     创建多级页表的时候可以使用 kalloc() 来获取一页作为页表目录
//     可以使用 V bit 来判断页表项是否存在
//     */


// }

