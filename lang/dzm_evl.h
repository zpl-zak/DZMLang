// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_EVL_H)

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

static inline OBJECT *
make_lambda(OBJECT *Parameters, OBJECT *Body)
{
    return(make_pair(LambdaSymbol,
                     make_pair(Parameters, Body)));
}

static inline b32
is_lambda(OBJECT *Exp)
{
    return(is_tagged_list(Exp, LambdaSymbol));
}

static inline OBJECT *
lambda_parameters(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}

static inline OBJECT *
lambda_body(OBJECT *Exp)
{
    return(pair_get_b(pair_get_b(Exp)));
}

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
                fprintf(stderr, "Else clause isn't last cond->if\n");
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

static inline OBJECT *
make_application(OBJECT *Operator, OBJECT *Operands)
{
    return(make_pair(Operator, Operands));
}

static inline b32
is_application(OBJECT *Exp)
{
    return(is_pair(Exp));
}

static inline OBJECT *
operator(OBJECT *Exp)
{
    return(pair_get_a(Exp));
}

static inline OBJECT *
operands(OBJECT *Exp)
{
    return(pair_get_b(Exp));
}

static inline b32
is_no_operands(OBJECT *Ops)
{
    return(is_nil(Ops));
}

static inline OBJECT *
first_operand(OBJECT *Ops)
{
    return(pair_get_a(Ops));
}

static inline OBJECT *
rest_operands(OBJECT *Ops)
{
    return(pair_get_b(Ops));
}

static inline b32
is_let(OBJECT *Exp)
{
    return(is_tagged_list(Exp, LetSymbol));
}

static inline OBJECT *
let_bindings(OBJECT *Exp)
{
    return(pair_get_a(pair_get_b(Exp)));
}

static inline OBJECT *
let_body(OBJECT *Exp)
{
    return(pair_get_b(pair_get_b(Exp)));
}

static inline OBJECT *
binding_parameter(OBJECT *Binding)
{
    return(pair_get_a(Binding));
}

static inline OBJECT *
binding_argument(OBJECT *Binding)
{
    return(pair_get_a(pair_get_b(Binding)));
}

static inline OBJECT *
bindings_parameters(OBJECT *Bindings)
{
    return(is_nil(Bindings) ?
           Nil :
           make_pair(binding_parameter(pair_get_a(Bindings)),
                     bindings_parameters(pair_get_b(Bindings))));
}

static inline OBJECT *
bindings_arguments(OBJECT *Bindings)
{
    return(is_nil(Bindings) ?
           Nil :
           make_pair(binding_argument(pair_get_a(Bindings)),
                     bindings_arguments(pair_get_b(Bindings))));
}

static inline OBJECT *
let_parameters(OBJECT *Exp)
{
    return(bindings_parameters(let_bindings(Exp)));
}

static inline OBJECT *
let_arguments(OBJECT *Exp)
{
    return(bindings_arguments(let_bindings(Exp)));
}

static inline OBJECT *
let_to_application(OBJECT *Exp)
{
    return(make_application(
        make_lambda(let_parameters(Exp),
                    let_body(Exp)),
        let_arguments(Exp)));
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
        return(make_pair(pair_get_a(Args),
                         prepare_apply_operands(pair_get_b(Args))));
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

static inline OBJECT *
eval(OBJECT *Exp, OBJECT *Env)
{
    OBJECT *Procedure;
    OBJECT *Arguments;
    OBJECT *Result;
    
tailcall:
    if(is_self_evaluating(Exp))
    {
        return(Exp);
    }
    else if(is_variable(Exp))
    {
        return(lookup_variable_value(Exp, Env));
    }
    else if(is_quoted(Exp))
    {
        OBJECT *R = text_of_quotation(Exp);
        //Exp->Mark = 1;
        //R->Mark = 1;
        return(R);
    }
    else if(is_assignment(Exp))
    {
        return(eval_assignment(Exp, Env));
    }
    else if(is_definition(Exp))
    {
        return(eval_definition(Exp, Env));
    }
    else if(is_if(Exp))
    {
        Exp = is_true(eval(if_predicate(Exp), Env)) ?
            if_consequent(Exp) : if_alternative(Exp);
        goto tailcall;
    }
    else if(is_lambda(Exp))
    {
        return(make_compound(lambda_parameters(Exp),
                             lambda_body(Exp),
                             Env));
    }
    else if(is_begin(Exp))
    {
        Exp = begin_actions(Exp);
        while(!is_last_exp(Exp))
        {
            eval(first_exp(Exp), Env);
            Exp = rest_exps(Exp);
        }
        Exp = first_exp(Exp);
        goto tailcall;
    }
    else if(is_cond(Exp))
    {
        Exp = cond_to_if(Exp);
        goto tailcall;
    }
    else if(is_let(Exp))
    {
        Exp = let_to_application(Exp);
        goto tailcall;
    }
    else if(is_and(Exp))
    {
        Exp = and_tests(Exp);
        if(is_nil(Exp))
        {
            return(True);
        }
        while(!is_last_exp(Exp))
        {
            Result = eval(first_exp(Exp), Env);
            if(is_false(Result))
            {
                return Result;
            }
            Exp = rest_exps(Exp);
        }
        Exp = first_exp(Exp);
        goto tailcall;
    }
    else if(is_or(Exp))
    {
        Exp = or_tests(Exp);
        if(is_nil(Exp))
        {
            return(True);
        }
        while(!is_last_exp(Exp))
        {
            Result = eval(first_exp(Exp), Env);
            if(is_true(Result))
            {
                return Result;
            }
            Exp = rest_exps(Exp);
        }
        Exp = first_exp(Exp);
        goto tailcall;
    }
    else if(is_application(Exp))
    {
        Procedure = eval(operator(Exp), Env);
        Arguments = list_of_values(operands(Exp), Env);
        //Exp->Mark = 1;

        if(is_procedure(Procedure) &&
           Procedure->uData.PROCEDURE.Fn == apply_proc)
        {
            Procedure = apply_operator(Arguments);
            Arguments = apply_operands(Arguments);
        }
        
        if(is_procedure(Procedure) &&
           Procedure->uData.PROCEDURE.Fn == eval_proc)
        {
            Exp = eval_expression(Arguments);
            Env = eval_env(Arguments);
            goto tailcall;
        }
        
        if(is_procedure(Procedure))
        {
            OBJECT *Res = (Procedure->uData.PROCEDURE.Fn)(Arguments);
            
            /*if(is_self_evaluating(Res))
            {
                Res->Mark = 1;
            }
            else if(is_pair(Res))
            {
                b32 ShouldMark = 0;
                OBJECT *It = Res;
                
                while((It) != Nil)
                {
                    if(is_self_evaluating(pair_get_a(It)))
                    {
                        //(pair_get_a(It))->Mark = 1;
                        ShouldMark = 1;
                    }
                    It = pair_get_b(It);
                }
                
                if(ShouldMark)
                {
                    //Res->Mark = 1;
                }
            }*/
            
            return(Res);
        }
        else if(is_compound(Procedure))
        {
            Env = extend_env(Procedure->uData.COMPOUND.Parameters,
                             Arguments,
                             Procedure->uData.COMPOUND.Env);
            Exp = make_begin(Procedure->uData.COMPOUND.Body);
            goto tailcall;
        }
        else
        {
            fprintf(stderr, "Unknown procedure type\n");
            InvalidCodePath;
        }
    }
    else
    {
        fprintf(stderr, "Unknown operation for expression\n");
        InvalidCodePath;
    }
    
    Unreachable(Nil);
}

#define DZM_EVL_H
#endif