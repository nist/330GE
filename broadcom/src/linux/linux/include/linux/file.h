/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * Wrapper functions for accessing the file_struct fd array.
 */

#ifndef __LINUX_FILE_H
#define __LINUX_FILE_H

extern void FASTCALL(fput(struct file *));
extern struct file * FASTCALL(fget(unsigned int fd));
 
static inline int get_close_on_exec(unsigned int fd)
{
	struct files_struct *files = current->files;
	int res;
	read_lock(&files->file_lock);
	res = FD_ISSET(fd, files->close_on_exec);
	read_unlock(&files->file_lock);
	return res;
}

static inline void set_close_on_exec(unsigned int fd, int flag)
{
	struct files_struct *files = current->files;
	write_lock(&files->file_lock);
	if (flag)
		FD_SET(fd, files->close_on_exec);
	else
		FD_CLR(fd, files->close_on_exec);
	write_unlock(&files->file_lock);
}

static inline struct file * fcheck_files(struct files_struct *files, unsigned int fd)
{
	struct file * file = NULL;

	if (fd < files->max_fds)
		file = files->fd[fd];
	return file;
}

/*
 * Check whether the specified fd has an open file.
 */
static inline struct file * fcheck(unsigned int fd)
{
	struct file * file = NULL;
	struct files_struct *files = current->files;

	if (fd < files->max_fds)
		file = files->fd[fd];
	return file;
}

extern void put_filp(struct file *);

extern int get_unused_fd(void);

static inline void __put_unused_fd(struct files_struct *files, unsigned int fd)
{
	FD_CLR(fd, files->open_fds);
	if (fd < files->next_fd)
		files->next_fd = fd;
}

static inline void put_unused_fd(unsigned int fd)
{
	struct files_struct *files = current->files;

	write_lock(&files->file_lock);
	__put_unused_fd(files, fd);
	write_unlock(&files->file_lock);
}

void fd_install(unsigned int fd, struct file * file);
void put_files_struct(struct files_struct *fs);

#endif /* __LINUX_FILE_H */
