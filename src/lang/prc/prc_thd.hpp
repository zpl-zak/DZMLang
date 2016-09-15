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

#ifndef DZMLANG_PRC_THD_HPP
#define DZMLANG_PRC_THD_HPP

void
*exec_unit(void *Ptr)
{
    OBJECT *Obj = (OBJECT *)Ptr;
    eval(Obj, GlobalEnv);
    return(0);
}

def_proc(parallel_exec)
{
    mi ThreadCount = 0;
    TEMP_MEMORY TempThreadPool = begin_temp(&TempArena);

    while(!is_nil(pair_get_a(Args)))
    {
        ++ThreadCount;
#ifdef WIN32
        HANDLE Thread = (HANDLE*)push_size(&TempArena, sizeof(HANDLE), default_arena_params());
		  Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_unit, (LPVOID*)pair_get_a(Args), 0, 0);
#else
        pthread_t *Thread = (pthread_t *)push_size(&TempArena, sizeof(pthread_t), default_arena_params());
        pthread_create(Thread, 0, exec_unit, (void *)pair_get_a(Args));
#endif
        Args = pair_get_b(Args);
    }

    for(mi Idx=0; Idx<ThreadCount; Idx++)
    {
#ifdef WIN32
        HANDLE Thread = (HANDLE*)(TempArena.Base + (sizeof(HANDLE) * Idx));
		 WaitForSingleObject(Thread, INFINITE);
#else
        pthread_t *Thread = (pthread_t *)(TempArena.Base + (sizeof(pthread_t) * Idx));
        pthread_join(*Thread, 0);
#endif
    }

    end_temp(TempThreadPool);
    return(OKSymbol);
}

def_proc(thread)
{
    OBJECT *ThreadList = Nil;
    while(!is_nil(pair_get_a(Args)))
    {
#ifdef WIN32
        HANDLE Thread = (HANDLE*)push_size(&TempArena, sizeof(HANDLE), default_arena_params());
		 Thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)exec_unit, (LPVOID*)pair_get_a(Args), 0, 0);
#else
        pthread_t *Thread = (pthread_t *)push_size(GlobalArena, sizeof(pthread_t), default_arena_params());
        pthread_create(Thread, 0, exec_unit, (void *)pair_get_a(Args));

#endif
        ThreadList = make_pair(make_fixnum((s64)Thread), ThreadList);
        Args = pair_get_b(Args);
    }
    return(ThreadList);
}

def_proc(thread_join)
{
#ifdef WIN32
    HANDLE *Thread = (HANDLE *)(pair_get_a(Args)->uData.MDL_FIXNUM.Value);
	 return(make_fixnum((s64)WaitForSingleObject(Thread, INFINITE)));
#else
    pthread_t *Thread = (pthread_t *)(pair_get_a(Args)->uData.MDL_FIXNUM.Value);
    return(make_fixnum(pthread_join(*Thread, 0)));
#endif
}

/*
def_proc(thread_exit)
{
     int errcode = (is_fixnum(pair_get_a(Args))) ? pair_get_a(Args)->uData.MDL_FIXNUM.Value : 0;
     pthread_exit((void *)errcode);
     return(OKSymbol);
}
*/

static inline void
install_thd_module(OBJECT *Env)
{
    add_procedure("parallel-exec", parallel_exec_proc);
    add_procedure("make-thread", thread_proc);
    add_procedure("thread-join", thread_join_proc);
}

#endif //DZMLANG_PRC_THD_HPP
