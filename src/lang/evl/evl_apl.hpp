// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

static inline OBJECT *
apply_operator(OBJECT *Exp)
{
    return(pair_get_a(Exp));
}

static inline OBJECT *
prepare_apply_operands(OBJECT *Args)
{
    if(is_nil(pair_get_b(Args)))
    {
        return(pair_get_a(Args));
    }
    else
    {
        OBJECT *r0 = pair_get_a(Args);
        OBJECT *r1 = prepare_apply_operands(pair_get_b(Args));
        return(MAKE1(PAIR, r0, r1));
    }
}

static inline OBJECT *
apply_operands(OBJECT *Args)
{
    return(prepare_apply_operands(pair_get_b(Args)));
}

static inline OBJECT *
eval_expression(OBJECT *Args)
{
    return(pair_get_a(Args));
}

static inline OBJECT *
eval_env(OBJECT *Args)
{
    return(pair_get_a(pair_get_b(Args)));
}
