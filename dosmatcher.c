asm (".code16gcc\n"
        "call  dosmain\n"
        "mov   $0x4C,%ah\n"
        "int   $0x21\n");

static void print(char *string)
{
    asm volatile ("mov   $0x09, %%ah\n"
            "int   $0x21\n"
    : /* no output */
    : "d"(string)
    : "ah");
}

int dosmain(void)
{
    print("Hello, World!\n$");
    return 0;
}