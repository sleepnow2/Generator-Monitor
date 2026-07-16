#include "webFileServer.h"
// this files job is to initialize and handle the webserver and file system.  

// Adds the Fat file system to the number of file systems to be maintained by the file manager.
// since there is only one, This should be all.
void addFileSystem(void) {
  if (!filemgr.AddFS(FILE_SYSTEM, "Flash/FFat", false)) {
    while (1) {
      Serial.println(F("Adding file system failed."));
      delay(1000);
    }
  }
  Serial.println(F("File System initialized"));
}

uint32_t checkFileFlags(fs::FS &fs, String filename, uint32_t flags) {
  // Show file/path in Lists 
  // filenames start without "/", pathnames start with "/"
  if (flags & (ESPFMfGK::flagCheckIsFilename | ESPFMfGK::flagCheckIsPathname)) {
    if (flags | ESPFMfGK::flagCheckIsFilename) {
      if (filename.startsWith(".")) {
        // Serial.println(filename + " flagIsNotVisible");
        return ESPFMfGK::flagIsNotVisible;
      }
    }
    /*
       this will catch a pathname like /.test, but *not* /foo/.test
       so you might use .indexOf()
    */
    if (flags | ESPFMfGK::flagCheckIsPathname) {
      if (filename.startsWith("/.")) {
        // Serial.println(filename + " flagIsNotVisible");
        return ESPFMfGK::flagIsNotVisible;
      }
    }
  }
  
  // Checks if target file name is valid for action. This will simply allow everything by returning the queried flag
  if (flags & ESPFMfGK::flagIsValidAction) {
    return flags & (~ESPFMfGK::flagIsValidAction);
  }

  // Checks if target file name is valid for action.
  if (flags & ESPFMfGK::flagIsValidTargetFilename) {
    return flags & (~ESPFMfGK::flagIsValidTargetFilename);
  }

  // Default actions
  uint32_t defaultflags = ESPFMfGK::flagCanDelete | ESPFMfGK::flagCanRename | ESPFMfGK::flagCanGZip |  // ^t
                          ESPFMfGK::flagCanDownload | ESPFMfGK::flagCanUpload | ESPFMfGK::flagCanEdit | // ^t
                          ESPFMfGK::flagAllowPreview;

  return defaultflags;
}

bool setupFilemanager(void) {
  addFileSystem();
  // See above.
  filemgr.checkFileFlags = checkFileFlags;

  filemgr.WebPageTitle = "Genset File Manager";
  filemgr.BackgroundColor = "white";
  filemgr.textareaCharset = "accept-charset=\"utf-8\"";

  // If you want authentication
  // filemgr.HttpUsername = "my";
  // filemgr.HttpPassword = "secret";

  // display the file date? change here. does not work well if you never set configTzTime()
  // filemgr.FileDateDisplay = ESPFMfGK::fddNone;

  if (filemgr.begin()) {
    Serial.print(F("Open Filemanager with http://"));
    Serial.print(WiFi.localIP());
    Serial.print(F(":"));
    Serial.print(FILE_MANAGER_PORT);
    Serial.print(F("/"));
    Serial.println();
    return true;
  } else {
    Serial.print(F("Filemanager: did not start"));
    return false;
  }
}