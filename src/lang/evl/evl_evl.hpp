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

static inline OBJECT *
eval(OBJECT *Exp, OBJECT *Env);

static inline b32
is_variadic(OBJECT *Exp)
{
    return(is_tagged_list(Exp, VarSymbol));
}

static inline OBJECT *
list_of_values(OBJECT *Exps, OBJECT *Env, b32 ShouldEval)
{
    if(is_no_operands(Exps))
    {
        return(Nil);
    }

	OBJECT *Arg = first_operand(Exps);
	if(1)//ShouldEval || is_quoted(Arg) || is_variable(Arg) || is_procedure(Arg))
	{
		Arg = eval(Arg, Env);

	}
	return(make_pair(Arg,
	                 list_of_values(rest_operands(Exps), Env, ShouldEval)));
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
