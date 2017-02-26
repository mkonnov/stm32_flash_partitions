#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

#define FLASH_PAGE_SIZE 2048
#define FLASH_START 0x08000000
/* address by page number */
#define FLASH_PAGE_START_ADDR(PAGE_N) (FLASH_START + FLASH_PAGE_SIZE * PAGE_N)
/* page number by address */
#define FLASH_PAGE_NUM(ADDR) ((ADDR - FLASH_START) / FLASH_PAGE_SIZE)

extern void flash_unlock(void);
extern void flash_lock(void);
extern int flash_program_16bit(uint32_t address, uint16_t data);
int flash_write_block(uint32_t src, uint32_t dst, int size);
extern int flash_erase_page(uint32_t address);
void test_flash(void);
#endif /* FLASH_H */
