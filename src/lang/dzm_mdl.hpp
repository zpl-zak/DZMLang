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

#if !defined(DZM_MDL_H)

enum OBJECT_TYPE
{
     MDL_UNKNOWN,
     MDL_FIXNUM,
     MDL_REALNUM,
     MDL_BOOLEAN,
     MDL_CHARACTER,
     MDL_STRING,
     MDL_NIL,
     MDL_PAIR,
     MDL_SYMBOL,
     MDL_PROCEDURE,
     MDL_COMPOUND,
     MDL_INPUT,
     MDL_OUTPUT,
     MDL_SOCKET,
     MDL_EOF_ID,
};

struct OBJECT
{
     u8 Type;
     OBJECT *Name;
     union
     {
          struct
          {
               s64 Value;
          } MDL_FIXNUM;
          struct
          {
               real64 Value;
          } MDL_REALNUM;
          struct
          {
               b32 Value;
          } MDL_BOOLEAN;
          struct
          {
               u8  Value;
          } MDL_CHARACTER;
          struct
          {
               u8 *Value;
          } MDL_STRING;
          struct
          {
               OBJECT *A;
               OBJECT *B;
          } MDL_PAIR;
          struct
          {
               u8 *Value;
          } MDL_SYMBOL;
          struct
          {
               OBJECT *(*Fn)(OBJECT *Args);
               b32 Lazy;
          } MDL_PROCEDURE;
          struct
          {
               OBJECT *Parameters;
               OBJECT *Body;
               OBJECT *Env;
          } MDL_COMPOUND;
          struct
          {
               FILE *Stream;
          } MDL_INPUT;
          struct
          {
               s32 SocketId;
          } MDL_SOCKET;
          struct
          {
               FILE *Stream;
          } MDL_OUTPUT;
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

static inline OBJECT *
alloc_object(void)
{
     //push_type(GlobalArena, OBJECT, align_noclear(sizeof(OBJECT)));
     OBJECT *Obj = (OBJECT *)malloc(sizeof(OBJECT));
     //GlobalArena->Used += sizeof(OBJECT);
     Obj->Name = Nil;

     zassert(Obj);
     return(Obj);
}

static inline OBJECT *
make_fixnum(s64 Value)
{
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_FIXNUM;
     Obj->uData.MDL_FIXNUM.Value = Value;
     return(Obj);
}

static inline OBJECT *
make_realnum(real64 Value)
{
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_REALNUM;
     Obj->uData.MDL_REALNUM.Value = Value;
     return(Obj);
}

static inline OBJECT *
make_character(u8 Value)
{
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_CHARACTER;
     Obj->uData.MDL_CHARACTER.Value = Value;
     return(Obj);
}

static inline OBJECT *
make_string(u8 *Value)
{
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_STRING;
     Obj->uData.MDL_STRING.Value = (u8 *)malloc(strlen((char *)Value) + 1);
     zassert(Obj->uData.MDL_STRING.Value != NULL);
     string_copy(Obj->uData.MDL_STRING.Value, Value);
     return(Obj);
}

static inline OBJECT *
make_pair(OBJECT *A, OBJECT *B)
{
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_PAIR;
     Obj->uData.MDL_PAIR.A = A;
     Obj->uData.MDL_PAIR.B = B;
     return(Obj);
}

static inline OBJECT *
make_input(FILE *In)
{
     OBJECT *Obj = alloc_object();

     Obj->Type = MDL_INPUT;
     Obj->uData.MDL_INPUT.Stream = In;
     return(Obj);
}

static inline OBJECT *
make_output(FILE *Out)
{
     OBJECT *Obj = alloc_object();

     Obj->Type = MDL_OUTPUT;
     Obj->uData.MDL_OUTPUT.Stream = Out;
     return(Obj);
}

static inline OBJECT *
make_socket()
{
#ifdef WIN32
     static b32 WSARuns = 0;
     if (!WSARuns)
     {
          WSARuns = 1;
          WSADATA wsaData;
          int startUpResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
          if (startUpResult != NO_ERROR)
          {
               LOG(ERR_WARN, "Could not start WindowsSocket.");
               return (Nil);
          }
     }
#endif
     OBJECT *Obj = alloc_object();
     Obj->Type = MDL_SOCKET;

     int sockId = (int)socket(AF_INET, SOCK_STREAM, 0);
     Obj->uData.MDL_SOCKET.SocketId = sockId;
     return(Obj);
}

static inline b32
is_fixnum(OBJECT *Obj)
{
     return(Obj->Type == MDL_FIXNUM);
}

static inline b32
is_realnum(OBJECT *Obj)
{
     return(Obj->Type == MDL_REALNUM);
}

static inline b32
is_boolean(OBJECT *Obj)
{
     return(Obj->Type == MDL_BOOLEAN);
}

static inline b32
is_input(OBJECT *Obj)
{
     return(Obj->Type == MDL_INPUT);
}

static inline b32
is_output(OBJECT *Obj)
{
     return(Obj->Type == MDL_OUTPUT);
}

static inline b32
is_eof_id(OBJECT *Obj)
{
     return(Obj->Type == MDL_EOF_ID);
}

static inline b32
is_character(OBJECT *Obj)
{
     return(Obj->Type == MDL_CHARACTER);
}

static inline b32
is_string(OBJECT *Obj)
{
     return(Obj->Type == MDL_STRING);
}

static inline b32
is_nil(OBJECT *Obj)
{
     return(Obj->Type == MDL_NIL);
}

static inline b32
is_pair(OBJECT *Obj)
{
     return(Obj->Type == MDL_PAIR);
}

static inline b32
is_symbol(OBJECT *Obj)
{
     return(Obj->Type == MDL_SYMBOL);
}

static inline b32
is_eof_obj(OBJECT *Obj)
{
     return(Obj->Type == MDL_EOF_ID);
}

static inline b32
is_socket(OBJECT *Obj)
{
     return(Obj->Type == MDL_SOCKET);
}

// == GLOBAL VARS
OBJECT *False;
OBJECT *True;

OBJECT *QuoteSymbol;
OBJECT *DefineSymbol;
OBJECT *ShortDefineSymbol;
OBJECT *OKSymbol;
OBJECT *ShortSetSymbol;
OBJECT *SetSymbol;
OBJECT *LambdaSymbol;
OBJECT *ShortLambdaSymbol;
OBJECT *BeginSymbol;
OBJECT *CondSymbol;
OBJECT *ArrayAccessSymbol;
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
     Obj->Type = MDL_PROCEDURE;
     Obj->uData.MDL_PROCEDURE.Fn = Fn;
     Obj->uData.MDL_PROCEDURE.Lazy = 0;
     return(Obj);
}


static inline OBJECT *
make_compound(OBJECT *Parameters, OBJECT *Body, OBJECT *Env)
{
     OBJECT *Obj = alloc_object();

     Obj->Type = MDL_COMPOUND;
     Obj->uData.MDL_COMPOUND.Parameters = Parameters;
     Obj->uData.MDL_COMPOUND.Body = Body;
     Obj->uData.MDL_COMPOUND.Env = Env;
     return(Obj);
}

static inline b32
is_procedure(OBJECT *Obj)
{
     return(Obj->Type == MDL_PROCEDURE);
}

static inline b32
is_compound(OBJECT *Obj)
{
     return(Obj->Type == MDL_COMPOUND);
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
                    if(is_nil(Vals))return(Nil);
                    if(!is_nil(pair_get_b(Vals)) && is_nil(pair_get_b(Vars)))
                    {
                         return(Vals);
                    }
                    return(pair_get_a(Vals));
               }
               Vars = pair_get_b(Vars);
               Vals = pair_get_b(Vals);
          }
          Env = enclosing_env(Env);
     }
     LOG(ERR_WARN, "Unbound variable: %s", Var->uData.MDL_SYMBOL.Value);
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

     Val->Name = Var;
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
write(FILE *Out, OBJECT *Obj, b32 StripQuotes = 0);

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

#include "dzm_prc.hpp"

static inline void
init_defs(void)
{
     PrintMemUsage = 0;

     False = alloc_object();
     False->Type = MDL_BOOLEAN;
     False->uData.MDL_BOOLEAN.Value = 0;

     True = alloc_object();
     True->Type = MDL_BOOLEAN;
     True->uData.MDL_BOOLEAN.Value = 1;

     Nil = alloc_object();
     Nil->Type = MDL_NIL;

     SymbolTable = Nil;
     QuoteSymbol = make_symbol(UL_"quote");
     DefineSymbol = make_symbol(UL_"define");
     ShortDefineSymbol = make_symbol(UL_"?");
     OKSymbol = make_symbol(UL_"ok");
     SetSymbol = make_symbol(UL_"set!");
     ArrayAccessSymbol = make_symbol(UL_"access");
     ShortSetSymbol = make_symbol(UL_"!");
     IfSymbol = make_symbol(UL_"if");
     LambdaSymbol = make_symbol(UL_"lambda");
     ShortLambdaSymbol = make_symbol(UL_"&");
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
          return(Pair->uData.MDL_PAIR.A);
     else
          return(Nil);
}

static inline OBJECT *
pair_get_b(OBJECT *Pair)
{
     return(Pair->uData.MDL_PAIR.B);
}

static inline OBJECT *
pair_nth(OBJECT *Pair, s64 n)
{
     if(Pair == Nil)
     {
          LOG(ERR_WARN, "Can't access elements of an empty list!");
          return(Nil);
     }
     if(n < 0)
     {
          LOG(ERR_WARN, "Can't access negative element of a list!");
          return(Nil);
     }
     OBJECT *Obj = Pair;
     while(n-- != 0)
     {
          if(!is_pair(Obj))
          {
               LOG(ERR_WARN, "Can't access element out of list's bounds!");
               return(Nil);
          }

          Obj = pair_get_b(Obj);
     }
     return(pair_get_a(Obj));
}

static inline void
pair_set_a(OBJECT *Pair, OBJECT *A)
{
     Pair->uData.MDL_PAIR.A = A;
}

static inline void
pair_set_b(OBJECT *Pair, OBJECT *B)
{
     Pair->uData.MDL_PAIR.B = B;
}

static inline OBJECT *
make_symbol(u8 *Value)
{
     OBJECT *Obj = alloc_object();
     OBJECT *Elem = SymbolTable;

     while(Elem != 0 && !is_nil(Elem))
     {
          if (!strcmp((char *)pair_get_a(Elem)->uData.MDL_SYMBOL.Value, (char *)Value))
          {
               return(pair_get_a(Elem));
          }
          Elem = pair_get_b(Elem);
     }
     Obj->Type = MDL_SYMBOL;
     Obj->uData.MDL_SYMBOL.Value = (u8 *)malloc(strlen((char *)Value) + 1);//push_size(GlobalArena, strlen((char *)Value) + 1, default_arena_params());
     zassert(Obj->uData.MDL_SYMBOL.Value != 0);

     string_copy(Obj->uData.MDL_SYMBOL.Value, Value);

     SymbolTable = make_pair(Obj, SymbolTable);

     return(Obj);
}

#define DZM_MDL_H
#endif
