#include <linux/module.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define DNAME_INLINE_LEN 32


struct page_info{
	unsigned long page_flags;
	void* page_mapping;
	long vm_address;
};


struct dentry_info{
	unsigned char name[DNAME_INLINE_LEN];
	unsigned int inode_uid;
	unsigned int inode_gid;
	unsigned int inode_flags;
};


static void write_page_info(struct page_info* input_page_info){
	printf("Page flags: %d\n", input_page_info->page_flags);
	printf("Virtual address: %x, Page address: %x\n", input_page_info->vm_address, input_page_info->page_mapping);
}

static void write_dentry_info(struct dentry_info* input_dentry_info){
	int i;
	printf("Dentry name:");
	for(i=0; i<DNAME_INLINE_LEN;i++){
		printf("%u", input_dentry_info->name[i]);
	}
	printf("\nInode UID: %d  Inode GID: %d\n", input_dentry_info->inode_uid, input_dentry_info->inode_gid);
	printf("Inode flags: %d\n", input_dentry_info->inode_flags);
}

int main(){
	int ans = 0;
	int pid;
	char* file_path = malloc(sizeof(char) * 256);
	struct page_info* my_page_info = malloc(sizeof(struct page_info));
	struct dentry_info *my_dentry_info = malloc(sizeof(struct dentry_info));

	printf("Input PID:\n");
	scanf("%d", &pid);
	ans = syscall(548, pid, my_page_info);
	if (my_page_info == NULL){
		printf("Cannot find page by PID");
	} else {
		write_page_info(my_page_info);
	}
	printf("Input file path:\n");
	scanf("%s", file_path);
	printf("File path in bytes:\n");
	int i;
	for (i = 0; i < sizeof(file_path); i++){
		printf("%d", file_path[i]);
	}
	printf("\n");
	ans = syscall(549, file_path, 256, my_dentry_info);
	printf("%d\n", ans);
	write_dentry_info(my_dentry_info);
	free(my_page_info);
	free(my_dentry_info);
	return 0;
}
