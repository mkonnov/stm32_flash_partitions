#include "flash.h"
#include "stm32f10x_flash.h"

#define FLASH_KEY1	((uint32_t)0x45670123)
#define FLASH_KEY2	((uint32_t)0xCDEF89AB)

#define TIMEOUT_ERASE	((uint32_t)0x000B0000)
#define TIMEOUT_PROGRAM	((uint32_t)0x00002000)

static uint16_t data_flash[512];

static int flash_busywait(uint32_t timeout) {
	uint32_t status;
	do {
		status = FLASH->SR;
		if (status & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR))
			return -1;
		timeout--;
	} while (status & FLASH_SR_BSY && timeout);
	if (status & FLASH_SR_BSY)
		return -2;
	else
		return 0;
}

void flash_unlock(void) {
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void flash_lock(void) {
	FLASH->CR |= FLASH_CR_LOCK;
}

int flash_erase_page(uint32_t address) {
	int res = 0;
	if ((res = flash_busywait(TIMEOUT_ERASE)) < 0)
		return res;
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = address;
	FLASH->CR |= FLASH_CR_STRT;
	res = flash_busywait(TIMEOUT_ERASE);
	FLASH->CR &= ~FLASH_CR_PER;
	return res;
}

int flash_program_16bit(uint32_t address, uint16_t data) {
	int res = 0;
	if ((res = flash_busywait(TIMEOUT_PROGRAM)) < 0)
		return res;
	FLASH->CR |= FLASH_CR_PG;
	*(uint16_t *)address = data;
	res = flash_busywait(TIMEOUT_PROGRAM);
	FLASH->CR &= ~FLASH_CR_PG;
	return res;
}

int flash_write_block(uint32_t src, uint32_t dst, int size) {
	int res = 0;
	flash_unlock();
	__disable_irq();
	while (size > 0) {
		res = flash_program_16bit(src, *(uint16_t*)(dst));
		if (res != 0)
			break;
		src += 2;
		dst += 2;
		size -=2;
	}
	flash_lock();
	__enable_irq();
	return res;
}


void test_flash(void) {
	int test_data_addr = 0x08009000;
	for (int i = 0; i < 512; i++) {
		data_flash[i] = *(uint16_t*)(test_data_addr + i * 2);
	}

	flash_unlock();

	flash_erase_page(test_data_addr);
	
	for (int i = 0; i < 512; i++) {
		flash_program_16bit(test_data_addr + i * 2, 0xa5);
	}

	flash_lock();

	for (int i = 0; i < 512; i++) {
		data_flash[i] = *(uint16_t*)(test_data_addr + i * 2);
	}
}




