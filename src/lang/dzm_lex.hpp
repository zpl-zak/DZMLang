// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#if !defined(DZM_LEX_H)

static inline b32
is_delimiter(s32 C)
{
    return(isspace(C) || C == EOF ||
           C == '('   || C == ')' ||
           C == '"'   || C == ';');
}

static inline b32
is_initial(s32 C)
{
    return(isalpha(C) || C == '*' || C == '/' || C == '>' ||
           C == '<'   || C == '=' || C == '?' || C == '!' ||
           C == '_'   || C == '%');
}

static inline s32
peek(FILE *In)
{
    s32 C = getc(In);
    ungetc(C, In);
    return(C);
}

static inline u8 *
trim_string(u8 *String)
{
     u8 *Char = String;
     u32 Size = strlen((char *)String);

     while(isspace(Char[Size-1])) Char[--Size] = 0;
     while(*Char && isspace(*Char)) ++Char, --Size;

     memmove(String, Char, Size+1);
     return(String);
}

static inline void
eat_whitespace(FILE *In)
{
    s32 C;
    
    while((C = getc(In)) != EOF)
    {
        if(isspace(C))
        {
            continue;
        }
        else if(C == ';')
        {
            while(((C = getc(In)) != EOF) && (C != '\n'));
            continue;
        }
        
        ungetc(C, In);
        break;
    }
}

static inline void
eat_expected_string(FILE *In, const char *Str)
{
    s32 C;
    
    while(*Str != '\0')
    {
        C = getc(In);
        if(C != *Str)
        {
            fprintf(stderr, "Unexpected char '%c'", C);
            InvalidCodePath;
        }
        Str++;
    }
}

static inline void
peek_expected_delimiter(FILE *In)
{
    if(!is_delimiter(peek(In)))
    {
        push_log("Char not followed by delimiter", ERR_WARN);
        InvalidCodePath;
    }
}

static inline OBJECT *
read_character(FILE *In)
{
    s32 C = getc(In);
    
    switch(C)
    {
        case EOF:
        {
            push_log("Incomplete char literal", ERR_WARN);
            InvalidCodePath;
        }break;

    case 't':
    {
         if(peek(In) == 'a')
         {
              eat_expected_string(In, "ab");
              peek_expected_delimiter(In);
              return(make_character('\t'));
         }
    }break;
        case 's':
        {
            if(peek(In) == 'p')
            {
                eat_expected_string(In, "pace");
                peek_expected_delimiter(In);
                return(make_character(' '));
            }
        }break;
        
        case 'n':
        {
            if(peek(In) == 'e')
            {
                eat_expected_string(In, "ewline");
                peek_expected_delimiter(In);
                return(make_character('\n'));
            }
        }break;
    }
    peek_expected_delimiter(In);
    return(make_character(C));
}

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
    if(C == ')')
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
        if(C != ')')
        {
            push_log("Pair does not end with ')'", ERR_WARN);
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
        real32 Sign = 1.0f;
        real64 Num = 0.0f;
        real64 Realcnt = 1.0f;
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
                Realcnt *= 10.0f;
            }
            Num = (Num * 10.0f) + (C - '0');
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

#define DZM_LEX_H
#endif
