// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

static inline OBJECT *
make_if(OBJECT *Predicate, OBJECT *Consequent, OBJECT *Alternative)
{
    OBJECT *r3 = MAKE1(PAIR, Alternative, Nil);
    OBJECT *r2 = MAKE1(PAIR, Consequent, r3);
    OBJECT *r1 = MAKE1(PAIR, Predicate, r2);
    OBJECT *r0 = MAKE1(PAIR, IfSymbol, r1);
    
    return(r0);
}

static inline b32
is_if(OBJECT *Exp)
{
    return(is_tagged_list(Exp, IfSymbol));
}

static inline OBJECT *
if_predicate (OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}

static inline OBJECT *
if_consequent(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(pair_get_b(Exp))));
}

static inline OBJECT *
if_alternative(OBJECT *Exp)
{
    if(is_nil(pair_get_b(pair_get_b(pair_get_b(Exp)))))
    {
        return(Nil);
    }
    else
    {
        return(pair_get_a(pair_get_b(pair_get_b(pair_get_b(Exp)))));
    }
}

static inline b32
is_and(OBJECT *Exp)
{
    return(is_tagged_list(Exp, AndSymbol));
}

static inline OBJECT *
and_tests(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}

static inline b32
is_or(OBJECT *Exp)
{
    return(is_tagged_list(Exp, OrSymbol));
}

static inline OBJECT *
or_tests(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}