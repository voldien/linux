// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sys_ppc32.c: Conversion between 32bit and 64bit native syscalls.
 *
 * Copyright (C) 2001 IBM
 * Copyright (C) 1997,1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 *
 * These routines maintain argument size conversion between 32bit and 64bit
 * environment.
 */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h> 
#include <linux/mm.h> 
#include <linux/file.h> 
#include <linux/signal.h>
#include <linux/resource.h>
#include <linux/times.h>
#include <linux/smp.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include <linux/poll.h>
#include <linux/personality.h>
#include <linux/stat.h>
#include <linux/in.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/sysctl.h>
#include <linux/binfmts.h>
#include <linux/security.h>
#include <linux/compat.h>
#include <linux/ptrace.h>
#include <linux/elf.h>
#include <linux/ipc.h>
#include <linux/slab.h>

#include <asm/ptrace.h>
#include <asm/types.h>
#include <linux/uaccess.h>
#include <asm/unistd.h>
#include <asm/time.h>
#include <asm/mmu_context.h>
#include <asm/ppc-pci.h>
#include <asm/syscalls.h>
#include <asm/switch_to.h>

COMPAT_SYSCALL_DEFINE6(ppc_pread64,
		       unsigned int, fd,
		       char __user *, ubuf, compat_size_t, count,
		       u32, reg6, u32, pos1, u32, pos2)
{
	return ksys_pread64(fd, ubuf, count, merge_64(pos1, pos2));
}

COMPAT_SYSCALL_DEFINE6(ppc_pwrite64,
		       unsigned int, fd,
		       const char __user *, ubuf, compat_size_t, count,
		       u32, reg6, u32, pos1, u32, pos2)
{
	return ksys_pwrite64(fd, ubuf, count, merge_64(pos1, pos2));
}

COMPAT_SYSCALL_DEFINE5(ppc_readahead,
		       int, fd, u32, r4,
		       u32, offset1, u32, offset2, u32, count)
{
	return ksys_readahead(fd, merge_64(offset1, offset2), count);
}

COMPAT_SYSCALL_DEFINE4(ppc_truncate64,
		       const char __user *, path, u32, reg4,
		       unsigned long, len1, unsigned long, len2)
{
	return ksys_truncate(path, merge_64(len1, len2));
}

COMPAT_SYSCALL_DEFINE4(ppc_ftruncate64,
		       unsigned int, fd, u32, reg4,
		       unsigned long, len1, unsigned long, len2)
{
	return ksys_ftruncate(fd, merge_64(len1, len2));
}

COMPAT_SYSCALL_DEFINE6(ppc32_fadvise64,
		       int, fd, u32, unused, u32, offset1, u32, offset2,
		       size_t, len, int, advice)
{
	return ksys_fadvise64_64(fd, merge_64(offset1, offset2), len,
				 advice);
}

COMPAT_SYSCALL_DEFINE6(ppc_sync_file_range2,
		       int, fd, unsigned int, flags,
		       unsigned int, offset1, unsigned int, offset2,
		       unsigned int, nbytes1, unsigned int, nbytes2)
{
	loff_t offset = merge_64(offset1, offset2);
	loff_t nbytes = merge_64(nbytes1, nbytes2);

	return ksys_sync_file_range(fd, offset, nbytes, flags);
}
