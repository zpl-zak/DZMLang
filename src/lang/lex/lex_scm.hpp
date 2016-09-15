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

#ifndef DZMLANG_LEX_SCM_HPP

static inline OBJECT *
read(FILE *In);

static inline OBJECT *
read_pair(FILE *In)
{
    s32 C;
    OBJECT *A;
    OBJECT *B;
    
    eat_whitespace(In);
    
    C = getc(In);
    if(C == ')' || C == ']')
    {
        return(Nil);
    }
    ungetc(C, In);
    
    A = read(In);
    
    eat_whitespace(In);
    
    C = getc(In);
    if(C == '.')
    {
        C = peek(In);
        if(!is_delimiter(C))
        {
            push_log("Dot is not followed by delimiter", ERR_WARN);
            InvalidCodePath;
        }
        B = read(In);
        
        eat_whitespace(In);
        
        C = getc(In);
        if(C != ')' && C != ']')
        {
            push_log("Pair does not end with ')' or ']'", ERR_WARN);
            InvalidCodePath;
        }
        return(make_pair(A,B));
    }
    else
    {
        ungetc(C, In);
        B = read_pair(In);
        return(make_pair(A,B));
    }
}

static inline OBJECT *
read(FILE *In)
{
    s32 C;
    u8 *Buffer = StringArena.Base;
    
    eat_whitespace(In);
    
    C = getc(In);
    
    // NOTE(zaklaus): Literal
    if(C == '#')
    {
        C = getc(In);
        
        switch(C)
        {
            case 't':
            {
                return (True);
            }break;
            
            case 'f':
            {
                return(False);
            }break;
            
            case '\\':
            {
                return(read_character(In));
            }break;
            
            InvalidDefaultCase;
        }
    }
    // NOTE(zaklaus): FIXNUM | REALNUM
    else if(isdigit(C) || (C == '-' && (isdigit(peek(In)))))
    {
        real32 Sign = 1.0;
        real64 Num = 0.0;
        real64 Realcnt = 1.0;
        b32 Real = 0;
        if(C == '-')
        {
            Sign = -1;
        }
        else
        {
            ungetc(C, In);
        }
        
        while(isdigit(C = getc(In)) || C == '.')
        {
            if(C == '.')
            {
                Real=1;
                continue;
            }
            if(Real)
            {
                Realcnt *= 10.0;
            }
            Num = (Num * 10.0) + (C - '0');
        }
        if(Real)
            Num = (real64)Num / (real64)Realcnt;
        Num *= Sign;
        
        if(is_delimiter(C))
        {
            ungetc(C, In);
            if(!Real)
                return(make_fixnum((s64)Num));
            else
                return(make_realnum(Num));
        }
        else
        {
            push_log("Number not followed by delimiter", ERR_WARN);
            InvalidCodePath;
        }
    }
    // NOTE(zaklaus): SYMBOL
    else if(is_initial(C) ||
            ((C == '+' || C == '-') &&
             is_delimiter(peek(In))))
    {
        s32 Idx = 0;
        while(is_initial(C) || isdigit(C) ||
              C == '+' || C == '-')
        {
            Buffer[Idx++] = C;
            C = getc(In);
        }
        if(is_delimiter(C))
        {
            Buffer[Idx] = 0;
            ungetc(C, In);
            return(make_symbol(Buffer));
        }
        else
        {
            LOG(ERR_WARN, "Symbol not followed by delimiter");
            InvalidCodePath;
        }
    }
    // NOTE(zaklaus): VARIADIC
    else if(C == '.')
    {
        return(VarSymbol);
    }
    // NOTE(zaklaus): STRING
    else if(C == '"')
    {
        u32 Idx = 0;
        
        while((C = getc(In)) != '"')
        {
            if(C == '\\')
            {
                C = getc(In);
                if(C == 'n')
                {
                    C = '\n';
                }
            }
            if(C == EOF)
            {
                LOG(ERR_WARN, "Non-terminated string literal\n");
                InvalidCodePath;
            }
            Buffer[Idx++] = C;
        }
        Buffer[Idx] = 0;
        return(make_string(Buffer));
    }
        // NOTE(zaklaus): Char
    else if (C == '`') {
        u32 Idx = 0;

        while ((C = getc(In)) != '`') {
            if (C == '\\') {
                C = getc(In);
                if (C == 'n') {
                    C = '\n';
                }
            }
            if (C == EOF) {
                LOG(ERR_WARN, "Non-terminated character literal\n");
                InvalidCodePath;
            }
            Buffer[Idx++] = C;
        }
        Buffer[Idx] = 0;
        return (make_character(*Buffer));
    }
    // NOTE(zaklaus): PAIR/NIL
    else if(C == '(')
    {
        return(read_pair(In));
    }
    else if(C == '\'')
    {
        //raise(SIGABRT);
        return(make_pair(QuoteSymbol, make_pair(read(In), Nil)));
    }
    else if(C == '$')
    {
         return(make_pair(LambdaSymbol, make_pair(Nil, make_pair(read(In), Nil))));
    }
    else if(C == EOF)
    {
        return(0);
    }
    else
    {
        LOG(ERR_WARN, "Bad Input. Unexpected '%c'", C);
        
        InvalidCodePath;
    }
    LOG(ERR_WARN, "Read illegal state");
    InvalidCodePath;
    
    return(Nil);
}

#define DZMLANG_LEX_SCM_HPP
#endif