// (c) ZaKlaus 2016; All Rights Reserved;;

#if !defined(DZM_PRC_H)

#define def_proc(Name) static inline OBJECT * Name##_proc(OBJECT *Args) 
#define add_procedure(Name, Call)        \
define_variable(make_symbol((u8 *)Name), \
make_procedure(Call),                    \
Env);

// NOTE(zaklaus): Arithmetic operators
static inline OBJECT *
add_proc(OBJECT *Args)
{
    real32 Result = 0;
    b32 Real = 0;
    
    while(!is_nil(Args))
    {
        if(is_fixnum(pair_get_a(Args)))
            Result += (pair_get_a(Args))->uData.FIXNUM.Value;
        else 
        {
            Real = 1;
            Result += (pair_get_a(Args))->uData.REALNUM.Value;
        }
        Args = pair_get_b(Args);
    }
    
    if(!Real)
        return(make_fixnum((s32)Result));
    else
        return(make_realnum(Result));
}

static inline OBJECT *
sub_proc(OBJECT *Args)
{
    real32 Result = 0;
    b32 Real = 0;
    if(!is_nil(Args))
    {
        if(is_fixnum(pair_get_a(Args)))
            Result = (pair_get_a(Args))->uData.FIXNUM.Value;
        else 
        {
            Real = 1;
            Result = (pair_get_a(Args))->uData.REALNUM.Value;
        }
        Args = pair_get_b(Args);
    }
    
    b32 IsAlone = 1;
    while(!is_nil(Args))
    {
        Result -= (pair_get_a(Args))->uData.FIXNUM.Value;
        Args = pair_get_b(Args);
        IsAlone = 0;
    }
    
    if(IsAlone)
    {
        Result = -Result;
    }
    
    if(!Real)
        return(make_fixnum((s32)Result));
    else
        return(make_realnum(Result));
}

static inline OBJECT *
div_proc(OBJECT *Args)
{
    real32 Result = 0;
    b32 Real = 0;
    if(!is_nil(Args))
    {
        if(is_fixnum(pair_get_a(Args)))
            Result = (pair_get_a(Args))->uData.FIXNUM.Value;
        else 
        {
            Real = 1;
            Result = (pair_get_a(Args))->uData.REALNUM.Value;
        }
        Args = pair_get_b(Args);
   }
    
    while(!is_nil(Args))
    {
        if((pair_get_a(Args))->uData.FIXNUM.Value == 0)
        {
            fprintf(stderr, "Division by zero\n");
            InvalidCodePath;
        }
        
        if(is_fixnum(pair_get_a(Args)))
            Result /= (pair_get_a(Args))->uData.FIXNUM.Value;
        else 
        {
            Real = 1;
            Result /= (pair_get_a(Args))->uData.REALNUM.Value;
        }
        Args = pair_get_b(Args);
    }
    if(!Real)
        return(make_fixnum((s32)Result));
    else
        return(make_realnum(Result));
}

static inline OBJECT *
mod_proc(OBJECT *Args)
{
    s32 Result = 0;
    
    if(!is_nil(Args))
    {
        Result = (pair_get_a(Args))->uData.FIXNUM.Value;
        Args = pair_get_b(Args);
    }
    
    while(!is_nil(Args))
    {
        if((pair_get_a(Args))->uData.FIXNUM.Value == 0)
        {
            fprintf(stderr, "Division by zero\n");
            InvalidCodePath;
        }
        
        Result %= (pair_get_a(Args))->uData.FIXNUM.Value;
        Args = pair_get_b(Args);
    }
    return(make_fixnum(Result));
    
}

static inline OBJECT *
mul_proc(OBJECT *Args)
{
    real32 Result = 1;
    b32 Real = 0;
    
    while(!is_nil(Args))
    {
        if(is_fixnum(pair_get_a(Args)))
            Result *= (pair_get_a(Args))->uData.FIXNUM.Value;
        else 
        {
            Real = 1;
            Result *= (pair_get_a(Args))->uData.REALNUM.Value;
        }
        Args = pair_get_b(Args);
    }
    if(!Real)
        return(make_fixnum((s32)Result));
    else
        return(make_realnum(Result));
}

// NOTE(zaklaus): Existence checks

def_proc(is_nil)
{
    return(is_nil(pair_get_a(Args)) ? True : False);
}

def_proc(is_boolean)
{
    return(is_boolean(pair_get_a(Args)) ? True : False);
}

def_proc(is_symbol)
{
    return(is_symbol(pair_get_a(Args)) ? True : False);
}

def_proc(is_integer)
{
    return(is_fixnum(pair_get_a(Args)) ? True : False);
}

def_proc(is_real)
{
    return(is_realnum(pair_get_a(Args)) ? True : False);
}

def_proc(is_char)
{
    return(is_character(pair_get_a(Args)) ? True : False);
}

def_proc(is_string)
{
    return(is_string(pair_get_a(Args)) ? True : False);
}

def_proc(is_pair)
{
    return(is_pair(pair_get_a(Args)) ? True : False);
}

def_proc(is_compound)
{
    return(is_compound(pair_get_a(Args)) ? True : False);
}

def_proc(is_procedure)
{
    return((is_procedure(pair_get_a(Args))) ? True : False);
}

// NOTE(zaklaus): Type conversions

def_proc(char_to_integer)
{
    return(make_fixnum((pair_get_a(Args))->uData.CHARACTER.Value));
}

def_proc(integer_to_char)
{
    return(make_character((pair_get_a(Args))->uData.FIXNUM.Value));
}

def_proc(number_to_string)
{
    char Buffer[MAX_STRING_SIZE];
    sprintf(Buffer, "%1d", (pair_get_a(Args))->uData.FIXNUM.Value);
    return(make_string((u8 *)Buffer));
}

def_proc(string_to_number)
{
    return(make_fixnum(atoi((char *)((pair_get_a(Args))->uData.STRING.Value))));
}

def_proc(symbol_to_string)
{
    return(make_string((pair_get_a(Args))->uData.SYMBOL.Value));
}

def_proc(string_to_symbol)
{
    return(make_symbol((pair_get_a(Args))->uData.STRING.Value));
}

def_proc(is_number_equal)
{
    s32 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
    
    while(!is_nil(Args = pair_get_b(Args)))
    {
        if(Value != ((pair_get_a(Args))->uData.FIXNUM.Value))
        {
            return False;
        }
    }
    return True;
}

def_proc(is_less_than)
{
    s32 Previous = (pair_get_a(Args))->uData.FIXNUM.Value;
    s32 Next;
    
    while(!is_nil(Args = pair_get_b(Args)))
    {
        Next = (pair_get_a(Args))->uData.FIXNUM.Value;
        if(Previous < Next)
        {
            Previous = Next;
        }
        else
        {
            return False;
        }
    }
    return True;
}

def_proc(is_greater_than)
{
    s32 Previous = (pair_get_a(Args))->uData.FIXNUM.Value;
    s32 Next;
    
    while(!is_nil(Args = pair_get_b(Args)))
    {
        Next = (pair_get_a(Args))->uData.FIXNUM.Value;
        if(Previous > Next)
        {
            Previous = Next;
        }
        else
        {
            return False;
        }
    }
    return True;
}

def_proc(concat)
{
    char *Result = 0;
    OBJECT *Text = pair_get_a(Args);
concat_tailcall:
   
    if(is_string(Text))
    {
        Result = malloc(strlen((char *)Text->uData.STRING.Value)+1);
        strcpy(Result, (char *)Text->uData.STRING.Value);
    }
    else if(is_pair(Text))
    {
        Text = concat_proc(Text);
        goto concat_tailcall;
    }
    else
    {
        Result = malloc(2);
        Result[0] = (char)Text->uData.CHARACTER.Value;
        Result[1] = 0;
    }
    while(!is_nil(Args = pair_get_b(Args)))
    {
        Text = pair_get_a(Args);
        
        concat_tailcall2:
        if(is_string(Text))
        {
            Result = realloc(Result, strlen(Result) + strlen((char *)Text->uData.STRING.Value)+1);
            strcat(Result, (char *)Text->uData.STRING.Value);
        }
        else if(is_nil(Text))
        {
            break;
        }
        else if(is_pair(Text))
    {
        Text = concat_proc(Text);
        goto concat_tailcall2;
    }
        else
        {
            mi ResultEnd = strlen(Result);
            Result = realloc(Result, strlen(Result) + 2);
            Result[ResultEnd] = (char)Text->uData.CHARACTER.Value;
            Result[ResultEnd+1] = 0;
        }
    }
    
    return(make_string((u8 *)Result));
}

def_proc(cons)
{
    pair_set_b(Args, pair_get_a(pair_get_b(Args)));
    return(Args);
}

def_proc(car)
{
    if(is_string(pair_get_a(Args)))
        return(make_character(pair_get_a(Args)->uData.STRING.Value[0]));
    else
        return(pair_get_a(pair_get_a(Args)));
}

def_proc(cdr)
{
    if(is_string(pair_get_a(Args)) &&
       (strlen((char *)pair_get_a(Args)->uData.STRING.Value) > 1))
        return(make_string(pair_get_a(Args)->uData.STRING.Value+1));
    else if(is_pair(pair_get_a(Args)))
        return(pair_get_b(pair_get_a(Args)));
    else
        return(Nil);
}

def_proc(set_car)
{
    pair_set_a(pair_get_a(Args), pair_get_a(pair_get_b(Args)));
    return OKSymbol;
}

def_proc(set_cdr)
{
    pair_set_b(pair_get_a(Args), pair_get_a(pair_get_b(Args)));
    return OKSymbol;
}

def_proc(list)
{
    return(Args);
}

def_proc(is_eq)
{
    OBJECT *Obj1 = pair_get_a(Args);
    OBJECT *Obj2 = pair_get_a(pair_get_b(Args));
    
    if(Obj1->Type != Obj2->Type)
    {
        return(False);
    }
    
    switch(Obj1->Type)
    {
        case REALNUM:
        case FIXNUM:
        {
            return((Obj1->uData.FIXNUM.Value == 
                    Obj2->uData.FIXNUM.Value) ?
                   True : False);
        }break;
        
        case CHARACTER:
        {
            return((Obj1->uData.CHARACTER.Value == 
                    Obj2->uData.CHARACTER.Value) ?
                   True : False);
        }break;
        
        case STRING:
        {
            return((!strcmp((char *)Obj1->uData.STRING.Value,
                            (char *)Obj2->uData.STRING.Value)) ?
                   True : False);
        }break;
        
        default:
        {
            return((Obj1 == Obj2) ? True : False);
        }break;
    }
}

def_proc(apply)
{
    fprintf(stderr, "illegal state: The body of the apply should not execute\n");
    InvalidCodePath;
    
    Unreachable(Args);
}

def_proc(eval)
{
    fprintf(stderr, "illegal state: The body of the apply should not execute\n");
    InvalidCodePath;
    
    Unreachable(Args);
}

def_proc(interaction_env)
{
    return(GlobalEnv);
    Unreachable(Args);
}

def_proc(nil_env)
{
    return(setup_env());
    Unreachable(Args);
}

def_proc(env)
{
    return(make_env());
    Unreachable(Args);
}

def_proc(load)
{
    u8 *Filename;
    FILE *In;
    OBJECT *Exp;
    OBJECT *Result = 0;
    OBJECT *Env = GlobalEnv;
    
    Filename = (pair_get_a(Args))->uData.STRING.Value;
    In = fopen((char *)Filename, "r");
    if(In == 0)
    {
        fprintf(stderr, "Could not load file \"%s\"\n", Filename);
        InvalidCodePath;
    }
    
    if(!is_nil(pair_get_b(Args)))
    {
        Env = pair_get_b(pair_get_a(Args));
    }
    
    while((Exp = read(In)) != 0)
    {
        Result = eval(Exp, Env);
    }
    fclose(In);
    return(Result);
}

def_proc(read)
{
    FILE *In;
    OBJECT *Result;
    
    In = is_nil(Args) ? stdin : (pair_get_a(Args))->uData.INPUT.Stream;
    Result = read(In);
    return((Result == 0) ? EOF_Obj : Result);
}

def_proc(write)
{
    OBJECT *Exp;
    FILE *Out;
    
    Exp = pair_get_a(Args);
    Args = pair_get_b(Args);
    Out = is_nil(Args) ? stdout : (pair_get_a(Args))->uData.OUTPUT.Stream;
    write(Out, Exp);
    fflush(Out);
    return(OKSymbol);
}

def_proc(peek_char)
{
    FILE *In;
    s32 Result;
    
    In = is_nil(Args) ? stdin : (pair_get_a(Args))->uData.INPUT.Stream;
    Result = peek(In);
    return((Result == EOF) ? EOF_Obj : make_character(Result));
}

def_proc(open_input)
{
    u8 *Filename;
    FILE *In;
    
    Filename = (pair_get_a(Args))->uData.STRING.Value;
    In = fopen((char *)Filename, "r");
    if(In == 0)
    {
        fprintf(stderr, "Could not load file \"%s\"\n", Filename);
        InvalidCodePath;
    }
    return(make_input(In));
}

def_proc(close_input)
{
    s32 Result;
    
    Result = fclose((pair_get_a(Args))->uData.INPUT.Stream);
    if(Result == EOF)
    {
        fprintf(stderr, "Could not close input\n");
        InvalidCodePath;
    }
    return(OKSymbol);
}

def_proc(is_input)
{
    return(is_input(pair_get_a(Args)) ? True : False);
}

def_proc(open_output)
{
    u8 *Filename;
    FILE *In;
    
    Filename = (pair_get_a(Args))->uData.STRING.Value;
    In = fopen((char *)Filename, "w");
    if(In == 0)
    {
        fprintf(stderr, "Could not load file \"%s\"\n", Filename);
        InvalidCodePath;
    }
    return(make_output(In));
}

def_proc(close_output)
{
    s32 Result;
    
    Result = fclose((pair_get_a(Args))->uData.INPUT.Stream);
    if(Result == EOF)
    {
        fprintf(stderr, "Could not close output\n");
        InvalidCodePath;
    }
    return(OKSymbol);
}

def_proc(is_output)
{
    return(is_output(pair_get_a(Args)) ? True : False);
}

def_proc(is_eof_obj)
{
    return(is_eof_obj(pair_get_a(Args)) ? True : False);
}

def_proc(write_char)
{
    OBJECT *Char;
    FILE *Out;
    
    Char = pair_get_a(Args);
    Args = pair_get_b(Args);
    Out = is_nil(Args) ? stdout : (pair_get_a(Args))->uData.OUTPUT.Stream;
    putc(Char->uData.CHARACTER.Value, Out);
    fflush(Out);
    return(OKSymbol);
}

def_proc(write_string)
{
    OBJECT *String;
    FILE *Out;
    
    String = pair_get_a(Args);
    Args = pair_get_b(Args);
    Out = is_nil(Args) ? stdout : (pair_get_a(Args))->uData.OUTPUT.Stream;
    fprintf(Out, (char *)String->uData.STRING.Value);
    fflush(Out);
    return(OKSymbol);
}

def_proc(read_char)
{
    s32 Result;
    FILE *In;
    
    if(is_string(pair_get_a(Args)))
    {
        Result = *(pair_get_a(Args)->uData.STRING.Value);
        return(make_character(Result));
    }
    
    In = is_nil(Args) ? stdin : (pair_get_a(Args))->uData.INPUT.Stream;
    Result = getc(In);
    return((Result == EOF) ? EOF_Obj : make_character(Result));
}

def_proc(system)
{
    if(!is_string(pair_get_a(Args)))
    {
        return(Nil);
    }
    
    FILE *Out = stdout;
    if(!is_nil(pair_get_a(pair_get_b(Args))))
    {
        Out = (pair_get_a(pair_get_b(Args)))->uData.OUTPUT.Stream;
    }
    
    FILE *Sys = popen((char *)(pair_get_a(Args)->uData.STRING.Value), "r");
    if(!Sys)
    {
        goto system_end;
    }
    
    s32 C;
    while((C = getc(Sys)) != EOF)
    {
        putc((char)C, Out);
    }
    pclose(Sys);
system_end:
    
    if(is_nil(pair_get_a(pair_get_b(Args))))
        return(OKSymbol);
    else
        return(pair_get_a(pair_get_b(Args)));
    //return(make_string((u8 *)system((char *)pair_get_a(Args)->uData.STRING.Value)));
}

def_proc(arena_mem)
{
    return(make_pair(make_fixnum(
        get_arena_size_remaining(GlobalArena, default_arena_params())),
        make_fixnum(GlobalArena->Size)));
    Unreachable(Args);
}

def_proc(log_mem)
{
    PrintMemUsage = !PrintMemUsage;
    return(Args);
}

static inline void
init_builtins(OBJECT *Env)
{
    define_variable(make_symbol(UL_"+"),
                    make_procedure(add_proc),
                    Env);
    
    define_variable(make_symbol(UL_"-"),
                    make_procedure(sub_proc),
                    Env);
    
    define_variable(make_symbol(UL_"*"),
                    make_procedure(mul_proc),
                    Env);
    
    define_variable(make_symbol(UL_"/"),
                    make_procedure(div_proc),
                    Env);
    
    define_variable(make_symbol(UL_"%"),
                    make_procedure(mod_proc),
                    Env);
    
    add_procedure("nil?"     , is_nil_proc);
    add_procedure("null?"     , is_nil_proc);
    add_procedure("boolean?"  , is_boolean_proc);
    add_procedure("symbol?"   , is_symbol_proc);
    add_procedure("integer?"  , is_integer_proc);
    add_procedure("char?"     , is_char_proc);
    add_procedure("string?"   , is_string_proc);
    add_procedure("pair?"     , is_pair_proc);
    add_procedure("procedure?", is_procedure_proc);
    
    add_procedure("char->integer" , char_to_integer_proc);
    add_procedure("integer->char" , integer_to_char_proc);
    add_procedure("number->string", number_to_string_proc);
    add_procedure("string->number", string_to_number_proc);
    add_procedure("symbol->string", symbol_to_string_proc);
    add_procedure("string->symbol", string_to_symbol_proc);
      
    add_procedure("="        , is_number_equal_proc);
    add_procedure("<"        , is_less_than_proc);
    add_procedure(">"        , is_greater_than_proc);

    add_procedure("cons"    , cons_proc);
    add_procedure("car"     , car_proc);
    add_procedure("cdr"     , cdr_proc);
    add_procedure("set-car!", set_car_proc);
    add_procedure("set-cdr!", set_cdr_proc);
    add_procedure("list"    , list_proc);
    
    add_procedure("apply"   , apply_proc);
    add_procedure("eval"    , eval_proc);

    add_procedure("eq?", is_eq_proc);
    
    add_procedure("env"     , env_proc);
    add_procedure("i-env"   , interaction_env_proc);
    add_procedure("nil-env" , nil_env_proc);
    
    add_procedure("load"             , load_proc);
    add_procedure("open-input"  , open_input_proc);
    add_procedure("close-input" , close_input_proc);
    add_procedure("input?"      , is_input_proc);
    add_procedure("read"             , read_proc);
    add_procedure("read-char"        , read_char_proc);
    add_procedure("peek-char"        , peek_char_proc);
    add_procedure("eof?"      , is_eof_obj_proc);
    add_procedure("open-output" , open_output_proc);
    add_procedure("close-output", close_output_proc);
    add_procedure("output?"     , is_output_proc);
    add_procedure("write-char"       , write_char_proc);
    add_procedure("write-string"     , write_string_proc);
    add_procedure("write"            , write_proc);
    
    add_procedure("concat"      , concat_proc);
    add_procedure("system"      , system_proc);
    
    add_procedure("arena-mem"   , arena_mem_proc);
    add_procedure("log-mem"     , log_mem_proc);
}

#define DZM_PRC_H
#endif