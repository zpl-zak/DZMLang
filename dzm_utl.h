// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_UTL_H)

void
string_copy(u8 *Dest, u8 *Source)
{
    u8 *Dst = Dest;
    u8 *Src = Source;
    while((*(Dst++) = *(Src++)) != 0);
}

size_t
string_len(u8 *String)
{
    size_t Result = 0;
    u8 *Str = String;
    while(*(Str++) != 0)
    {
        Result++;
    }
    return(Result);
}

size_t
string_len1(u8 *Str0)
{
    u8 *Str1 = Str0;
    while(*(Str1++) != 0);
    return(Str1 - Str0);
}

b32
string_compare(u8 *Str0, u8 *Str1)
{
    u8 *S0 = Str0;
    u8 *S1 = Str1;
    
    while(*(S0++) != 0)
    {
        if(*S0 != *(S1++))
        {
            return(1);
        }
    }
    return(0);
}

#define DZM_UTL_H
#endif