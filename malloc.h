/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 10:52:06 by jvaquer           #+#    #+#             */
/*   Updated: 2023/04/13 11:20:16 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <unistd.h>
# include <pthread.h>

# define HEAP_SHIFT(start) ((void *)start + sizeof(t_heap))
# define BLOCK_SHIFT(start) ((void *)start + sizeof(t_block))

# define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
# define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
# define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
# define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

typedef enum	e_bool
{
	TRUE,
	FALSE
}				t_bool;

typedef enum	e_heap_group
{
	TINY,
	SMALL,
	LARGE
}				t_heap_group;

/*
** A heap stores data about one mapped zone
*/

typedef struct		s_heap
{
	struct s_heap	*prev;
	struct s_heap	*next;
	t_heap_group	group;
	size_t			total_size;
	size_t			free_size;
	size_t			block_count;
}					t_heap;

extern pthread_mutex_t		g_ft_malloc_mutex;
extern t_heap				*g_heap_anchor;

/*
** A block stores data for each malloc call
*/

typedef struct		s_block
{
	struct s_block	*prev;
	struct s_block	*next;
	size_t			data_size;
	t_bool			freed;
}					t_block;

/*
** Methods
*/

void			*malloc(size_t size);
void			*calloc(size_t count, size_t size);
void			free(void *ptr);

t_heap_group    get_heap_group_from_block_size(size_t size);

void			set_block(t_block *tmp, size_t size);
static void     divide_block(size_t size, t_block *block, t_heap *heap);
t_block			*try_fill_free_block(size_t size);
void            find_free_block(size_t size, t_heap **in_heap, t_block **in_block);


#endif
