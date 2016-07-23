// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

static inline OBJECT *
make_if(OBJECT *Predicate, OBJECT *Consequent, OBJECT *Alternative)
{
    return(make_pair(IfSymbol,
                     make_pair(Predicate,
                               make_pair(Consequent,
                                         make_pair(Alternative, Nil)))));
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