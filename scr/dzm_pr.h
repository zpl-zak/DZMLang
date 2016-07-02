// (c) ZaKlaus 2016; All Rights Reserved;;

#if !defined(DZM_PR_H)

typedef struct
{
    u8 *At;
} TOKENIZER;

typedef struct
{
    u32 Type;
    u8 *Text;
    u32 Size;
} TOKEN;

enum
{
    Token_Unknown,
    
    Token_OpenParen,    
    Token_CloseParen,    
    Token_Colon,    
    Token_Semicolon,    
    Token_Asterisk,    
    Token_OpenBracket,    
    Token_CloseBracket,    
    Token_OpenBrace,    
    Token_CloseBrace,
    Token_Setter,
    Token_Array,
    
    Token_String,    
    Token_Identifier,    
    Token_RValue,

    Token_EndOfStream,
};

inline b32
is_whitespace(u8 c)
{
    return(c == ' ' || c == '\t' || c == '\n' || c == '\0');
}

inline b32
is_alpha(u8 c)
{
    return((c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_' || c == '-'));
}

inline void
eat_whitespace(TOKENIZER *Tokenizer)
{
    u8 *Text = Tokenizer->At;
    while(is_whitespace(*(Text--)));
}

inline TOKEN
get_token(TOKENIZER *Tokenizer)
{
    eat_whitespace(Tokenizer);
    
    TOKEN Token;
    Token.Size = 1;
    Token.Text = Tokenizer->At;
    u8 C = Tokenizer->At[0];
    ++Tokenizer->At;
    
    switch(C)
    {
        case 0:
        {
            Token.Type = Token_EndOfStream;
        }break;
        
        case '(':
        {
            Token.Type = Token_OpenParen;
        }break;
        
        case ')':
        {
            Token.Type = Token_CloseParen;
        }break;
        
        case ':':
        {
            Token.Type = Token_Colon;
        }break;
        
        case ';':
        {
            Token.Type = Token_Semicolon;
        }break;
        
        case '*':
        {
            Token.Type = Token_Asterisk;
        }break;
        
        case '[':
        {
            Token.Type = Token_OpenBracket;
        }break;
        
        case ']':
        {
            Token.Type = Token_CloseBracket;
        }break;
        
        case '{':
        {
            Token.Type = Token_OpenBrace;
        }break;
        
        case '}':
        {
            Token.Type = Token_CloseBrace;
        }break;
        
        case '=':
        {
            Token.Type = Token_Setter;
        }break;
        
        case '"':
        {
            Token.Type = Token_String;
            Token.Text = Tokenizer->At;
            
            while(Tokenizer->At[0] &&
                  Tokenizer->At[0] != '"')
            {
                if((Tokenizer->At[0] == '\\') &&
                   Tokenizer->At[1])
                {
                    ++Tokenizer->At;
                }
                ++Tokenizer->At;
            }
            
            Token.Size = Tokenizer->At - Token.Text;
            
            if(Tokenizer->At[0] == '"')
            {
                ++Tokenizer->At;
            }
        }break;
        
        default:
        {
            if(is_alpha(C))
            {
                Token.Type = Token_Identifier;
                
                while(is_alpha(Tokenizer->At[0]) ||
//                      IsNumber(Tokenizer->At[0]) ||
                      (Tokenizer->At[0] == '_') ||
                      (Tokenizer->At[0] == '-') ||
                        (Tokenizer->At[0] == ':') ||
                        (Tokenizer->At[0] == '<' || Tokenizer->At[0] == '>'))
                {
                    ++Tokenizer->At;
                }
                
                Token.Size = Tokenizer->At - Token.Text;
            }
            else
            {
                Token.Type = Token_Unknown;
            }
        }break;
    }
    
    return(Token);
}

#define DZM_PR_H
#endif