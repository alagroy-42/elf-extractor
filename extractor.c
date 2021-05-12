/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extractor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alagroy- <alagroy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/11 16:02:12 by alagroy-          #+#    #+#             */
/*   Updated: 2021/05/12 09:15:27 by alagroy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static void	print_human_readable_code(void *file, Elf64_Shdr *code)
{
	char		*text;
	uint64_t	size;
	uint64_t	i;

	i = -1;
	size = code->sh_size;
	text = file + code->sh_offset;
	printf(".text section : \n");
	while (++i < size)
		printf("\\x%02hhx", text[i]);
	printf("\n");
}

static void	*find_text_section(void *file)
{
	Elf64_Ehdr	*hdr;
	Elf64_Shdr	*section;
	uint16_t	index;
	uint16_t	nb_sects;
	char		*strtab;
	uint16_t	i;

	i = -1;
	hdr = file;
	section = file + hdr->e_shoff;
	nb_sects = hdr->e_shnum;
	index = hdr->e_shstrndx;
	if (index == SHN_UNDEF)
		return (NULL);
	if (index == SHN_LORESERVE)
		index = section->sh_link;
	strtab = file + section[index].sh_offset;
	while (++i < nb_sects)
		if (!strcmp(".text", strtab + section[i].sh_name))
			return (section + i);
	return (NULL);
}

int			main(int ac, char **av)
{
	int			fd;
	off_t		filesize;
	void		*file;
	Elf64_Shdr	*text;
	char		magic[] = {'\x7f','E','L','F', '\0'};

	errno = 0;
	if (ac != 2)
	{
		dprintf(2, "extractor : wrong arguments number\n\tusage : ./extractor filaname\n");
		exit(EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) == -1
		|| (filesize = lseek(fd, 0, SEEK_END)) == (off_t)-1
		|| (file = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED
		|| strncmp(magic, file, 4))
	{
		printf("fd : %d, filesize : %lu, file : %p\n", fd, filesize, file);
		perror("extractor");
		exit(EXIT_FAILURE);
	}
	if (!(text = find_text_section(file)))
		exit(EXIT_FAILURE);
	print_human_readable_code(file, text);
	close(fd);
	munmap(file, filesize);
}