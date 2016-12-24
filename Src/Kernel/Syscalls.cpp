#include <sys/stat.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

extern "C" void _exit(int) {
  
}

extern "C" int close(int) {
  return -1;
}

extern "C" int execve(const char*,
           char* const*,
           char* const*)
{
  return -1;
}

extern "C" int fork() {
  return -1;
}

extern "C" int fstat(int, struct stat* st) {
  return 0;
}

extern "C" int getpid() {
  return 1;
}

extern "C" int isatty(int file) {
  return 0;
}

extern "C" int link(const char*, const char*) {
  return -1;
}

extern "C" int unlink(const char*) {
  return -1;
}

extern "C" off_t lseek(int, off_t, int) {
  return 0;
}

extern "C" int open(const char*, int, ...) {
  return -1;
}

extern "C" int read(int, void*, size_t) {
  return 0;
}

extern "C" int write(int file, const void* ptr, size_t len) {
return 0;
}

extern "C" void* sbrk(ptrdiff_t incr) {
	return (void*)-1;
}


extern "C" int stat(const char*, struct stat *st) {
  return 0;
}

clock_t times(struct tms*) {
  return -1;
}

int wait(int*) {
  return -1;
}

extern "C" int gettimeofday(struct timeval* p, void*) {
  return 0;
}

extern "C" int kill(pid_t pid, int sig) {
  return -1;
}

extern "C" size_t get_crash_context_length()
{
  return 0;
}
extern "C" char*  get_crash_context_buffer()
{
  return (char*)-1;
}


// No continuation from here
extern "C" void panic(const char* why) {
}

// Shutdown the machine when one of the exit functions are called
extern "C" void default_exit() {
}

// To keep our sanity, we need a reason for the abort
void abort_ex(const char* why) {
}

// Basic second-resolution implementation - using CMOS directly for now.
extern "C" int clock_gettime(clockid_t clk_id, struct timespec* tp) {
  return -1;
}

extern "C" void _init_syscalls()
{
}
