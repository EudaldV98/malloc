/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 10:38:26 by jvaquer           #+#    #+#             */
/*   Updated: 2023/04/13 11:20:02 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t g_ft_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void    *my_malloc(size_t size)
{
    void    *res;
    t_heap  *heap;
    t_block *block;

    pthread_mutex_lock(&g_ft_malloc_mutex);
    //log
    if (!size)
        return (NULL);
    size = (size + 15) & ~15;
    if ((block = try_fill_free_block(size)))
        return (BLOCK_SHIFT(block));
    if (!(heap = get_heap_of_block_size((const size_t)size)))
        return NULL;
    res = append_empty_block(heap, size);
    //log
    pthread_mutex_unlock(&g_ft_malloc_mutex);
    return (res);
}

int main(void)
{
    char *lol;
    lol = malloc(sizeof(char) * 1 + 1);
    return 0;
}