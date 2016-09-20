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

#if !defined(DZM_PRT_H)

static inline void
write_pair(FILE *Out, OBJECT *Obj)
{
     OBJECT *A = pair_get_a(Obj);
     OBJECT *B = pair_get_b(Obj);

     write(Out, A);
     if(is_pair(B))
     {
          fprintf(Out, " ");
          write_pair(Out, B);
     }
     else if(is_nil(B))
     {
          return;
     }
     else
     {
          fprintf(Out, " . ");
          write(Out, B);
     }
}

static inline void
write(FILE *Out, OBJECT *Obj, b32 StripQuotes)
{
     if(Obj == 0)
     {
          return;
     }
     switch(Obj->Type)
     {
     case MDL_FIXNUM:
     {
          fprintf(Out, "%" PRId64, Obj->uData.MDL_FIXNUM.Value);
     }break;

     case MDL_REALNUM:
     {
          fprintf(Out, "%.17Lg", Obj->uData.MDL_REALNUM.Value);
     }break;

     case MDL_BOOLEAN:
     {
          fprintf(Out, "#%c", is_false(Obj) ? 'f' : 't');
     }break;

     case MDL_CHARACTER:
     {
          u8 C = Obj->uData.MDL_CHARACTER.Value;
          if(!StripQuotes)fprintf(Out, "#\\");
          switch(C)
          {
          case '\n':
          {
               if(!StripQuotes)fprintf(Out, "newline");
               else fprintf(Out, "\n");
          }break;

          case ' ':
          {
               if(!StripQuotes)fprintf(Out, "space");
               else fprintf(Out, " ");
          }break;

          case '\t':
          {
               if(!StripQuotes)fprintf(Out, "tab");
               else fprintf(Out, "\t");
          }break;

          default:
          {
               fprintf(Out, "%c", C);
          }break;
          }
     }break;

     case MDL_STRING:
     {
          u8 *Str = Obj->uData.MDL_STRING.Value;
          if(!StripQuotes)fprintf(Out, "%c", '"');

          while(*Str != 0)
          {
               // TODO(zaklaus): To print or not to print un-escaped specials?
               // CONCL(zaklaus): Since this is just a simple echo machine, we can't print
               // un-escaped chars. Maybe later in more specialized built-in or so...
               switch(*Str)
               {
               case '\n':
               {
                    fprintf(Out, "\n");
               }break;

               case '\t':
               {
                    fprintf(Out, "\t");
               }break;

               case '\\':
               {
                    fprintf(Out, "\\\\");
               }break;

               case '"':
               {
                    fprintf(Out, "\\\"");
               }break;

               default:
               {
                    fprintf(Out, "%c", *Str);
               }break;
               }
               Str++;
          }
          if(!StripQuotes)fprintf(Out, "%c", '"');
     }break;

     case MDL_NIL:
     {
          fprintf(Out, "'()");
     }break;

     case MDL_PAIR:
     {
          fprintf(Out, "(");
          write_pair(Out, Obj);
          fprintf(Out, ")");
     }break;

     case MDL_SYMBOL:
     {
          if(Obj == VarSymbol)break;

          if(!(Obj == OKSymbol) || ((Obj == OKSymbol) && PrintOk))
          {
               fprintf(Out, "%s", Obj->uData.MDL_SYMBOL.Value);
          }
     }break;

     case MDL_COMPOUND:
     {
          OBJECT *Name = (is_nil(Obj->Name)) ? make_symbol(UL_"lambda") : Obj->Name;
          write(Out, make_pair(DefineSymbol,
                               make_pair(
                                       make_pair(Name,
                                                 Obj->uData.MDL_COMPOUND.Parameters),
                                       Obj->uData.MDL_COMPOUND.Body)), StripQuotes);
     }break;

     case MDL_PROCEDURE:
     {
          fprintf(Out, "#<NATIVE>");
     }break;

     case MDL_INPUT:
     {
          fprintf(Out, "#<INPUT>");
     }break;

     case MDL_OUTPUT:
     {
          fprintf(Out, "#<OUTPUT>");
     }break;

     case MDL_SOCKET:
     {
          fprintf(Out, "#<SOCKET>");
     }break;

     default:
     {
          fprintf(stderr, "Unknown value");// for '%#08x'\n", (unsigned int*)&Obj);
          InvalidCodePath;
     }break;
     }

     if(PrintMemUsage)
     {
          //LOG(ERR_INFO, "Free: %d, Total: %d", (int)get_arena_size_remaining(GlobalArena, default_arena_params()), (int)GlobalArena->Size);
     }
}

#define DZM_PRT_H
#endif
