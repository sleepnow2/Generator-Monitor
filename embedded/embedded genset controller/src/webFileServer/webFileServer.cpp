#line 1 "webFileServer"
// this files job is to initialize and handle the webserver and file system.  

#include "webFileServer.h"
#define FILE_SYSTEM FFat
#define FILE_SYSTEM_NAME "Flash/FFat"

namespace WebFileServer {
	ESPFMfGK filemgr(FILE_MANAGER_PORT);
	/*
	this initializes the file system so everything else can use it. we have a grad total of 9.9 MB to use.
	*/
	bool init(bool formatOnFailiure = true) {
		LOG_TRACE();
		// initialize the file system so configuration files can be read.
		LOG_DEBUG("Initializing FileSystem");
		if (!FILE_SYSTEM.begin() && formatOnFailiure) {
			LOG_WARN("Failiure to initialize filesystem.");
			LOG_WARN("Reformating file system.");
			FILE_SYSTEM.format();
			delay(500);
			return false;
		}
		LOG_INFO("Success initializing filesystem.");
		return true;
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

	bool begin() {
		using WebFileServer::filemgr;
		LOG_TRACE();
		if (!WiFi.isConnected() && WiFi.getMode() == WIFI_MODE_STA) {
			LOG_ERROR("Internet connction required for file manager to function.");
			return false;
		}
		if (!filemgr.AddFS(FILE_SYSTEM, FILE_SYSTEM_NAME, false)) {
			LOG_ERROR("File System failed to be added to the file manager.");
			return false;
		}
		LOG_INFO(F("File System added to file manager"));

		// See above.
		filemgr.checkFileFlags = WebFileServer::checkFileFlags;

		filemgr.WebPageTitle = "Genset File Manager";
		filemgr.BackgroundColor = "white";
		filemgr.textareaCharset = "accept-charset=\"utf-8\"";

		// If you want authentication
		// filemgr.HttpUsername = "my";
		// filemgr.HttpPassword = "secret";

		// display the file date? change here. does not work well if you never set configTzTime()
		// filemgr.FileDateDisplay = ESPFMfGK::fddNone;

		if (!filemgr.begin()) {
			LOG_ERROR(F("Filemanager: did not start"));
			return false;
		}
		LOG_INFO("Open Filemanager with http://"+WiFi.localIP().toString()+":"+FILE_MANAGER_PORT);

		return true;
	}

	/*
		reads the configuration file mentoned in the path, and places the contents into the provided doc argument.

		
	*/
	bool readConfig(JsonDocument &doc, const char* path) {
		LOG_TRACE();
		File config_file = FILE_SYSTEM.open(path, "r");		
		if (!config_file) {
			return false;
		}
		String config_str = config_file.readString();
		LOG_DEBUG(config_str);
		DeserializationError err = deserializeJson(doc, config_str);
		if (err != DeserializationError::Ok) {
			LOG_ERROR(err.c_str());
			return false;
		}
		return true;
	}

	void handleClient() {
		LOG_TRACE();
		WebFileServer::filemgr.handleClient();
	}
}