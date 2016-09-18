/* 
  Copyright 2016 Dominik Madarasz

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#if !defined(DZM_REP_H)

b32 UseSchemeSyntax = 1;

static inline void
test_load_file(const char *Name)
{
    b32 OldOk = PrintOk;
    PrintOk = 0;

    FILE *File = fopen(Name, "r");
    
    if(File == 0)
    {
        if(!strcmp(Name,"std/stdlib.scm"))
        {
            LOG(ERR_WARN, "%s\n", "Standard Library couldn't be loaded! Running in minimal mode!");
            return;
        }
        else
        {
            fprintf(stderr, "File not found\n");
            exit(0);
        }
    }
    OBJECT *Exp;
    while(1)
    {
		if(UseSchemeSyntax)
			Exp = read(File);
		else
			Exp = read_fn(File);

        if(Exp == 0)
        {
            break;
        }
        write(stdout, eval(Exp, GlobalEnv));
    }
    fclose(File);
    PrintOk = OldOk;
}

static inline void
test_repl(void)
{
    PrintOk = 1;
    for(;;)
    {
        //printf(": ");
        FILE *Stream = read_input(stdin);
        write(stdout, eval(read_fn(Stream), GlobalEnv));
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

	if(read_fn == 0)
	{
		read_fn = read;
		read_pair_fn = read_pair;
	}
    
    init_defs();
    test_load_file("std/stdlib.scm");
    
	UseSchemeSyntax = 0;

    if(argc < 2)
    {
        printf("DZMLang REPL; By ZaKlaus.\nUse ^C to exit.\n");
        printf("Version: %s\n", DZM_VERSION);
        test_repl();
        return;
    }
    //printf("DZMLang Interpreter; By ZaKlaus.\n");
    
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
