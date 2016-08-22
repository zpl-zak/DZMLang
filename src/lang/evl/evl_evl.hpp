// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

static inline OBJECT *
eval(OBJECT *Exp, OBJECT *Env);

static inline b32
is_variadic(OBJECT *Exp)
{
    return(is_tagged_list(Exp, VarSymbol));
}

static inline OBJECT *
list_of_values(OBJECT *Exps, OBJECT *Env)
{
    if(is_no_operands(Exps))
    {
        return(Nil);
    }
    else
    {
        OBJECT *r0 = eval(first_operand(Exps), Env);
        OBJECT *r1 = list_of_values(rest_operands(Exps), Env);
        return(MAKE1(PAIR, r0, r1));
    }
}

static inline OBJECT *
eval_assignment(OBJECT *Exp, OBJECT *Env)
{
    set_variable_value(assignment_variable(Exp),
                       eval(assignment_value(Exp), Env),
                       Env);
    return(OKSymbol);
}

static inline OBJECT *
eval_definition(OBJECT *Exp, OBJECT *Env)
{
    define_variable(definition_variable(Exp),
                    eval(definition_value(Exp), Env),
                    Env);
    return(OKSymbol);
}