#ifndef HEADER_H
#define HEADER_H


typedef struct  view
{
     /*File Info */ 
    char file_name[15];     //stroning mp3 file name;
    FILE *fptr_view;    
    char ID[4];
    char  TPE1[100];         //Artist name
    char  TIT2[100];         //Title/Song name
    char TALB[100];          //Album
    char TYER[5];           //Year
    char TCON[100];          //content Type
    char COMM[100];          //Composer

}Tag_view;

typedef struct  edit
{
     /*File Info */ 
    char file_name[15];     //stroning mp3 file name;
    char edit_text[25];
    FILE *fptr_edit; 
    FILE *fptr_output;   
    char option[3];
    char output_file_name[15]; 
    char ID[4];
    char  TIT2[100];         //Title/Song name
    char  TPE1[100];         //Artist name
    char TALB[50];          //Album
    char TYER[5];           //Year
    char TCON[50];          //content Type
    char COMM[50];          //Composer

}Tag_edit;

typedef enum
{
    FAILURE,
    SUCCESS
}status;
typedef enum
{
    view_tag,
    edit_tag,
    help,
    unsupported
} Operation_Type;


/*----------------->For View Tags<---------------------------*/
Operation_Type check_operation(char *argv[]);
/*for displaying the help command*/
status display_help();
/*for reading the tags from the file */
status read_tags(Tag_view *view,int argc,char *argv[]);
/*For checking  view  arguments are correct or not*/
status check_view_args(Tag_view *view,int argc,char *argv[]);
/*Opening the file in read mode*/
status open_file(Tag_view *view);
/*Reading Version / ID of song*/
status check_ID(Tag_view *view);
/*reading and storing all the tags*/
status read_store_tags(Tag_view *view);
/*Converting Big Endian to little Endian */
void convert(char*ptr,int size);
/*----------------->For Editing Tags<---------------------------*/
status edit_tags(Tag_edit *edit,int argc,char *argv[]);
status check_edit_args(Tag_edit *edit,int argc,char *argv[]);
status open_edit_file(Tag_edit *edit);
status check_ID_edit(Tag_edit *edit);
status copy_output_header(Tag_edit *edit);
status read_edit_tag(Tag_edit *edit);
#endif