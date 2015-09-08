#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#ifdef _WIN32
#include <windows.h>
#include <Lmcons.h>
#include <io.h>
#include <time.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//https://accounts.google.com/o/oauth2/auth?scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive&redirect_uri=urn:ietf:wg:oauth:2.0:oob&response_type=code&client_id=683279313279-ault0vspek6089g9sjth6gslg80hb2cq.apps.googleusercontent.com
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *access_token_new ;
char *refresh_token_new;
size_t handleResponse(char *ptr, size_t size, size_t nmemb, void *stream)
{
    int i;
    size_t len = size * nmemb;

    for(i = 0; i < len; i++){
        fprintf((FILE *) stream,"%c", ptr[i]);
    }
    return len;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t retcode;
  curl_off_t nread;
 
  retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
 
  return retcode;
}

void get_toks(){
	char *tok;
	char *tok2;
	char str1[] = "\"refresh_token\":\"";
    char *str2;
    char *sep = ":,\"";
	char auth[] = "\"access_token\":\"";
    char *str_acs;
   	FILE *ref_tok = fopen("refresh_tok.txt","r");
	  if(ref_tok){
	  fseek(ref_tok, 0, SEEK_END);
    long fsize = ftell(ref_tok);
        
    fseek(ref_tok, 0, SEEK_SET);
    char *resF = (char *) malloc ( sizeof(char) * fsize);
    memset(resF, '\0', sizeof(char) * fsize);
    fread(resF, sizeof(char), fsize, ref_tok);
    tok = strstr(resF,str1);
	 str2 = strtok(tok,sep);
	 str2 = strtok(NULL,sep);
	 refresh_token_new = str2;
	 printf("\nRefresh Token is : \n\n %s\n",refresh_token_new);
	 fseek(ref_tok, 0, SEEK_SET);
    char *acsF = (char *) malloc ( sizeof(char) * fsize);
    memset(acsF, '\0', sizeof(char) * fsize);
    fread(acsF, sizeof(char), fsize, ref_tok);
	 tok2 = strstr(acsF,auth);
	  str_acs = strtok(tok2,sep);
	 str_acs = strtok(NULL,sep);
      access_token_new = str_acs;
	  }
   fclose(ref_tok);
}

void auth_tok()
{
char url[] = "https://login.live.com/oauth20_token.srf";
char scope[100] = "scope=wl.offline_access,wl.skydrive_update";
char redirect_uri[]= "&redirect_uri=https%3A%2F%2Flogin.live.com%2Foauth20_desktop.srf";
char response_type[]="&response_type=token";
char client_id[1024]="&client_id=00000000441607D2";
char grant_type[1024]="&grant_type=authorization_code";
char code[1024] = "&code=M529bf93e-1235-89e5-81f8-29cf8a4dfb43";
char client_secret[1024] = "&client_secret=UoW-UK0bHvOhwa66yGDtZkl4LHgkfQQN";
CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};//{"https://login.live.com/oauth20_authorize.srf?client_id=00000000441607D2&display=page&locale=en&redirect_uri=http%3A%2F%2Flogin.live.com/oauth20_desktop.srf&response_type=token&scope=wl.skydrive_update&state=redirect_type%3Dauth%26display%3Dpage%26request_ts%3D1441603159895%26response_method%3Durl%26secure_cookie%3Dfalse"};
FILE *auth = fopen("auth_tok.txt","w");
curl = curl_easy_init();
  if(curl) {
	 //strcpy(POST_DATA,url);
	strcat(POST_DATA,scope);
	strcpy(POST_DATA,grant_type);
	strcat(POST_DATA,redirect_uri);
	strcat(POST_DATA,response_type);
	strcat(POST_DATA,client_id);
	strcat(POST_DATA,code);
	strcat(POST_DATA,client_secret);
	printf("POST_DATA is : %s\n",POST_DATA);
    curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, auth);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }
  //return 0;
}

void refresh_tok(){
	printf("Taking refresh token ... ..... ....... \n");
char url[] = "https://login.live.com/oauth20_token.srf";
char client_id[1024]="client_id=00000000441607D2";
char client_secret[1024] = "&client_secret=UoW-UK0bHvOhwa66yGDtZkl4LHgkfQQN";
char grant_type [1024] = "&grant_type=refresh_token";
char redirect_uri[]= "&redirect_uri=https%3A%2F%2Flogin.live.com%2Foauth20_desktop.srf";
char refresh_token[1024] = "&refresh_token=";//MCrrxdH*p5m9lrs6Tqe1NRf8daTfAowHCp4tvXrL0PiootT1TrMFQ*xqLvlBgYLhKK6qnQL29gNIfKVqJ7!WdLwcu1PmMeJonj2AmD!kmq3R32*6IUldLuVvmOM5gs15MksV57fziEpg*7JkNdQTUBQcniQLghMKBvZwniGLxBw0NmhDvehAnwDKNoN!c9yC82jV!oNecP6BJQPLL!1zhIE7cYj1PflF0Rx1bjQ6qmjwuKJKqH*0Uthx7jUAWTRUR4BkKWX24iVrkL2fJgcJbTOiUJgfdawWTN4pBELwNznXPvfu!S7nWmKF6ksEJiEdmJ1hJu4XpxfgOa0HaIPH6tSgBbE!9kQNctxSOxDatlb5x";
strcat(refresh_token,refresh_token_new);
CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};
//Downlaod refresh token to new file.
FILE *refresh = fopen("refresh_tok_new.txt","w");
curl = curl_easy_init();
  if(curl) {
	 
	strcpy(POST_DATA,client_id);
	strcat(POST_DATA,client_secret);
	strcat(POST_DATA,redirect_uri);
	strcat(POST_DATA,refresh_token);
	strcat(POST_DATA,grant_type);
    curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, refresh);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }
  	fseek(refresh, 0, SEEK_END);
    long fsize = ftell(refresh);    
    fseek(refresh, 0, SEEK_SET);
  fclose(refresh);
  if(fsize >300){
	//If new file size is normal(contain refresh token then rename it with refresh_tok.txt file
	  remove("refresh_tok.txt");
  rename("refresh_tok_new.txt","refresh_tok.txt");  
  }

}

void read_fold(){
//char url[] = "https://apis.live.net/v5.0/me/skydrive/folder.4fb721677bca6bf8.4FB721677BCA6BF8!138/files";
char url[] = "https://apis.live.net/v5.0/folder.4fb721677bca6bf8.4FB721677BCA6BF8!138/files";
char client_secret[1024] = "?access_token=";
strcat(client_secret,access_token_new);
CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};
FILE *refresh = fopen("folderlist.txt","w");
curl = curl_easy_init();
  if(curl) {
	 
	strcat(POST_DATA,url);
	strcat(POST_DATA,client_secret);
    curl_easy_setopt(curl, CURLOPT_URL, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, refresh);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }
  fclose(refresh);
//  return 0;

}

void create_fold(){
//char url[] = "https://apis.live.net/v5.0/me/skydrive/folder.4fb721677bca6bf8.4FB721677BCA6BF8!138/files";
char url[] = "https://apis.live.net/v5.0/folder.4fb721677bca6bf8.4FB721677BCA6BF8!138";
	char client_secret[1024] = "?access_token=";
strcat(client_secret,access_token_new);
CURL *curl;
CURLcode res;
char POST_DATA[2048] ={0};
char h_url[2048] = {0};
char HEADER[1024] = {0};
//FILE *refresh = fopen("folderlist.txt","w");
curl = curl_easy_init();
  if(curl) {
	 struct curl_slist *chunk = NULL;
	 strcpy(HEADER,"Authorization: Bearer ");
	 strcat(HEADER,access_token_new);
	 chunk = curl_slist_append(chunk, HEADER);
	 chunk = curl_slist_append(chunk,"Content-Type: application/json");
	  strcpy(h_url,url);
	  printf("\nHEADERS are : %s \n",HEADER);
	strcpy(POST_DATA,"\n{\n");
	strcat(POST_DATA,"    \"name\": \"My New Folder\"");
	strcat(POST_DATA,"\n}\n");
	//strcat(h_url,client_secret);
	printf("\nURL is : %s\n ",h_url);

    curl_easy_setopt(curl, CURLOPT_URL, h_url);
	printf("\nPOST_DATA is : %s\n",POST_DATA);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, refresh);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    res = curl_easy_perform(curl);
	 if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
	  curl_easy_cleanup(curl);
	   }

}


void down_data(){
char url[1024] = "https://apis.live.net/v5.0/file.4fb721677bca6bf8.4FB721677BCA6BF8!166/content?access_token=";
strcat(url,access_token_new);
CURL *curl;
//id = 4FB721677BCA6BF8
CURLcode res;
//char POST_DATA[2048] ={0};
FILE *refresh = fopen("downfile.txt","w");
curl = curl_easy_init();
  if(curl) {
   
    curl_easy_setopt(curl, CURLOPT_URL, url);
  //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POST_DATA);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.125 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, refresh);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    res = curl_easy_perform(curl);
   if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
    curl_easy_cleanup(curl);
     }
  fclose(refresh);
}


void putdata(char *file)
{
  CURL *curl;
  CURLcode res;
  FILE * hd_src ;
  struct stat file_info;
 //https://onedrive.live.com/redir?resid=4FB721677BCA6BF8!135&authkey=!AEbne5Fcl8XBedg&ithint=file%2ctxt
  char url[1024]="https://apis.live.net/v5.0/me/skydrive/files/";
   strcat(url,file);
   strcat(url,"?");
   strcat(url,"access_token=");
   strcat(url,access_token_new);
  stat(file, &file_info);
 
  /* get a FILE * of the same file, could also be made with
     fdopen() from the previous descriptor, but hey this is just
     an example! */ 
  hd_src = fopen(file, "rb");
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    curl_easy_setopt(curl, CURLOPT_PUT, 1L);
 
    curl_easy_setopt(curl, CURLOPT_URL, url);
 
    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
 
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    curl_easy_cleanup(curl);
  }
  fclose(hd_src); /* close the local file */ 
 
  curl_global_cleanup();
}


int main()

{
   
   int choice;
   char filename[200] = {0};
   printf("Enetr ur choice\n 1.auth_tok\n 2.refresh_tok\n 3.putdata\n 4.down_data \n 5.read_fold\n 6.Create Folder\n"); 
	scanf("%d",&choice);
	if(choice == 1)
		auth_tok();
	if(choice == 2){
        get_toks();
		refresh_tok();

	}
    //read_fold();
    //uplaod_data();
    if(choice == 3){
      printf("\nEnter File name: ");
      gets(filename);
	  get_toks();
      putdata("img.jpg");
  }
  if(choice ==4)
  {
	  get_toks();
    down_data();

  }
  if(choice ==5){
   get_toks();
	  read_fold();
  }
  if(choice ==6){
  get_toks();
  create_fold();
  }
	main();
}