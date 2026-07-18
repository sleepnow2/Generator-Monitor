#ifndef EMBEDDED_GENSET_CONTROLLER_WEB_FILE_SERVER_H
#define EMBEDDED_GENSET_CONTROLLER_WEB_FILE_SERVER_H
#include "main.h"

#define FILE_MANAGER_PORT 8080
#define FILE_SYSTEM FFat

namespace WebFileServer {
    extern ESPFMfGK filemgr;
    extern TaskHandle_t filemgrHandle;
    
    /*
    Mounts the partition as a FFat file system. 
    Formats the disk if there is difficulty in finding the partition.
    */
    bool init(bool formatOnFailiure);
    bool begin();
    /*
	reads the configuration file mentoned in the path, and places the contents into the provided doc argument.
	*/
    bool readConfig(JsonDocument &doc, const char* path);
};

#endif //EMBEDDED_GENSET_CONTROLLER_WEB_FILE_SERVER_H