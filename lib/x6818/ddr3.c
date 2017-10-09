/*
 * ddr3.c
 *
 *  Created on: 2017年10月7日
 *      Author: lucius
 */

#include "x6818/ddr3.h"

enum {
  FORCE_CALIBRATION = 0b00u,
  LONG_CALIBRATION = 0b01u,
  SHORT_CALIBRATION = 0b10
} ;

/*3. ZQ Calibration*/
static inline void ddr3_phy_zq_calibration(uint32_t mode)
{
  ddrphy_t *ddrphy = (ddrphy_t *)DDR3_DDRPHY_BASE;
  uint32_t tmp;

  /*
  1. After power-up and system PLL locking time, system reset (rst_n) is released.
  */
  /*
  2. Set ctrl_zq_clk_div[31:0] to proper value (= 0x7)
  */
  ddrphy->zq_con2 = 0x7;

  /*
  3. Set ZQ_CLK_DIV_EN from 1’b0 to 1’b1 to update divider settings (ctrl_zq_clk_div[31:0] = 0x7).
  */
  tmp = ddrphy->zq_con0;
  tmp &= ~(DDR3_DDRPHY_ZQ_DIV_EN_MASK);
  tmp |= DDR3_DDRPHY_ZQ_DIV_ENABLE;
  ddrphy->zq_con0 = tmp;

  /*
  4. Set zq_manual_mode
      Long calibration mode: 2’b01
      Short calibration mode: 2’b10
  */
  tmp = ddrphy->zq_con0;
  tmp &= ~(DDR3_DDRPHY_ZQ_MODE_MASK);
  tmp |= (mode << DDR3_DDRPHY_ZQ_MODE_OFFSET);
  ddrphy->zq_con0 = tmp;

  /*
  5. Start ZQ I/O calibration by setting ZQ_MANUAL_STR from 1’b0 to 1’b1
  */
  tmp = ddrphy->zq_con0;
  tmp &= ~(DDR3_DDRPHY_ZQ_STR_MASK);
  tmp |= (mode << DDR3_DDRPHY_ZQ_STR_START);
  ddrphy->zq_con0 = tmp;
  /*
  6. When calibration is done, zq_done (= PHY_CON17[0]) will be set. For four cycles (system clock)
  */
  while((ddrphy->zq_con1 & DDR3_DDRPHY_ZQ_DONE) == 0);

  /*
  7. After zq_done (= PHY_CON17[0]) is asserted, clear ZQ_MANUAL_STR
  */
  tmp = ddrphy->zq_con0;
  tmp &= ~(DDR3_DDRPHY_ZQ_STR_MASK);
  tmp |= (mode << DDR3_DDRPHY_ZQ_STR_STOP);
  ddrphy->zq_con0 = tmp;

  /*
  8. Clear ZQ_CLK_DIV_EN
  */
  tmp = ddrphy->zq_con0;
  tmp &= ~(DDR3_DDRPHY_ZQ_DIV_EN_MASK);
  tmp |= DDR3_DDRPHY_ZQ_DIV_DISABLE;
  ddrphy->zq_con0 = tmp;
}

/*
2. Set the PHY for DDR3 operation mode, RL/WL/BL register and processed ZQ calibration. Refer to
"INITIALIZATION" in PHY manual.
*/
static inline void ddr3_phy_init(void)
{
  ddrphy_t *ddrphy = (ddrphy_t *)DDR3_DDRPHY_BASE;
  drex_t *drex = (drex_t *)DDR3_DREX_BASE;
  uint32_t tmp;

  /*1. Select Memory Type*/
  /*1.1 */
  tmp = ddrphy->phy_con0;
  tmp &= ~DDR3_DDRPHY_MODE_MASK;
  tmp |= DDR3_DDRPHY_MODE_DDR3;
  ddrphy->phy_con0 = tmp;

  /*1.2 set default read command */
  tmp = ddrphy->lp_ddr_con3;
  tmp &= ~DDR3_DDRPHY_COMMAND_MASK;
  tmp |= DDR3_DDRPHY_COMMAND_READ;
  ddrphy->lp_ddr_con3 = tmp;

  /*1.2 set default active command */
  tmp = ddrphy->lp_ddr_con4;
  tmp &= ~DDR3_DDRPHY_COMMAND_MASK;
  tmp |= DDR3_DDRPHY_COMMAND_ACTIVE;
  ddrphy->lp_ddr_con4 = tmp;

  /*1.3 set phy update mode  */
  tmp = ddrphy->offsetd_con0;
  tmp &= ~DDR3_DDRPHY_UPD_MASK;
  tmp |= DDR3_DDRPHY_UPD_MC;
  ddrphy->offsetd_con0 = tmp;

  /*2. Set Read Latency (RL), Burst Length (BL) and Write Latency (WL)*/
  tmp = ddrphy->phy_con4;
  tmp &= ~(DDR3_DDRPHY_WRLAT_MASK | DDR3_DDRPHY_BSTLEN_MASK | DDR3_DDRPHY_RDLAT_MASK);
  tmp |= (6 << DDR3_DDRPHY_RDLAT_OFFSET); /* */
  tmp |= (8 << DDR3_DDRPHY_BSTLEN_OFFSET); /* */
  tmp |= (6 << DDR3_DDRPHY_WRLAT_OFFSET); /* */
  ddrphy->phy_con4 = tmp;

  /*3. ZQ Calibration */
  ddr3_phy_zq_calibration(LONG_CALIBRATION);

  /* 4. Memory Controller should assert "DFI_INIT_START" from LOW to HIGH. */
  tmp = drex->con_control;
  tmp &= ~(DDR3_DREX_DFI_START_MASK);
  tmp |= DDR3_DREX_DFI_START_RESET;  /* force clear*/
  drex->con_control = tmp;
  tmp = drex->con_control;
  tmp &= ~(DDR3_DREX_DFI_START_MASK);
  tmp |= DDR3_DREX_DFI_START_SET;  /* set */
  drex->con_control = tmp;

  /*5. Memory Controller should wait until "DFI_INIT_COMPLETE" is set*/
  while((drex->phy_status & DDR3_DREX_DFI_COMPLETE_SET) == 0);

  /*6. Enable DQS pull down mode*/
  tmp = ddrphy->lp_con0;
  tmp &= ~(DDR3_DDRPHY_PULLD_DQS_MASK);
  tmp |= (0x1FF << DDR3_DDRPHY_PULLD_DQS_OFFSET);
  ddrphy->lp_con0 |= tmp;

  /*7. Memory Controller should assert "DFI_CTRLUPD_REQ" after "DFI_INIT_COMPLETE" is set*/

  /*8. Start Memory Initialization by memory controller*/

  /**/

  /**/
}

void ddr3_init(void)
{
  /*
  1. Apply power. RESET# pin of memory needs to be maintained for minimum 200us with stable power. CKE is
  pulled "Low" anytime before RESET# being de-asserted (min. time 10 ns)
  */

  /*
  2. Set the PHY for DDR3 operation mode, RL/WL/BL register and processed ZQ calibration. Refer to
  "INITIALIZATION" in PHY manual.
  */
  ddr3_phy_init();

  /*
  3. Assert the CONCONTROL.DFI_INIT_START field to high but leave as default value for other fields.
  (AREF_EN and IO_PD_CON should be off) Clock gating in CGCONTROL should be disabled in initialization
  and training sequence.
  */

  /*
  4. Wait for the PHYSTATUS0.DFI_INIT_COMPLETE field to change to "1".
  */

  /*
  5. De-assert the CONCONTROL.DFI_INIT_START field to low.
  */

  /*
  6. Set the PHY for DQS pull-down mode. (Refer to PHY manual)
  */

  /*
  7. Set the PHYCONTROL0.FP_RESYNC bit-field to "1" to update DLL information.
  */

  /*
  8. Set the PHYCONTROL0.FP_RESYNC bit-field to '0'.
  */

  /*
  9. Set the MEMBASECONFIG0 register and MEMBASECONFIG1 register if needed.
  */

  /*
  10. Set the MEMCONFIG0 register and MEMCONFIG1 if needed.
  */

  /*
  11. Set the PRECHCONFIG and PWRDNCONFIG registers.
  */

  /*
  12. Set the TIMINGAREF, TIMINGROW, TIMINGDATA and TIMINGPOWER registers according to memory AC
  parameters.
  */

  /*
  13. If QoS scheme is required, set the QosControl0 to 15 and QosConfig0 to 15 registers.
  */
  /*
  14. Confirm that after RESET# is de-asserted, 500 us have passed before CKE becomes active.
  */

  /*
  15. Confirm that clocks (CK, CK#) need to be started and stabilized for at least 10 ns or 5 tCK (which is larger)
  before CKE goes active.
  */

  /*
  16. Issue a NOP command using the DIRECTCMD register to assert and to hold CKE to a logic high level.
  */

  /*
  17. Wait for tXPR (max (5nCK, tRFC (min) + 10 ns)) or set tXP to tXPR value before step 17. If the system set
  tXP to tXPR, then the system must set tXP to proper value before normal memory operation.
  */

  /*
  18. Issue an EMRS2 command using the DIRECTCMD register to program the operating parameters. Dynamic
  ODT should be disabled. A10 and A9 should be low.
  */

  /*
  19. Issue an EMRS3 command using the DIRECTCMD register to program the operating parameters.
  */

  /*
  20. Issue an EMRS command using the DIRECTCMD register to enable the memory DLL.
  */

  /*
  21. Issue a MRS command using the DIRECTCMD register to reset the memory DLL.
  */

  /*
  22. Issues a MRS command using the DIRECTCMD register to program the operating parameters without
  resetting the memory DLL.
  */

  /*
  23. Issues a ZQINIT commands using the DIRECTCMD register.
  */

  /*
  24. If there are more external memory chips, perform steps 17 to 24 procedures for other memory device.
  */

  /*
  25. If any leveling/training is needed, enable CTRL_ATGATE, P0_CMD_EN, INITDESKEWEN and
  BYTE_RDLVL_EN. Disable CTRL_DLL_ON and set CTRL_FORCE value. (Refer to PHY manual)
  */

  /*
  26. If write leveling is not needed, skip this procedure. If write leveling is needed, set DDR3 into write leveling
  mode using MRS direct command, set ODT pin high and tWLO using WRLVL_CONFIG0 register (offset =
  0x120) and set the related PHY SFR fields through PHY APB I/F (Refer to PHY manual). To generate 1 cycle
  pulse of dfi_wrdata_en_p0, write 0x1 to WRLVL_CONFIG1 register (offset addr = 0x124). To read the value
  of memory data, use CTRL_IO_RDATA (offset = 0x150). If write leveling is finished, disable write leveling
  mode in PHY register and set ODT pin low and disable write leveling mode of DDR3.
  */

  /*
  27. If gate leveling is not needed, skip 27 to 28. If gate leveling is needed, set DDR3 into MPR mode using MRS
  direct command and set the related PHY SFR fields through PHY APB I/F. Do the gate leveling. (Refer to
  PHY manual)
  */

  /*
  28. If gate leveling is finished, set DDR3 into normal operation mode using MRS command and disable DQS
  pull-down mode. (Refer to PHY manual)
  */

  /*
  29. If read leveling is not needed skip 29 to 30. If read leveling is needed, set DDR3 into MPR mode using MRS
  direct command and set proper value to PHY control register. Do the read leveling. (Refer to PHY manual)
  30. If read leveling is finished, set DDR3 into normal operation mode using MRS direct command.
  31. If write training is not needed, skip 31. If write training is needed, set the related PHY SFR fields through PHY
  APB I/F (Refer to PHY manual). To issue ACT command, enable and disable
  WRTRACONFIG.WRITE_TRAINING_EN. Refer to this register definition for row and bank address. Do write
  training. (Refer to PHY manual)
  SAMSUNG
  Confidential
  nexell / ys.kim at 2015.02.12
  32. After all leveling/training are completed, enable CTRL_DLL_ON. (Refer to PHY manual)
  33. Set the PHYCONTROL0.FP_RESYNC bit-field to "1" to update DLL information.
  34. Set the PHYCONTROL0.FP_RESYNC bit-field to "0".
  35. Disable PHY gating control through PHY APB I/F if necessary (CTRL_ATGATE, refer to PHY manual).
  36. Issue PALL to all chips using direct command. This is an important step if write training has been done.
  37. Set the MEMCONTROL and PHYCONTROL0 register.
  38. Set the CONCONTROL register. AREF_EN should be turn on.
  39. Set the CGCONTROL register for clock gating enable.

  */
}

