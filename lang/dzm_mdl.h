// (c) ZaKlaus 2016; All Rights Reserved;;

#if !defined(DZM_MDL_H)

#define MAX_STRING_SIZE 1000

enum OBJECT_TYPE_
{
    UNKNOWN,
    FIXNUM,
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

typedef struct OBJECT_
{
    u8 Type;
    union
    {
        struct
        {
            s32 Value;
        } FIXNUM;
        
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
            struct OBJECT_ *A;
            struct OBJECT_ *B;
        } PAIR;
        
        struct
        {
            u8 *Value;
        } SYMBOL;
        
        struct
        {
            struct OBJECT_ *(*Fn)(struct OBJECT_ *Args);
        } PROCEDURE;
        
        struct
        {
            struct OBJECT_ *Parameters;
            struct OBJECT_ *Body;
            struct OBJECT_ *Env;
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
} OBJECT;

OBJECT *
alloc_object(void)
{
    OBJECT *Obj = malloc(sizeof(OBJECT));
    
    zassert(Obj != 0);
    
    return(Obj);
}

OBJECT *
make_fixnum(s32 Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = FIXNUM;
    Obj->uData.FIXNUM.Value = Value;
    return(Obj);
}

OBJECT *
make_character(u8 Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = CHARACTER;
    Obj->uData.CHARACTER.Value = Value;
    return(Obj);
}

OBJECT *Nil;

OBJECT *
make_string(u8 *Value)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = STRING;
    if(Value == 0)
    {
        Obj->uData.STRING.Value = malloc(1);
        *Obj->uData.STRING.Value = 0;
        return(Nil);
    }
    Obj->uData.STRING.Value = malloc(strlen((char *)Value) + 1);
    zassert(Obj->uData.STRING.Value != NULL);
    string_copy(Obj->uData.STRING.Value, Value);
    return(Obj);
}

OBJECT *
make_pair(OBJECT *A, OBJECT *B)
{
    OBJECT *Obj = alloc_object();
    Obj->Type = PAIR;
    Obj->uData.PAIR.A = A;
    Obj->uData.PAIR.B = B;
    return(Obj);
}

OBJECT *
make_input(FILE *In)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = INPUT;
    Obj->uData.INPUT.Stream = In;
    return(Obj);
}

OBJECT *
make_output(FILE *Out)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = OUTPUT;
    Obj->uData.OUTPUT.Stream = Out;
    return(Obj);
}

b32
is_fixnum(OBJECT *Obj)
{
    return(Obj->Type == FIXNUM);
}

b32
is_boolean(OBJECT *Obj)
{
    return(Obj->Type == BOOLEAN);
}

b32
is_input(OBJECT *Obj)
{
    return(Obj->Type == INPUT);
}

b32
is_output(OBJECT *Obj)
{
    return(Obj->Type == OUTPUT);
}

b32
is_eof_id(OBJECT *Obj)
{
    return(Obj->Type == EOF_ID);
}

b32
is_character(OBJECT *Obj)
{
    return(Obj->Type == CHARACTER);
}

b32
is_string(OBJECT *Obj)
{
    return(Obj->Type == STRING);
}

b32
is_nil(OBJECT *Obj)
{
    return(Obj->Type == NIL);
}

b32
is_pair(OBJECT *Obj)
{
    return(Obj->Type == PAIR);
}

b32
is_symbol(OBJECT *Obj)
{
    return(Obj->Type == SYMBOL);
}

b32
is_eof_obj(OBJECT *Obj)
{
    return(Obj->Type == EOF_ID);
}

// == GLOBAL VARS
OBJECT *False;
OBJECT *True;
OBJECT *Nil;
OBJECT *SymbolTable;

OBJECT *QuoteSymbol;
OBJECT *DefineSymbol;
OBJECT *OKSymbol;
OBJECT *SetSymbol;
OBJECT *LambdaSymbol;
OBJECT *BeginSymbol;
OBJECT *CondSymbol;
OBJECT *ElseSymbol;
OBJECT *LetSymbol;
OBJECT *AndSymbol;
OBJECT *OrSymbol;
OBJECT *IfSymbol;

OBJECT *EOF_Obj;

OBJECT *NilEnv;
OBJECT *GlobalEnv;
// -- GLOBAL VARS

OBJECT *
make_symbol(u8 *Value);

OBJECT *
pair_get_a(OBJECT *Pair);
OBJECT *
pair_get_b(OBJECT *Pair);
void
pair_set_a(OBJECT *Pair, OBJECT *A);
void
pair_set_b(OBJECT *Pair, OBJECT *B);

OBJECT *
make_procedure(OBJECT *(*Fn)(OBJECT *Args))
{
    OBJECT *Obj = alloc_object();
    Obj->Type = PROCEDURE;
    Obj->uData.PROCEDURE.Fn = Fn;
    return(Obj);
}


OBJECT *
make_compound(OBJECT *Parameters, OBJECT *Body, OBJECT *Env)
{
    OBJECT *Obj = alloc_object();
    
    Obj->Type = COMPOUND;
    Obj->uData.COMPOUND.Parameters = Parameters;
    Obj->uData.COMPOUND.Body = Body;
    Obj->uData.COMPOUND.Env = Env;
    return(Obj);
}

b32
is_procedure(OBJECT *Obj)
{
    return(Obj->Type == PROCEDURE);
}

b32
is_compound(OBJECT *Obj)
{
    return(Obj->Type == COMPOUND);
}

OBJECT *
enclosing_env(OBJECT *Env)
{
    return(pair_get_b(Env));
}

OBJECT *
first_frame(OBJECT *Env)
{
    return(pair_get_a(Env));
}

OBJECT *
make_frame(OBJECT *Vars, OBJECT *Vals)
{
    return(make_pair(Vars, Vals));
}

OBJECT *
frame_variables(OBJECT *Frame)
{
    return(pair_get_a(Frame));
}

OBJECT *
frame_values(OBJECT *Frame)
{
    return(pair_get_b(Frame));
}

void
add_binding_to_frame(OBJECT *Var, OBJECT *Val, OBJECT *Frame)
{
    pair_set_a(Frame, make_pair(Var, pair_get_a(Frame)));
    pair_set_b(Frame, make_pair(Val, pair_get_b(Frame)));
}

OBJECT *
extend_env(OBJECT *Vars, OBJECT *Vals, OBJECT *Base)
{
    return(make_pair(make_frame(Vars, Vals), Base));
}

OBJECT *
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
                return(pair_get_a(Vals));
            }
            Vars = pair_get_b(Vars);
            Vals = pair_get_b(Vals);
        }
        Env = enclosing_env(Env);
    }
    fprintf(stderr, "Unbound variable\n");
    Unreachable(Nil);
}

void
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
    fprintf(stderr, "Unbound variable\n");
    InvalidCodePath;
}

void
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

OBJECT *
setup_env(void)
{
    OBJECT *InitEnv;
    
    InitEnv = extend_env(Nil, Nil, NilEnv);
    return(InitEnv);
}

void
init_builtins(OBJECT *Env);

OBJECT *
make_env(void)
{
    OBJECT * Env = setup_env();
    init_builtins(Env);
    return(Env);
}

OBJECT *
eval(OBJECT *Exp, OBJECT *Env);

OBJECT *
read(FILE *In);

void
write(FILE *Out, OBJECT *Obj);

s32
peek(FILE *In);

#include "dzm_prc.h"

void
init_defs(void)
{
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
    AndSymbol = make_symbol(UL_"and");
    OrSymbol = make_symbol(UL_"or");
    
    NilEnv = Nil;
    GlobalEnv = make_env();
}

b32
is_false(OBJECT *Obj)
{
    return(Obj == False);
}

b32
is_true(OBJECT *Obj)
{
    return(Obj == True);
}

OBJECT *
pair_get_a(OBJECT *Pair)
{
	if (is_pair(Pair))
		return(Pair->uData.PAIR.A);
	else
		return(Nil);
}

OBJECT *
pair_get_b(OBJECT *Pair)
{
    return(Pair->uData.PAIR.B);
}

void
pair_set_a(OBJECT *Pair, OBJECT *A)
{
    Pair->uData.PAIR.A = A;
}

void
pair_set_b(OBJECT *Pair, OBJECT *B)
{
    Pair->uData.PAIR.B = B;
}

OBJECT *
make_symbol(u8 *Value)
{
    OBJECT *Obj = alloc_object();
    OBJECT *Elem = SymbolTable;
    
    while(!is_nil(Elem))
    {
        if (!strcmp((char *)pair_get_a(Elem)->uData.SYMBOL.Value, (char *)Value))
        {
            return(pair_get_a(Elem));
        }
        Elem = pair_get_b(Elem);
    }
    
    Obj->Type = SYMBOL;
    Obj->uData.SYMBOL.Value = malloc(strlen((char *)Value) + 1);
    zassert(Obj->uData.SYMBOL.Value != 0);
    
    string_copy(Obj->uData.SYMBOL.Value, Value);
    
    SymbolTable = make_pair(Obj, SymbolTable);
    
    return(Obj);
}

#define DZM_MDL_H
#endif