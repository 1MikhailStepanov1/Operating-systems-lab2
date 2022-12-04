#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/slab.h>

SYSCALL_DEFINE1(dentry_read, char*, file_path){
	struct path* my_path = kmalloc(sizeof(struct page), GFP_ATOMIC);

	printk(KERN_INFO "Syscall dentry_read started.");
	int error = kern_path(file_path, LOOKUP_FOLLOW, my_path);
	if (error){
		printk(KERN_INFO "Syscall ended with error.\n");
		return error;
	}

	printk(KERN_INFO "Dentry name: %s\n", my_path->dentry->d_name.name);
	printk(KERN_INFO "Dentry parent name: %s\n", my_path->dentry->d_parent->d_name.name);
	printk(KERN_INFO "Inode UID: %d  Inode GID: %d\n", my_path->dentry->d_inode->i_uid, my_path->dentry->d_inode->i_gid);
	printk(KERN_INFO "Inode flags: %d", my_path->dentry->d_inode->i_flags);
	kfree(my_path);
	return 0;
}
