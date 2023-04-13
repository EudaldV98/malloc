/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:17:07 by jvaquer           #+#    #+#             */
/*   Updated: 2023/04/13 11:19:44 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void            set_block(t_block *tmp, size_t size)
{
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->data_size = size;
	tmp->freed = FALSE;
}

static void     divide_block(size_t size, t_block *block, t_heap *heap)
{
	t_block *free_block;

	free_block = BLOCK_SHIFT(block) + size;
	set_block(free_block, block->next - free_block);
	free_block->freed = TRUE;
	free_block->prev = block;
	free_block->next = block->next;
	heap->block_count++;
	block->next = free_block;
	block->data_size = size;
	block->freed = FALSE;
}

t_block         *try_fill_free_block(size_t size)
{
	t_block *block;
	t_heap  *heap;

	find_free_block(size, &heap, &block);
	if (block && heap)
	{
		divide_block(size, block, heap);
		return block;
	}
	return NULL;
}

void            find_free_block(size_t size, t_heap **in_heap, t_block **in_block)
{
	t_block         *block;
	t_heap          *heap;
	t_heap_group    group;

	heap = g_heap_anchor;
	group = get_heap_group_from_block_size(size);
	while (heap)
	{
		block = (t_block *)HEAP_SHIFT(heap);
		while (heap->group == group && block)
		{
			if (block->freed && (block->data_size >= size + sizeof(t_block)))
			{
				*in_heap = heap;
				*in_block = block;
				return ;
			}
			block = block->next;
		}
		heap = heap->next;
	}
	*in_heap = NULL;
	*in_block = NULL;
}
