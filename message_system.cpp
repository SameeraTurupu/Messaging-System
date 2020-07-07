#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
//struct rec{
//	char x;
//	int y, z;
//};
//int main(){
//	int counter;
//	FILE *ptr_myfile;
//	struct rec my_record;
//	ptr_myfile = fopen("test.bin", "wb");
//	if (!ptr_myfile)
//	{
//		printf("Unable to open file!");
//		return 1;
//	}
//	char * x = "A\nBCD\nEFG\nHIJ";
//	for (counter = 0; counter <= 9; counter++){
//		my_record.x = x[counter];
//		fwrite(&my_record, sizeof(rec), 1, ptr_myfile);
//	}
//	fclose(ptr_myfile);
//	ptr_myfile = fopen("test.bin", "rb");
//	for (counter = 9; counter >= 0; counter--)
//	{
//		fseek(ptr_myfile, sizeof(rec)*counter, SEEK_SET);
//		fread(&my_record, sizeof(struct rec), 1, ptr_myfile);
//		printf("%c\n", my_record.x);
//	}
//	fclose(ptr_myfile);
//	_getch();
//	return 0;
//}

struct reply{
	char * reply_;	
	reply * next;
};

struct message{
	char * msg;			//Original message by user
	reply replies;	
	message * next;
};

struct categories{
	char * cat_name;	//Category name
	message * msgs;
};

struct INODE{
	/* For single user */
	int cat_count;
	categories category_lis[5];
	int ID;
};

struct user_hub{
	int user_count;
	INODE * user_list;	/*Indirect addressing*/
};

user_hub add_user_to_hub(user_hub ports, INODE user){
	ports.user_count++;
	ports.user_list = (INODE *)realloc(ports.user_list, sizeof(INODE)*ports.user_count);
	ports.user_list[ports.user_count - 1] = user;
	return ports;
}

INODE creat_inode_for_user(int id){
	INODE temp;
	temp.ID = id;
	temp.cat_count = 0;
	return temp;
}

int genID(int prev){
	if (prev < 20)
		return prev + 1;
	return -1;
}

bool create_category(INODE user){
	/*
		@Params INODE category count,category list
	*/
	if (user.cat_count == 5)
		return false;
	user.cat_count++;
	int cat_count = user.cat_count;
	printf("Enter a category name :");
	char * cat_name = (char *)malloc(sizeof(char)* 30);
	scanf("%s", cat_name);
	user.category_lis[cat_count - 1].cat_name = cat_name;
	return true;
}

int logIn(INODE sess,user_hub userHub){
	/*
		@Parms : sess -> session_manager; userHub -> users_hub
	*/
	printf("Logged in as user : %d\n", sess.ID);
	printf("[1] Create category\n[2] Show categories of %d\n[3] Show all categories", sess.ID);
	int ack;	//acknowledge number
	scanf("%d", &ack);
	switch (ack){
	case 1:{
			   bool crt = create_category(sess);
			   if (!crt){
				   printf("ERROR :: Exceeded category count(5).\n");
			   }
			   else
				   printf("Successfully created %s category\n", sess.category_lis[sess.cat_count].cat_name);
			   break;
		}
	case 2 : {
				for (int i = 0; i < sess.cat_count; i++){
					printf("%s\n", sess.category_lis[i].cat_name);
				}
		}
	case 3:{
			   //Performing pagination with buffer size : 10. Coming up with %'s as if it is divisible, we can consider it as a save point
			   int uCount = userHub.user_count, buffer = 1;
			   for (int i = 0; i < uCount; i++){
				   INODE in = userHub.user_list[i];
				   for (int j = 0; j < in.cat_count; j++){
					   if (buffer % 10 == 0){
						   printf("Press any key to continue...\n");
						   char temp = getchar();
					   }

					   buffer++;
				   }
			   }
		}
	}
	return 0;
}

void save(user_hub users){
	FILE * fp = fopen("data.bin", "wb");
	//First inserting user_hub details into bin file
	fwrite(&users, sizeof(users), 1, fp);
	fclose(fp);
}

void main(){
	//Creates the user id and login
	printf("[1] Create id\n[2] Login\n");
	int ack, prev_id = 0;
	scanf("%d", &ack);
	user_hub users;
	users.user_count = 0;
	users.user_list = NULL;
	fflush(stdin);
	int ID;
	switch (ack){
	case 1:{
			    ID = genID(prev_id);
				if (ID < 0){
					printf("Completed users count");
					break;
				}
				printf("Account created. Your id is %d\n", ID);
				INODE session = creat_inode_for_user(ID);
				users = add_user_to_hub(users, session);
			    logIn(session,users); 
				break;
		}
	case 2:{
			    printf("Enter your id :");
				scanf("%d", &ID);
				fflush(stdin);
				INODE session = creat_inode_for_user(ID);
				logIn(session,users); break;
		}
	}
	save(users);
	_getch();
}