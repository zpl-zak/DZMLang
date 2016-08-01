// (c) ZaKlaus 2016; MIT Licensed, see LICENSE;;

#if !defined(DZM_EVL_H)

#include "evl/evl_lit.h"
#include "evl/evl_ops.h"
#include "evl/evl_cond.h"
#include "evl/evl_app.h"
#include "evl/evl_mod.h"
#include "evl/evl_lbd.h"
#include "evl/evl_apl.h"
#include "evl/evl_evl.h"


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
        Procedure = eval(op(Exp), Env);
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
            LOG(ERR_WARN, "Unknown procedure type");
            InvalidCodePath;
        }
    }
    else
    {
        LOG(ERR_WARN, "Unknown operation for expression");
        InvalidCodePath;
    }
    
    return(Nil);
}

#define DZM_EVL_H
#endif