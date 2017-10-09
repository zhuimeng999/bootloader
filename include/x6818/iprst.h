/*
 * iprst.h
 *
 *  Created on: 2017年9月28日
 *      Author: lucius
 */

#ifndef X6818_IPRST_H_
#define X6818_IPRST_H_

#include <stdint.h>

struct iprst_type_t {
	volatile uint32_t reg[3];
};

#define IPRST_BASE (0xC0012000)
#define IPRST ((struct iprst_type_t *)IPRST_BASE)


static inline void iprst_do_reset(uint32_t index, uint32_t offset)
{
	struct iprst_type_t *iprst = IPRST;
	uint32_t tmp = iprst->reg[index];
	tmp &= ~(1 << offset);
	iprst->reg[index] = tmp;
	tmp |=  (1 << offset);
	iprst->reg[index] = tmp;
}

static inline void iprst_assert(uint32_t index, uint32_t offset)
{
	struct iprst_type_t *iprst = IPRST;
	uint32_t tmp = iprst->reg[index];
	tmp &= ~(1 << offset);
	iprst->reg[index] = tmp;
}

static inline void iprst_release(uint32_t index, uint32_t offset)
{
	struct iprst_type_t *iprst = IPRST;
	uint32_t tmp = iprst->reg[index];
	tmp |=  (1 << offset);
	iprst->reg[index] = tmp;
}

#endif /* X6818_IPRST_H_ */
