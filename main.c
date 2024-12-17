#include<stdio.h>
#include"header.h"
int main(int argc,char *argv[])
{
    printf("\033[48;2;128;0;128m\033[38;2;255;253;208m\t\t\t\t\t🎺🎺 MP3 TAG READER AND EDITOR🎺🎺  \033[0m\t\t\t\t\n");
    if(argc<2)
    {
        printf("\033[1;31mError ❌:Invalid arguments\033[0m\n");
        printf("\033[0;33mUSAGE :\033[0m\n");
        printf("\t\033[0;32mTo view please pass like :\033[0;35m./a.out -v mp3filename\033[0m\n\n");
        printf("\t\033[0;32mTo edit please pass like :\033[0;36m ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename\033[0m\n \n");
        printf("\t\033[0;32mTo get help pass like :\033[0;33m ./a.out --help\033[0m\n\n");
        return FAILURE;
    }
    if(check_operation(argv)==view_tag)
    {
        Tag_view view;
        if(read_tags(&view,argc,argv)==SUCCESS)           /*total operation*/
        {
           printf("\n\t\t\t\033[38;5;214m-------------------✅ Details Displayed Successfully ✅----------------------\033[0m\n\n") ;
           return SUCCESS;          
        }
    }
    else if(check_operation(argv)==edit_tag)
    {
        Tag_edit edit;
         if(edit_tags(&edit,argc,argv)==SUCCESS)           /*total operation*/
        {
            //printf("\033[0;36moo\n");
             printf("\n\t\t\t\033[38;5;214m-------------------✅ Details Edited Successfully ✅----------------------\033[0m\n\n") ;
  
            return SUCCESS;
        }
    }
    else if(check_operation(argv)==help)
    {
       
        if(display_help()==SUCCESS)
            return 0;

    }
    else if(check_operation(argv)==unsupported)
    {

        printf("\033[1;31mError ❌:Invalid arguments\033[0m\n");
        printf("\033[0;33mUSAGE :\033[0m\n");
        printf("\t\033[0;32mTo view please pass like :\033[0;35m./a.out -v mp3filename\n");
        printf("\t\033[0;32mTo edit please pass like :\033[0;36m ./a.out -e -t/-a/-A/-m/-y/-c changing text mp3filename \n");
        printf("\t\033[0;32mTo get help pass like :\033[0;33m ./a.out --help\n");
    }

}