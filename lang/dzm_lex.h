// (c) ZaKlaus 2016; All Rights Reserved;;

#if !defined(DZM_LEX_H)

b32
is_delimiter(s32 C)
{
    return(isspace(C) || C == EOF ||
           C == '('   || C == ')' ||
           C == '"'   || C == ';');
}

b32
is_initial(s32 C)
{
    return(isalpha(C) || C == '*' || C == '/' || C == '>' ||
           C == '<'   || C == '=' || C == '?' || C == '!' ||
           C == '_'   || C == '%');
}

s32
peek(FILE *In)
{
    s32 C = getc(In);
    ungetc(C, In);
    return(C);
}

void
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

void
eat_expected_string(FILE *In, char *Str)
{
    s32 C;
    
    while(*Str != '\0')
    {
        C = getc(In);
        if(C != *Str)
        {
            fprintf(stderr, "Unexpected char '%c'\n", C);
            InvalidCodePath;
        }
        Str++;
    }
}

void
peek_expected_delimiter(FILE *In)
{
    if(!is_delimiter(peek(In)))
    {
        fprintf(stderr, "Char not followed by delimiter\n");
        InvalidCodePath;
    }
}

OBJECT *
read_character(FILE *In)
{
    s32 C = getc(In);
    
    switch(C)
    {
        case EOF:
        {
            fprintf(stderr, "Incomplete char literal\n");
            InvalidCodePath;
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

OBJECT *
read(FILE *In);

OBJECT *
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
            fprintf(stderr, "Dot is not followed by delimiter\n");
            InvalidCodePath;
        }
        B = read(In);
        
        eat_whitespace(In);
        
        C = getc(In);
        if(C != ')')
        {
            fprintf(stderr, "Pair does not end with ')'\n");
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

OBJECT *
read(FILE *In)
{
    s32 C;
    u8 Buffer[MAX_STRING_SIZE]; // TODO(zaklaus): DYNAMIC!!!!
    
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
    // NOTE(zaklaus): FIXNUM
    else if(isdigit(C) || (C == '-' && (isdigit(peek(In)))))
    {
        s16 Sign = 1;
        s32 Num = 0;
        if(C == '-')
        {
            Sign = -1;
        }
        else
        {
            ungetc(C, In);
        }
        
        while(isdigit(C = getc(In)))
        {
            Num = (Num * 10) + (C - '0');
        }
        Num *= Sign;
        if(is_delimiter(C))
        {
            ungetc(C, In);
            return(make_fixnum(Num));
        }
        else
        {
            fprintf(stderr, "Number not followed by delimiter\n");
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
            if(Idx < MAX_STRING_SIZE - 1)
            {
                Buffer[Idx++] = C;
            }
            else
            {
                fprintf(stderr, "Symbol too long. Max. Len is %d\n", MAX_STRING_SIZE);
                InvalidCodePath;
            }
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
            fprintf(stderr, "Symbol not followed by delimiter\n");
            InvalidCodePath;
        }
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
                fprintf(stderr, "Non-terminated string literal\n");
                InvalidCodePath;
            }
            
            // TODO(zaklaus): Make it dynamic...
            if(Idx < MAX_STRING_SIZE - 1)
            {
                Buffer[Idx++] = C;
            }
            else
            {
                fprintf(stderr, "String too long. Max. Len is %d\n", MAX_STRING_SIZE);
                InvalidCodePath;
            }
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
        return(make_pair(QuoteSymbol, make_pair(read(In), Nil)));
    }
    else if(C == EOF)
    {
        return(0);
    }
    else
    {
        fprintf(stderr, "Bad Input. Unexpected '%c'\n", C);
        
        InvalidCodePath;
    }
    fprintf(stderr, "Read illegal state\n");
    InvalidCodePath;
    
    Unreachable(0);
}

#define DZM_LEX_H
#endif