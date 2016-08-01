// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

static inline b32
is_assignment(OBJECT *Exp)
{
    return(is_tagged_list(Exp, SetSymbol));
}

static inline b32
is_definition(OBJECT *Exp)
{
    return(is_tagged_list(Exp, DefineSymbol));
}

static inline OBJECT *
assignment_variable(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}

static inline OBJECT *
assignment_value(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(pair_get_b(Exp))));
}

static inline OBJECT *
definition_variable(OBJECT *Exp)
{
    if(is_symbol(pair_get_a(pair_get_b(Exp))))
    {
        return(pair_get_a(pair_get_b(Exp)));
    }
    else
    {
        return(pair_get_a(pair_get_a(pair_get_b(Exp))));
    }
}

static inline OBJECT *
make_lambda(OBJECT *Parameters, OBJECT *Body);

static inline OBJECT *
definition_value(OBJECT *Exp)
{
    if(is_symbol(pair_get_a(pair_get_b(Exp))))
    {
        return(pair_get_a(pair_get_b(pair_get_b(Exp))));
    }
    else
    {
        return(make_lambda(pair_get_b(pair_get_a(pair_get_b(Exp))), 
                           pair_get_b(pair_get_b(Exp))));
    }
}
