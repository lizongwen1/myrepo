/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017, The Linux Foundation. All rights reserved.
 */

#ifndef __WCD934X_IRQ_H_
#define __WCD934X_IRQ_H_

enum {
	/* INTR_REG 0 */
	WCD934X_IRQ_MISC = 1,
	WCD934X_IRQ_HPH_PA_OCPL_FAULT,
	WCD934X_IRQ_HPH_PA_OCPR_FAULT,
	WCD934X_IRQ_EAR_PA_OCP_FAULT,
	WCD934X_IRQ_HPH_PA_CNPL_COMPLETE,
	WCD934X_IRQ_HPH_PA_CNPR_COMPLETE,
	WCD934X_IRQ_EAR_PA_CNP_COMPLETE,
	/* INTR_REG 1 */
	WCD934X_IRQ_MBHC_SW_DET,
	WCD934X_IRQ_MBHC_ELECT_INS_REM_DET,
	WCD934X_IRQ_MBHC_BUTTON_PRESS_DET,
	WCD934X_IRQ_MBHC_BUTTON_RELEASE_DET,
	WCD934X_IRQ_MBHC_ELECT_INS_REM_LEG_DET,
	WCD934X_IRQ_RESERVED_0,
	WCD934X_IRQ_RESERVED_1,
	WCD934X_IRQ_RESERVED_2,
	/* INTR_REG 2 */
	WCD934X_IRQ_LINE_PA1_CNP_COMPLETE,
	WCD934X_IRQ_LINE_PA2_CNP_COMPLETE,
	WCD934X_IRQ_SLNQ_ANALOG_ERROR,
	WCD934X_IRQ_RESERVED_3,
	WCD934X_IRQ_SOUNDWIRE,
	WCD934X_IRQ_VDD_DIG_RAMP_COMPLETE,
	WCD934X_IRQ_RCO_ERROR,
	WCD934X_IRQ_CPE_ERROR,
	/* INTR_REG 3 */
	WCD934X_IRQ_MAD_AUDIO,
	WCD934X_IRQ_MAD_BEACON,
	WCD934X_IRQ_MAD_ULTRASOUND,
	WCD934X_IRQ_VBAT_ATTACK,
	WCD934X_IRQ_VBAT_RESTORE,
	WCD934X_IRQ_CPE1_INTR,
	WCD934X_IRQ_RESERVED_4,
	WCD934X_IRQ_SLNQ_DIGITAL,
	WCD934X_NUM_IRQS,
};

#endif
