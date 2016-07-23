// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

static inline OBJECT *
eval(OBJECT *Exp, OBJECT *Env);

static inline OBJECT *
list_of_values(OBJECT *Exps, OBJECT *Env)
{
    if(is_no_operands(Exps))
    {
        return(Nil);
    }
    else
    {
        return(make_pair(eval(first_operand(Exps), Env),
                         list_of_values(rest_operands(Exps), Env)));
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