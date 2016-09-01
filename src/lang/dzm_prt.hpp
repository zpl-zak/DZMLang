// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

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
     case FIXNUM:
     {
          fprintf(Out, "%" PRId64, Obj->uData.FIXNUM.Value);
     }break;

     case REALNUM:
     {
          fprintf(Out, "%1lf", Obj->uData.REALNUM.Value);
     }break;

     case BOOLEAN:
     {
          fprintf(Out, "#%c", is_false(Obj) ? 'f' : 't');
     }break;

     case CHARACTER:
     {
          u8 C = Obj->uData.CHARACTER.Value;
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

     case STRING:
     {
          u8 *Str = Obj->uData.STRING.Value;
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

     case NIL:
     {
          fprintf(Out, "()");
     }break;

     case PAIR:
     {
          fprintf(Out, "(");
          write_pair(Out, Obj);
          fprintf(Out, ")");
     }break;

     case SYMBOL:
     {
          if(Obj == VarSymbol)break;

          if(!(Obj == OKSymbol))
          {
               fprintf(Out, "%s", Obj->uData.SYMBOL.Value);
          }
     }break;

     case COMPOUND:
     {
          fprintf(Out, "#<BUILTIN>");
     }break;

     case PROCEDURE:
     {
          fprintf(Out, "#<PROCEDURE>");
          //write_pair(Out, *(Obj->uData.PROCEDURE.Fn));
     }break;

     case INPUT:
     {
          fprintf(Out, "#<INPUT>");
     }break;

     case OUTPUT:
     {
          fprintf(Out, "#<OUTPUT>");
     }break;

     case SOCKET:
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
          LOG(ERR_INFO, "Free: %d, Total: %d", (int)get_arena_size_remaining(GlobalArena, default_arena_params()), (int)GlobalArena->Size);
     }
}

#define DZM_PRT_H
#endif
