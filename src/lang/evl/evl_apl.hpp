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
apply_operator(OBJECT *Exp)
{
    return(pair_get_a(Exp));
}

static inline OBJECT *
prepare_apply_operands(OBJECT *Args)
{
     
     if(!is_pair(pair_get_a(Args)))
     {
          return(make_pair(pair_get_a(Args), Nil));
     }
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
