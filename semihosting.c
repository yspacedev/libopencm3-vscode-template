
//from https://bogdanthegeek.github.io/blog/insights/jlink-rtt-for-the-masses/
int SEMIHOST_SysCall(int reason, void *arg)
{
    int value;
    __asm volatile(
        "mov r0, %[rsn] \n" // place semihost operation code into R0
        "mov r1, %[arg] \n" // R1 points to the argument array
        "bkpt 0xAB      \n" // call debugger
        "mov %[val], r0 \n" // debugger has stored result code in R0

        : [val] "=r"(value)                 // outputs
        : [rsn] "r"(reason), [arg] "r"(arg) // inputs
        : "r0", "r1", "r2", "memory");
    return value; // return result code, stored in R0
}

int getchar(void)
{
    return SEMIHOST_SysCall(SYS_READC, NULL);
}

int _write(int fd, const char *buf, int size)
{
    if (fd == SEMIHOST_STDOUT || fd == SEMIHOST_STDERR)
    {
        int32_t args[3] = {fd, (int32_t)buf, size};
        return SEMIHOST_SysCall(SYS_WRITE, &args[0]);
    }
    return 0; // Unsupported file descriptor
}