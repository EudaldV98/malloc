/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:14:16 by jvaquer           #+#    #+#             */
/*   Updated: 2023/04/13 11:20:00 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_heap_group    get_heap_group_from_block_size(size_t size)
{
	if (size <= (size_t)TINY_BLOCK_SIZE)
		return TINY;
	else if (size <= (size_t)SMALL_BLOCK_SIZE)
		return SMALL;
	return LARGE;
}
