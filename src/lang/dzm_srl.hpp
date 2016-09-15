// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#ifndef DZMLANG_DZM_SRL_HPP
#define DZMLANG_DZM_SRL_HPP

static inline char *
serialize(char *Out, OBJECT *Obj)
{
    if(is_nil(Obj))
    {
        memcpy((char *)Out, (char *)&Obj, sizeof(OBJECT));
        Out += sizeof(OBJECT);
        return(Out);
    }

    memcpy((char *)Out, (char *)&Obj->Type, sizeof(u8));
    Out += sizeof(u8);

    if(is_pair(Obj))
    {
        Out = serialize(Out, pair_get_a(Obj));
        Out = serialize(Out, pair_get_b(Obj));
    }
    else if(!is_string(Obj) && !is_symbol(Obj))
    {
        //HACK(zaklaus): Calculate size from offsets instead!
        size_t DataSize = sizeof(OBJECT) - ((char *)&Obj->uData - (char *)Obj);
        memcpy((char *)Out, (char *)&Obj->uData, DataSize);
        Out += DataSize;
    }
    else
    {
        memcpy((char *)Out, (char *)Obj->uData.STRING.Value, strlen((char *)Obj->uData.STRING.Value)+1);
        Out += strlen((char *)Obj->uData.STRING.Value)+1;
    }
    return(Out);
}

static inline char *
deserialize(OBJECT *Out, char *Data)
{
    memcpy((char *)&Out->Type, (char *)Data, sizeof(u8));
    Data += sizeof(u8);

    if(is_pair(Out))
    {
        OBJECT *A = alloc_object();
        OBJECT *B = alloc_object();
        Data = deserialize(A, Data);
        Data = deserialize(B, Data);

        pair_set_a(Out, A);
        pair_set_b(Out, B);
    }
    else if(!is_string(Out) && !is_symbol(Out))
    {
        size_t DataSize = sizeof(OBJECT) - ((char *) &Out->uData - (char *) Out);
        memcpy((char *) &Out->uData, (char *) Data, DataSize);
        Data += DataSize;
    }
    else
    {
        size_t StrLen = strlen(Data)+1;
        char *Str = (char *)malloc(StrLen);
        memcpy((char *)Str, (char *)Data, StrLen);
        Data += StrLen;
        Out->uData.STRING.Value = (u8 *)Str;
    }
    return(Data);
}


#endif //DZMLANG_DZM_SRL_HPP
