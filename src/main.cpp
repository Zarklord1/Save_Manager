#include <nsysnet/socket.h>
#include <iosuhax.h>
#include <iosuhax_devoptab.h>
#include "Application.h"
#include "dynamic_libs/padscore_functions.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"

#include "main.h"

/* Entry point */
extern "C" int Menu_Main(void){
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitPadScoreFunctionPointers();
	socket_lib_init();
    log_init("192.168.1.50");

    //Otherwise apply the patches!!!
    log_printf("Save_Manager Starting!!!\n\n");

    //!*******************************************************************
    //!                    Initialize heap memory                        *
    //!*******************************************************************
    log_print("Initialize memory management\n");
    memoryInitialize();

    int res = IOSUHAX_Open(NULL);
    if(res < 0)
    {
        //coldboot hax support
        //res = IOSUHAX_Open("/dev/mcp");
        if(res < 0)
        {
            log_printf("IOSUHAX_open failed\n");
        }
    }
    log_printf("It Got Here!\n");
    int fsaFd = IOSUHAX_FSA_Open();
    if(fsaFd < 0)
    {
        log_printf("IOSUHAX_FSA_Open failed\n");
    }
    
    log_printf("Mount SD partition\n");
    mount_fs("sd", fsaFd, "/dev/sdcard01", "/vol/storage_sdcard");
    log_printf("Mount MLC partition\n");
    mount_fs("storage_mlc", fsaFd, NULL, "/vol/storage_mlc01");
    log_printf("Mount USB partition\n");
    mount_fs("storage_usb", fsaFd, NULL, "/vol/storage_usb01");

    //!*******************************************************************
    //!                    Enter main application                        *
    //!*******************************************************************

    log_printf("Start main application\n");
    Application::instance()->exec();
    log_printf("Main application stopped\n");

    unmount_fs("sd");
    unmount_fs("storage_mlc");
    unmount_fs("storage_usb");
    IOSUHAX_FSA_Close(fsaFd);
    IOSUHAX_Close();

    Application::destroyInstance();
    memoryRelease();

    log_print("Leaving the application\n");
    log_deinit();
    
    return EXIT_SUCCESS;
}
