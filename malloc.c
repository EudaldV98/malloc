/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:38:26 by jvaquer           #+#    #+#             */
/*   Updated: 2023/03/13 14:21:48 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t g_ft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void set_block(t_block *tmp, size_t size)
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

t_heap_group    get_heap_group_from_block_size(size_t size)
{
    if (size <= (size_t)TINY_BLOCK_SIZE)
        return TINY;
    else if (size <= (size_t)SMALL_BLOCK_SIZE)
        return SMALL;
    return LARGE;
}

void    find_free_block(size_t size, t_heap **in_heap, t_block **in_block)
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

t_block *try_fill_free_block(size_t size)
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

void    *my_malloc(size_t size)
{
    void    *res;
    t_heap  *heap;
    t_block *block;

    pthread_mutex_lock(&g_ft_malloc_mutex);
    if (!size)
        return (NULL);
    size = (size + 15) & ~15;
    if ((block = try_fill_available_block(size)))
        return (BLOCK_SHIFT(block));
    if (!(heap = get_heap_of_block_size((const size_t)size)))
        return NULL;
    res = append_empty_block(heap, size);

    pthread_mutex_unlock(&g_ft_malloc_mutex);
    return (res);
}

int main(void)
{
    char *lol;
    lol = malloc(sizeof(char) * 1 + 1);
    return 0;
}