#ifndef _GDEBUG_H_
#define _GDEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

void GDBG_INFO(const int level, const char *format, ...);
void GDBG_ERROR(const char *fn, const int line, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* _GDEBUG_H_ */