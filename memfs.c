extern int sprintf(char*, const char*, ...);

extern void console_log(char*);
extern void quit();
extern void read_in(void* dest, void* extern_src, unsigned int size);
extern void write_out(void* extern_dest, void* src, unsigned int size);

#define EXPORT __attribute__((used))

#define SIZE(ty, siz) \
  typedef ty i##siz; \
  typedef unsigned ty u##siz;
SIZE(char, 8);
SIZE(short, 16);
SIZE(int, 32);
SIZE(long long, 64);
#undef SIZE

typedef u32 size_t;

typedef u64 rights_t;
typedef u8 filetype_t;
typedef u16 fdflags_t;
typedef u32 fd_t;

typedef struct fdstat_t {} fdstat;
typedef struct ciovec_t {
  void* buf;
  size_t len;
} ciovec;

const fd_t STDIN_FILENO = 0;
const fd_t STDOUT_FILENO = 1;
const fd_t STDERR_FILENO = 2;

typedef i32 wasi_errno;

EXPORT wasi_errno fd_fdstat_get(i32 fd, i32 fdstat_ptr) {
  console_log("Calling stub: fd_fdstat_get");
  // const fdstat = load_wasi_fdstat_t(fdstat_ptr);
  // console_log("fdstat: ", fdstat);
  return 0;
}
EXPORT wasi_errno fd_close(i32 fd) {
  console_log("Calling stub: fd_close");
  return 0;
}
EXPORT wasi_errno fd_seek(i32 a, i64 b, i32 c, i32 d) {
  console_log("Calling stub: fd_seek");
  return 0;
}
EXPORT wasi_errno fd_write(fd_t fd, ciovec* io, size_t iovs_len, size_t* nwritten) {
  console_log("Calling stub: fd_write");
  char str[1024];
  size_t written = 0;
  char buffer[1024];
  char* bufPtr = buffer;
  for (int i = 0; i < iovs_len; ++i) {
    ciovec my_io;
    read_in(&my_io, &io[i], sizeof(ciovec));
    sprintf(str, "fd: %d, io: {%p , %d}, len: %d", fd, my_io.buf, my_io.len, iovs_len);
    console_log(str);
    read_in(bufPtr, my_io.buf, my_io.len);
    bufPtr += my_io.len;
    written += my_io.len;
  }
  *bufPtr = 0;
  console_log(buffer);
  write_out(nwritten, &written, sizeof(written));
  return 0;
}
