#include <reent.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <rtthread.h>

#ifdef RT_USING_DFS
#include <dfs_posix.h>
#endif

#ifdef RT_USING_PTHREADS 
#include <pthread.h>
#endif

/* Reentrant versions of system calls.  */

int
_close_r(struct _reent *ptr, int fd)
{
#ifndef RT_USING_DFS
	return 0;
#else
	return close(fd);
#endif
}

int
_execve_r(struct _reent *ptr, const char * name, char *const *argv, char *const *env)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fork_r(struct _reent *ptr)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_getpid_r(struct _reent *ptr)
{
	return 0;
}

int
_isatty_r(struct _reent *ptr, int fd)
{
	if (fd >=0 && fd < 3) return 1;

	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_kill_r(struct _reent *ptr, int pid, int sig)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_link_r(struct _reent *ptr, const char *old, const char *new)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

_off_t
_lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
#ifndef RT_USING_DFS
	return 0;
#else
	_off_t rc;

	rc = lseek(fd, pos, whence);
	return rc;
#endif
}

int
_mkdir_r(struct _reent *ptr, const char *name, int mode)
{
#ifndef RT_USING_DFS
	return 0;
#else
	int rc;

	rc = mkdir(name, mode);
	return rc;
#endif
}

int
_open_r(struct _reent *ptr, const char *file, int flags, int mode)
{
#ifndef RT_USING_DFS
	return 0;
#else
	int rc;

	rc = open(file, flags, mode);
	return rc;
#endif
}

_ssize_t 
_read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
#ifndef RT_USING_DFS
	return 0;
#else
	_ssize_t rc;

	rc = read(fd, buf, nbytes);
	return rc;
#endif
}

int
_rename_r(struct _reent *ptr, const char *old, const char *new)
{
#ifndef RT_USING_DFS
	return 0;
#else
	int rc;

	rc = rename(old, new);
	return rc;
#endif
}

void *
_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
	/* no use this routine to get memory */
	return RT_NULL;
}

int
_stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
#ifndef RT_USING_DFS
	return 0;
#else
	int rc;

	rc = stat(file, pstat);
	return rc;
#endif
}

_CLOCK_T_
_times_r(struct _reent *ptr, struct tms *ptms)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

int
_unlink_r(struct _reent *ptr, const char *file)
{
#ifndef RT_USING_DFS
	return 0;
#else
	int rc;

	rc = unlink(file);
	return rc;
#endif
}

int
_wait_r(struct _reent *ptr, int *status)
{
	/* return "not supported" */
	ptr->_errno = ENOTSUP;
	return -1;
}

_ssize_t
_write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
#ifndef RT_USING_DFS
	return 0;
#else
	_ssize_t rc;

	rc = write(fd, buf, nbytes);
	return rc;
#endif
}

int
gettimeofday(struct timeval *__tp, void *__tzp)
{
#ifndef RT_USING_RTC
	/* return "not supported" */
	return -1;
#else
	static time_t sysnow = 0;
	static uint16_t sysms = 0;
	time_t nownow = time(NULL);
	uint16_t nowms = rt_tick_get() % 1000;
	if (sysnow == 0 && sysms == 0)
	{
		sysnow = nownow;
		sysms = nowms;
	}
	else
	{
		//如果毫秒走的比较快，在秒上面增加
		if ((sysnow == nownow)
				&& (nowms<sysnow))
			sysnow = nownow+1;
		else
			sysnow = nownow;
		sysms = nowms;
	}
	__tp->tv_sec = sysnow;
	__tp->tv_usec = sysms * 1000l;
	return 0;
#endif
}
RTM_EXPORT(gettimeofday);

int
settimeofday(const struct timeval *__tp, const struct timezone *__tzp)
{
#ifndef RT_USING_RTC
	/* return "not supported" */
	return -1;
#else
	rt_device_t device;
	device = rt_device_find("rtc");
    if (device == RT_NULL)
        return -1;
    /* update to RTC device. */
    rt_device_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, (void *)&__tp->tv_sec);
	return 0;
#endif
}
RTM_EXPORT(settimeofday);

/* Memory routine */
void *
_malloc_r (struct _reent *ptr, size_t size)
{
	void* result;

	result = (void*)rt_malloc (size);
	if (result == RT_NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void *
_realloc_r (struct _reent *ptr, void *old, size_t newlen)
{
	void* result;

	result = (void*)rt_realloc (old, newlen);
	if (result == RT_NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void *_calloc_r (struct _reent *ptr, size_t size, size_t len)
{
	void* result;

	result = (void*)rt_calloc (size, len);
	if (result == RT_NULL)
	{
		ptr->_errno = ENOMEM;
	}

	return result;
}

void 
_free_r (struct _reent *ptr, void *addr)
{
	rt_free (addr);
}

void
_exit (int status)
{
#ifdef RT_USING_MODULE
	rt_module_t module;

	module = rt_module_self();
	if (module != RT_NULL)
	{
		struct rt_list_node *list;
		struct rt_object *object;

		rt_enter_critical();
		
        /* delete all threads in the module */
        list = &module->module_object[RT_Object_Class_Thread].object_list;
        while (list->next != list)
        {
            object = rt_list_entry(list->next, struct rt_object, list);
            if (rt_object_is_systemobject(object) == RT_TRUE)
            {
                /* detach static object */
                rt_thread_detach((rt_thread_t)object);
            }
            else
            {
                /* delete dynamic object */
                rt_thread_delete((rt_thread_t)object);
            }
        }
		/* delete main thread */
		rt_thread_delete(module->module_thread);
		rt_exit_critical();

		/* re-schedule */
		rt_schedule();
	}
#endif
#ifdef RT_USING_PROCESS
	rt_process_t process;

	process = rt_process_self();
	if (process != RT_NULL)
	{
		/* unload assertion module */
		rt_process_unload(process, (status&0xff)<<8);

		/* re-schedule */
		rt_schedule();
	}
#endif
	
	rt_kprintf("thread:%s exit with %d\n", rt_thread_self()->name, status);
	RT_ASSERT(0);

	while (1);
}

void 
_system(const char *s)
{
    /* not support this call */
    return;
}

void __libc_init_array(void)
{
	/* we not use __libc init_aray to initialize C++ objects */
}

void abort(void)
{
    if (rt_thread_self())
    {
        rt_thread_t self = rt_thread_self();

        rt_kprintf("thread:%-8.*s abort!\n", RT_NAME_MAX, self->name);
        rt_thread_suspend(self);

        rt_schedule();
    }

	while (1);
}

#ifdef RT_USING_PROCESS
#if 0
#define PRESS_DEBUG_FILE rt_kprintf
#else
#define PRESS_DEBUG_FILE(...)
#endif
#if 1
#define PRESS_DEBUG_NOSYS rt_kprintf
#else
#define PRESS_DEBUG_NOSYS(...)
#endif
#if 1
#define PRESS_DEBUG_IOCTL rt_kprintf
#else
#define PRESS_DEBUG_IOCTL(...)
#endif

#include <rthw.h>
#include "linux-syscall.h"
#include "linux-usedef.h"

extern void *rt_process_conv_ptr(rt_process_t module, rt_uint32_t ptr, rt_int32_t size);
extern rt_uint32_t rt_process_brk(rt_process_t module, rt_uint32_t addr);
extern int rt_process_fork(rt_process_t module);
extern int rt_process_vfork(rt_process_t module);
extern int rt_process_execve(rt_process_t module, const char*file, const char **argv, const char **envp);
extern int rt_process_waitpid(rt_process_t module, pid_t pid, int* status, int opt);
extern int rt_process_savefile(rt_process_t module, int fileno);
extern int rt_process_convfile(rt_process_t module, int fileno);
extern int rt_process_setfile(rt_process_t module, int fileno, int newfileno);
extern int rt_process_clearfile(rt_process_t module, int fileno);
extern int rt_process_kill(rt_process_t module, int pid, int sig);
static inline int ret_err(int ret)
{
    if (ret < 0)
    {
        int err = errno;
        if (err == 0)
            err = rt_get_errno();
        if (err != 0)
            return (err>0)?(-err):(err);
    }
    return ret;
}

rt_uint32_t sys_call_switch(rt_uint32_t nbr, rt_uint32_t parm1,
        rt_uint32_t parm2, rt_uint32_t parm3,
        rt_uint32_t parm4, rt_uint32_t parm5,
        rt_uint32_t parm6)
{
	register rt_base_t temp;
	rt_process_t module = rt_process_self();

    RT_ASSERT(module != RT_NULL);
    RT_ASSERT((nbr&SYS_BASE) == SYS_BASE);
    //rt_kprintf("syscall %d in\n",nbr-SYS_BASE);
    __asm volatile("msr cpsr_c, #0x13");

    switch(nbr)
    {
    case SYS_exit:
    {
        _exit(parm1);
        return -ENOTSUP;
    }
    case SYS_brk:
    {
        return rt_process_brk(module,parm1);
    }
    case SYS_chmod:
    case SYS_fchmod:
    {
        return 0;
    }
    case SYS_chown:
    case SYS_fchown:
    case SYS_lchown:
    {
        return 0;
    }
    case SYS_nanosleep:
    {
        struct timespec *tim1 = (parm1)?(rt_process_conv_ptr(module,parm1,sizeof(struct timespec))):(0);
        struct timespec *tim2 = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(struct timespec))):(0);
        if (tim1 == RT_NULL)
            return -EINVAL;
        //长时间休眠，分段休眠
        extern void __udelay(unsigned long usecs);
        while (tim1->tv_sec > 10*24*3600)
        {
            rt_thread_delay(10*24*3600*1000);
            tim1->tv_sec -= 10*24*3600;
        }
        //休息剩下的时间
        int ms = tim1->tv_sec*1000+tim1->tv_nsec/1000000l;
        if (ms <= 0)
            __udelay(tim1->tv_nsec/1000l+1);
        else
            rt_thread_delay(ms);
        if (tim2)
        {
            tim2->tv_sec = 0;
            tim2->tv_nsec = 0;
        }
        return 0;
    }
    case SYS_getpid:
    {
        return module->tpid;
    }
    case SYS_getppid:
    {
        if (pidinfo[module->tpid-1][0] < 100)
            return -ESRCH;
        return pidinfo[module->tpid-1][1];
    }
    case SYS_getpgrp:
    {
        if (pidinfo[module->tpid-1][0] < 100 || pidinfo[module->tpid-1][2] == 0)
            return -ESRCH;
        return pidinfo[module->tpid-1][2];
    }
    case SYS_getpgid:
    {
    	if (parm1 == 0)
    		parm1 = module->tpid;
    	if (parm1 < 0 && parm1 > MAX_PID_SIZE)
    		return -EINVAL;
    	if (pidinfo[parm1-1][0] < 100 || pidinfo[parm1-1][2] == 0)
    		return -ESRCH;
    	return pidinfo[parm1-1][2];
    }
    case SYS_getsid:
    {
    	if (parm1 == 0)
    		parm1 = module->tpid;
    	if (parm1 < 0 && parm1 > MAX_PID_SIZE)
    		return -EINVAL;
    	if (pidinfo[parm1-1][0] < 100 || pidinfo[parm1-1][3] == 0)
    		return -ESRCH;
    	return pidinfo[parm1-1][3];
    }
    case SYS_setpgid:
    {
    	if (parm1 == 0)
    		parm1 = module->tpid;
    	if (parm2 == 0)
    		parm2 = module->tpid;
    	if (parm1 < 0 && parm1 > MAX_PID_SIZE)
    		return -EINVAL;
    	if (pidinfo[parm1-1][0] < 100)
    		return -ESRCH;
    	if (parm2 < 0 && parm2 > MAX_PID_SIZE)
    		return -EINVAL;
    	if (pidinfo[parm2-1][0] < 100)
    		return -ESRCH;
    	temp = rt_hw_interrupt_disable();
    	pidinfo[parm1-1][2] = parm2;
    	rt_hw_interrupt_enable(temp);
    	return parm2;
    }
    case SYS_setsid:
    {
    	if (module->tpid == pidinfo[module->tpid-1][2])
    		return -EPERM;
    	temp = rt_hw_interrupt_disable();
    	pidinfo[module->tpid-1][2] = module->tpid;
    	pidinfo[module->tpid-1][3] = module->tpid;
    	rt_hw_interrupt_enable(temp);
    	return module->tpid;
    }
    case SYS_reboot:
    {
        if ((parm1 != 0xfee1dead) || (parm2 != 0x28121969))
            return -EINVAL;
        if (parm3 == RB_ENABLE_CAD || parm3 == RB_DISABLE_CAD)
            return 0;
        extern void sys_reboot(void);
        sys_reboot();
        return 0;
    }
    case SYS_fork:
    {
    	return rt_process_fork(module);
    }
    case SYS_vfork:
    {
    	return rt_process_vfork(module);
    }
    case SYS_execve:
    {
        const char *file = (const char *)rt_process_conv_ptr(module,parm1,0);
        const char **argv = (parm2)?(rt_process_conv_ptr(module,parm2,-1)):(0);
        const char **envp = (parm3)?(rt_process_conv_ptr(module,parm3,-1)):(0);
        return rt_process_execve(module,file,argv,envp);
    }
    case SYS_sigaction:
    {
        struct sigaction *sig = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(struct sigaction))):(0);
        struct sigaction *osig = (parm3)?(rt_process_conv_ptr(module,parm3,sizeof(struct sigaction))):(0);
        if (parm1 >= NSIG)
            return -EINVAL;
        //保存上一个数据
        temp = rt_hw_interrupt_disable();
        if (osig != RT_NULL)
            *osig = module->sigact[parm1];
        //设置新值
        if (sig != RT_NULL)
            module->sigact[parm1] = *sig;
        rt_hw_interrupt_enable(temp);
        return 0;
    }
    case SYS_sigprocmask:
    {
        sigset_t *sig = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(sigset_t))):(0);
        sigset_t *osig = (parm3)?(rt_process_conv_ptr(module,parm3,sizeof(sigset_t))):(0);
        if (parm1 > SIG_UNBLOCK)
            return -EINVAL;
        //保存上一个数据
        temp = rt_hw_interrupt_disable();
        if (osig != RT_NULL)
            *osig = module->sigset;
        //设置新值
        if (sig != RT_NULL)
        {
            if (parm1 == SIG_BLOCK)
                module->sigset |= *sig;
            else if (parm1 == SIG_UNBLOCK)
                module->sigset &= ~(*sig);
            else if (parm1 == SIG_SETMASK)
                module->sigset = *sig;
        }
        rt_hw_interrupt_enable(temp);
        return 0;
    }
    case SYS_kill:
    {
        if (parm2 >= NSIG)
            return -EINVAL;
        return rt_process_kill(module,parm1,parm2);
    }
    case SYS_wait4:
    {
        int *status = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(int *))):(0);
        return rt_process_waitpid(module,(pid_t)parm1,status,parm3);
    }
    case SYS_getuid:
    case SYS_getgid:
    case SYS_geteuid:
    case SYS_getegid:
    {
        return 0;
    }
    case SYS_setuid:
    case SYS_setgid:
    case SYS_setgroups:
    {
        return 0;
    }
    case SYS_getgroups:
    {
        if (parm1 == 0 || parm2 == 0)
            return 1;
        gid_t *groups = rt_process_conv_ptr(module,parm2,parm1*sizeof(gid_t));
        groups[0] = 0;
        return 1;
    }
    case SYS_link:
    {
        errno = 0;
        const char *oldpath = (const char *)rt_process_conv_ptr(module,parm1,0);
        const char *newpath = (const char *)rt_process_conv_ptr(module,parm2,0);
        return ret_err(link(oldpath,newpath));
    }
    case SYS_symlink:
    {
        errno = 0;
        const char *oldpath = (const char *)rt_process_conv_ptr(module,parm1,0);
        const char *newpath = (const char *)rt_process_conv_ptr(module,parm2,0);
        return ret_err(symlink(oldpath,newpath));
    }
    case SYS_readlink:
    {
        errno = 0;
        const char *file = (const char *)rt_process_conv_ptr(module,parm1,0);
        char *buf = (char *)rt_process_conv_ptr(module,parm2,parm3);
        return ret_err(readlink(file,buf,parm3));
    }
    case SYS_truncate:
    {
        errno = 0;
        const char *file = (const char *)rt_process_conv_ptr(module,parm1,0);
        return ret_err(truncate(file,parm2));
    }
    case SYS_ftruncate:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        return ret_err(ftruncate(fileno,parm2));
    }
    case SYS_uname:
    {
        struct utsname* uname = (struct utsname*)rt_process_conv_ptr(module,parm1,sizeof(struct utsname));
        strcpy(uname->sysname,"RT-Thread");
        strcpy(uname->nodename,"local");
        sprintf(uname->release,"%ld.%ld.%ld",RT_VERSION,RT_SUBVERSION,RT_REVISION);
        strcpy(uname->version,__DATE__);
        strcpy(uname->machine,"ARM");
        strcpy(uname->__domainname,"localdomain");
        return 0;
    }
    case SYS_unlink:
    case SYS_rmdir:
    {
        errno = 0;
        const char *file = (const char *)rt_process_conv_ptr(module,parm1,0);
        return ret_err(unlink(file));
    }
    case SYS_rename:
    {
        errno = 0;
        const char *oldpath = (const char *)rt_process_conv_ptr(module,parm1,0);
        const char *newpath = (const char *)rt_process_conv_ptr(module,parm2,0);
        return ret_err(rename(oldpath,newpath));
    }
    case SYS_mkdir:
    {
        errno = 0;
        const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
        return ret_err(mkdir(path,parm2));
    }
    case SYS_lseek:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        return ret_err(lseek(fileno,parm2,parm3));
    }
    case SYS_stat:
    {
        errno = 0;
        const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
        struct stat *buf = (struct stat *)rt_process_conv_ptr(module,parm2,sizeof(struct stat));
        return ret_err(stat(path,buf));
    }
    case SYS_lstat:
    {
        errno = 0;
        const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
        struct stat *buf = (struct stat *)rt_process_conv_ptr(module,parm2,sizeof(struct stat));
        return ret_err(lstat(path,buf));
    }
    case SYS_fstat:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        struct stat *buf = (struct stat *)rt_process_conv_ptr(module,parm2,sizeof(struct stat));
        return ret_err(fstat(fileno,buf));
    }
    case SYS_statfs:
    {
        errno = 0;
        const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
        struct statfs *buf = (struct statfs *)rt_process_conv_ptr(module,parm2,sizeof(struct statfs));
        return ret_err(statfs(path,buf));
    }
    case SYS_fstatfs:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        struct statfs *buf = (struct statfs *)rt_process_conv_ptr(module,parm2,sizeof(struct statfs));
        return ret_err(fstatfs(fileno,buf));
    }
    case SYS_creat:
    {
        return sys_call_switch(SYS_open,parm1,O_TRUNC|O_WRONLY|O_CREAT,0,0,0,0);
    }
    case SYS_open:
    {
       errno = 0;
       if (parm1 == 0)
           return -EINVAL;
       const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
       int fileno = ret_err(open(path,parm2,0));
       int retno = fileno;
       if (retno >= 0)
       {
           retno = rt_process_savefile(module,fileno);
           if (retno < 0)
               close(fileno);
       }
       //如果是打开0文件句柄，设置控制终端的相关参数
       if (retno == 0)
       {
            pid_t sid = pidinfo[module->tpid-1][2];
            ioctl(fileno,RT_DEVICE_CTRL_SPGRP,&sid);
            sid = pidinfo[module->tpid-1][3];
            ioctl(fileno,RT_DEVICE_CTRL_SSID,&sid);
       }
       PRESS_DEBUG_FILE("syscall pid:%d/%d open  file %3d/%3d lnk %s\n",module->pid,module->tpid,retno,fileno,path);
       return retno;
    }
    case SYS_dup:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        struct dfs_fd *d = fd_get(fileno);
        if (d == NULL)
        {
            rt_process_clearfile(module,parm1);
            return -EBADF;
        }
        int retno = rt_process_savefile(module,fileno);
        if (retno < 0)
        {
            fd_put(d);
            return -ENOMEM;
        }
        PRESS_DEBUG_FILE("syscall pid:%d/%d dup   file %3d/%3d ret %d\n",module->pid,module->tpid,parm1,fileno,retno);
        return retno;
    }
    case SYS_dup2:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        struct dfs_fd *d = fd_get(fileno);
        if (d == NULL)
        {
            rt_process_clearfile(module,parm1);
            return -EBADF;
        }
        if (parm1 == parm2)
        {
            fd_put(d);
            return parm1;
        }
        //关闭原有打开的文件
        if (rt_process_convfile(module,parm2) >= 0)
        {
            if (sys_call_switch(SYS_close,parm2,0,0,0,0,0) != 0)
                return -EBADF;
        }
        if (rt_process_setfile(module,parm2,fileno) < 0)
        {
            fd_put(d);
            return -ENOMEM;
        }
        PRESS_DEBUG_FILE("syscall pid:%d/%d dup2  file %3d/%3d ret %d\n",module->pid,module->tpid,parm1,fileno,parm2);
        return parm2;
    }
    case SYS_read:
    {
       errno = 0;
       int fileno = rt_process_convfile(module,parm1);
       if (fileno < 0)
           return -EBADF;
       void *buf = rt_process_conv_ptr(module,parm2,3);
       return ret_err(read(fileno,buf,parm3));
    }
    case SYS_write:
    {
       errno = 0;
       int fileno = rt_process_convfile(module,parm1);
       if (fileno < 0)
           return -EBADF;
       void *buf = rt_process_conv_ptr(module,parm2,3);
       return ret_err(write(fileno,buf,parm3));
    }
    case SYS_close:
    {
       errno = 0;
       int fileno = rt_process_convfile(module,parm1);
       if (fileno < 0)
          return -EBADF;
       struct dfs_fd *d = fd_get(fileno);
       if (d == NULL)
       {
           rt_process_clearfile(module,parm1);
           return -EBADF;
       }
       fd_put(d);
       //多个连接不能真正关闭
       if (d->ref_count >= 2)
       {
           int ret = rt_process_clearfile(module,parm1);
           if (ret == 0)
               fd_put(d);
           return ret;
       }
       //真正关闭
       int ret = ret_err(close(fileno));
       if (ret == 0)
          ret = rt_process_clearfile(module,parm1);
       PRESS_DEBUG_FILE("syscall pid:%d/%d close file %3d/%3d ret %d\n",module->pid,module->tpid,parm1,fileno,ret);
       return ret;
    }
    case SYS_getdents:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        struct dirent* buf = (struct dirent*)rt_process_conv_ptr(module,parm2,parm3);
        return ret_err(getdents(fileno,buf,parm3));
    }
    case SYS_sendfile:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        int fileno2 = rt_process_convfile(module,parm2);
        if (fileno2 < 0)
            return -EBADF;
        off_t *off = (parm3)?(rt_process_conv_ptr(module,parm3,sizeof(off_t))):(0);
        return ret_err(sendfile(fileno,fileno2,off,parm4));
    }
    case SYS_getcwd:
    {
        char *buf = rt_process_conv_ptr(module,parm1,parm2);
        return (rt_uint32_t)getcwd(buf,parm2);
    }
    case SYS_chdir:
    {
        errno = 0;
        const char *path = (const char *)rt_process_conv_ptr(module,parm1,0);
        return ret_err(chdir(path));
    }
    case SYS_fsync:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        return ret_err(fsync(fileno));
    }
    case SYS_sync:
    {
        rt_kprintf("sync\n");
        return 0;
    }
    case SYS_poll:
    {
        errno = 0;
        //纯粹睡眠
        if (parm1 == 0 || parm2 == 0)
        {
            if ((int)parm3 > 0)
                rt_thread_delay(parm3);
            return 0;
        }
        //找到最大的fd,标记set
        int i,maxfd = -1,pz = 0;
        int subsize = (DFS_FD_MAX+31)/32;
        rt_uint32_t *pollset = rt_malloc(3*subsize*sizeof(rt_uint32_t));
        if (pollset == RT_NULL)
            return -ENOMEM;
        rt_memset(pollset, 0, 3*subsize*sizeof(rt_uint32_t));
        struct pollfd *ufds = (struct pollfd *)rt_process_conv_ptr(module,parm1,parm2*sizeof(struct pollfd));
        for (i=0; i<parm2; i++)
        {
            int fileno = rt_process_convfile(module,ufds[i].fd);
            if (fileno < 0)
                continue;
            if (maxfd < fileno)
                maxfd = fileno;
            if (ufds[i].events & (POLLIN|POLLPRI))
            {
                pz |= 0x01;
                pollset[fileno/32] |= (1<<(fileno%32));
                pollset[2*subsize+fileno/32] |= (1<<(fileno%32));
            }
            if (ufds[i].events & POLLOUT)
            {
                pz |= 0x02;
                pollset[subsize+fileno/32] |= (1<<(fileno%32));
                pollset[2*subsize+fileno/32] |= (1<<(fileno%32));
            }
        }
        if (pz == 0)
        {
            if ((int)parm3 > 0)
                rt_thread_delay(parm3);
            rt_free(pollset);
            return 0;
        }
        int ret = dfs_file_select(maxfd+1,
                ((pz&0x01)?&pollset[0]:RT_NULL),
                ((pz&0x02)?&pollset[subsize]:RT_NULL),
                &pollset[2*subsize],
                parm3);
        if (ret <= 0)
        {
            rt_free(pollset);
            return ret;
        }
        //设置相关set
        for (i=0; i<parm2; i++)
        {
            int fileno = rt_process_convfile(module,ufds[i].fd);
            if (fileno < 0)
                continue;
            if (ufds[i].events & (POLLIN|POLLPRI))
            {
                if (pollset[fileno/32] & (1<<(fileno%32)))
                    ufds[i].revents |= (POLLIN|POLLPRI);
            }
            if (ufds[i].events & POLLOUT)
            {
                if (pollset[subsize+fileno/32] & (1<<(fileno%32)))
                    ufds[i].revents |= POLLOUT;
            }
            if (pollset[2*subsize+fileno/32] & (1<<(fileno%32)))
                ufds[i].revents |= (POLLERR|POLLNVAL);
        }
        rt_free(pollset);
        return ret;
    }
    case SYS_alarm:
    {
        int ret = 0;
        if (module->alarm->parent.flag & RT_TIMER_FLAG_ACTIVATED)
            ret = (module->alarm->timeout_tick - rt_tick_get()) + 1;
        if (parm1 != 0)
        {
            //定时器不能超过10天的时间
            if (parm1 > 10*24*3600)
                parm1 = 10*24*3600;
            parm1 *= 1000;
            rt_timer_control(module->alarm, RT_TIMER_CTRL_SET_TIME, &parm1);
            rt_timer_start(module->alarm);
        }
        else if (ret != 0)
        {
            rt_timer_stop(module->alarm);
        }
        return ret/1000l;
    }
    case SYS_gettimeofday:
    {
        errno = 0;
        struct timeval *tim = (struct timeval *)rt_process_conv_ptr(module,parm1,sizeof(struct timeval));
        struct timezone *zone = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(struct timezone))):(0);
        return ret_err(gettimeofday(tim,zone));
    }
    case SYS_settimeofday:
    {
        errno = 0;
        struct timeval *tim = (struct timeval *)rt_process_conv_ptr(module,parm1,sizeof(struct timeval));
        struct timezone *zone = (parm2)?(rt_process_conv_ptr(module,parm2,sizeof(struct timezone))):(0);
        return ret_err(settimeofday(tim,zone));
    }
    case SYS_ioctl:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        switch(parm2)
        {
        case TIOCGWINSZ:
        {
            struct winsize *win = (struct winsize *)rt_process_conv_ptr(module,parm3,sizeof(struct winsize));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_GETWS,win));
        }
        case TIOCSWINSZ:
        {
            struct winsize *win = (struct winsize *)rt_process_conv_ptr(module,parm3,sizeof(struct winsize));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_SETWS,win));
        }
        case TCGETS:
        {
            struct termios *ios = (struct termios *)rt_process_conv_ptr(module,parm3,sizeof(struct termios));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_GETS,ios));
        }
        case TCSETS:
        case TCSETSW:
        case TCSETSF:
        {
            struct termios *ios = (struct termios *)rt_process_conv_ptr(module,parm3,sizeof(struct termios));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_SETS+(parm2-TCSETS),ios));
        }
        case TCFLSH:
        {
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_FLSH,(void *)parm3));
        }
        case TIOCGSID:
        {
            pid_t *tsid = (pid_t *)rt_process_conv_ptr(module,parm3,sizeof(pid_t));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_GSID,tsid));
        }
        case TIOCGPGRP:
        {
            pid_t *tsid = (pid_t *)rt_process_conv_ptr(module,parm3,sizeof(pid_t));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_GPGRP,tsid));
        }
        case TIOCSPGRP:
        {
            pid_t *tsid = (pid_t *)rt_process_conv_ptr(module,parm3,sizeof(pid_t));
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_SPGRP,tsid));
        }
        case TCSBRK:
        {
            return ret_err(ioctl(fileno,RT_DEVICE_CTRL_BRK,(void *)parm3));
        }
        }
        PRESS_DEBUG_IOCTL("syscall pid:%d/%d ioctl file:%d/%d cmd:0x%x data:0x%x\n",module->pid,module->tpid,parm1,fileno,parm2,parm3);
        return -ENOTSUP;
    }
    case SYS_fcntl:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        switch(parm2)
        {
        case F_DUPFD:
        {
        	return sys_call_switch(SYS_dup2,parm1,parm3,0,0,0,0);
        }
        case F_GETFD:
        {
        	struct dfs_fd *d = fd_get(fileno);
        	if (d == NULL)
        		return -EBADF;
        	fd_put(d);
        	return (d->flags&O_CLOEXEC)?(FD_CLOEXEC):0;
        }
        case F_GETFL:
        {
        	struct dfs_fd *d = fd_get(fileno);
        	if (d == NULL)
        		return -EBADF;
        	fd_put(d);
        	return d->flags;
        }
        case F_SETFD:
        {
        	struct dfs_fd *d = fd_get(fileno);
        	if (d == NULL)
        		return -EBADF;
        	if (parm3 == FD_CLOEXEC)
        		d->flags |= O_CLOEXEC;
        	fd_put(d);
        	return 0;
        }
        case F_SETFL:
        {
        	struct dfs_fd *d = fd_get(fileno);
        	if (d == NULL)
        		return -EBADF;
        	d->flags = parm3;
        	fd_put(d);
        	return 0;
        }
        }
        PRESS_DEBUG_IOCTL("syscall pid:%d/%d fcntl file:%d/%d cmd:0x%x data:0x%x\n",module->pid,module->tpid,parm1,fileno,parm2,parm3);
        return -ENOTSUP;
    }
    case SYS_BASE+901:
    case SYS_BASE+903:
    {
        rt_mutex_take(module->page_mutex, RT_WAITING_FOREVER);
        return 0;
    }
    case SYS_BASE+902:
    case SYS_BASE+904:
    {
        rt_mutex_release(module->page_mutex);
        return 0;
    }
    case SYS_BASE+1001:
    {
        errno = 0;
        int fileno = rt_process_convfile(module,parm1);
        if (fileno < 0)
            return -EBADF;
        char *name = (char *)rt_process_conv_ptr(module,parm2,parm3);
        struct dfs_fd *d = fd_get(fileno);
        //必须是devfs
        if (d == RT_NULL || d->fs == RT_NULL || rt_strcmp(d->fs->path,"/dev") != 0)
        {
        	if (d)
        		fd_put(d);
        	return -ENOTTY;
        }
        //必须是字符类设备
        rt_device_t device = (rt_device_t)d->data;
        if (device == RT_NULL || device->type != RT_Device_Class_Char)
        {
        	fd_put(d);
        	return -ENOTTY;
        }
        //复制设备名称/dev
        rt_snprintf(name,parm3,"/dev%s",d->path);
        fd_put(d);
        return 0;
    }
    case SYS_BASE+2001:
    {
        jmp_buf *sigjmp = rt_process_conv_ptr(module,parm1,sizeof(jmp_buf));
        longjmp(*sigjmp, 2001);
        return 0;
    }
    }
    PRESS_DEBUG_NOSYS("syscall %d not supported\n",nbr-SYS_BASE);
    return -ENOTSUP;
}

rt_uint32_t sys_call_signal(rt_int32_t ret)
{
	int i,deal = 0;
	register rt_base_t temp;
	rt_process_t module = rt_process_self();

    RT_ASSERT(module != RT_NULL);
    //rt_kprintf("signale ret:%d in\n",ret);

	//没有信号产生或者全部信号屏蔽
	if (module->siginfo == 0 || module->sigset == 0xffffffff)
		return ret;

	temp = rt_hw_interrupt_disable();
	for (i=1; i<NSIG; i++)
	{
		//没有信号需要处理
		if (!(module->siginfo & (1<<i)))
			continue;
		//忽略的信号不用处理
		if (module->sigact[i].sa_handler == SIG_IGN
					|| module->sigact[i].sa_handler == SIG_ERR)
			continue;
		//信号被阻止，不能进行处理
		if (module->sigset & (1<<i))
			continue;

		deal = 1;
		//默认消息一般是退出处理
		if (module->sigact[i].sa_handler == SIG_DFL)
		{
			//这消息默认是不处理
			if (i==SIGCHLD || i==SIGWINCH || i==SIGURG)
			{
				module->siginfo &= ~(1<<i);
				continue;
			}
			rt_hw_interrupt_enable(temp);
			rt_process_unload(module,i);
			rt_schedule();
		}
		//调用处理函数，回调相关代码
		sigset_t oldset = module->sigset;
		module->siginfo &= ~(1<<i);
		module->sigset |= (module->sigact[i].sa_mask|(1<<i));
		rt_hw_interrupt_enable(temp);
		{jmp_buf sigjmp;
		if (setjmp(sigjmp) == 0)
		{
			extern void rt_hw_context_signal(rt_uint32_t sig, rt_uint32_t to, rt_uint32_t jmp);
			rt_hw_context_signal(i,(rt_uint32_t)module->sigact[i].sa_handler,(rt_uint32_t)&sigjmp);
		}}
		temp = rt_hw_interrupt_disable();
		module->sigset = oldset;
		//清除标志，会丢失信号但是避免了递归的问题
		module->siginfo &= ~(1<<i);
	}
	rt_hw_interrupt_enable(temp);

	return (deal && ret<0)?(-EINTR):(ret);
}

void rt_hw_printinfo(rt_int32_t r0, rt_int32_t r1, rt_int32_t r2)
{
	rt_kprintf("\nhw_print R0:%08x,R1:%08x,R2:%08x\n",r0,r1,r2);
}
#endif
