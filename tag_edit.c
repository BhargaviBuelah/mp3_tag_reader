#include<stdio.h>
#include<string.h>
#include"header.h"

status edit_tags(Tag_edit *edit,int argc,char *argv[])           /*important  function for view tags*/
{
    
    if(check_edit_args(edit,argc,argv)==SUCCESS)
    {
        printf("\n\033[0;33m🖍️  Choosed edit mode for %s file:\033[0m\n",edit->file_name);
       // printf("correct arguments\n");
       printf("\033[0;35m✔️  Arguments verified successfully\033[0m\n");
        if(open_edit_file(edit)==SUCCESS)
        {
            printf("\033[38;2;147;112;219m📄 File opening Successfully.\033[0m\n");
            if(check_ID_edit(edit)==SUCCESS)
            {
                if(copy_output_header(edit)==SUCCESS)
                {
                    if(read_edit_tag(edit)==SUCCESS)
                    {
                        printf("\033[38;2;128;128;0m📝 Data Edited Successfully.\033[0m\n");
                        return SUCCESS;
                    }
                }
                    
            }
        }
    }
    return FAILURE;
}

status copy_output_header(Tag_edit *edit)     //copying data from sample to output file
{
    long int var = ftell(edit->fptr_edit);
  //  printf("-------------------------->%ld\n",var);
  char buffer[10];                  //size is 10 because header size 10  ->>4 ID +2 flag +4 size
    fseek(edit->fptr_edit,0,SEEK_SET);      
    fread(buffer,10,1,edit->fptr_edit);
    fwrite(buffer,10,1,edit->fptr_output);

    return SUCCESS;
}
status check_ID_edit(Tag_edit *edit)            
{
    //printf("heloooooooooooo from ID\n");
    char buffer[4];                     //size 4 because ID present in 4 bytes
    fread(buffer,4,1,edit->fptr_edit);
   // printf("------------------------------------->>>%s\n",buffer);
   strcpy(edit->ID,buffer);
  // printf("------------------------------------->>>%s\n",edit->ID);
    return SUCCESS;
}
status check_edit_args(Tag_edit *edit,int argc,char *argv[])
{
    if(argc==5)     /*../a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename*/
    {
        const char *valid_options[6] = {"-t", "-a", "-A", "-m", "-y", "-c"};
        
        for (int i = 0; i < 6; i++) 
        {
            if (strcmp(argv[2], valid_options[i]) == 0)
            {
                strcpy(edit->option,argv[2]);
                //printf("\033[0;35m✔️ Arguments verified successfully\033[0m\n");
                
                if(argv[3]!=NULL)
                {
                    strcpy(edit->edit_text,argv[3]);    //stroring the updating text
                    //printf("-------------->>hhhhhhhhhhhhhhhhhhhh>>%s\n\n",edit->edit_text);
                    if(strstr(argv[4],".mp3"))
                    {
                        strcpy(edit->file_name,argv[4]);
                        // printf("-------------->>hhhhhhhhhhhhhhhhhhhh>>%s\n\n",edit->file_name);
                        //printf("\033[0;35m✔️ Arguments verified successfully\033[0m\n");
                        return SUCCESS;
                    }
                    else
                    {
                        printf("\033[1;31mError ❌provide .mp3 file \033[0m\n");
                        return FAILURE;
                    }
                }
                
            }
        }
        printf("\033[1;31mError ❌provide valid tag type\033[0m\n");
        return FAILURE;
    }
    else
    {
        printf("\033[1;31mError ❌Argument count must be 5\033[0m\n");
        return FAILURE;
    }
}
status open_edit_file(Tag_edit *edit)
{
    edit->fptr_edit=fopen(edit->file_name,"r+");
    if(edit->fptr_edit==NULL)
    {

        printf("\033[1;31mError ❌Unable to open output file\033[0m\n"); 
        return FAILURE;
    }
    //printf("------------------>Hellooooooooooooo\n");
    strcpy(edit->output_file_name,"output.mp3");
    edit->fptr_output=fopen(edit->output_file_name,"w");
    if(edit->fptr_output==NULL)
    {
        printf("\033[1;31mError ❌Unable to open output file\033[0m\n");
        return FAILURE;
    }
    //printf("File opened Successfully\n");
    return SUCCESS;
}
status read_edit_tag(Tag_edit *edit)
{
    char buffer[4];
    int bytes_read;
    
    while ((bytes_read = fread(buffer, 1, 4, edit->fptr_edit)) > 0 )
    {
        fwrite(buffer, 4, 1, edit->fptr_output);
        if ((!strcmp(buffer, "TIT2"))&&(!strcmp(edit->option,"-t"))) 
        {  // edit song title -t
            unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;
            int size=len;
           // printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;
            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

           // printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
            fwrite("\0",1,1,edit->fptr_output);
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else if ((!strcmp(buffer, "TPE1"))&&(!strcmp(edit->option,"-a")))       //editing artist name
        {  
            unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;
            int size=len;
           // printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;
            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

            //printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
             fwrite("\0",1,1,edit->fptr_output);
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else if ((!strcmp(buffer, "TALB"))&&(!strcmp(edit->option,"-A")))           //album name
        {  
             unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;
            int size=len;
           // printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;
            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

            //printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
             fwrite("\0",1,1,edit->fptr_output);
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else if ((!strcmp(buffer, "TYER"))&&(!strcmp(edit->option,"-y")))        //editing year
        {  
             unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;
            int size=len;
            //printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;

            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

            //printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
            fwrite("\0",1,1,edit->fptr_output);
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length-1 because 
                                                                    //1 byte stored "\0"           
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else if ((!strcmp(buffer, "TCON"))&&(!strcmp(edit->option,"-m")))            //editing content
        {  
             unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;
            int size=len;
           // printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;
            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

           // printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
             fwrite("\0",1,1,edit->fptr_output);
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else if ((!strcmp(buffer, "COMM"))&&(!strcmp(edit->option,"-c"))) 
        {   unsigned int len=strlen(edit->edit_text)+1;       //taking user input size;//+1 for null character
            int size=len;
           // printf("---------------------1st-------->%u\n",len);
            convert((char *)&len, sizeof(int));             //converting to big Endian
            //printf("---------------------1st-------->%u\n",len);
            fwrite(&len, sizeof(int), 1, edit->fptr_output);         //storing size of text
            unsigned int x;
            fread(&x, sizeof(int), 1, edit->fptr_edit) ;    //reading size from sample.mp3 file
            convert((char *)&x, sizeof(int));               //converting  size to little Endian Sysytem

            //printf("Size before: %d\n", x);
            char flag[2];
            fread(flag, 1, 2, edit->fptr_edit);      //reading flag
        
            fwrite(flag, 2, 1, edit->fptr_output);  // writing only 2 bytes flag into output file
            fwrite("\0",1,1,edit->fptr_output);  
            fwrite(edit->edit_text,size-1,1,edit->fptr_output);      //writing user input text with length
            fseek(edit->fptr_edit,x,SEEK_CUR);  //skipping the pointer in sample file upto size times x
            
            break;
        }
        else
        {
            unsigned int x;
            if (fread(&x, sizeof(int), 1, edit->fptr_edit) != 1)    //reading size
            {
                perror("Failed to read size");
                break;
            }
            fwrite(&x, sizeof(x), 1, edit->fptr_output);
            convert((char *)&x, sizeof(int));
            //printf("Size before------------------->: %d\n", x);

            char flag[2];
            if (fread(flag, 1, 2, edit->fptr_edit) != 2)        //reading flag
            {
                perror("Failed to read flag");
                break;
            }
            fwrite(flag, 2, 1, edit->fptr_output);  // Corrected to write only 2 bytes
            char temp[x];
            fread(temp, x, 1, edit->fptr_edit);
            fwrite(temp, x, 1, edit->fptr_output);
        }

    }
    //copying remaining data 
    char ch;
    while(fread(&ch, 1, 1, edit->fptr_edit) > 0)
    {
        fwrite(&ch, 1, 1, edit -> fptr_output);
    }
    return SUCCESS;
}
