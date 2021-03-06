#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "common/types.h"

int MountFS(void *pClient, void *pCmd, char **mount_path);
int UmountFS(void *pClient, void *pCmd, const char *mountPath);

int LoadFileToMem(const char *filepath, u8 **inbuffer, u32 *size);

//! todo: C++ class
int CreateSubfolder(const char * fullpath);
int CheckFile(const char * filepath);

#ifdef __cplusplus
}
#endif