/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright 2014-2020, Intel Corporation */

/*
 * libpmem.h -- definitions of libpmem entry points
 *
 * This library provides support for programming with persistent memory (pmem).
 *
 * libpmem provides support for using raw pmem directly.
 *
 * See libpmem(7) for details.
 */

#ifndef LIBPMEM_H
#define LIBPMEM_H 1

#include <sys/types.h>

#ifdef _WIN32
#include <pmemcompat.h>

#ifndef PMDK_UTF8_API
#define pmem_map_file pmem_map_fileW
#define pmem_check_version pmem_check_versionW
#define pmem_errormsg pmem_errormsgW
#else
#define pmem_map_file pmem_map_fileU
#define pmem_check_version pmem_check_versionU
#define pmem_errormsg pmem_errormsgU
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This limit is set arbitrary to incorporate a pool header and required
 * alignment plus supply.
 */
#define PMEM_MIN_PART ((size_t)(1024 * 1024 * 2)) /* 2 MiB */

/*
 * flags supported by pmem_map_file()
 */
#define PMEM_FILE_CREATE	(1 << 0)
#define PMEM_FILE_EXCL		(1 << 1)
#define PMEM_FILE_SPARSE	(1 << 2)
#define PMEM_FILE_TMPFILE	(1 << 3)

#ifndef _WIN32
void *pmem_map_file(const char *path, size_t len, int flags, mode_t mode,
	size_t *mapped_lenp, int *is_pmemp);
#else
void *pmem_map_fileU(const char *path, size_t len, int flags, mode_t mode,
	size_t *mapped_lenp, int *is_pmemp);
void *pmem_map_fileW(const wchar_t *path, size_t len, int flags, mode_t mode,
	size_t *mapped_lenp, int *is_pmemp);
#endif

//
int pmem_wrap_unmap(void *addr, size_t len, const char *file, int line);
int pmem_unmap(void *addr, size_t len);
//
int pmem_is_pmem(const void *addr, size_t len);
//
void pmem_wrap_persist(const void *addr, size_t len, const char *file, int line);
void pmem_persist(const void *addr, size_t len);
int pmem_wrap_msync(const void *addr, size_t len, const char *file, int line);
int pmem_msync(const void *addr, size_t len);
//
int pmem_has_auto_flush(void);
void pmem_flush(const void *addr, size_t len);
void pmem_deep_flush(const void *addr, size_t len);
int pmem_deep_drain(const void *addr, size_t len);
int pmem_deep_persist(const void *addr, size_t len);
//
void pmem_wrap_drain(const char *file, int line);
void pmem_drain();
//
int pmem_has_hw_drain(void);

//
void *pmem_wrap_memmove_persist(void *pmemdest, const void *src, size_t len, const char *file, int line);
void *pmem_wrap_memcpy_persist(void *pmemdest, const void *src, size_t len, const char *file, int line);
void *pmem_wrap_memset_persist(void *pmemdest, int c, size_t len, const char *file, int line);
void *pmem_memmove_persist(void *pmemdest, const void *src, size_t len);
void *pmem_memcpy_persist(void *pmemdest, const void *src, size_t len);
void *pmem_memset_persist(void *pmemdest, int c, size_t len);
//
void *pmem_memmove_nodrain(void *pmemdest, const void *src, size_t len);
void *pmem_memcpy_nodrain(void *pmemdest, const void *src, size_t len);
void *pmem_memset_nodrain(void *pmemdest, int c, size_t len);

#define PMEM_F_MEM_NODRAIN	(1U << 0)

#define PMEM_F_MEM_NONTEMPORAL	(1U << 1)
#define PMEM_F_MEM_TEMPORAL	(1U << 2)

#define PMEM_F_MEM_WC		(1U << 3)
#define PMEM_F_MEM_WB		(1U << 4)

#define PMEM_F_MEM_NOFLUSH	(1U << 5)

#define PMEM_F_MEM_VALID_FLAGS (PMEM_F_MEM_NODRAIN | \
				PMEM_F_MEM_NONTEMPORAL | \
				PMEM_F_MEM_TEMPORAL | \
				PMEM_F_MEM_WC | \
				PMEM_F_MEM_WB | \
				PMEM_F_MEM_NOFLUSH)

//
void *pmem_wrap_memmove(void *pmemdest, const void *src, size_t len, unsigned flags, const char *file, int line);
void *pmem_wrap_memcpy(void *pmemdest, const void *src, size_t len, unsigned flags, const char *file, int line);
void *pmem_wrap_memset(void *pmemdest, int c, size_t len, unsigned flags, const char *file, int line);
void *pmem_memmove(void *pmemdest, const void *src, size_t len, unsigned flags);
void *pmem_memcpy(void *pmemdest, const void *src, size_t len, unsigned flags);
void *pmem_memset(void *pmemdest, int c, size_t len, unsigned flags);
//

/*
 * PMEM_MAJOR_VERSION and PMEM_MINOR_VERSION provide the current version of the
 * libpmem API as provided by this header file.  Applications can verify that
 * the version available at run-time is compatible with the version used at
 * compile-time by passing these defines to pmem_check_version().
 */
#define PMEM_MAJOR_VERSION 1
#define PMEM_MINOR_VERSION 1

#ifndef _WIN32
const char *pmem_check_version(unsigned major_required,
	unsigned minor_required);
#else
const char *pmem_check_versionU(unsigned major_required,
	unsigned minor_required);
const wchar_t *pmem_check_versionW(unsigned major_required,
	unsigned minor_required);
#endif

#ifndef _WIN32
const char *pmem_errormsg(void);
#else
const char *pmem_errormsgU(void);
const wchar_t *pmem_errormsgW(void);
#endif

#ifdef __cplusplus
}
#endif

void force_abort_drain(const char *file, int line);
void force_set_abortflag(const char *file, int line);
void rand_set_abortflag_plus_persistcount(const char *file, int line);
void pmem_drain_nowrap();
void debug_print_line(const char *file, int line);

#define pmem_persist(addr, len) pmem_wrap_persist((addr), (len), __FILE__, __LINE__)
#define pmem_msync(addr, len) pmem_wrap_msync((addr), (len), __FILE__, __LINE__)
#define pmem_memmove_persist(pmemdest, src, len) pmem_wrap_memmove_persist((pmemdest), (src), (len), __FILE__, __LINE__)
#define pmem_memcpy_persist(pmemdest, src, len) pmem_wrap_memcpy_persist((pmemdest), (src), (len), __FILE__, __LINE__)
#define pmem_memset_persist(pmemdest, c, len) pmem_wrap_memset_persist((pmemdest), (c), (len), __FILE__, __LINE__)
#define pmem_memmove(pmemdest, src, len, flags) pmem_wrap_memmove((pmemdest), (src), (len), (flags), __FILE__, __LINE__)
#define pmem_memcpy(pmemdest, src, len, flags) pmem_wrap_memcpy((pmemdest), (src), (len), (flags), __FILE__, __LINE__)
#define pmem_memset(pmemdest, c, len, flags) pmem_wrap_memset((pmemdest), (c), (len), (flags), __FILE__, __LINE__)
#define pmem_unmap(addr, len) pmem_wrap_unmap((addr), (len),  __FILE__, __LINE__)
#define pmem_drain() pmem_wrap_drain(__FILE__, __LINE__)

#define PMEMWRAP_FORCE_ABORT() force_abort_drain(__FILE__, __LINE__)
#define PMEMWRAP_SET_ABORTFLAG() force_set_abortflag(__FILE__, __LINE__)
#define PMEMWRAP_PRINT_OFFSET(__stream, p, p2) fprintf((__stream), "fprint_offset: %lx\n", (p2) - (p))
#define PMEMWRAP_RAND_SET_ABORTFLAG() rand_set_abortflag_plus_persistcount(__FILE__, __LINE__)
#define PMEMWRAP_DRAIN_NOWRAP() pmem_drain_nowrap()
#define PMEMWRAP_PRINT_LINE() debug_print_line(__FILE__, __LINE__)

#endif	/* libpmem.h */
