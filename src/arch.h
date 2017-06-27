#ifndef ARCH_H
#define ARCH_H

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) ||             \
    defined(_M_AMD64)
#define BF_JIT_X64
#elif defined(__arm__) || defined(__arm) || defined(__ARM__) || defined(__ARM)
#define BF_JIT_ARM
#endif

#endif // ARCH_H
