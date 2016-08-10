// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_MDL_H)

enum OBJECT_TYPE
{
    UNKNOWN,
    FIXNUM,
    REALNUM,
    BOOLEAN,
    CHARACTER,
    STRING,
    NIL,
    PAIR,
    SYMBOL,
    PROCEDURE,
    COMPOUND,
    INPUT,
    OUTPUT,
    EOF_ID,
};

struct OBJECT
{
    OBJECT *Next;
    OBJECT *Parent;
    u8 Type;
    b32 Mark;
    union
    {
        struct
        {
            s64 Value;
        } FIXNUM;
        
        struct
        {
            real64 Value;
        } REALNUM;
        
        struct
        {
            b32 Value;
        } BOOLEAN;
        
        struct
        {
            u8  Value;
        } CHARACTER;
        
        struct
        {
            u8 *Value;
        } STRING;
        
        struct
        {
            OBJECT *A;
            OBJECT *B;
        } PAIR;
        
        struct
        {
            u8 *Value;
        } SYMBOL;
        
        struct
        {
            OBJECT *(*Fn)(OBJECT *Args);
            b32 Lazy;
        } PROCEDURE;
        
        struct
        {
            OBJECT *Parameters;
            OBJECT *Body;
            OBJECT *Env;
        } COMPOUND;
        
        struct
        {
            FILE *Stream;
        } INPUT;

        struct
        {
            FILE *Stream;
        } OUTPUT;
    } uData;
};

MEMORY_ARENA *GlobalArena;
OBJECT *GlobalTable;
OBJECT *GlobalEnv;
OBJECT *Nil;
OBJECT *SymbolTable;

// TODO(zaklaus): Move to dzm_log.h!!!
b32 PrintMemUsage;

static inline OBJECT *
pair_get_a(OBJECT *Pair);

static inline OBJECT *
pair_get_b(OBJECT *Pair);

static inline b32
is_nil(OBJECT *Obj);

static inline void
free_special_object(OBJECT *Obj)
{
    switch(Obj->Type)
    {
        case SYMBOL:
        case STRING:
        {
            if(Obj->uData.STRING.Value)
            {
                free(Obj->uData.STRING.Value);
                Obj->uData.STRING.Value = 0;
            }
        }break;
        
        case PAIR:
        {
            // TODO(zaklaus): Decide what to do with pair-wise objects.
        }break;
        
        default:
        {
            
        }break;
    }
}

static inline OBJECT *
alloc_object(void)
{
    OBJECT *Obj = 0;
    
    if(GlobalTable)
    for(OBJECT *It = GlobalTable->Next;
        It;
        It = It->Next)
    {
        // NOTE(zaklaus): EXPERIMENTAL
        /*{
            OBJECT *StackObj = GlobalEnv;
            b32 IsDead = 1;
            while(StackObj != 0 && !is_nil(pair_get_a(StackObj)))
            {
                if(It == pair_get_a(StackObj))
                {
                    IsDead = 0;
                    break;
                }
                StackObj = pair_get_b(StackObj);
            }
            
            if(IsDead)
            {
                It->Mark = 1;
            }
        }*/
        
        if(It->Mark == 255)// && (It->Parent == Nil || It->Type == SYMBOL))
        {
            free_special_object(It);
            It->Type = NIL;
            
            /*if(Obj->Parent == SymbolTable)
            {
                SymbolTable->Mark = 1;
                SymbolTable = pair_get_b(SymbolTable);
                Obj->Parent = Nil;
            }*/
            
            if(!Obj)
            {
                Obj = It;
                Obj->Mark = 0;
            }
        }    
    }
    
    //OBJECT *Obj = malloc(sizeof(OBJECT));
    
    if(Obj == 0)
    {
        Obj = push_type(GlobalArena, OBJECT, align_noclear(sizeof(OBJECT)));
        
        if(GlobalTable == 0)
        {
            GlobalTable = Obj;
            GlobalTable->Next = 0;
        }
        else
        {
            Obj->Next = GlobalTable;
            GlobalTable = Obj;
        }
        
        Obj->Mark = 0;
        Obj->Parent = Nil;
    }
    
    zassert(Obj != 0);
    
    return(Obj);
}

static inline OBJECT *
make_object(u8 Type, void *Arg0=0, void *Arg1=0, void *Arg2=0)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = Type;
    
    switch(Type)
    {
        case FIXNUM:
        {
            Obj->uData.FIXNUM.Value = *((s64*)Arg0);
        }break;
        case REALNUM:
        {
            Obj->uData.REALNUM.Value = *((r64*)Arg0);
        }break;
        case CHARACTER:
        {
            Obj->uData.CHARACTER.Value = *((u8*)Arg0);
        }break;
        case STRING:
        {
            Obj->uData.STRING.Value = (u8 *)malloc(strlen((char *)Arg0) + 1);
            string_copy(Obj->uData.STRING.Value, (u8 *)Arg0);
        }break;
        case PAIR:
        {
            Obj->uData.PAIR.A = (OBJECT *)Arg0;
            Obj->uData.PAIR.B = (OBJECT *)Arg1;
        }break;
        case INPUT:
        {
            Obj->uData.INPUT.Stream = (FILE *)Arg0;
        }break;
        case OUTPUT:
        {
            Obj->uData.OUTPUT.Stream = (FILE *)Arg0;
        }break;
        case PROCEDURE:
        {
            Obj->uData.PROCEDURE.Fn = (OBJECT* (*)(OBJECT*))Arg0;
            Obj->uData.PROCEDURE.Lazy = 0; // TODO(zaklaus): replace with variadic solution!
        }break;
        case COMPOUND:
        {
            Obj->uData.COMPOUND.Parameters = (OBJECT *)Arg0;
            Obj->uData.COMPOUND.Body = (OBJECT *)Arg1;
            Obj->uData.COMPOUND.Env = (OBJECT *)Arg2;
        }break;
        
        default:
        {}
    }
    return(Obj);
    Unreachable((OBJECT *)Arg0);
    Unreachable((OBJECT *)Arg1);
}

static inline OBJECT *
make_realnum(real64 Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = REALNUM;
    Obj->uData.REALNUM.Value = Value;
    return(Obj);
}

static inline OBJECT *
make_character(u8 Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = CHARACTER;
    Obj->uData.CHARACTER.Value = Value;
    return(Obj);
}

static inline OBJECT *
make_string(u8 *Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = STRING;
    Obj->uData.STRING.Value = (u8 *)malloc(strlen((char *)Value) + 1);
    zassert(Obj->uData.STRING.Value != NULL);
    string_copy(Obj->uData.STRING.Value, Value);
    return(Obj);
}

static inline OBJECT *
make_pair(OBJECT *A, OBJECT *B)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = PAIR;
    Obj->uData.PAIR.A = A; A->Parent = Obj;
    Obj->uData.PAIR.B = B; B->Parent = Obj;
    return(Obj);
}

static inline OBJECT *
make_input(FILE *In)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = INPUT;
    Obj->uData.INPUT.Stream = In;
    return(Obj);
}

static inline OBJECT *
make_output(FILE *Out)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = OUTPUT;
    Obj->uData.OUTPUT.Stream = Out;
    return(Obj);
}

static inline b32
is_fixnum(OBJECT *Obj)
{
    return(Obj->Type == FIXNUM);
}

static inline b32
is_realnum(OBJECT *Obj)
{
    return(Obj->Type == REALNUM);
}

static inline b32
is_boolean(OBJECT *Obj)
{
    return(Obj->Type == BOOLEAN);
}

static inline b32
is_input(OBJECT *Obj)
{
    return(Obj->Type == INPUT);
}

static inline b32
is_output(OBJECT *Obj)
{
    return(Obj->Type == OUTPUT);
}

static inline b32
is_eof_id(OBJECT *Obj)
{
    return(Obj->Type == EOF_ID);
}

static inline b32
is_character(OBJECT *Obj)
{
    return(Obj->Type == CHARACTER);
}

static inline b32
is_string(OBJECT *Obj)
{
    return(Obj->Type == STRING);
}

static inline b32
is_nil(OBJECT *Obj)
{
    return(Obj->Type == NIL);
}

static inline b32
is_pair(OBJECT *Obj)
{
    return(Obj->Type == PAIR);
}

static inline b32
is_symbol(OBJECT *Obj)
{
    return(Obj->Type == SYMBOL);
}

static inline b32
is_eof_obj(OBJECT *Obj)
{
    return(Obj->Type == EOF_ID);
}

// == GLOBAL VARS
OBJECT *False;
OBJECT *True;

OBJECT *QuoteSymbol;
OBJECT *DefineSymbol;
OBJECT *OKSymbol;
OBJECT *SetSymbol;
OBJECT *LambdaSymbol;
OBJECT *BeginSymbol;
OBJECT *CondSymbol;
OBJECT *ElseSymbol;
OBJECT *LetSymbol;
OBJECT *LetrecSymbol;
OBJECT *AndSymbol;
OBJECT *OrSymbol;
OBJECT *IfSymbol;
OBJECT *VarSymbol;

OBJECT *EOF_Obj;

OBJECT *NilEnv;
// -- GLOBAL VARS

static inline OBJECT *
make_symbol(u8 *Value);

static inline OBJECT *
pair_get_a(OBJECT *Pair);
static inline OBJECT *
pair_get_b(OBJECT *Pair);
static inline void
pair_set_a(OBJECT *Pair, OBJECT *A);
static inline void
pair_set_b(OBJECT *Pair, OBJECT *B);

static inline OBJECT *
make_procedure(OBJECT *(*Fn)(OBJECT *Args))
{
    OBJECT *Obj = alloc_object();
    Obj->Type = PROCEDURE;
    Obj->uData.PROCEDURE.Fn = Fn;
    Obj->uData.PROCEDURE.Lazy = 0;
    return(Obj);
}


static inline OBJECT *
make_compound(OBJECT *Parameters, OBJECT *Body, OBJECT *Env)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = COMPOUND;
    Obj->uData.COMPOUND.Parameters = Parameters;
    Obj->uData.COMPOUND.Body = Body;
    Obj->uData.COMPOUND.Env = Env;
    return(Obj);
}

static inline b32
is_procedure(OBJECT *Obj)
{
    return(Obj->Type == PROCEDURE);
}

static inline b32
is_compound(OBJECT *Obj)
{
    return(Obj->Type == COMPOUND);
}

static inline OBJECT *
enclosing_env(OBJECT *Env)
{
    return(pair_get_b(Env));
}

static inline OBJECT *
first_frame(OBJECT *Env)
{
    return(pair_get_a(Env));
}

static inline OBJECT *
make_frame(OBJECT *Vars, OBJECT *Vals)
{
    return(make_pair(Vars, Vals));
}

static inline OBJECT *
frame_variables(OBJECT *Frame)
{
    return(pair_get_a(Frame));
}

static inline OBJECT *
frame_values(OBJECT *Frame)
{
    return(pair_get_b(Frame));
}

static inline void
add_binding_to_frame(OBJECT *Var, OBJECT *Val, OBJECT *Frame)
{
    pair_set_a(Frame, make_pair(Var, pair_get_a(Frame)));
    pair_set_b(Frame, make_pair(Val, pair_get_b(Frame)));
}

static inline OBJECT *
extend_env(OBJECT *Vars, OBJECT *Vals, OBJECT *Base)
{
    return(make_pair(make_frame(Vars, Vals), Base));
}

static inline OBJECT *
lookup_variable_value(OBJECT *Var, OBJECT *Env)
{
    OBJECT *Frame;
    OBJECT *Vars;
    OBJECT *Vals;
    while(!is_nil(Env))
    {
        Frame = first_frame(Env);
        Vars = frame_variables(Frame);
        Vals = frame_values(Frame);
        while(!is_nil(Vars))
        {
            if(Var == pair_get_a(Vars))
            {
                (pair_get_a(Vars))->Mark = 1;
                return(pair_get_a(Vals));
            }
            Vars = pair_get_b(Vars);
            Vals = pair_get_b(Vals);
        }
        Env = enclosing_env(Env);
    }
    LOG(ERR_WARN, "Unbound variable: %s", Var->uData.SYMBOL.Value);
    Var->Mark = 1;
    Unreachable(Nil);
}

static inline void
set_variable_value(OBJECT *Var, OBJECT *Val, OBJECT *Env)
{
    OBJECT *Frame;
    OBJECT *Vars;
    OBJECT *Vals;
    while(!is_nil(Env))
    {
        Frame = first_frame(Env);
        Vars = frame_variables(Frame);
        Vals = frame_values(Frame);
        while(!is_nil(Vars))
        {
            if(Var == pair_get_a(Vars))
            {
                pair_set_a(Vals, Val);
                return;
            }
            
            Vars = pair_get_b(Vars);
            Vals = pair_get_b(Vals);
        }
        Env = enclosing_env(Env);
    }
    Var->Mark = 1;
    LOG(ERR_WARN, "Unbound variable");
    InvalidCodePath;
}

static inline void
define_variable(OBJECT *Var, OBJECT *Val, OBJECT *Env)
{
    OBJECT *Frame = first_frame(Env);
    OBJECT *Vars = frame_variables(Frame);
    OBJECT *Vals = frame_values(Frame);
    while(!is_nil(Vars))
    {
        if(Var == pair_get_a(Vars))
        {
            pair_set_a(Vals, Val);
            return;
        }
        Vars = pair_get_b(Vars);
        Vals = pair_get_b(Vals);
    }
    add_binding_to_frame(Var, Val, Frame);
}

static inline OBJECT *
setup_env(void)
{
    OBJECT *InitEnv;
    
    InitEnv = extend_env(Nil, Nil, NilEnv);
    return(InitEnv);
}

static inline void
init_builtins(OBJECT *Env);

static inline OBJECT *
make_env(void)
{
    OBJECT * Env = setup_env();
    init_builtins(Env);
    return(Env);
}

static inline OBJECT *
eval(OBJECT *Exp, OBJECT *Env);

static inline OBJECT *
read(FILE *In);

static inline void
write(FILE *Out, OBJECT *Obj);

static inline s32
peek(FILE *In);

static inline b32
is_false(OBJECT *Obj)
{
    return(Obj == False);
}

static inline b32
is_true(OBJECT *Obj)
{
    return(Obj == True);
}

#include "dzm_prc.h"

static inline void
init_defs(void)
{
    PrintMemUsage = 0;
    
    False = alloc_object();
    False->Type = BOOLEAN;
    False->uData.BOOLEAN.Value = 0;
    
    True = alloc_object();
    True->Type = BOOLEAN;
    True->uData.BOOLEAN.Value = 1;
    
    Nil = alloc_object();
    Nil->Type = NIL;
    
    SymbolTable = Nil;
    QuoteSymbol = make_symbol(UL_"quote");
    DefineSymbol = make_symbol(UL_"define");
    OKSymbol = make_symbol(UL_"\0");
    SetSymbol = make_symbol(UL_"set!");
    IfSymbol = make_symbol(UL_"if");
    LambdaSymbol = make_symbol(UL_"lambda");
    BeginSymbol = make_symbol(UL_"begin");
    CondSymbol = make_symbol(UL_"cond");
    ElseSymbol = make_symbol(UL_"else");
    LetSymbol = make_symbol(UL_"let");
    LetrecSymbol = make_symbol(UL_"letrec");
    AndSymbol = make_symbol(UL_"and");
    OrSymbol = make_symbol(UL_"or");
    VarSymbol = make_symbol(UL_".");
    
    NilEnv = Nil;
    GlobalEnv = make_env();
}

static inline OBJECT *
pair_get_a(OBJECT *Pair)
{
	if (is_pair(Pair))
		return(Pair->uData.PAIR.A);
	else
		return(Nil);
}

static inline OBJECT *
pair_get_b(OBJECT *Pair)
{
    return(Pair->uData.PAIR.B);
}

static inline void
pair_set_a(OBJECT *Pair, OBJECT *A)
{
    Pair->uData.PAIR.A = A; A->Parent = Pair;
}

static inline void
pair_set_b(OBJECT *Pair, OBJECT *B)
{
    Pair->uData.PAIR.B = B; B->Parent = Pair;
}

static inline OBJECT *
make_symbol(u8 *Value)
{
    OBJECT *Obj = alloc_object();
    OBJECT *Elem = SymbolTable;
    
    while(Elem != 0 && !is_nil(Elem))
    {
        if (!strcmp((char *)pair_get_a(Elem)->uData.SYMBOL.Value, (char *)Value))
        {
            return(pair_get_a(Elem));
        }
        Elem = pair_get_b(Elem);
    }
    Obj->Type = SYMBOL;
    Obj->uData.SYMBOL.Value = (u8 *)push_size(GlobalArena, strlen((char *)Value) + 1, default_arena_params());
    zassert(Obj->uData.SYMBOL.Value != 0);
    
    string_copy(Obj->uData.SYMBOL.Value, Value);
    
    SymbolTable = make_pair(Obj, SymbolTable);
    
    return(Obj);
}

#define DZM_MDL_H
#endif