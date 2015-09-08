#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char str1[] = "\"refresh_token\":\"";
char *str2;
char auth[] = "\"access_token\":\"";
char *str_acs;
char *sep = ":,\"";
int main(){
	char *tok;
	//tok = strtok(str2,str1);
    
	FILE *ref_tok = fopen("refresh_tok.txt","r");
	FILE *acs_tok = fopen("refresh_tok.txt","r");
	  if(ref_tok){
	  fseek(ref_tok, 0, SEEK_END);
    long fsize = ftell(ref_tok);
        if(fsize < 500)
        return;
    fseek(ref_tok, 0, SEEK_SET);
    char *resF = (char *) malloc ( sizeof(char) * fsize);
    memset(resF, '\0', sizeof(char) * 1024);
    fread(resF, sizeof(char), fsize, ref_tok);
	 // printf("\nFile Content is : \n%s \n",resF);
	  printf("size of refresh token file is %d\n",fsize);  
	  
    tok = strstr(resF,str1);

   // printf("Tok is \n%s\n",tok);
	  
	 str2 = strtok(tok,sep);
	 str2 = strtok(NULL,sep);
	 printf("\nRefresh token is \t\n\n\t %s\n\n\n",str2);
	 fclose(ref_tok);
	  }

	  if(acs_tok){
		  char *tok2;
		   fseek(acs_tok, 0, SEEK_END);
    long fsize = ftell(acs_tok);
        if(fsize < 500)
        return;
    fseek(acs_tok, 0, SEEK_SET);
    char *resF = (char *) malloc ( sizeof(char) * fsize);
    memset(resF, '\0', sizeof(char) * 1024);
    fread(resF, sizeof(char), fsize, acs_tok);
	  //printf("\nFile Content is : \n%s \n",resF);
	  printf("size of access token file is %d\n",fsize);  
    tok2 = strstr(resF,auth);

    //printf("Tok is \n%s\n",tok2);
	  
	  str_acs = strtok(tok2,sep);
	 str_acs = strtok(NULL,sep);
	 printf("\nAccess token is \t\n\n\t %s\n\n\n",str_acs);
	  fclose(acs_tok);
	  }
  
    return 0;
}