#include <string.h>

#include "stm32f10x.h"
#include "core_cm3.h"

#include "partitions.h"

static prtn_table_t *partition_table;
static prtn_callbacks_t *callbacks;


void partitions_register_callbacks(prtn_callbacks_t *cb) {
	callbacks = cb;
}

void partition_table_init(prtn_table_t *prtns) {
	int i = 0;
	partition_table = prtns;

	for (; i < PARTITIONS_MAX_COUNT; i++) {
		
	}
}


int partition_copy(char *dest, char *src) {
	int res;

	prtn_desc_t *src_d, *dest_d;
	
	src_d = partition_get_by_name(src);
	dest_d = partition_get_by_name(dest);

	if (dest_d->size < src_d->size)
		return -3;

	partition_erase(dest);
	
	res = callbacks->copy(dest_d->origin_addr,
				src_d->origin_addr,
				src_d->size);
	return res;
}

bool partition_erase(char *name) {
	bool res = true;

	prtn_desc_t *pd = partition_get_by_name(name);

	uint16_t start_page_num = (pd->origin_addr - \
		partition_table->flash_params.flash_start ) / \
		partition_table->flash_params.flash_size;

	uint16_t end_page_num = (start_page_num + pd->size) / \
		partition_table->flash_params.flash_size;

	uint16_t cur_page_num = start_page_num;

	uint32_t flash_page_start_addr;

	callbacks->critical_section_start();
	callbacks->flash_unlock();

	for (; cur_page_num < end_page_num; cur_page_num++) {
		flash_page_start_addr = partition_table->flash_params.flash_start + \
				partition_table->flash_params.flash_page_size * 
				cur_page_num;
		
		if (callbacks->erase_page(flash_page_start_addr) < 0) {
			res = false;
			break;
		}
	}
	
	callbacks->flash_lock();
	callbacks->critical_section_end();
	return res;
}

bool partition_is_empty(char *name) {
	int i;
	prtn_desc_t *pd = partition_get_by_name(name);
	int size = pd->size / sizeof(int);
	for (i = 0; i < size; i++) {
		if (*(unsigned int*)(pd->origin_addr + i * sizeof(int)) != 0xffffffff) {
			return false;
			break;
		}
	}
	return true;
}

prtn_desc_t *partition_get_by_name(char *name) {
	int i = 0;
	for (; i < PARTITIONS_MAX_COUNT; i++) {
		if (strcmp(partition_table->partitions[i].name, name) == 0)
			break;
	}
	return &(partition_table->partitions[i]);
}


