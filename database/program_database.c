#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <pwd.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <syslog.h>
#include <time.h>
#include <wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 8080
#define SIZE 1024
#define table_size 100

char user[1024];
 
int createServerSocket(struct sockaddr_in *address, int *addrlen);
int DDL_DDM(char buffer[], int *flag);
char *pathing(char path1[], char path2[], char result[]);
int Aunthentication_Authorization(char buffer[], int *flag);
void addtsv(FILE *files, char nama[], char publisher[], char tahun[], char ekstensi[], char path[]);
int deletefile(FILE *files, char name[], char source[], char result[]);
void see(FILE *files, char check[], char display[]);
void find(FILE *files, char check[], char display[], char buffer[]);
int locate(char fullpath[], char sy);
void logging(FILE* log,int tipe, char nama[]);

void db_create_database(char database[]);
void db_create_table(char buffer[], int *flag);
void db_drop_database(char buffer[], int *flag);
void db_drop_table(char buffer[], int *flag);
void db_drop_column(char buffer[], int *flag);
void db_insert(char buffer[], int *flag);
void db_update(char buffer[], int *flag);
void dump(char buffer[], int *flag);

//typedef struct program_database{
//	unsigned int key;
//	unsigned int nim;
//	char name[100];
//	char gender[10];
//	char address[20];
//	char entry_date[4];
//	
//}student_record;

	char *c_create_user = "CREATE USER";
	char *c_use = "USE";
	char *c_grant_permission = "GRANT PERMISSION";
	char *c_create_database = "CREATE DATABASE";
	char *c_create_table = "CREATE TABLE";
	char *c_drop_database = "DROP DATABASE";
	char *c_drop_table = "DROP TABLE";
	char *c_drop_column = "DROP COLUMN";
	char *c_insert_into = "INSERT INTO";
	char *c_update = "UPDATE";
	char *c_delete_from = "DELETE FROM";
	char *c_select = "SELECT";
	char *c_exit = "EXIT";

int main(int argc, char const *argv[]){
	
    struct sockaddr_in address;
    int new_socket, addrlen, flag = 1, info, auth = 0;
    char delim[] = ":";
    char buffer[1024];
    char check[1024];
    char source[100] = "/home/alvancho/Documents/IO5/FP/database";
    char result[100];
    char display[10000];
    char username[1024];
	char password[1024];
    
	FILE* akun;
    FILE* files;
    FILE* log;	
    
    akun = fopen(pathing(source, "/account.tsv", result), "a+");
    files = fopen(pathing(source, "/files.tsv", result), "a+");
    log = fopen(pathing(source, "/log.tsv", result), "a+");
    //int status = mkdir(pathing(source, "/Database", result),0777); //folder FILES
 
    int server_fd = createServerSocket(&address, &addrlen);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    
        while (flag) {
        while(flag == 1 && auth == 0){
        	
            fclose(akun);
            akun = fopen(pathing(source, "/account.tsv", result), "a+");
            
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);
            
            info = Aunthentication_Authorization(buffer, &flag);
            send(new_socket, buffer, strlen(buffer), 0);
            
            if(info == 1) { //CREATE USER
                int pesan = 0;
                char login[1024];
                char pass[1024];
                char * token;
				      
                memset(buffer, 0, sizeof(buffer)); //id
                read(new_socket, buffer, 1024);
                token = strtok(buffer," ");
                for(int i=0; i<=5; i++) {
					//printf( " %s\n", token );
					    if(i==2){
    						//printf ("Username\n");
    						strcpy(login, buffer);
						}
						else if(i==5){
							//printf ("Password\n");
							strcpy(pass, buffer);
						}
    				token = strtok(NULL," ");
   				}
                //strcpy(login, buffer);
                
                //send(new_socket, "Expecting pass...", strlen("Expecting pass..."), 0);
                
                //memset(buffer, 0, sizeof(buffer)); // pass
                //read(new_socket, buffer, 1024);
                //strcpy(pass, buffer);
                
                while(fgets(check, sizeof(check), akun))
                {
                    char *ptr = strtok(check,delim);
                    puts(ptr);
                    puts(login);
                    if (strcmp(ptr, login) == 0) 
                    {
                        pesan = 1;
                        break;
                    }
                }
                if(pesan == 0){
                    fputs(login, akun);
                    fputs(":", akun);
                    fputs(pass, akun);
                    fputs("\n", akun);
                }

                if(pesan == 0)
                send(new_socket, "Berhasil mendaftar.", strlen("Berhasil mendaftar."), 0);
                else send(new_socket, "Gagal mendaftar.", strlen("Gagal mendaftar."), 0);
            } 
			else if(info == 2){ //USE
			//	cd to folder?
			    char use[1024];
                char access[1024];
                char * token;
                
                memset(buffer, 0, sizeof(buffer)); //id
                read(new_socket, buffer, 1024);
                token = strtok(buffer," ");
                
                for(int i=0; i<=1; i++) {
					//printf( " %s\n", token );
					    if(i==1){
    						//printf ("Username\n");
    						strcpy(use, buffer);
						}
    				token = strtok(NULL," ");
					}	
				send(new_socket, "Berhasil mengakses.", strlen("Berhasil mengakses."), 0);
                
            }
            else if(info == 3){ //GRANT PERMISSION
        	//  chown -hR root /u Change the owner of /u and subfiles to "root".
        	//	int chown(const char *pathname, uid_t owner, gid_t group);
        	//  chown(pathing(source, "/Database", result),0777);
        		char database[1024];
                char username[1024];
                char * token;
                
                memset(buffer, 0, sizeof(buffer)); //id
                read(new_socket, buffer, 1024);
                token = strtok(buffer," ");
                
                for(int i=0; i<=4; i++) {
					//printf( " %s\n", token );
					    if(i==2){
    						//printf ("Username\n");
    						strcpy(database, buffer);
						}
						else if(i==4){
							//printf ("Password\n");
							strcpy(username, buffer);
						}
    				token = strtok(NULL," ");
				}
				//int status = chown(pathing(source, "/Database", result),uid_t owner, gid_t group);
				send(new_socket, "Berhasil memberi akses.", strlen("Berhasil memberi akses."), 0);
			}
			else if(info == 4){ //Access Database
			    char login[1024];
                char pass[1024];
				
				memset(buffer, 0, sizeof(buffer)); //id
			    read(new_socket, buffer, 1024);
//                for(int i=0; i<=5; i++) {
//					//printf( " %s\n", token );
//					    if(i==2){
//    						//printf ("Username\n");
//    						strcpy(login, buffer);
//						}
//						else if(i==5){
//							//printf ("Password\n");
//							strcpy(pass, buffer);
//						}
//    				token = strtok(NULL," ");
//   				}
			    strcpy(login, buffer);
			    
			    
    
			    memset(buffer, 0, sizeof(buffer)); //password
				read(new_socket, buffer, 1024);
			    strcpy(pass, buffer);
    
                while(fgets(check, sizeof(check), akun))
                {
                    char *ptr = strtok(check,delim);
                    if (strcmp(ptr, login) == 0) 
                    {
                        ptr = strtok(NULL,delim);
                        ptr[strcspn(ptr,"\n")]='\0';
                        // printf("%d\n",strcmp(ptr, pass));
                        if (strcmp(ptr, pass) == 0) 
                        {
                            sprintf(user,"%s:%s", login, pass);//warning
                            auth=1;
                            break;
                        }
                    }
                }
                if(auth == 1)
                	send(new_socket, "Login berhasil", strlen("Login berhasil"), 0);
                else{
                	send(new_socket, "Login gagal", strlen("Login gagal"), 0);
				}
            }
        }
        fclose(files);
        fclose(log);
        
        //files = fopen(pathing(source, "/files.tsv", result), "a+");
        //log = fopen(pathing(source, "/log.tsv", result), "a+");
        
        memset(display, 0, strlen(display));
        memset(buffer, 0, sizeof(buffer));
        
        read(new_socket, buffer, 1024);
        info = DDL_DDM(buffer, &flag);
        
        if(info == 1){
        //Create Database
        	char database[1024];
            char * token;
        	send(new_socket, buffer, strlen(buffer), 0);
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=2; i++) {
				//printf( " %s\n", token );
					if(i==2){
    					printf ("database_name\n");
    					strcpy(database[1024], buffer);
					}
    			token = strtok(NULL," ");
			}
			//int status = mkdir(pathing(source, "/Database", result),0777);
			db_create_database(database);
        	send(new_socket, "Berhasil membuat database.", strlen("Berhasil membuat database."), 0);
        }
        else if(info == 2){
        //Create Table
            char table[1024];
            char column_name[1024];
            char data_type[1024];
            char * token;
			char * token_2;	
			send(new_socket, buffer, strlen(buffer), 0);
        	char login[1024];
            char pass[1024];
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=table_size; i++) {
				//printf( " %s\n", token );
    				if(i==2){
    					printf ("nama_table\n");
    					strcpy(table, buffer);
					}
					else if(i==3){
						//token_2 = strtok (token,"(");
						printf ("special\n",token_2);
						strcpy(column_name, buffer);
					}
					else if(i>3&&(i%2==1)){
						printf ("nama_kolom\n");
						strcpy(column_name, buffer);
					}
					else if(i>3&&(i%2==0)){
						printf ("data_type\n");
						strcpy(data_type, buffer);
					}
    			token = strtok(NULL," ");
			}
			//int status_2 = mknod(pathing(source, "/Database/table.tsv", result),0777, dev);
			send(new_socket, "Berhasil membuat table.", strlen("Berhasil membuat table."), 0);
		}
		else if(info == 3){
		//Drop Database
            char database[1024];
            char * token;
			send(new_socket, buffer, strlen(buffer), 0);
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=2; i++) {
				//printf( " %s\n", token );
					if(i==2){
    					printf ("database_name\n");
    					strcpy(database, buffer);
					}
    			token = strtok(NULL," ");
			}
			//int status_3 = rmdir(pathing(source, "/Database", result));
			send(new_socket, "Berhasil menghapus database.", strlen("Berhasil menghapus database."), 0);	
		}
		else if(info == 4){
		//Drop Table
            char table[1024];
            char * token;
            
			send(new_socket, buffer, strlen(buffer), 0);   
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=2; i++) {
				//printf( " %s\n", token );
					if(i==2){
    					printf ("table_name\n");
    					strcpy(table, buffer);
					}
    			token = strtok(NULL," ");
			}
			//int status_4 = rm(pathing(source, "/Database", result),0777);
			send(new_socket, "Berhasil menghapus table.", strlen("Berhasil menghapus table."), 0);	
		}
		else if(info == 5){
		//Drop Column
			send(new_socket, buffer, strlen(buffer), 0);
        	char column_name[1024];
            char table[1024];
            char * token;
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=4; i++) {
				//printf( " %s\n", token );
					if(i==2){
    					//printf ("Username\n");
    					strcpy(column_name, buffer);
					}
					else if(i==4){
						//printf ("Password\n");
						strcpy(table, buffer);
					}
    			token = strtok(NULL," ");
			}
			//int status_2 = mknod(pathing(source, "/Database", result),0777);
			send(new_socket, "Berhasil menghapus column.", strlen("Berhasil menghapus column."), 0);	
		}
		else if(info == 6){
		//Insert Into
			char comp[1024];
            char nama[1024];
            char publish[1024];
            char tahun[1024];
            char eks[1024];
            char pathnya[1024];
            
            send(new_socket, buffer, strlen(buffer), 0);
            
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);
            strcpy(publish,buffer);
            send(new_socket, "Tahun publikasi:", strlen("Tahun publikasi:"), 0);
            
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);
            strcpy(tahun,buffer);
            send(new_socket, "Filepath:", strlen("Filepath:"), 0);
            
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);
            send(new_socket, "Transferring..", strlen("Transferring.."), 0);
            strcpy(comp,buffer);
            
            char *ptr;
            
            ptr = comp + locate(comp,'/') + 1;
            strcpy(nama,ptr);
            
            logging(log,1,nama);
            strcpy(comp,nama);
            
            ptr = comp + locate(comp,'.') + 1;
            strcpy(eks,ptr);
            
            char *p = strtok(comp,".");
            strcpy(nama,p);
            
            sprintf(display,"%s/FILES/%s.%s",source,nama,eks);
            
            char data[1024];
            FILE *rcv;
            rcv = fopen(display,"wb");
            int n;
            while (1) {
                n = recv(new_socket, data, 1024, 0);
                if (strcmp(data,"END")==0){
                break;
                }
                fwrite(data,1,sizeof(data),rcv);
                // puts("...");
                bzero(data, 1024);
            }
            fclose(rcv);
            addtsv(files,nama,publish,tahun,eks,display);
            send(new_socket, "Berhasil melakukan insert.", strlen("Berhasil melakukan insert."), 0);
            continue;				
		}
		else if(info == 7){
		//Update
			send(new_socket, buffer, strlen(buffer), 0);
        	int status = mkdir(pathing(source, "/Database", result),0777);
        	char table[1024];
            char column_name[1024];
            char value[1024];
            char * token;
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=3; i++) {
				//printf( " %s\n", token );
					if(i==1){
    					//printf ("Username\n");
    					strcpy(table, buffer);
					}
					else if(i==3){
						//printf ("Password\n");
						strcpy(column_name, buffer);
						strcpy(value, buffer);
					}
    			token = strtok(NULL," ");
			}
			//
			send(new_socket, "Berhasil melakukan update.", strlen("Berhasil melakukan update."), 0);	
		}
		else if(info == 8){
		//Delete
        	send(new_socket, buffer, strlen(buffer), 0);
        	char table[1024];
            char * token;
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=2; i++) {
				//printf( " %s\n", token );
					if(i==2){
    					//printf ("Username\n");
    					strcpy(table, buffer);
					}
    			token = strtok(NULL," ");
			}
			//
			send(new_socket, "Berhasil melakukan delete.", strlen("Berhasil melakukan delete."), 0);	
		}
		else if(info == 9){
		//Select
        	send(new_socket, buffer, strlen(buffer), 0);
        	char column_name[1024];
            char table[1024];
            char * token;
                
            memset(buffer, 0, sizeof(buffer)); //id
            read(new_socket, buffer, 1024);
            token = strtok(buffer," ");
                
            for(int i=0; i<=3; i++) {
				//printf( " %s\n", token );
					if(i==1){
    					//printf ("Username\n");
    					strcpy(column_name, buffer);
					}
					else if(i==3){
						//printf ("Password\n");
						strcpy(table, buffer);
					}
    			token = strtok(NULL," ");
			}
			//
			send(new_socket, "Berhasil melakukan select.", strlen("Berhasil melakukan select."), 0);	
		}
		else if(info == 0){
		//Exit
				
		}
//		else if(info == 2){
//            sprintf(buffer, "Processing..");
//            send(new_socket, buffer, strlen(buffer), 0);
//            memset(buffer, 0, sizeof(buffer));
//            read(new_socket, buffer, 1024);
//            if(deletefile(files,buffer,source,result) == 1)
//            logging(log,2,buffer);
//            fclose(files);
//            remove(pathing(source, "/files.tsv", result));
//
//            char result2[100];
//            strcpy(result2,pathing(source, "/files.tsv", result));
//            rename(pathing(source, "/temp.tsv", result), result2);
//
//            files = fopen(pathing(source, "/files.tsv", result), "a+");
//            sprintf(buffer, "Processed..");
//        } else if(info == 3){
//            see(files, check, display);
//            send(new_socket, display, strlen(display), 0);
//            continue;
//        } else if(info == 4){
//            sprintf(buffer, "Finding..");
//            send(new_socket, buffer, strlen(buffer), 0);
//            memset(buffer, 0, sizeof(buffer));
//            read(new_socket, buffer, 1024);
//            find(files, check, display, buffer);
//            send(new_socket, display, strlen(display), 0);
//            continue;
//        } else if(info == 5){
//            int muncul =0;
//            
//            sprintf(buffer, "Downloading..");
//            send(new_socket, buffer, strlen(buffer), 0);
//            memset(buffer, 0, sizeof(buffer));
//            
//            read(new_socket, buffer, 1024);
//            sprintf(display,"%s/FILES/%s",source,buffer);
//            
//            char *p;
//            char na[1024],nana[1024], eks[1024];
//            
//            strcpy(na,buffer);
//            p = buffer + locate(buffer,'.');
//            strcpy(eks,p);
//            
//            char *pna = strtok(na,".");
//            strcpy(nana,pna);
//            
//            while(fgets(check, 1024 , files)) //check validity
//            {
//                int i = 0;
//                char comp[1024];
//                
//                strcpy(comp,check);
//                
//                char iter[5][1024];
//                char *ptr = strtok(comp,"\t");
//
//                while( ptr != NULL){
//                    strcpy(iter[i++],ptr);
//                    ptr = strtok(NULL,"\t");
//                }
//                
//                if (strcmp(iter[0], nana) == 0 && strcmp(iter[3], eks) == 0) 
//                {
//                    muncul++;
//                }
//            }
//            fclose(files);
//            files = fopen(pathing(source, "/files.tsv", result), "a+");
//            if(muncul == 0) send(new_socket, "END", sizeof("END"), 0);
//            if(muncul > 0){
//                FILE *fp;
//                fp = fopen(display,"rb");
//                if (fp == NULL) {
//                    perror("[-]Error in reading file.");
//                    exit(1);
//                }
//                char data[SIZE] = {0};
//                int n;
//                do{
//                    n = fread(data, 1,1024, fp);
//                    send(new_socket, data, sizeof(data), 0);
//                    // puts(",,,");
//                } while(n == sizeof(data));
//                memset(data, 0, sizeof(data));
//                fclose(fp);
//                send(new_socket, "END", sizeof("END"), 0);
//            } else {
//                sprintf(buffer, "File tidak valid, mohon dicek kembali");
//            }
//            continue;
//        }
        send(new_socket, buffer, strlen(buffer), 0);
        // fclose(files);
        // files = fopen(pathing(source, "/files.tsv", result), "a+");
    }
    
    fclose(akun);
    fclose(files);
    return 0;
	
	return 0;	
}

void logging(FILE* log,int tipe, char nama[]){
    char result[1024];
    if(tipe == 1){
        sprintf(result,"Tambah : %s (%s)\n",nama,user);//warning
        fputs(result,log);
    } else if(tipe == 2){
        sprintf(result,"Hapus : %s (%s)\n",nama,user);//warning
        fputs(result,log);
    }
    
}

int locate(char fullpath[], char sy){
    int len = strlen(fullpath);
    while(--len){
        if(fullpath[len] == sy) break;
    }
    return len;
}

int deletefile(FILE *files, char name[], char source[], char result[]){
    int flag=0;
    char check[1024];
    FILE *temp;
    temp = fopen(pathing(source, "/temp.tsv", result), "a+");
    char *p;
    char na[1024],nana[1024], eks[1024];
    strcpy(na,name);
    p = name + locate(name,'.') +1;
    strcpy(eks,p);
    char *pna = strtok(na,".");
    strcpy(nana,pna);
    // puts(nana);
    //     puts(eks);
    while(fgets(check, sizeof(check), files) != NULL)
    {
        int i = 0;
        char comp[1024];
        
        strcpy(comp,check);
        
        char iter[5][1024];
        char *ptr = strtok(comp,"\t");

        while( ptr != NULL){
            strcpy(iter[i++],ptr);
            ptr = strtok(NULL,"\t");
        }
        
        if (strcmp(iter[0], nana) == 0 && strcmp(iter[3], eks) == 0) 
        {
            flag=1;
            continue;
        }
        fputs(check, temp);
    }
    fclose(temp);

    char result3[100];
    strcpy(result3,pathing(pathing(pathing(source, "/FILES/", result), "old-", result),name,result));
    rename( pathing(pathing(source, "/FILES/", result), name, result), result3);

    return flag;
}

void addtsv(FILE *files, char nama[], char publisher[], char tahun[], char ekstensi[], char path[]){
    fputs(nama, files);
    fputc("\t", files);
    fputs(publisher, files);
    fputc("\t", files);
    fputs(tahun, files);
    fputc("\t", files);
    fputs(ekstensi, files);
    fputc("\t", files);
    fputs(path, files);
    fputc("\t", files);
}

char *pathing(char path1[], char path2[], char result[]){
    strcpy(result, path1);
    strcat(result, path2);
    return result;
}

int Aunthentication_Authorization(char buffer[], int *flag)
{
    if (strcmp(buffer, "CREATE USER") == 0) {
        sprintf(buffer, "Expecting id...");
        return 1;
    }
    else if (strcmp(buffer, "USE") == 0) {
        sprintf(buffer, "Expecting id...");
        return 2;
    }
	else if (strcmp(buffer, "GRANT PERMISSION") == 0) {
        sprintf(buffer, "Expecting id...");
        return 3;
    }
    else if (strcmp(buffer, "./program_database") == 0) {
        sprintf(buffer, "Expecting id...");
        return 3;
    }
	 
	else if(strcmp(buffer, "EXIT") == 0){
        (*flag) = 0;
        sprintf(buffer, "Exiting..");
        return 0;
    }
    return 0;
}

int DDL_DDM(char buffer[], int *flag)
{
    if (strcmp(buffer, "CREATE DATABASE") == 0) {
        //sprintf(buffer, "Publisher:");
        return 1;
    } 
	else if(strcmp(buffer, "CREATE TABLE") == 0){
        return 2;
    } 
	else if(strcmp(buffer, "DROP DATABASE") == 0){
        return 3;
    } 
	else if(strcmp(buffer, "DROP TABLE") == 0){
        return 4;
    } 
	else if(strcmp(buffer, "DROP COLUMN") == 0){
        return 5;
    } 
	else if(strcmp(buffer, "INSERT INTO") == 0){
        return 6;
    } 
	else if(strcmp(buffer, "UPDATE") == 0){
        return 7;
    } 
    else if(strcmp(buffer, "DELETE FROM") == 0){
        return 8;
    } 
    else if(strcmp(buffer, "SELECT") == 0){
        return 9;
    } 
	else if(strcmp(buffer, "EXIT") == 0){
        (*flag) = 0;
        sprintf(buffer, "Exiting..");
        return 0;
    }
    return 0;
}

//void Function_Make_Folders(pid_t child_id, int * status, char *directory_name[], char *stevany_directory_name[]) {
//
//	if((child_id = fork()) == 0) {
//		char *argv[] = {"mkdir", "-p", stevany_directory_name[0], stevany_directory_name[1],stevany_directory_name[2], NULL};
//		execv("/bin/mkdir", argv);
//	}
//	while(wait(status) > 0);
//
//}

void db_create_database(char database[]){
	//int status = mkdir(pathing(source, database, result),0777);
}

void db_create_table(char buffer[], int *flag){

}

void db_drop_database(char buffer[], int *flag){
	
}

void db_drop_table(char buffer[], int *flag){
	
}

void db_drop_column(char buffer[], int *flag){
	
}

void db_insert(char buffer[], int *flag){
	
}

void db_update(char buffer[], int *flag){
	
}

void db_delete(char buffer[], int *flag){
	
}

void db_select(char buffer[], int *flag){
	
}

void dump(char buffer[], int *flag){

}

int createServerSocket(struct sockaddr_in *address, int *addrlen)
{
    int fd, opt = 1;
 
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
 
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);
    *addrlen = sizeof(*address);
 
    if (bind(fd, (struct sockaddr *)address, *addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return fd;
}
