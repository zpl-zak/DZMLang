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

#if !defined(DZM_LEX_H)

OBJECT *(*read_pair_fn)(FILE *In) = 0;
OBJECT *(*read_fn)(FILE *In) = 0;

#define SET_SYNTAX(x) read_fn = x;
#define SET_SYNTAX_PAIR(x) read_pair_fn = x;

static inline b32
is_delimiter(s32 C)
{
    return(isspace(C) || C == EOF ||
           C == '(' || C == ')' || C == '[' || C == ']' ||
           C == '"' || C == ';' || C == '\'');
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
     if(String == 0 || *String == 0) return(String);
     u8 *Char = String;
     size_t Size = strlen((char *)String);
     if(Size == 0) return(String);

     while(isspace(Char[Size-1])) Char[--Size] = 0;
     while(*Char && isspace(*Char)) ++Char, --Size;

     memmove(String, Char, Size+1);
     return(String);
}

static inline void
eat_whitespace(FILE *In, b32 EndOnWhitespace = 0)
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
        if(EndOnWhitespace)
        {
             ungetc(C, In);
        }
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
    case 'v':
    {
         if(peek(In) == 'a')
         {
              eat_expected_string(In, "ar");
              return(True);
         }
    }
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
    return (make_character((u8) C));
}

#include "lex/lex_scm.hpp"

#define DZM_LEX_H
#endif
