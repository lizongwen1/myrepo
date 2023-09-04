// SPDX-License-Identifier: GPL-2.0
/*
 * gadget.h - DesignWare USB3 DRD Gadget Header
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com
 *
 * Authors: Felipe Balbi <balbi@ti.com>,
 *	    Sebastian Andrzej Siewior <bigeasy@linutronix.de>
 */

#ifndef __DRIVERS_USB_DWC3_GADGET_H
#define __DRIVERS_USB_DWC3_GADGET_H

#include <linux/list.h>
#include <linux/usb/gadget.h>
#include "io.h"

struct dwc3;
#define to_dwc3_ep(ep)		(container_of(ep, struct dwc3_ep, endpoint))
#define gadget_to_dwc(g)	(container_of(g, struct dwc3, gadget))

/* DEPCFG parameter 1 */
#define DWC3_DEPCFG_INT_NUM(n)		(((n) & 0x1f) << 0)
#define DWC3_DEPCFG_XFER_COMPLETE_EN	BIT(8)
#define DWC3_DEPCFG_XFER_IN_PROGRESS_EN	BIT(9)
#define DWC3_DEPCFG_XFER_NOT_READY_EN	BIT(10)
#define DWC3_DEPCFG_FIFO_ERROR_EN	BIT(11)
#define DWC3_DEPCFG_STREAM_EVENT_EN	BIT(13)
#define DWC3_DEPCFG_BINTERVAL_M1(n)	(((n) & 0xff) << 16)
#define DWC3_DEPCFG_STREAM_CAPABLE	BIT(24)
#define DWC3_DEPCFG_EP_NUMBER(n)	(((n) & 0x1f) << 25)
#define DWC3_DEPCFG_BULK_BASED		BIT(30)
#define DWC3_DEPCFG_FIFO_BASED		BIT(31)

/* DEPCFG parameter 0 */
#define DWC3_DEPCFG_EP_TYPE(n)		(((n) & 0x3) << 1)
#define DWC3_DEPCFG_MAX_PACKET_SIZE(n)	(((n) & 0x7ff) << 3)
#define DWC3_DEPCFG_FIFO_NUMBER(n)	(((n) & 0x1f) << 17)
#define DWC3_DEPCFG_BURST_SIZE(n)	(((n) & 0xf) << 22)
#define DWC3_DEPCFG_DATA_SEQ_NUM(n)	((n) << 26)
/* This applies for core versions earlier than 1.94a */
#define DWC3_DEPCFG_IGN_SEQ_NUM		BIT(31)
/* These apply for core versions 1.94a and later */
#define DWC3_DEPCFG_ACTION_INIT		(0 << 30)
#define DWC3_DEPCFG_ACTION_RESTORE	BIT(30)
#define DWC3_DEPCFG_ACTION_MODIFY	(2 << 30)

/* DEPXFERCFG parameter 0 */
#define DWC3_DEPXFERCFG_NUM_XFER_RES(n)	((n) & 0xffff)

/* -------------------------------------------------------------------------- */

#define to_dwc3_request(r)	(container_of(r, struct dwc3_request, request))

irqreturn_t dwc3_interrupt(int irq, void *_dwc);
void dwc3_bh_work(struct work_struct *w);
/**
 * next_request - gets the next request on the given list
 * @list: the request list to operate on
 *
 * Caller should take care of locking. This function return %NULL or the first
 * request available on @list.
 */
static inline struct dwc3_request *next_request(struct list_head *list)
{
	return list_first_entry_or_null(list, struct dwc3_request, list);
}

/**
 * dwc3_gadget_move_started_request - move @req to the started_list
 * @req: the request to be moved
 *
 * Caller should take care of locking. This function will move @req from its
 * current list to the endpoint's started_list.
 */
static inline void dwc3_gadget_move_started_request(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->started = true;
	list_move_tail(&req->list, &dep->started_list);
}

static inline void dwc3_gadget_move_pending_list_front(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->started = false;
	list_move(&req->list, &dep->pending_list);
}

/**
 * dwc3_gadget_move_cancelled_request - move @req to the cancelled_list
 * @req: the request to be moved
 *
 * Caller should take care of locking. This function will move @req from its
 * current list to the endpoint's cancelled_list.
 */
static inline void dwc3_gadget_move_cancelled_request(struct dwc3_request *req)
{
	struct dwc3_ep		*dep = req->dep;

	req->started = false;
	list_move_tail(&req->list, &dep->cancelled_list);
}

static inline enum dwc3_link_state dwc3_get_link_state(struct dwc3 *dwc)
{
	u32 reg;

	reg = dwc3_readl(dwc->regs, DWC3_DSTS);
	return DWC3_DSTS_USBLNKST(reg);
}

void dwc3_gadget_giveback(struct dwc3_ep *dep, struct dwc3_request *req,
		int status);

void dwc3_ep0_interrupt(struct dwc3 *dwc,
		const struct dwc3_event_depevt *event);
void dwc3_ep0_out_start(struct dwc3 *dwc);
void dwc3_ep0_end_control_data(struct dwc3 *dwc, struct dwc3_ep *dep);
void dwc3_ep0_stall_and_restart(struct dwc3 *dwc);
int __dwc3_gadget_ep0_set_halt(struct usb_ep *ep, int value);
int dwc3_gadget_ep0_set_halt(struct usb_ep *ep, int value);
int dwc3_gadget_ep0_queue(struct usb_ep *ep, struct usb_request *request,
		gfp_t gfp_flags);
int __dwc3_gadget_ep_set_halt(struct dwc3_ep *dep, int value, int protocol);
void dwc3_stop_active_transfer(struct dwc3 *dwc, u32 epnum, bool force, bool interrupt);
int dwc3_stop_active_transfer_noioc(struct dwc3 *dwc, u32 epnum, bool force);
void dwc3_ep_inc_enq(struct dwc3_ep *dep);
void dwc3_ep_inc_deq(struct dwc3_ep *dep);

static inline dma_addr_t dwc3_trb_dma_offset(struct dwc3_ep *dep,
		struct dwc3_trb *trb)
{
	u32 offset = (char *) trb - (char *) dep->trb_pool;

	return dep->trb_pool_dma + offset;
}

/**
 * dwc3_gadget_ep_get_transfer_index - Gets transfer index from HW
 * @dep: dwc3 endpoint
 *
 * Caller should take care of locking. Returns the transfer resource
 * index for a given endpoint.
 */
static inline void dwc3_gadget_ep_get_transfer_index(struct dwc3_ep *dep)
{
	u32			res_id;

	res_id = dwc3_readl(dep->regs, DWC3_DEPCMD);
	dep->resource_index = DWC3_DEPCMD_GET_RSC_IDX(res_id);
}

#endif /* __DRIVERS_USB_DWC3_GADGET_H */
