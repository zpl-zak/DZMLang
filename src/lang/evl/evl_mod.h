// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

static inline OBJECT *
make_begin(OBJECT *Exp)
{
    return(make_pair(BeginSymbol, Exp));
}

static inline b32
is_begin(OBJECT *Exp)
{
    return(is_tagged_list(Exp, BeginSymbol));
}

static inline OBJECT *
begin_actions(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}

static inline b32
is_last_exp(OBJECT *Seq)
{
    return(is_nil(pair_get_b(Seq)));
}

static inline OBJECT *
first_exp(OBJECT *Seq)
{
    return(pair_get_a(Seq));
}

static inline OBJECT *
rest_exps(OBJECT *Seq)
{
    return(pair_get_b(Seq));
}

static inline b32
is_cond(OBJECT *Exp)
{
    return(is_tagged_list(Exp, CondSymbol));
}

static inline OBJECT *
cond_clauses(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}

static inline OBJECT *
cond_predicate(OBJECT *Clause)
{
    return(pair_get_a(Clause));
}

static inline OBJECT *
cond_actions(OBJECT *Clause)
{
    return(pair_get_b(Clause));
}

static inline b32
is_cond_else_clause(OBJECT *Clause)
{
    return(cond_predicate(Clause) == ElseSymbol);
}

static inline OBJECT *
sequence_to_exp(OBJECT *Seq)
{
    if(is_nil(Seq))
    {
        return(Seq);
    }
    else if(is_last_exp(Seq))
    {
        return(first_exp(Seq));
    }
    else
    {
        return(make_begin(Seq));
    }
}

static inline OBJECT *
expand_clauses(OBJECT *Clauses)
{
    OBJECT *First;
    OBJECT *Rest;
    
    if(is_nil(Clauses))
    {
        return(False);
    }
    else
    {
        First = pair_get_a(Clauses);
        Rest = pair_get_b(Clauses);
        if(is_cond_else_clause(First))
        {
            if(is_nil(Rest))
            {
                return(sequence_to_exp(cond_actions(First)));
            }
            else
            {
                LOG(ERR_WARN, "Else clause isn't last cond->if");
                InvalidCodePath;
            }
        }
        else
        {
            return(make_if(cond_predicate(First),
                           sequence_to_exp(cond_actions(First)),
                           expand_clauses(Rest)));
        }
    }
    Unreachable(Nil);
}

static inline OBJECT *
cond_to_if(OBJECT *Exp)
{
    return(expand_clauses(cond_clauses(Exp)));
}