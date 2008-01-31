/* -*- mode: c; c-basic-offset: 8; indent-tabs-mode: nil; -*-
 * vim:expandtab:shiftwidth=8:tabstop=8:
 *
 *  Copyright (C) 2003 Cluster File Systems, Inc.
 *
 *   This file is part of the Lustre file system, http://www.lustre.org
 *   Lustre is a trademark of Cluster File Systems, Inc.
 *
 *   You may have signed or agreed to another license before downloading
 *   this software.  If so, you are bound by the terms and conditions
 *   of that agreement, and the following does not apply to you.  See the
 *   LICENSE file included with this distribution for more information.
 *
 *   If you did not agree to a different license, then this copy of Lustre
 *   is open source software; you can redistribute it and/or modify it
 *   under the terms of version 2 of the GNU General Public License as
 *   published by the Free Software Foundation.
 *
 *   In either case, Lustre is distributed in the hope that it will be
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   license text for more details.
 *
 */

/* Intramodule declarations for ptlrpc. */

#ifndef PTLRPC_INTERNAL_H
#define PTLRPC_INTERNAL_H

#include "../ldlm/ldlm_internal.h"

struct ldlm_namespace;
struct obd_import;
struct ldlm_res_id;
struct ptlrpc_request_set;
extern int test_req_buffer_pressure;

void ptlrpc_request_handle_notconn(struct ptlrpc_request *);
void lustre_assert_wire_constants(void);
int ptlrpc_import_in_recovery(struct obd_import *imp);
int ptlrpc_set_import_discon(struct obd_import *imp, __u32 conn_cnt);
void ptlrpc_handle_failed_import(struct obd_import *imp);
int ptlrpc_replay_next(struct obd_import *imp, int *inflight);
void ptlrpc_initiate_recovery(struct obd_import *imp);

int lustre_unpack_req_ptlrpc_body(struct ptlrpc_request *req, int offset);
int lustre_unpack_rep_ptlrpc_body(struct ptlrpc_request *req, int offset);

#ifdef LPROCFS
void ptlrpc_lprocfs_register_service(struct proc_dir_entry *proc_entry,
                                     struct ptlrpc_service *svc);
void ptlrpc_lprocfs_unregister_service(struct ptlrpc_service *svc);
void ptlrpc_lprocfs_rpc_sent(struct ptlrpc_request *req);
void ptlrpc_lprocfs_do_request_stat (struct ptlrpc_request *req,
                                     long q_usec, long work_usec);
#else
#define ptlrpc_lprocfs_register_service(params...) do{}while(0)
#define ptlrpc_lprocfs_unregister_service(params...) do{}while(0)
#define ptlrpc_lprocfs_rpc_sent(params...) do{}while(0)
#define ptlrpc_lprocfs_do_request_stat(params...) do{}while(0)
#endif /* LPROCFS */

/* recovd_thread.c */
int llog_init_commit_master(void);
int llog_cleanup_commit_master(int force);

int ptlrpc_expire_one_request(struct ptlrpc_request *req);

/* pers.c */
void ptlrpc_fill_bulk_md(lnet_md_t *md, struct ptlrpc_bulk_desc *desc);
void ptlrpc_add_bulk_page(struct ptlrpc_bulk_desc *desc, cfs_page_t *page,
                          int pageoffset, int len);
void ptl_rpc_wipe_bulk_pages(struct ptlrpc_bulk_desc *desc);

/* pack_generic.c */
struct ptlrpc_reply_state *lustre_get_emerg_rs(struct ptlrpc_service *svc);
void lustre_put_emerg_rs(struct ptlrpc_reply_state *rs);

/* pinger.c */
int ptlrpc_start_pinger(void);
int ptlrpc_stop_pinger(void);
void ptlrpc_pinger_sending_on_import(struct obd_import *imp);
void ptlrpc_pinger_wake_up(void);
void ptlrpc_ping_import_soon(struct obd_import *imp);
#ifdef __KERNEL__
int ping_evictor_wake(struct obd_export *exp);
#else
#define ping_evictor_wake(exp)     1
#endif

/* sec_null.c */
int  sptlrpc_null_init(void);
void sptlrpc_null_fini(void);

/* sec_plain.c */
int  sptlrpc_plain_init(void);
void sptlrpc_plain_fini(void);

/* sec_bulk.c */
int  sptlrpc_enc_pool_init(void);
void sptlrpc_enc_pool_fini(void);
int sptlrpc_proc_read_enc_pool(char *page, char **start, off_t off, int count,
                               int *eof, void *data);

/* sec_lproc.c */
int  sptlrpc_lproc_init(void);
void sptlrpc_lproc_fini(void);

/* sec_gc.c */
int sptlrpc_gc_start_thread(void);
void sptlrpc_gc_stop_thread(void);

/* sec.c */
int  __init sptlrpc_init(void);
void __exit sptlrpc_fini(void);

static inline int ll_rpc_recoverable_error(int rc)
{ 
        return (rc == -ENOTCONN || rc == -ENODEV);
}
#endif /* PTLRPC_INTERNAL_H */
