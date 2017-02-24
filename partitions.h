#ifndef PARTITIONS_H
#define PARTITIONS_H

#include <stdint.h>
#include <stdbool.h>

#include "partition_table.h"

#define EXPAND_AS_ENUM(name, ...) name,

#define EXPAND_AS_PARTITIONS_SIZES_INIT(name, partition_size) \
	{.size = (partition_size)},

/*
 * PARTITION_TABLE should be defined
 */
typedef enum {
	PARTITION_TABLE(EXPAND_AS_ENUM)
	PARTITIONS_MAX_NUM
} partition_id_t;

	
/*
 * Partition descriptor.
 * Sizes initialized according to PARTITIONS_TABLE,
 * origins calculated within partitions_init()
 */
typedef struct {
	uint32_t size;
	uint32_t origin;
} partition_t;


bool partitions_init(uint32_t *addr);
int partition_copy(partition_id_t dst_id, partition_id_t src_id_t);
bool partition_erase(partition_id_t id);
bool partition_is_empty(partition_id_t id);
int partition_get_origin(partition_id_t id);
int partition_get_size(partition_id_t id);
void partitions_test(void);



#endif /* PARTITIONS_H */
