/*
 * ddr3.h
 *
 *  Created on: 2017年10月7日
 *      Author: lucius
 */

#ifndef X6818_DDR3_H_
#define X6818_DDR3_H_

#include <stdint.h>

typedef struct {
  volatile uint32_t phy_con0;
  volatile uint32_t phy_con1;
  volatile uint32_t phy_con2;
  volatile uint32_t phy_con3;
  volatile uint32_t phy_con4;
  volatile uint32_t phy_con5;
  volatile uint32_t lp_con0;                      // 0x18
  volatile uint32_t r_odt_con0;                   // 0x1C
  volatile uint32_t  offset_r_con[3];                // 0x20 ~ 0x28
  volatile uint32_t  __rsvd0;                        // 0x2C
  volatile uint32_t  offset_w_con[3];                // 0x30 ~ 0x38
  volatile uint32_t  __rsvd1;                        // 0x3C
  volatile uint32_t  offset_c_con[3];                // 0x40 ~ 0x48
  volatile uint32_t  shift_c_con[1];                 // 0x4C
  volatile uint32_t  offsetd_con0;                // 0x50
  volatile uint32_t  __rsvd2;                        // 0x54
  volatile uint32_t  lp_ddr_con0;                  // 0x58 ~ 0x68
  volatile uint32_t  lp_ddr_con1;                  // 0x58 ~ 0x68
  volatile uint32_t  lp_ddr_con2;                  // 0x58 ~ 0x68
  volatile uint32_t  lp_ddr_con3;                  // 0x58 ~ 0x68
  volatile uint32_t  lp_ddr_con4;                  // 0x58 ~ 0x68
  volatile uint32_t  wr_lvl_con[4];                  // 0x6C ~ 0x78
  volatile uint32_t  ca_deskew_con[4];               // 0x7C ~ 0x88
  volatile uint32_t  __rsvd3[(0x94 - 0x8C) / 4];     // 0x8C ~ 0x90
  volatile uint32_t  ca_deskew_con4;                 // 0x94
  volatile uint32_t  __rsvd4;                        // 0x98
  volatile uint32_t  drv_ds_con[2];                  // 0x9C ~ 0xA0
  volatile uint32_t  __rsvd5[(0xB0 - 0xA4) / 4];     // 0xA4 ~ 0xAC
  volatile uint32_t  mdll_con[2];                    // 0xB0 ~ 0xB4
  volatile uint32_t  __rsvd6[(0xC0 - 0xB8) / 4];     // 0xB8 ~ 0xBC
  volatile uint32_t  zq_con0;                      // 0xC0 ~ 0xCC
  volatile uint32_t  zq_con1;                      // 0xC0 ~ 0xCC
  volatile uint32_t  zq_con2;                      // 0xC0 ~ 0xCC
  volatile uint32_t  zq_con3;                      // 0xC0 ~ 0xCC
  volatile uint32_t  t_rd_data_con[3];               // 0xD0 ~ 0xD8
  volatile uint32_t  cal_wl_stat;                    // 0xDC
  volatile uint32_t  cal_fail_stat[4];               // 0xE0 ~ 0xEC
  volatile uint32_t  cal_gt_vwmc[3];                 // 0xF0 ~ 0xF8
  volatile uint32_t  cal_gt_cyc;                     // 0xFC
  volatile uint32_t  cal_rd_vwmc[3];                 // 0x100 ~ 0x108
  volatile uint32_t  __rsvd7;                        // 0x10C
  volatile uint32_t  cal_rd_vwml[3];                 // 0x110 ~ 0x118
  volatile uint32_t  __rsvd8;                        // 0x11C
  volatile uint32_t  cal_rd_vwmr[3];                 // 0x120 ~ 0x128
  volatile uint32_t  __rsvd9;                        // 0x12C
  volatile uint32_t  cal_wr_vwmc[3];                 // 0x130 ~ 0x138
  volatile uint32_t  __rsvd10;                       // 0x13C
  volatile uint32_t  cal_wr_vwml[3];                 // 0x140 ~ 0x148
  volatile uint32_t  __rsvd11;                       // 0x14C
  volatile uint32_t  cal_wr_vwmr[3];                 // 0x150 ~ 0x158
  volatile uint32_t  __rsvd12;                       // 0x15C
  volatile uint32_t  cal_dm_vwmc[3];                 // 0x160 ~ 0x168
  volatile uint32_t  __rsvd13;                       // 0x16C
  volatile uint32_t  cal_dm_vwml[3];                 // 0x170 ~ 0x178
  volatile uint32_t  __rsvd14;                       // 0x17C
  volatile uint32_t  cal_dm_vwmr[3];                 // 0x180 ~ 0x188
} ddrphy_t;

typedef char sizecheck_ddrphy_t[(sizeof(ddrphy_t) - 0x18C)?-1:1];

#define DDR3_DDRPHY_BASE (0xC00E1000U)

#define DDR3_DDRPHY_MODE_OFFSET (11u)
#define DDR3_DDRPHY_MODE_MASK   (0b11 << DDR3_DDRPHY_MODE_OFFSET)
#define DDR3_DDRPHY_MODE_DDR3   (0b01u << DDR3_DDRPHY_MODE_OFFSET)

#define DDR3_DDRPHY_COMMAND_OFFSET (0u)
#define DDR3_DDRPHY_COMMAND_MASK (0x1FFFu << DDR3_DDRPHY_COMMAND_OFFSET)
#define DDR3_DDRPHY_COMMAND_READ (0x105Eu << DDR3_DDRPHY_COMMAND_OFFSET)
#define DDR3_DDRPHY_COMMAND_ACTIVE (0x107Fu << DDR3_DDRPHY_COMMAND_OFFSET)

#define DDR3_DDRPHY_UPD_OFFSET (28u)
#define DDR3_DDRPHY_UPD_MASK (0b1u << DDR3_DDRPHY_UPD_OFFSET)
#define DDR3_DDRPHY_UPD_MC (0b1u << DDR3_DDRPHY_UPD_OFFSET)
#define DDR3_DDRPHY_UPD_PHY (0b0u << DDR3_DDRPHY_UPD_OFFSET)

#define DDR3_DDRPHY_WRLAT_OFFSET (16u)
#define DDR3_DDRPHY_WRLAT_MASK (0x1Fu << DDR3_DDRPHY_WRLAT_OFFSET)

#define DDR3_DDRPHY_BSTLEN_OFFSET (8u)
#define DDR3_DDRPHY_BSTLEN_MASK (0x1Fu << DDR3_DDRPHY_BSTLEN_OFFSET)

#define DDR3_DDRPHY_RDLAT_OFFSET (0u)
#define DDR3_DDRPHY_RDLAT_MASK (0x1Fu << DDR3_DDRPHY_RDLAT_OFFSET)

#define DDR3_DDRPHY_ZQ_DIV_EN_OFFSET (18u)
#define DDR3_DDRPHY_ZQ_DIV_EN_MASK (0b1u << DDR3_DDRPHY_ZQ_DIV_EN_OFFSET)
#define DDR3_DDRPHY_ZQ_DIV_ENABLE (0b1u << DDR3_DDRPHY_ZQ_DIV_EN_OFFSET)
#define DDR3_DDRPHY_ZQ_DIV_DISABLE (0b0u << DDR3_DDRPHY_ZQ_DIV_EN_OFFSET)

#define DDR3_DDRPHY_ZQ_MODE_OFFSET (2u)
#define DDR3_DDRPHY_ZQ_MODE_MASK (0b11 << DDR3_DDRPHY_ZQ_MODE_OFFSET)
#define DDR3_DDRPHY_ZQ_MODE_FORCE (0b00 << DDR3_DDRPHY_ZQ_MODE_OFFSET)
#define DDR3_DDRPHY_ZQ_MODE_LONG (0b01 << DDR3_DDRPHY_ZQ_MODE_OFFSET)
#define DDR3_DDRPHY_ZQ_MODE_SHORT (0b10 << DDR3_DDRPHY_ZQ_MODE_OFFSET)

#define DDR3_DDRPHY_ZQ_STR_OFFSET (1u)
#define DDR3_DDRPHY_ZQ_STR_MASK (0b1 << DDR3_DDRPHY_ZQ_STR_OFFSET)
#define DDR3_DDRPHY_ZQ_STR_START (0b1 << DDR3_DDRPHY_ZQ_STR_OFFSET)
#define DDR3_DDRPHY_ZQ_STR_STOP (0b0 << DDR3_DDRPHY_ZQ_STR_OFFSET)

#define DDR3_DDRPHY_ZQ_DONE_OFFSET (0u)
#define DDR3_DDRPHY_ZQ_DONE (0b1u << DDR3_DDRPHY_ZQ_DONE_OFFSET)

#define DDR3_DDRPHY_PULLD_DQS_OFFSET (0u)
#define DDR3_DDRPHY_PULLD_DQS_MASK (0x1FF << DDR3_DDRPHY_PULLD_DQS_OFFSET)

typedef struct {
  volatile uint32_t con_control;                    // 0x0
  volatile uint32_t mem_control;                    // 0x4
  volatile uint32_t cg_control;                     // 0x8
  volatile uint32_t  __rsvd0,                        // 0xC
                direct_cmd,                     // 0x10
                prech_config_0,                 // 0x14
                phy_control[1],                 // 0x18
                prech_config_1,                 // 0x1C
                timing_rfc_pb,                  // 0x20
                __rsvd1,                        // 0x24
                pwr_dn_config,                  // 0x28
                timing_pzq,                     // 0x2C
                timing_aref,                    // 0x30
                timing_row,                     // 0x34
                timing_data_0,                  // 0x38
                timing_power_0;                 // 0x3C
  volatile uint32_t phy_status,                     // 0x40
                etc_timing,                     // 0x44
                chip_status,                    // 0x48
                __rsvd2[(0x54 - 0x4C) / 4],     // 0x4C ~ 0x50
                mr_status,                      // 0x54
                __rsvd3[(0x60 - 0x58) / 4],     // 0x58 ~ 0x5C
                qos_control[32],                // 0x60 ~ 0xDC
                timing_set_sw,                  // 0xE0
                __rsvd4,                        // 0xE4
                timing_data_1,                  // 0xE8
                timing_power_1,                 // 0xEC
                __rsvd5,                        // 0xF0
                wr_tra_config,                  // 0xF4
                rd_lvl_config,                  // 0xF8
                __rsvd6,                        // 0xFC
                brb_rsv_control,                // 0x100
                brb_rsv_config,                 // 0x104
                brb_qos_config,                 // 0x108
                __rsvd7[(0x120 - 0x10C) / 4],   // 0x10C ~ 0x11C
                wr_lvl_config[2],               // 0x120 ~ 0x124
                wr_lvl_status,                  // 0x128
                __rsvd8[(0x150 - 0x12C) / 4],   // 0x12C ~ 0x14C
                ctrl_io_rdata,                  // 0x150
                __rsvd9[(0x160 - 0x154) / 4],   // 0x154 ~ 0x15C
                ca_cal_config[2],               // 0x160 ~ 0x164
                ca_cal_status,                  // 0x168
                __rsvd10[(0x200 - 0x16C) / 4],  // 0x16C ~ 0x1FC
                emergent_config[2],             // 0x200 ~ 0x204
                __rsvd11[(0x210 - 0x208) / 4];  // 0x208 ~ 0x20C

        struct {
                volatile uint32_t \
                        control,                // 0x0
                        config_r,               // 0x4
                        config_w,               // 0x8
                        __rsvd;                 // 0xC
        } bp[4];                                // 0x210 ~ 0x24C
} drex_t ;

#define DDR3_DREX_BASE (0xC00E0000)

#define DDR3_DREX_DFI_START_OFFSET (28u)
#define DDR3_DREX_DFI_START_MASK (0b1 << DDR3_DREX_DFI_START_OFFSET)
#define DDR3_DREX_DFI_START_SET (0b1 << DDR3_DREX_DFI_START_OFFSET)
#define DDR3_DREX_DFI_START_RESET (0b0 << DDR3_DREX_DFI_START_OFFSET)

#define DDR3_DREX_DFI_COMPLETE_OFFSET (3u)
#define DDR3_DREX_DFI_COMPLETE_SET (1u << DDR3_DREX_DFI_COMPLETE_OFFSET)

void ddr3_init(void);

#endif /* X6818_DDR3_H_ */
