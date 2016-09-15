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

#if !defined(DZM_LOG_H)

enum ERR_TYPE
{
    ERR_INFO,
    ERR_WARN,
    ERR_FATAL,
};

u8 LogBarrier;
b32 IsVerbose;

FILE *LogOutput;
char ErrPrefix[3][6] = {"INFO", "WARN", "FATAL"};
char *LogBuffer = 0;
b32 PrintOk = 0;

#define LOG(t, f,...) sprintf(LogBuffer, f, ## __VA_ARGS__); push_log(LogBuffer, t)

static inline void
push_log(const char *String, u8 ErrType)
{
    if(ErrType > ERR_FATAL)
    {
        push_log("Can't push log with unknown type!", ERR_WARN);
        return;
    }
    
    mi Prefix = (ErrType >= (char)LogBarrier) ? 2 : ErrType;
    
    fprintf(LogOutput, "%s: %s\n", ErrPrefix[Prefix], String);
    fflush(LogOutput);
    
    if(IsVerbose)
    {
        fprintf(stderr, "%s: %s\n", ErrPrefix[Prefix], String);
    }
    
    if(ErrType >= LogBarrier)
    {
        TRAP();
    }
}

static inline void
error_reporting(u8 ErrType)
{
    if(ErrType == ERR_INFO)
    {
        push_log("Do you really want to consider INFO logs as fatal??", ERR_WARN);
    }
    
    LogBarrier = ErrType;
}

static inline void
set_log_output(FILE *Out)
{
    LogOutput = (Out == 0) ? stdout : Out;
}

static inline void
set_log_verbose(b32 Verbose)
{
    IsVerbose = Verbose;
}

static inline void
init_logging(void)
{
    LogBarrier = ERR_FATAL;
    LogBuffer = (char *)StringArena.Base;
    LogOutput = stderr;
    IsVerbose = 0;
}

#define DZM_LOG_H
#endif