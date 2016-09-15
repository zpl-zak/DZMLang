/* 
  Copyright 2016 Dominik Madarasz

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#if !defined(DZM_EVL_H)

#include "evl/evl_lit.hpp"
#include "evl/evl_ops.hpp"
#include "evl/evl_cond.hpp"
#include "evl/evl_app.hpp"
#include "evl/evl_mod.hpp"
#include "evl/evl_lbd.hpp"
#include "evl/evl_apl.hpp"
#include "evl/evl_evl.hpp"


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
    else if(is_tagged_list(Exp, LetrecSymbol))
    {
        Exp = let_to_application(Exp);
        Exp->uData.MDL_PROCEDURE.Lazy = 1;
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

        if(is_procedure(Procedure) &&
           Procedure->uData.MDL_PROCEDURE.Fn == apply_proc)
        {
            Procedure = apply_operator(Arguments);
            Arguments = apply_operands(Arguments);
        }
        if(is_procedure(Procedure) &&
           Procedure->uData.MDL_PROCEDURE.Fn == eval_proc)
        {
            Exp = eval_expression(Arguments);
            Env = eval_env(Arguments);
            goto tailcall;
        }
        if(is_procedure(Procedure))
        {
            OBJECT *Res = (Procedure->uData.MDL_PROCEDURE.Fn)(Arguments);
            return(Res);
        }
        else if(is_compound(Procedure))
        {
            Env = extend_env(Procedure->uData.MDL_COMPOUND.Parameters,
                             Arguments,
                             Procedure->uData.MDL_COMPOUND.Env);
            Exp = make_begin(Procedure->uData.MDL_COMPOUND.Body);
            goto tailcall;
        }
        else
        {
             //LOG(ERR_WARN, "Unknown procedure type: %s", Exp->uData.SYMBOL.Value);
             //InvalidCodePath;
             Exp = pair_get_a(Exp);
             goto tailcall;
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
