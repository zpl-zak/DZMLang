#include "dzm.h"

#include <stdio.h>
#include <stdlib.h>

enum
{
    Monster_Brute,
    Monster_Slasher,
};

typedef struct MONSTER_
{
    struct MONSTER_ *Next;
    u8 Type;
    MEMORY_ARENA *Props;
} MONSTER;

typedef struct
{
    u32 Fuel;
} PROPS_BRUTE;

static void
monster_attack(MONSTER *MonsterInit)
{
    for(MONSTER *Monster = MonsterInit;
        Monster;
        Monster = Monster->Next)
    {
        switch(Monster->Type)
        {
            case Monster_Brute:
            {
                MEMORY_ARENA *Props = (MEMORY_ARENA *)Monster->Props;
                PROPS_BRUTE *BruteProps = (PROPS_BRUTE *)Props->Base;
                u32 *Fuel = &BruteProps->Fuel;
                
                if(*Fuel > 0)
                {
                    printf("%s\n", "Brute uses chainsaw!");
                    *Fuel -= 1;
                }
                else
                {
                    printf("%s %d\n", "No fuel left! Fuel: ", *Fuel);
                }
            }break;
            
            case Monster_Slasher:
            {
                printf("%s\n", "Slasher throws knives!");
            }break;
            
            InvalidDefaultCase;
        }
    }
}

int
main(void)
{
    MONSTER Brute = { .Next = 0, .Type = Monster_Brute, .Props = 0 };
    MONSTER Slasher = { .Next = 0, .Type = Monster_Slasher, .Props = 0 };
    Brute.Next = &Slasher;
    
    MEMORY_ARENA BruteProps;
    void *MemBase = malloc(sizeof(PROPS_BRUTE));
    initialize_arena(&BruteProps, sizeof(PROPS_BRUTE), MemBase);
    PROPS_BRUTE PropsBrute = { .Fuel=1 };
    
    PROPS_BRUTE *PropsPtr = push_struct(&BruteProps, PROPS_BRUTE, default_arena_params());
    *PropsPtr = PropsBrute;
    
    Brute.Props = &BruteProps;
    
    monster_attack(&Brute);
    monster_attack(&Brute);
    
#if 0
    SCRIPT test = load_script("fib.nieco");
    exec_script(&test);
    cache_script(test, [FILE]);
    
    call_event(&test, "onUpdate");
    
    register_function(&test, &setPlayerHealth, ...);
    
    global_vtable[] <- OnUpdate
    
    
    
#endif
    
    return(0);
}
