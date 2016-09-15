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

static inline b32
is_assignment(OBJECT *Exp)
{
    return(is_tagged_list(Exp, SetSymbol) ||
           is_tagged_list(Exp, ShortSetSymbol));
}

static inline b32
is_definition(OBJECT *Exp)
{
    return(is_tagged_list(Exp, DefineSymbol) ||
           is_tagged_list(Exp, ShortDefineSymbol));
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
