// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

static inline b32
is_self_evaluating(OBJECT *Exp)
{
    return(is_boolean(Exp)   ||
           is_fixnum(Exp)    ||
           is_realnum(Exp)   ||
           is_character(Exp) ||
           is_string(Exp));
}

static inline b32
is_variable(OBJECT *Exp)
{
    return(is_symbol(Exp));
}

static inline b32
is_tagged_list(OBJECT *Exp, OBJECT *Tag)
{
    OBJECT *A;
    
    if(is_pair(Exp))
    {
        A = pair_get_a(Exp);
        return(is_symbol(A) && (A == Tag));
    }
    return(0);
}

static inline b32
is_quoted(OBJECT *Exp)
{
    return(is_tagged_list(Exp, QuoteSymbol));
}

static inline OBJECT *
text_of_quotation(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}