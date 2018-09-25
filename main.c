#include <stdio.h>
#include <string.h>
#include <cJSON.c>

typedef struct Student Student;
struct Student {
    char name[10];
    int age;
    int height;
};
int compareByAge(Student,Student);
int compareByHeight(Student,Student);
int compareByName(Student,Student);
int sort(Student[],int,int (*compare)(Student,Student));
int get_array_students(Student[],int,cJSON *);
int text_length(FILE *);
void text_copy(char [],int ,FILE *);
int main() {
    FILE * pfile = NULL;
    pfile = fopen("/Users/daquan/CLionProjects/json/json.txt","r");
    int size = text_length(pfile)+1;
    char text[size];
    text_copy(text,size,pfile);
    fclose(pfile);
    cJSON * root = cJSON_Parse(text);
    if(!root) {
        printf("no json\n");
        return -1;
    }
    printf("%s\n",cJSON_Print(root));
    int array_size = cJSON_GetArraySize(root);
    Student stus[array_size];
    get_array_students(stus,array_size,root);
    cJSON_Delete(root);
    sort(stus,array_size,compareByAge);
    for (int i = 0; i < array_size; ++i) {
        printf("name:%s\tage:%d\theight:%d\n",stus[i].name,stus[i].age,stus[i].height);
    }
    return 0;
}
int sort(Student stus[],int size,int (*compare)(Student,Student)){
    for (int i = 0; i < size; ++i) {
        for (int j = i+1; j < size; ++j) {
            if (compare(stus[i],stus[j])>=0){
                Student temp = stus[i];
                stus[i] = stus[j];
                stus[j] = temp;
            }
        }
    }
}
int compareByAge(Student a, Student b){
    return (b.age-a.age);
}
int compareByHeight(Student a,Student b){
    return (b.height-a.height);
}
int compareByName(Student a,Student b){
    return strcmp(a.name,b.name);
}
int get_array_students(Student stus[],int size,cJSON * root){
    cJSON *item;
    cJSON *name;
    cJSON *age;
    cJSON *height;
    for(int i=0; i< size; i++) {
        item = cJSON_GetArrayItem(root, i);
//        printf("%s\n",cJSON_Print(item));
        name = cJSON_GetObjectItem(item, "name");
        if (!name) {
            printf("no name!\n");
            return -1;
        }
        strcpy(stus[i].name,(name->valuestring));
        age = cJSON_GetObjectItem(item, "age");
        if (!age) {
            printf("no age!\n");
            return -1;
        }
        stus[i].age=(age->valueint);
        height = cJSON_GetObjectItem(item, "height");
        if (!height) {
            printf("no height!\n");
            return -1;
        }
        stus[i].height=(height->valueint);
    }
    return size;
}
int text_length(FILE * pfile){
    int all;
    while (1){
        int m = fgetc(pfile);
        if (m==EOF){
            break;
        }
        all++;
    }
    fseek(pfile,0L,0);
    return all;
}
void text_copy(char as[],int size,FILE * pfile){
    for (int i = 0; i < size-1; ++i) {
        as[i]= (char)fgetc(pfile);
    }
    as[size-1]='\0';
}
