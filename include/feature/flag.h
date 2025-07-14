#ifndef FLAG_H
#define FLAG_H

#ifdef ENABLE_LEA_LOG
#pragma message "ENABLE_LEA_LOG is [ENABLED] Disable it before deployment!"
#endif

#ifdef ENABLE_LEA_FMT
#pragma message "ENABLE_LEA_FMT is [ENABLED] Disable it before deployment!"
#endif

#ifdef DISABLE_BUMP_ALLOCATOR
#pragma message "DISABLE_BUMP_ALLOCATOR is [DISABLED]"
#endif

#endif // FLAG_H
