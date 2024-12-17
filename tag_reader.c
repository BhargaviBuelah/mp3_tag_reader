#include<stdio.h>
#include<string.h>
#include"header.h"

Operation_Type check_operation(char *argv[])
{
    if(!strcmp(argv[1],"-v"))
        return view_tag;
    else if(!strcmp(argv[1],"-e"))
        return edit_tag;
    else if(!strcmp(argv[1],"--help"))
        return help;
    else
        return unsupported;
}

status display_help()
{
    printf("\033[0;36m\t\t<----------------------------------------------------------------------->\033[0m\n");
    printf("\033[0;36m\t\t|\t\t\t\t\033[0;35m>>HELP MENU<<\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t<----------------------------------------------------------------------->\033[0m\n");
    printf("\033[0;36m\t\t|\033[1;31m1.-v ->To view mp3 file contents\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|2.-e ->To edit file contents\t\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\033[38;2;128;128;0m\t 2.1. -t ->To edit song title\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\033[0;33m\t 2.21. -a ->To edit artist name\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\033[38;2;255;105;180m\t 2.3. -A ->To edit album name\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\t 2.4. -y ->To edit year\t\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\033[0;35m\t 2.5. -m ->To edit content\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t|\033[0;32m\t 2.6. -c ->To edit comment\t\t\t\t\t\033[0;36m|\033[0m\n");
    printf("\033[0;36m\t\t<----------------------------------------------------------------------->\033[0m\n");
    
    return SUCCESS;

}
status read_tags(Tag_view *view,int argc,char *argv[])           /*important  function for view tags*/
{
    if(check_view_args(view,argc,argv)==SUCCESS)
    {
        //printf("correct arguments\n");
        if(open_file(view)==SUCCESS)
        {
          //  printf("file opening Successfully\n");
            if(check_ID(view)==SUCCESS)
            {
              //  printf("Version  Fetched Successfully\n");
                if(read_store_tags(view)==SUCCESS)
                {
                    //printf("All Tags Stored Successfully\n");
                    printf("\n\033[0;33m🧿 Choosed view mode for %s file:\033[0m\n",view->file_name);
                    printf("\n\n\033[0;36m\t\t<--------------------------------------------------------------------------------------->\n");
                    printf("\033[0;36m\t\t|\t\t\t\t\033[0;35m📜 MP3 TAG READER AND EDITOR\t\t\t\t\033[0;36m|\033[0m\n");
                    printf("\033[0;36m\t\t<--------------------------------------------------------------------------------------->\n");
                    printf("\033[0;36m\t\t|📝 \033[1;31m%-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","TITLE",view->TIT2);
                    printf("\033[0;36m\t\t|🕺 \033[38;2;128;128;0m%-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","ARTIST",view->TPE1);
                    printf("\033[0;36m\t\t|💽 \033[0;33m%-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","ALBUM",view->TALB);
                    printf("\033[0;36m\t\t|📆 \033[38;2;255;105;180m%-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","YEAR",view->TYER);
                    printf("\033[0;36m\t\t|🎷 %-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","MUSIC",view->TCON);
                    printf("\033[0;36m\t\t|🔠 \033[0;35m%-15s   :   %-50s\t\t\033[0;36m|\033[0m\n","LANGUAGE",view->COMM);
                    printf("\033[0;36m\t\t<--------------------------------------------------------------------------------------->\n");
                   
                    return SUCCESS;
                }
                else
                {
                    printf("\033[1;31mError ❌Unable to fetch the tags\033[0m\n");
                    return FAILURE;
                }
            }
            else
            {
                printf("\033[1;31mError ❌Unable to fetch Vesion \033[0m\n ");
                return FAILURE;
            }
        }
        else
        {
            printf("\033[1;31mError ❌Error:file not opening\033[0m \n");
            return FAILURE;
        }
    }
    else
    {
         printf("\033[38;5;214mProvide Valid arguments\033[0m\n");
         return FAILURE;
    }
}
status check_view_args(Tag_view *view,int argc,char *argv[])
{
    if(argc==3)     /*./a.out -v sample.mp3*/
    {
        if(strstr(argv[2],".mp3"))
        {
            // printf("-------------->>hhhhhhhhhhhhhhhhhhhh>>%s\n\n",argv[2]);
            strcpy(view->file_name,argv[2]);
          //  printf("-------------->>hhhhhhhhhhhhhhhhhhhh>>%s\n\n",view->file_name);
           // printf("arguments verified successfully\n");
            return SUCCESS;
        }
        else
        {
            printf("\033[1;31mError ❌provide .mp3 file.\033[0m \n");
            return FAILURE;
        }
        return SUCCESS;
    }
    else
    {
        printf("\033[1;31mError ❌Argument count must be 3\033[0m\n");
        return FAILURE;
    }
}
status open_file(Tag_view *view)
{
    view->fptr_view=fopen(view->file_name,"r");
    if(view->fptr_view==NULL)
    {
        printf("\033[1;31mError ❌Unable to open file\033[0m\n");
        return FAILURE;
    }
       // printf("File opened Successfully\n");
        return SUCCESS;
}
/*To fetch the ID from mp3 file*/
status check_ID(Tag_view *view)
{
    char buffer[4];
    fread(buffer,4,1,view->fptr_view);
   // printf("------------------------------------->>>%s",buffer);
   strcpy(view->ID,buffer);
   //printf("------------------------------------->>>%s\n",view->ID);
    return SUCCESS;
}
status read_store_tags(Tag_view *view)
{
    fseek(view->fptr_view,10,SEEK_SET);
    int count=6;  
    while(count)        //reading upto getting 6 tags
    {
        char buffer[4];
        fread(buffer,4,1,view->fptr_view);
        if(!strcmp(buffer,"TIT2"))
        {
           // printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
          //  printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
           // x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            //printf("bytesssssssssss%d",bytesRead);
            strcpy(view->TIT2,str);
            /*printing tagss*/
          // printf("TITLE            :           %-10s\n",view->TIT2);
        }
        else if(!strcmp(buffer,"TPE1"))
        {
           // printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
           // printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
           // x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            strcpy(view->TPE1,str);
           /*printing tagss*/
            // printf("ARTIST          :           %-10s\n",view->TPE1);
        }
        else if(!strcmp(buffer,"TALB"))
        {
           // printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
           // printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
           // x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            strcpy(view->TALB,str);
           /*printing tagss*/
            //  printf("ALBUM          :           %-10s\n",view->TALB);
        }
        else if(!strcmp(buffer,"TYER"))
        {
           //  printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
            //  printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
           // x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            strcpy(view->TYER,str);
           /*printing tagss*/
           // printf("YEAR          :           %-10s\n",view->TYER);
        }
        else if(!strcmp(buffer,"TCON"))
        {
           // printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
           // printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
            //x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            strcpy(view->TCON,str);
          // printf("------------------>>>  %s\n",str);
          /*printing tagss   */
           // printf("MUSIC          :           %-10s\n",view->TCON);
        }
        else if(!strcmp(buffer,"COMM"))
        {
           
            //printf("------------------------------------->>>%s\n",buffer);
            count--;
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
           // printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR);   //skipping 2 bytes for flag  and 1 byte  for  null terminator
           // x=x-3;  //decreasing the size because we skipped 3 bytes
            char str[x-1];
            //fscanf(view->fptr_view,"%s",str);
            int  bytesRead = fread(str, 1, x-1, view->fptr_view);
            str[bytesRead]='\0';
            strcpy(view->COMM,str);
           /*printing tagss   */
            //printf("LANGUAGE          :           %-10s\n",view->TCON);
        }
        else
        {
            
           // printf("I'm in else parttttttttttt\n");
            int x;      //five bytes 4 for size and 
            fread(&x, sizeof(int), 1, view->fptr_view);       //reading size//int  bytesRead = fread(&x, 4, 1, view->fptr_view);
           // printf("sizeeeeeeeee   beforeee  %d\n",x);
            convert((char *)&x,sizeof(int));
           // printf("sizeeeeeeeee  afterrrrrr %d\n",x);
           fseek(view->fptr_view,3,SEEK_CUR); 
            char temp[x-1];
            
            int  bytesRead=fread(temp, 1, x-1, view->fptr_view);
            
            temp[bytesRead] = '\0';
            
        }
    }
    return SUCCESS;
}

void convert(char*ptr,int size)
{
   // printf("---%d\n",size);
    for(int i=0 ;i<(size/2);i++)       //reversing the bytes
    {
        char temp= ptr[i];
        ptr[i]=ptr[size-i-1];
        ptr[size-i-1]=temp;
    }
}