/*
 * tieoff.h
 *
 *  Created on: 2017年9月28日
 *      Author: lucius
 */

#ifndef X6818_TIEOFF_H_
#define X6818_TIEOFF_H_

#include <stdint.h>

struct s5p6818_tieoff_reg
{
  volatile uint32_t con[131];
};

#define TIEOFF_BASE (0xC0011000)
#define TIEOFF ((struct s5p6818_tieoff_reg *)TIEOFF_BASE)

#endif /* X6818_TIEOFF_H_ */
