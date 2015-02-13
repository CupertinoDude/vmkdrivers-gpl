/*******************************************************************************

  Intel 10 Gigabit PCI Express Linux driver
  Copyright(c) 1999 - 2010 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#ifndef _IXGBE_H_
#define _IXGBE_H_


#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/vmalloc.h>
#ifdef __VMKLNX__
#define NODE_ADDRESS_SIZE ETH_ALEN
#endif /* __VMKLNX__ */

#ifdef SIOCETHTOOL
#include <linux/ethtool.h>
#endif
#ifdef NETIF_F_HW_VLAN_TX
#include <linux/if_vlan.h>
#endif
#if defined(CONFIG_DCA) || defined(CONFIG_DCA_MODULE)
#define IXGBE_DCA
#include <linux/dca.h>
#endif
#include "ixgbe_dcb.h"

#include "kcompat.h"
#ifdef __VMKLNX__
#include "kcompat_esx.h"
#endif /* __VMKLNX__ */

#if defined(CONFIG_FCOE) || defined(CONFIG_FCOE_MODULE)
#define IXGBE_FCOE
#include "ixgbe_fcoe.h"
#endif /* CONFIG_FCOE or CONFIG_FCOE_MODULE */

#include "ixgbe_api.h"

#define PFX "ixgbe: "
#define DPRINTK(nlevel, klevel, fmt, args...) \
	((void)((NETIF_MSG_##nlevel & adapter->msg_enable) && \
	printk(KERN_##klevel PFX "%s: %s: " fmt, adapter->netdev->name, \
		__FUNCTION__ , ## args)))
#ifdef __VMKLNX__

#define NETIF_MSG_VIRT                   0x8000
#endif /* __VMKLNX__ */

/* TX/RX descriptor defines */
#define IXGBE_DEFAULT_TXD		    512
#define IXGBE_MAX_TXD			   4096
#define IXGBE_MIN_TXD			     64

#define IXGBE_DEFAULT_RXD		    512
#define IXGBE_MAX_RXD			   4096
#define IXGBE_MIN_RXD			     64

#ifdef __VMKLNX__
#define IXGBE_JUMBO_FRAME_DEFAULT_RXD       512
#endif

/* flow control */
#define IXGBE_DEFAULT_FCRTL		0x10000
#define IXGBE_MIN_FCRTL			   0x40
#define IXGBE_MAX_FCRTL			0x7FF80
#define IXGBE_DEFAULT_FCRTH		0x20000
#define IXGBE_MIN_FCRTH			  0x600
#define IXGBE_MAX_FCRTH			0x7FFF0
#define IXGBE_DEFAULT_FCPAUSE		 0xFFFF
#define IXGBE_MIN_FCPAUSE		      0
#define IXGBE_MAX_FCPAUSE		 0xFFFF

/* Supported Rx Buffer Sizes */
#define IXGBE_RXBUFFER_64    64     /* Used for packet split */
#define IXGBE_RXBUFFER_128   128    /* Used for packet split */
#define IXGBE_RXBUFFER_256   256    /* Used for packet split */
#define IXGBE_RXBUFFER_2048  2048
#define IXGBE_RXBUFFER_4096  4096
#define IXGBE_RXBUFFER_8192  8192
#define IXGBE_MAX_RXBUFFER   16384  /* largest size for single descriptor */

#define IXGBE_RX_HDR_SIZE IXGBE_RXBUFFER_256

#define MAXIMUM_ETHERNET_VLAN_SIZE (VLAN_ETH_FRAME_LEN + ETH_FCS_LEN)

/* How many Rx Buffers do we bundle into one write to the hardware ? */
#define IXGBE_RX_BUFFER_WRITE	16	/* Must be power of 2 */

#define IXGBE_TX_FLAGS_CSUM		(u32)(1)
#define IXGBE_TX_FLAGS_VLAN		(u32)(1 << 1)
#define IXGBE_TX_FLAGS_TSO		(u32)(1 << 2)
#define IXGBE_TX_FLAGS_IPV4		(u32)(1 << 3)
#define IXGBE_TX_FLAGS_FCOE		(u32)(1 << 4)
#define IXGBE_TX_FLAGS_FSO		(u32)(1 << 5)
#define IXGBE_TX_FLAGS_VLAN_MASK	0xffff0000
#define IXGBE_TX_FLAGS_VLAN_PRIO_MASK	0x0000e000
#define IXGBE_TX_FLAGS_VLAN_SHIFT	16

#define IXGBE_MAX_RX_DESC_POLL          10

#define IXGBE_MAX_RSC_INT_RATE          162760

#define IXGBE_MAX_VF_MC_ENTRIES         30
#define IXGBE_MAX_VF_FUNCTIONS          64
#define IXGBE_MAX_VFTA_ENTRIES          128
#define MAX_EMULATION_MAC_ADDRS         16

#define VMDQ_P(p)   (p)

#ifdef __VMKLNX__
struct vf_stats {
	u32 gprc;
	u32 gptc;
	u32 mprc;
	u64 gorc;
	u64 gotc;
	u64 glsoc; /* lso big pkt count */
	u64 glsopc; /* lso mtu size pkt count */
	u64 glsooc;
};
#define	VF_PF_SHARED_FLAGS_DEFAULT_VLAN_PRESENT		0x1
#define	VF_PF_SHARED_FLAGS_DCB_ENABLED			0x2
#define	VF_PF_EXCEPTION_FLAGS_DESC_ERROR		0x1
#define	VF_PF_EXCEPTION_FLAGS_LSO_ERROR			0x2
#define	VF_PF_EXCEPTION_FLAGS_FCOE_UP_CONFLICT_ERROR	0x4
struct vf_shared_stats {
        u64 LSO_count; /* lso big pkt count */ 
        u64 LSO_pkt_count; /* lso mtu size pkt count */
        u64 LSO_byte_count;
};
struct vf_pf_errors {
	u32 exception_flags;
	u64 desc_errors;
	u64 lso_errors;
	u64 fcoe_up_conflict;
};
struct vf_pf_shared_mem {
	u32 flags;
	u16 npa_default_vlan_id;
	u16 fcoe_up;
	struct vf_pf_errors errors;
	struct vf_shared_stats shared_stats;
};
#else
#endif

struct vf_data_storage {
	unsigned char vf_mac_addresses[ETH_ALEN];
	u16 vf_mc_hashes[IXGBE_MAX_VF_MC_ENTRIES];
	u16 num_vf_mc_hashes;
	u16 default_vf_vlan_id;
	u16 vlans_enabled;
#ifdef __VMKLNX__
	bool allocated;
	bool init;
	u32 buffer_mode;
	u32 coml_method;
	u32 mtu;
	u32 irq_rate;
	u16 num_queue_pairs;
	struct vf_stats vfstats;
	struct vf_pf_shared_mem *vf_pf_shared_mem;
	u32 vf_pf_shared_mem_length;
	int rar;
#else
#endif
	bool pf_set_mac;
	u16 pf_vlan; /* When set, guest VLAN config not allowed. */
	u16 pf_qos;
};

/* wrapper around a pointer to a socket buffer,
 * so a DMA handle can be stored along with the buffer */
struct ixgbe_tx_buffer {
	struct sk_buff *skb;
	dma_addr_t dma;
	unsigned long time_stamp;
	u16 length;
	u16 next_to_watch;
	u16 mapped_as_page;
};

struct ixgbe_rx_buffer {
	struct sk_buff *skb;
	dma_addr_t dma;
	struct page *page;
	dma_addr_t page_dma;
	unsigned int page_offset;
};

struct ixgbe_queue_stats {
	u64 packets;
	u64 bytes;
};

/*
 * queues are splitted up into 2 distinct pools. This internal
 * resources split is due to the multiple clients of queues.
 */
enum ixgbe_netdev_type {
	IXGBE_NETDEV_NET     = 0,    /* Network Pools */
	IXGBE_NETDEV_CNA     = 1,    /* FCoE Pools */
	IXGBE_NETDEV_UNKNOWN = 0xFF
};

#define netdev_ring(adapter, ring) \
	((ring->netdev_type == IXGBE_NETDEV_CNA) \
			    ? adapter->cnadev : adapter->netdev)

struct ixgbe_ring {
	void *desc;			/* descriptor ring memory */
	union {
		struct ixgbe_tx_buffer *tx_buffer_info;
		struct ixgbe_rx_buffer *rx_buffer_info;
	};
	u8 atr_sample_rate;
	u8 atr_count;
	u16 count;			/* amount of descriptors */
	u16 rx_buf_len;
	u16 next_to_use;
	u16 next_to_clean;

	u8 queue_index; /* needed for multiqueue queue management */

#define IXGBE_RING_RX_PS_ENABLED                (u8)(1)
	u8 flags;			/* per ring feature flags */
	u16 head;
	u16 tail;

	unsigned int total_bytes;
	unsigned int total_packets;
	u32 non_eop_descs;             /* track hardware descriptor chaining */

	u16 work_limit;                /* max work per interrupt */
	u16 reg_idx;			/* holds the special value that gets the
					 * hardware register offset associated
					 * with this ring, which is different
					 * for DCB and RSS modes */

	struct ixgbe_queue_stats stats;
	int cpu;                       /* cpu for queue dca */
	unsigned long reinit_state;
	int numa_node;
	u32 restart_queue;             /* track tx queue restarts */
	u64 rsc_count;                 /* stat for coalesced packets */
	u64 rsc_flush;
#ifdef __VMKLNX__
	bool rsc_en;                   /* is RSC enabled for queue */
	u8 vector_idx;
	u8 active;
	u8 allocated;
	enum ixgbe_netdev_type netdev_type;
#define __IXGBE_RING_CLEAN_BUSY 1
	unsigned long clean_state;
#endif /* __VMKLNX__ */
	unsigned int size;		/* length in bytes */
	dma_addr_t dma;			/* phys. address of descriptor ring */
} ____cacheline_internodealigned_in_smp;

enum ixgbe_ring_f_enum {
	RING_F_NONE = 0,
	RING_F_DCB,
	RING_F_VMDQ,
	RING_F_RSS,
	RING_F_FDIR,
#ifdef IXGBE_FCOE
	RING_F_FCOE,
#endif /* IXGBE_FCOE */
	RING_F_ARRAY_SIZE      /* must be last in enum set */
};

#define IXGBE_MAX_DCB_INDICES   8
#define IXGBE_MAX_RSS_INDICES  16
#define IXGBE_MAX_VMDQ_INDICES 64
#define IXGBE_MAX_FDIR_INDICES 64
#ifdef IXGBE_FCOE
#define IXGBE_MAX_FCOE_INDICES 8
#define MAX_RX_QUEUES (IXGBE_MAX_FDIR_INDICES + IXGBE_MAX_FCOE_INDICES)
#define MAX_TX_QUEUES (IXGBE_MAX_FDIR_INDICES + IXGBE_MAX_FCOE_INDICES)
#else
#define MAX_RX_QUEUES IXGBE_MAX_FDIR_INDICES
#define MAX_TX_QUEUES IXGBE_MAX_FDIR_INDICES
#endif /* IXGBE_FCOE */
struct ixgbe_ring_feature {
	int indices;
	int mask;
};


#define MAX_RX_PACKET_BUFFERS ((adapter->flags & IXGBE_FLAG_DCB_ENABLED) \
                               ? 8 : 1)
#define MAX_TX_PACKET_BUFFERS MAX_RX_PACKET_BUFFERS

/* MAX_MSIX_Q_VECTORS of these are allocated,
 * but we only use one per queue-specific vector.
 */
struct ixgbe_q_vector {
	struct ixgbe_adapter *adapter;
	unsigned int v_idx; /* index of q_vector within array, also used for
	                     * finding the bit in EICR and friends that
	                     * represents the vector for this ring */
	DECLARE_BITMAP(rxr_idx, MAX_RX_QUEUES); /* Rx ring indices */
	DECLARE_BITMAP(txr_idx, MAX_TX_QUEUES); /* Tx ring indices */
	u8 rxr_count;     /* Rx ring count assigned to this vector */
	u8 txr_count;     /* Tx ring count assigned to this vector */
	u8 tx_itr;
	u8 rx_itr;
	u32 eitr;
	struct ixgbe_lro_list *lrolist;   /* LRO list for queue vector*/
	char name[IFNAMSIZ + 9];
#ifdef CONFIG_IXGBE_NAPI
	struct napi_struct napi;
#endif
#ifndef HAVE_NETDEV_NAPI_LIST
	struct net_device poll_dev;
#endif
} ____cacheline_internodealigned_in_smp;


/* Helper macros to switch between ints/sec and what the register uses.
 * And yes, it's the same math going both ways.  The lowest value
 * supported by all of the ixgbe hardware is 8.
 */
#define EITR_INTS_PER_SEC_TO_REG(_eitr) \
	((_eitr) ? (1000000000 / ((_eitr) * 256)) : 8)
#define EITR_REG_TO_INTS_PER_SEC EITR_INTS_PER_SEC_TO_REG

#define IXGBE_DESC_UNUSED(R) \
	((((R)->next_to_clean > (R)->next_to_use) ? 0 : (R)->count) + \
	(R)->next_to_clean - (R)->next_to_use - 1)

#define IXGBE_RX_DESC_ADV(R, i)	    \
	(&(((union ixgbe_adv_rx_desc *)((R).desc))[i]))
#define IXGBE_TX_DESC_ADV(R, i)	    \
	(&(((union ixgbe_adv_tx_desc *)((R).desc))[i]))
#define IXGBE_TX_CTXTDESC_ADV(R, i)	    \
	(&(((struct ixgbe_adv_tx_context_desc *)((R).desc))[i]))

#define IXGBE_MAX_JUMBO_FRAME_SIZE        16128
#ifdef IXGBE_FCOE
/* use 3K as the baby jumbo frame size for FCoE */
#define IXGBE_FCOE_JUMBO_FRAME_SIZE       3072
#endif /* IXGBE_FCOE */

#ifdef IXGBE_TCP_TIMER
#define TCP_TIMER_VECTOR 1
#else
#define TCP_TIMER_VECTOR 0
#endif
#define OTHER_VECTOR 1
#define NON_Q_VECTORS (OTHER_VECTOR + TCP_TIMER_VECTOR)

#define IXGBE_MAX_MSIX_VECTORS_82599 64
#define IXGBE_MAX_MSIX_Q_VECTORS_82599 64
#define IXGBE_MAX_MSIX_Q_VECTORS_82598 16
#define IXGBE_MAX_MSIX_VECTORS_82598 18

/*
 * Only for array allocations in our adapter struct.  On 82598, there will be
 * unused entries in the array, but that's not a big deal.  Also, in 82599,
 * we can actually assign 64 queue vectors based on our extended-extended
 * interrupt registers.  This is different than 82598, which is limited to 16.
 */
#define MAX_MSIX_Q_VECTORS IXGBE_MAX_MSIX_Q_VECTORS_82599
#define MAX_MSIX_COUNT IXGBE_MAX_MSIX_VECTORS_82599

#define MIN_MSIX_Q_FCOE_VECTORS 2
#define MIN_MSIX_Q_VECTORS (2 + MIN_MSIX_Q_FCOE_VECTORS)
#define MIN_MSIX_COUNT (MIN_MSIX_Q_VECTORS + NON_Q_VECTORS)

/* board specific private data structure */
struct ixgbe_adapter {
	struct timer_list watchdog_timer;
#ifdef NETIF_F_HW_VLAN_TX
	struct vlan_group *vlgrp;
#endif
	u16 bd_number;
	struct work_struct reset_task;
	struct ixgbe_q_vector *q_vector[MAX_MSIX_Q_VECTORS];
	struct ixgbe_dcb_config dcb_cfg;
	struct ixgbe_dcb_config temp_dcb_cfg;
	u8 dcb_set_bitmap;
	enum ixgbe_fc_mode last_lfc_mode;

	/* Interrupt Throttle Rate */
	u32 rx_itr_setting;
	u32 tx_itr_setting;
	u16 eitr_low;
	u16 eitr_high;

	/* TX */
	struct ixgbe_ring *tx_ring[MAX_TX_QUEUES] ____cacheline_aligned_in_smp;
	int num_tx_queues;
	int num_tx_cna_queues;
	u32 tx_timeout_count;
	bool detect_tx_hung;

	u64 restart_queue;
	u64 lsc_int;

	/* RX */
	struct ixgbe_ring *rx_ring[MAX_TX_QUEUES] ____cacheline_aligned_in_smp;
	int num_rx_queues;
	int num_rx_pools;               /* == num_rx_queues in 82598 */
	int num_rx_cna_queues;
	int num_rx_queues_per_pool;	/* 1 if 82598, can be many if 82599 */
	u64 hw_csum_rx_error;
	u64 hw_rx_no_dma_resources;
	u64 non_eop_descs;
#ifndef CONFIG_IXGBE_NAPI
	u64 rx_dropped_backlog;		/* count drops from rx intr handler */
#endif
	int num_msix_vectors;
	int max_msix_q_vectors;         /* true count of q_vectors for device */
	struct ixgbe_ring_feature ring_feature[RING_F_ARRAY_SIZE];
	struct msix_entry *msix_entries;
#ifdef IXGBE_TCP_TIMER
	irqreturn_t (*msix_handlers[MAX_MSIX_COUNT])(int irq, void *data,
	                                             struct pt_regs *regs);
#endif

	u64 rx_hdr_split;
	u32 alloc_rx_page_failed;
	u32 alloc_rx_buff_failed;

	/* Some features need tri-state capability,
	 * thus the additional *_CAPABLE flags.
	 */
	u32 flags;
#define IXGBE_FLAG_RX_CSUM_ENABLED              (u32)(1)
#define IXGBE_FLAG_MSI_CAPABLE                  (u32)(1 << 1)
#define IXGBE_FLAG_MSI_ENABLED                  (u32)(1 << 2)
#define IXGBE_FLAG_MSIX_CAPABLE                 (u32)(1 << 3)
#define IXGBE_FLAG_MSIX_ENABLED                 (u32)(1 << 4)
#ifndef IXGBE_NO_LLI
#define IXGBE_FLAG_LLI_PUSH                     (u32)(1 << 5)
#endif
#define IXGBE_FLAG_RX_1BUF_CAPABLE              (u32)(1 << 6)
#define IXGBE_FLAG_RX_PS_CAPABLE                (u32)(1 << 7)
#define IXGBE_FLAG_RX_PS_ENABLED                (u32)(1 << 8)
#define IXGBE_FLAG_IN_NETPOLL                   (u32)(1 << 9)
#define IXGBE_FLAG_DCA_ENABLED                  (u32)(1 << 10)
#define IXGBE_FLAG_DCA_CAPABLE                  (u32)(1 << 11)
#define IXGBE_FLAG_DCA_ENABLED_DATA             (u32)(1 << 12)
#define IXGBE_FLAG_MQ_CAPABLE                   (u32)(1 << 13)
#define IXGBE_FLAG_DCB_ENABLED                  (u32)(1 << 14)
#define IXGBE_FLAG_DCB_CAPABLE                  (u32)(1 << 15)
#define IXGBE_FLAG_RSS_ENABLED                  (u32)(1 << 16)
#define IXGBE_FLAG_RSS_CAPABLE                  (u32)(1 << 17)
#define IXGBE_FLAG_VMDQ_CAPABLE                 (u32)(1 << 18)
#define IXGBE_FLAG_VMDQ_ENABLED                 (u32)(1 << 19)
#define IXGBE_FLAG_FAN_FAIL_CAPABLE             (u32)(1 << 20)
#define IXGBE_FLAG_NEED_LINK_UPDATE             (u32)(1 << 22)
#define IXGBE_FLAG_IN_WATCHDOG_TASK             (u32)(1 << 23)
#define IXGBE_FLAG_IN_SFP_LINK_TASK             (u32)(1 << 24)
#define IXGBE_FLAG_IN_SFP_MOD_TASK              (u32)(1 << 25)
#define IXGBE_FLAG_FDIR_HASH_CAPABLE            (u32)(1 << 26)
#define IXGBE_FLAG_FDIR_PERFECT_CAPABLE         (u32)(1 << 27)
#ifdef IXGBE_FCOE
#define IXGBE_FLAG_FCOE_CAPABLE                 (u32)(1 << 28)
#define IXGBE_FLAG_FCOE_ENABLED                 (u32)(1 << 29)
#endif /* IXGBE_FCOE */
#define IXGBE_FLAG_SRIOV_CAPABLE                (u32)(1 << 30)
#define IXGBE_FLAG_SRIOV_ENABLED                (u32)(1 << 31)

	u32 flags2;
#define IXGBE_FLAG2_RSC_CAPABLE                  (u32)(1)
#define IXGBE_FLAG2_RSC_ENABLED                  (u32)(1 << 1)
#define IXGBE_FLAG2_SWLRO_ENABLED                (u32)(1 << 2)
#define IXGBE_FLAG2_VMDQ_DEFAULT_OVERRIDE        (u32)(1 << 3)
#define IXGBE_FLAG2_CNA_ENABLED                  (u32)(1 << 4)
#define IXGBE_FLAG2_TEMP_SENSOR_CAPABLE          (u32)(1 << 5)
#define IXGBE_FLAG2_IN_TEMP_SENSOR_TASK          (u32)(1 << 6)

/* default to trying for four seconds */
#define IXGBE_TRY_LINK_TIMEOUT (4 * HZ)

	/* OS defined structs */
	struct net_device *netdev;
	struct net_device *cnadev;
	struct pci_dev *pdev;
	struct net_device_stats net_stats;

#ifdef ETHTOOL_TEST
	u32 test_icr;
	struct ixgbe_ring test_tx_ring;
	struct ixgbe_ring test_rx_ring;
#endif

	/* structs defined in ixgbe_hw.h */
	struct ixgbe_hw hw;
	u16 msg_enable;
	struct ixgbe_hw_stats stats;
#ifndef IXGBE_NO_LLI
	u32 lli_port;
	u32 lli_size;
	u64 lli_int;
	u32 lli_etype;
	u32 lli_vlan_pri;
#endif /* IXGBE_NO_LLI */
	/* Interrupt Throttle Rate */
	u32 rx_eitr_param;
	u32 tx_eitr_param;

	unsigned long state;
	u32 *config_space;
	u64 tx_busy;
#ifdef __VMKLNX__
	u32 n_rx_queues_allocated;
	u32 n_tx_queues_allocated;
#endif
	unsigned int tx_ring_count;
	unsigned int rx_ring_count;

	u32 link_speed;
	bool link_up;
	unsigned long link_check_timeout;

	struct work_struct watchdog_task;
	struct work_struct sfp_task;
	struct timer_list sfp_timer;
	struct work_struct multispeed_fiber_task;
	struct work_struct sfp_config_module_task;
	u64 flm;
	u32 fdir_pballoc;
	u32 atr_sample_rate;
	spinlock_t fdir_perfect_lock;
	struct work_struct fdir_reinit_task;
#ifdef IXGBE_FCOE
	struct ixgbe_fcoe fcoe;
#endif /* IXGBE_FCOE */
	u64 rsc_total_count;
	u64 rsc_total_flush;
	u32 wol;
	u16 eeprom_version;
	bool netdev_registered;
	char lsc_int_name[IFNAMSIZ + 9];
#ifdef IXGBE_TCP_TIMER
	char tcp_timer_name[IFNAMSIZ + 9];
#endif
	struct work_struct check_overtemp_task;
	u32 interrupt_event;

	DECLARE_BITMAP(active_vfs, IXGBE_MAX_VF_FUNCTIONS);
	unsigned int num_vfs;
	bool repl_enable;
	bool l2switch_enable;
	struct vf_data_storage *vfinfo;
	int node;
};

enum ixbge_state_t {
	__IXGBE_TESTING,
	__IXGBE_RESETTING,
	__IXGBE_DOWN,
	__IXGBE_FDIR_INIT_DONE,
	__IXGBE_SFP_MODULE_NOT_FOUND
};

extern struct dcbnl_rtnl_ops dcbnl_ops;
extern int ixgbe_copy_dcb_cfg(struct ixgbe_dcb_config *src_dcb_cfg,
			      struct ixgbe_dcb_config *dst_dcb_cfg, int tc_max);

/* needed by ixgbe_main.c */
extern int ixgbe_validate_mac_addr(u8 *mc_addr);
extern void ixgbe_check_options(struct ixgbe_adapter *adapter);
extern void ixgbe_assign_netdev_ops(struct net_device *netdev);

/* needed by ixgbe_ethtool.c */
extern char ixgbe_driver_name[];
extern const char ixgbe_driver_version[];

extern int ixgbe_up(struct ixgbe_adapter *adapter);
extern void ixgbe_down(struct ixgbe_adapter *adapter);
extern void ixgbe_reinit_locked(struct ixgbe_adapter *adapter);
extern void ixgbe_reset(struct ixgbe_adapter *adapter);
extern void ixgbe_set_ethtool_ops(struct net_device *netdev);
extern int ixgbe_setup_rx_resources(struct ixgbe_adapter *,struct ixgbe_ring *);
extern int ixgbe_setup_tx_resources(struct ixgbe_adapter *,struct ixgbe_ring *);
extern void ixgbe_free_rx_resources(struct ixgbe_adapter *,struct ixgbe_ring *);
extern void ixgbe_free_tx_resources(struct ixgbe_adapter *,struct ixgbe_ring *);
extern void ixgbe_update_stats(struct ixgbe_adapter *adapter);
extern int ixgbe_init_interrupt_scheme(struct ixgbe_adapter *adapter);
extern void ixgbe_clear_interrupt_scheme(struct ixgbe_adapter *adapter);
extern bool ixgbe_is_ixgbe(struct pci_dev *pcidev);
extern void ixgbe_alloc_rx_buffers(struct ixgbe_adapter *adapter,
                                   struct ixgbe_ring *rx_ring,
                                   int cleaned_count);
extern void ixgbe_rx_desc_queue_enable(struct ixgbe_adapter *adapter, int rxr);

extern void ixgbe_configure_rscctl(struct ixgbe_adapter *adapter, int index);
extern void ixgbe_clear_rscctl(struct ixgbe_adapter *adapter, int index);
void ixgbe_set_rx_mode(struct net_device *netdev);

#ifdef ETHTOOL_OPS_COMPAT
extern int ethtool_ioctl(struct ifreq *ifr);

#endif
extern int ixgbe_dcb_netlink_register(void);
extern int ixgbe_dcb_netlink_unregister(void);


#ifdef IXGBE_FCOE
extern void ixgbe_configure_fcoe(struct ixgbe_adapter *adapter);
extern int ixgbe_fso(struct ixgbe_adapter *adapter,
                     struct ixgbe_ring *tx_ring, struct sk_buff *skb,
                     u32 tx_flags, u8 *hdr_len);
extern void ixgbe_cleanup_fcoe(struct ixgbe_adapter *adapter);
extern int ixgbe_fcoe_ddp(struct ixgbe_adapter *adapter,
                          union ixgbe_adv_rx_desc *rx_desc,
                          struct sk_buff *skb);
extern int ixgbe_fcoe_ddp_get(struct net_device *netdev, u16 xid,
                              struct scatterlist *sgl, unsigned int sgc);
extern int ixgbe_fcoe_ddp_put(struct net_device *netdev, u16 xid);
#ifdef HAVE_NETDEV_OPS_FCOE_ENABLE
extern int ixgbe_fcoe_enable(struct net_device *netdev);
extern int ixgbe_fcoe_disable(struct net_device *netdev);
#endif /* HAVE_NETDEV_OPS_FCOE_ENABLE */
#ifdef CONFIG_DCB
#ifdef HAVE_DCBNL_OPS_GETAPP
extern u8 ixgbe_fcoe_getapp(struct ixgbe_adapter *adapter);
extern u8 ixgbe_fcoe_setapp(struct ixgbe_adapter *adapter, u8 up, u8 tc);
#endif /* HAVE_DCBNL_OPS_GETAPP */
#endif /* CONFIG_DCB */
#ifdef HAVE_NETDEV_OPS_FCOE_GETWWN
extern int ixgbe_fcoe_get_wwn(struct net_device *netdev, u64 *wwn, int type);
#endif
#endif /* IXGBE_FCOE */

#ifdef __VMKLNX__
extern void ixgbe_alloc_rx_buffers(struct ixgbe_adapter *adapter,
                                   struct ixgbe_ring *rx_ring,
                                   int cleaned_count);
extern void ixgbe_clean_rx_ring(struct ixgbe_adapter *adapter,
                                struct ixgbe_ring *rx_ring);
extern void ixgbe_rx_desc_queue_enable(struct ixgbe_adapter *adapter, int rxr);
extern int ixgbe_calculate_rx_ring_size(struct ixgbe_adapter *adapter);
#endif /* __VMKLNX__ */

#endif /* _IXGBE_H_ */