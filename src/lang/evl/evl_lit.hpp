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