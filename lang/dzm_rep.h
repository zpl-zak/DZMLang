// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_REP_H)

static inline void
test_load_file(char *Name)
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
    printf("DZMLang REPL; By ZaKlaus.\nUse ^C to exit.\n");
    
    //init_defs();
    
    for(;;)
    {
        printf(": ");
        write(stdout, eval(read(stdin), GlobalEnv));
        printf("\n");
    }
}

static inline void
test_init(int argc, char** argv)
{
    GlobalArena = malloc(sizeof(MEMORY_ARENA));
    initialize_arena(GlobalArena, MAX_VM_SIZE, malloc(MAX_VM_SIZE));
    
    init_logging();
    FILE *Log = fopen("dzm_log.txt", "w");
    set_log_output(Log);
    set_log_verbose(1);
    
    if(argc < 2)
    {
        init_defs();
        
        test_load_file("stdlib.dzm");
        
        test_repl();
        return;
    }
    printf("DZMLang Interpreter; By ZaKlaus.\n");
    
    test_load_file(argv[1]);
    
    fclose(Log);
}

#define DZM_REP_H
#endif