// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#if !defined(DZM_PRC_H)

#include <ctime>
#include <cmath>

#define def_proc(Name) static inline OBJECT * Name##_proc(OBJECT *Args) 
#define add_procedure(Name, Call)        \
define_variable(make_symbol((u8 *)Name), \
make_procedure(Call),                    \
Env);

#define check_args(o)                                  \
if(is_nil(pair_get_a(Args)))                           \
LOG(LOG_WARN, o " " "is missing required arguments")

def_proc(inc)
{
     OBJECT *Arg0 = pair_get_a(Args);
     OBJECT *Obj = Arg0;

     if(is_realnum(Arg0))
     {
          Obj = make_realnum(Arg0->uData.REALNUM.Value + 1.0);
     }
     else if(is_fixnum(Arg0))
     {
          Obj = make_fixnum(Arg0->uData.FIXNUM.Value + 1);
     }
     else if(is_character(Arg0))
     {
          Obj = make_character(Arg0->uData.CHARACTER.Value + 1);
     }
     else if(is_string(Arg0))
     {
          Obj = make_string(Arg0->uData.STRING.Value + 1);
     }
     return(Obj);
}

def_proc(dec)
{
     OBJECT *Arg0 = pair_get_a(Args);
     OBJECT *Obj = Arg0;

     if(is_realnum(Arg0))
     {
          Obj = make_realnum(Arg0->uData.REALNUM.Value - 1.0);
     }
     else if(is_fixnum(Arg0))
     {
          Obj = make_fixnum(Arg0->uData.FIXNUM.Value - 1);
     }
     else if(is_character(Arg0))
     {
          Obj = make_character(Arg0->uData.CHARACTER.Value - 1);
     }
     else if(is_string(Arg0))
     {
          Obj = make_string(Arg0->uData.STRING.Value - 1);
     }
     return(Obj);
}

// NOTE(zaklaus): Arithmetic operators
static inline OBJECT *
add_proc(OBJECT *Args)
{
    real64 Result = 0;
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
        return(make_fixnum((s64)Result));
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
        return(make_fixnum((s64)Result));
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
            LOG(ERR_WARN, "Division by zero");
            return(Nil);
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
        return(make_fixnum((s64)Result));
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
            LOG(ERR_WARN, "Division by zero");
            return(Nil);
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
        return(make_fixnum((s64)Result));
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

def_proc(string_to_char)
{
    return(make_character(*((pair_get_a(Args))->uData.STRING.Value)));
}

def_proc(number_to_string)
{
    TEMP_MEMORY Mem = begin_temp(&StringArena);
    char *Buffer = (char *)push_size(&StringArena, 66, default_arena_params());
    if(is_fixnum(pair_get_a(Args))) sprintf(Buffer, "%" PRId64, (pair_get_a(Args))->uData.FIXNUM.Value);
    else sprintf(Buffer, "%lf", (double)((pair_get_a(Args))->uData.REALNUM.Value));
    end_temp(Mem);
    return(make_string((u8 *)Buffer));
}

def_proc(string_to_number)
{
    char * String = (char *)((pair_get_a(Args))->uData.STRING.Value);
    char * EndPtr = 0;
    s64 Number = strtoull(String, &EndPtr, 10);
    return(make_fixnum(Number));
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
     OBJECT *R = False;
    if(pair_get_a(Args)->Type == FIXNUM)
    {
        s64 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
        while(!is_nil(Args = pair_get_b(Args)))
        {
             if(is_fixnum(pair_get_a(Args)))
             {
                  R = (Value == pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
             }
             if(is_realnum(pair_get_a(Args)))
             {
                  R = ((r64)Value == pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
             }
        }
    }
    else if(pair_get_a(Args)->Type == REALNUM)
    {
        r64 Value = (pair_get_a(Args))->uData.REALNUM.Value;
        while(!is_nil(Args = pair_get_b(Args)))
        {
             if(is_fixnum(pair_get_a(Args)))
             {
                  R = (Value == (r64)pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
             }
             if(is_realnum(pair_get_a(Args)))
             {
                  R = (Value == pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
             }
             if(R == False) return(R);
        }
    }
    return(R);
}

def_proc(is_greater_than_or_equal)
{
     OBJECT* R = 0;
     if(pair_get_a(Args)->Type == FIXNUM)
     {
          s64 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value >= pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = ((r64)Value >= pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     else if(pair_get_a(Args)->Type == REALNUM)
     {
          r64 Value = (pair_get_a(Args))->uData.REALNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value >= (r64)pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = (Value >= pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     return(R);
}

def_proc(is_greater_than)
{
     OBJECT* R = 0;
     if(pair_get_a(Args)->Type == FIXNUM)
     {
          s64 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value > pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = ((r64)Value > pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     else if(pair_get_a(Args)->Type == REALNUM)
     {
          r64 Value = (pair_get_a(Args))->uData.REALNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value > (r64)pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = (Value > pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     return(R);
}

def_proc(is_less_than)
{
     OBJECT* R = 0;
     if(pair_get_a(Args)->Type == FIXNUM)
     {
          s64 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value < pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = ((r64)Value < pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     else if(pair_get_a(Args)->Type == REALNUM)
     {
          r64 Value = (pair_get_a(Args))->uData.REALNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value < (r64)pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = (Value < pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     return(R);
}

def_proc(is_less_than_or_equal)
{
     OBJECT* R = 0;
     if(pair_get_a(Args)->Type == FIXNUM)
     {
          s64 Value = (pair_get_a(Args))->uData.FIXNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value <= pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = ((r64)Value <= pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     else if(pair_get_a(Args)->Type == REALNUM)
     {
          r64 Value = (pair_get_a(Args))->uData.REALNUM.Value;
          while(!is_nil(Args = pair_get_b(Args)))
          {
               if(is_fixnum(pair_get_a(Args)))
               {
                    R = (Value <= (r64)pair_get_a(Args)->uData.FIXNUM.Value) ? True : False;
               }
               if(is_realnum(pair_get_a(Args)))
               {
                    R = (Value <= pair_get_a(Args)->uData.REALNUM.Value) ? True : False;
               }
               if(R == False) return(R);
          }
     }
     return(R);
}


def_proc(concat)
{
    TEMP_MEMORY StringTemp = begin_temp(&StringArena);
    char *Result = 0;
    OBJECT *Text = pair_get_a(Args);
concat_tailcall:
   
    if(is_string(Text))
    {
        Result = (char *)push_size(&StringArena, strlen((char *)Text->uData.STRING.Value)+1, default_arena_params());
        strcpy(Result, (char *)Text->uData.STRING.Value);
    }
    else if(is_pair(Text))
    {
        Text = concat_proc(Text);
        goto concat_tailcall;
    }
    else
    {
        Result = (char *)push_size(&StringArena,2,default_arena_params());
        Result[0] = (char)Text->uData.CHARACTER.Value;
        Result[1] = 0;
    }
    while(!is_nil(Args = pair_get_b(Args)))
    {
        Text = pair_get_a(Args);
        
concat_tailcall2:
        if(is_string(Text))
        {
            Result = (char *)push_copy(&StringArena, strlen(Result) + strlen((char *)Text->uData.STRING.Value)+1, Result, default_arena_params());
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
            Result = (char *)push_copy(&StringArena, strlen(Result) + 2, Result, default_arena_params());
            Result[ResultEnd] = (char)Text->uData.CHARACTER.Value;
            Result[ResultEnd+1] = 0;
        }
    }
    
    end_temp(StringTemp);
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
    LOG(ERR_WARN, "illegal state: The body of the apply should not execute");
    InvalidCodePath;
    
    Unreachable(Args);
}

def_proc(eval)
{
    LOG(ERR_WARN, "illegal state: The body of the eval should not execute");
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
        LOG(ERR_WARN, "Could not load file \"%s\"\n", Filename);
        return(Nil);
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
    
    In = is_nil(Args) ? read_input(stdin) : (pair_get_a(Args))->uData.INPUT.Stream;
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
    b32 StripQuotes = is_nil(Args) ? 1 : is_nil(pair_get_b(Args)) ? 1 : (pair_get_a(pair_get_b(Args)))->uData.BOOLEAN.Value;
    write(Out, Exp, StripQuotes);
    fflush(Out);
    return(OKSymbol);
}

def_proc(peek_char)
{
    FILE *In;
    s32 Result;
    
    In = is_nil(Args) ? read_input(stdin) : (pair_get_a(Args))->uData.INPUT.Stream;
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
        LOG(ERR_WARN, "Could not load file \"%s\"\n", Filename);
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
        LOG(ERR_WARN, "Could not close input");
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
        LOG(ERR_WARN, "Could not load file \"%s\"\n", Filename);
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
        LOG(ERR_WARN, "Could not close output");
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
    if(!is_string(String) && (is_realnum(String) || is_fixnum(String)))
    {
        TEMP_MEMORY Temp = begin_temp(&StringArena);
        
        char *Buffer = (char *)push_size(&StringArena, 66, default_arena_params());
        if(is_realnum(String))
        {
            sprintf(Buffer, "%lf", (double)String->uData.REALNUM.Value);
        }
        else sprintf(Buffer, "%" PRId64, String->uData.FIXNUM.Value);
        
        String = make_string((u8 *)Buffer);
        
        end_temp(Temp);
    }
    else if(!is_string(String))
    {
        LOG(ERR_WARN, "write-string expects 1 parameter to be: <STRING|REALNUM|FIXNUM>.");
        return(Nil);
    }
    Args = pair_get_b(Args);
    Out = is_nil(Args) ? stdout : (pair_get_a(Args))->uData.OUTPUT.Stream;
    fprintf(Out, "%s", (char *)String->uData.STRING.Value);
    fflush(Out);
    return(OKSymbol);
}

def_proc(read_string)
{
    FILE *In;
    OBJECT *Result;
    
    if(is_nil(pair_get_a(Args)))
    {
        LOG(ERR_WARN, "read-string expects 1 parameter to be: INPUT stream.");
        return(OKSymbol);
    }
    
    In = (pair_get_a(Args))->uData.INPUT.Stream;
    
    fseek(In, 0, SEEK_END);
    s32 Size = ftell(In);
    rewind(In);
    
    TEMP_MEMORY Temp = begin_temp(&StringArena);
    char *String = (char *)push_size(&StringArena, Size + 1, default_arena_params());
    fread(String, Size, 1, In);
    String[Size] = 0;
    Result = make_string((u8 *)String);
    
    end_temp(Temp);
    
    return((Result == 0) ? EOF_Obj : Result);
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
#if _ELEVATED == 1
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
#else
    LOG(ERR_WARN, "Procedure requires elevation!");
    return(OKSymbol);
    Unreachable(Args);
#endif
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

def_proc(error_reporting)
{
    if(is_nil(pair_get_a(Args)))
    {
        LOG(ERR_WARN, "error-reporting is missing: <error-level>");
        return(OKSymbol);
    }
    
    u8 Result = (u8)(pair_get_a(Args)->uData.FIXNUM.Value);
    error_reporting(Result);
    return(OKSymbol);
}

def_proc(random)
{
    srand(time(0));
    
    s64 RandomValue = (s64)rand();
    
    if(!is_nil(pair_get_a(Args)))
    {
        RandomValue %= (pair_get_a(Args))->uData.FIXNUM.Value;
    }
    
    return(make_fixnum(RandomValue));
}

def_proc(error)
{
    if(is_nil(pair_get_a(Args)))
    {
        LOG(ERR_WARN, "error is missing: <message>");
        return(OKSymbol);
    }
    LOG(ERR_WARN, "Exception -> %s", (char *)pair_get_a(Args)->uData.STRING.Value);
    return(OKSymbol);
}

def_proc(sin)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(sin((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(sin((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(cos)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(cos((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(cos((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(tan)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(tan((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(tan((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(asin)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(asin((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(asin((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(acos)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(acos((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(acos((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(atan)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(atan((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(atan((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(sqrt)
{
    if(pair_get_a(Args)->Type == REALNUM)
    {
        return(make_realnum(sqrt((double)(pair_get_a(Args)->uData.REALNUM.Value))));
    }
    return(make_realnum(sqrt((double)(pair_get_a(Args)->uData.FIXNUM.Value))));
}

def_proc(exit)
{
     s32 ErrorCode = 0;
     if(!is_nil(pair_get_a(Args)))
     {
          ErrorCode = pair_get_a(Args)->uData.FIXNUM.Value;
     }

     exit((int)ErrorCode);
}

def_proc(log_verbose)
{
    if(!is_nil(pair_get_a(Args)) && !is_boolean(pair_get_a(Args)))
    {
        LOG(ERR_WARN, "log-verbose is missing: <BOOLEAN>");
        return(OKSymbol);
    }
    b32 Result = 0;
    if(!is_nil(pair_get_a(Args)))
    {
        Result = (is_true(pair_get_a(Args))) ? 1 : 0;
    }
    else
    {
        Result = !IsVerbose;
    }
    
    set_log_verbose(Result);
    return(OKSymbol);
}

static inline void
init_builtins(OBJECT *Env)
{

     add_procedure("inc", inc_proc);
     add_procedure("dec", dec_proc);

    add_procedure("+"   , add_proc);
    add_procedure("-"   , sub_proc);
    add_procedure("*"   , mul_proc);
    add_procedure("/"   , div_proc);
    add_procedure("%"   , mod_proc);
    
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
    add_procedure("string->char" , string_to_char_proc);
    add_procedure("number->string", number_to_string_proc);
    add_procedure("string->number", string_to_number_proc);
    add_procedure("symbol->string", symbol_to_string_proc);
    add_procedure("string->symbol", string_to_symbol_proc);
      
    add_procedure("="        , is_number_equal_proc);
    add_procedure("<"        , is_less_than_proc);
    add_procedure(">"        , is_greater_than_proc);
    add_procedure(">="        , is_greater_than_or_equal_proc);
    add_procedure("<="        , is_less_than_or_equal_proc);
    
    add_procedure("sin"         , sin_proc);
    add_procedure("cos"         , cos_proc);
    add_procedure("tan"         , tan_proc);
    add_procedure("asin"        , asin_proc);
    add_procedure("acos"        , acos_proc);
    add_procedure("atan"        , atan_proc);
    add_procedure("sqrt"        , sqrt_proc);

    add_procedure("exit"        , exit_proc);

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
    add_procedure("read"               , read_proc);
    add_procedure("read-char"          , read_char_proc);
    add_procedure("read-string"        , read_string_proc);
    add_procedure("peek-char"          , peek_char_proc);
    add_procedure("eof?"        , is_eof_obj_proc);
    add_procedure("open-output" , open_output_proc);
    add_procedure("close-output", close_output_proc);
    add_procedure("output?"     , is_output_proc);
    add_procedure("write-char"       , write_char_proc);
    add_procedure("write-string"     , write_string_proc);
    add_procedure("write"            , write_proc);
    add_procedure("error"            , error_proc);
    
    add_procedure("concat"      , concat_proc);
    add_procedure("system"      , system_proc);
    
    add_procedure("arena-mem"      , arena_mem_proc);
    add_procedure("log-mem"        , log_mem_proc);
    add_procedure("error-reporting", error_reporting_proc);
    add_procedure("log-verbose"    , log_verbose_proc);
    
    add_procedure("random"         , random_proc);
}

#define DZM_PRC_H
#endif
