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

#if !defined(DZM_UTL_H)

void
string_copy(u8 *Dest, u8 *Source)
{
    u8 *Dst = Dest;
    u8 *Src = Source;
    while((*(Dst++) = *(Src++)) != 0);
}

size_t
string_len(u8 *String)
{
    size_t Result = 0;
    u8 *Str = String;
    while(*(Str++) != 0)
    {
        Result++;
    }
    return(Result);
}

size_t
string_len1(u8 *Str0)
{
    u8 *Str1 = Str0;
    while(*(Str1++) != 0);
    return(Str1 - Str0);
}

b32
string_compare(u8 *Str0, u8 *Str1)
{
    u8 *S0 = Str0;
    u8 *S1 = Str1;
    
    while(*(S0++) != 0)
    {
        if(*S0 != *(S1++))
        {
            return(1);
        }
    }
    return(0);
}

#define DZM_UTL_H
#endif