/*-
 *   BSD LICENSE
 *
 *   Copyright (c) Intel Corporation.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _NVMF_RDMA_H_
#define _NVMF_RDMA_H_

#include <infiniband/verbs.h>

#include "nvmf_internal.h"
#include "spdk/nvmf_spec.h"

/* Define the Admin Queue Rx/Tx Descriptors */

struct nvme_qp_rx_desc {
	union nvmf_h2c_msg	msg_buf;
	struct spdk_nvmf_conn	*conn;
	struct ibv_mr		*msg_buf_mr;
	struct ibv_sge		recv_sgl;
	struct ibv_sge		bb_sgl; /* must follow recv_sgl */
	struct ibv_mr		*bb_mr;
	uint8_t			*bb;
	uint32_t		bb_len;
	uint32_t		recv_bc;
	STAILQ_ENTRY(nvme_qp_rx_desc) link;
};

struct nvme_qp_tx_desc {
	union nvmf_c2h_msg	msg_buf;
	struct spdk_nvmf_conn	*conn;
	struct nvmf_request	req_state;
	struct ibv_mr		*msg_buf_mr;
	struct ibv_sge		send_sgl;
	struct nvme_qp_rx_desc	*rx_desc;
	STAILQ_ENTRY(nvme_qp_tx_desc) link;
};

int nvmf_post_rdma_read(struct spdk_nvmf_conn *conn,
			struct nvme_qp_tx_desc *tx_desc);
int nvmf_post_rdma_write(struct spdk_nvmf_conn *conn,
			 struct nvme_qp_tx_desc *tx_desc);
int nvmf_post_rdma_recv(struct spdk_nvmf_conn *conn,
			struct nvme_qp_rx_desc *rx_desc);
int nvmf_post_rdma_send(struct spdk_nvmf_conn *conn,
			struct nvme_qp_tx_desc *tx_desc);
int nvmf_rdma_init(void);
void nvmf_rdma_conn_cleanup(struct spdk_nvmf_conn *conn);

int nvmf_acceptor_start(void);
void nvmf_acceptor_stop(void);

#endif /* _NVMF_RDMA_H_ */
