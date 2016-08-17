// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#if !defined(DZM_REP_H)

static inline void
test_load_file(const char *Name)
{
    FILE *File = fopen(Name, "r");
    
    if(File == 0)
    {
        fprintf(stderr, "File not found\n");
        exit(1);
    }
    OBJECT *Exp;
    while(1)
    {
        Exp = read(File);
        if(Exp == 0)
        {
            break;
        }
        write(stdout, eval(Exp, GlobalEnv));
    }
    fclose(File);
}

static inline void
test_repl(void)
{
    for(;;)
    {
        printf(": ");
        FILE *Stream = read_input(stdin);
        write(stdout, eval(read(Stream), GlobalEnv));
        printf("\n");
    }
}

static inline void
test_init(int argc, char** argv)
{
    GlobalArena = (MEMORY_ARENA *)malloc(sizeof(MEMORY_ARENA));
    
    if(!GlobalArena)
    {
        printf("Memory initialization error!\n");
        exit(-1);
    }
    
    initialize_arena(GlobalArena, MAX_VM_SIZE, malloc(MAX_VM_SIZE));
    
    init_mem();
    
    init_logging();
    FILE *Log = fopen("dzm_log.txt", "w");
    set_log_output(Log);
    set_log_verbose(1);
    
    init_defs();
    test_load_file("stdlib.scm");
    
    if(argc < 2)
    {
        printf("DZMLang REPL; By ZaKlaus.\nUse ^C to exit.\n");
        printf("Version: %s\n", DZM_VERSION);
        test_repl();
        return;
    }
    printf("DZMLang Interpreter; By ZaKlaus.\n");
    
    if(!strcmp(argv[1], "--repl"))
    {
        test_load_file(argv[2]);
        test_repl();
    }
    else test_load_file(argv[1]);
    
    fclose(Log);
}

#define DZM_REP_H
#endif