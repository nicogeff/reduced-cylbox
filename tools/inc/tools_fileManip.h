#ifndef TOOLS_FILEMANIP_H
#define TOOLS_FILEMANIP_H

#ifdef __cplusplus
extern "C" {
#endif

T_tools_boolean tools_isFileExist(const char * filename);    
    
T_tools_status tools_readFile(const char* filename, uint8_t ** byte, size_t * length);

T_tools_status tools_writeFile(const char* filename, uint8_t * byte, size_t * length);

T_tools_status tools_copyFile(const char* srcFilename, const char* dstFilename);


T_tools_status tools_getWorkingDirectory(char ** dir);

T_tools_status tools_buildFilePath(const char * firstFolderPath, const char * secondFolderPath, const char * filename, char ** filePath);

T_tools_status tools_buildFolderPath(const char * firstFolderPath, const char * secondFolderPath, char ** folderPath);
    
#ifdef __cplusplus
}
#endif

#endif /* TOOLS_FILEMANIP_H */
