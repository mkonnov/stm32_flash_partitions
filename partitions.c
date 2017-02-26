#include "flash.h"
#include "partitions.h"

partition_t partitions[PARTITIONS_MAX_NUM] = {
	PARTITION_TABLE(EXPAND_AS_PARTITIONS_SIZES_INIT)
};

bool partitions_init(uint32_t *addr) {
	int8_t i;
	for (i = 0; i < PARTITIONS_MAX_NUM; i++) {
		/* first partition */
		if (i == 0)
			partitions[i].origin = (uint32_t)addr;
		else
			partitions[i].origin = partitions[i-1].origin + partitions[i-1].size;
	}
	return true;
}

int partition_copy(partition_id_t dst_id, partition_id_t src_id) {
	int res;
	if (partitions[dst_id].size < partitions[src_id].size)
		return -3;
	__disable_irq();
	partition_erase(dst_id);
	flash_unlock();
	res = FLASH_WRITE_FUNC(partitions[dst_id].origin, 
				partitions[src_id].origin,
				partitions[src_id].size);
	flash_lock();
	__enable_irq();
	return res;
}

bool partition_erase(partition_id_t id) {
	bool res = true;
	uint16_t start_page_num = FLASH_PAGE_NUM(partitions[id].origin);
	uint16_t end_page_num = start_page_num + (partitions[id].size / FLASH_PAGE_SIZE);
	uint16_t cur_page_num = start_page_num;
	__disable_irq();
	flash_unlock();
	for (; cur_page_num< end_page_num; cur_page_num++) {
		if (flash_erase_page(FLASH_PAGE_START_ADDR(cur_page_num)) < 0) {
			res = false;
			break;
		}
	}
	flash_lock();
	__enable_irq();
	return res;
}

bool partition_is_empty(partition_id_t id) {
	int i;
	int size = partitions[id].size / sizeof(int);
	for (i = 0; i < size; i++) {
		if (*(unsigned int*)(partitions[id].origin + i * sizeof(int)) != 0xffffffff) {
			return false;
			break;
		}
	}
	return true;
}

int partition_get_origin(partition_id_t id) {
	return partitions[id].origin;
}

int partition_get_size(partition_id_t id) {
	return partitions[id].size;
}
