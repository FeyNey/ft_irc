/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocketClass.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:50:41 by acoste            #+#    #+#             */
/*   Updated: 2025/10/23 17:50:42 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ASocketClass.hpp>

ASocket::ASocket() : _connected(false), _fd(-1)
{
	std::memset(&_addr, 0, sizeof(_addr));
}

ASocket::~ASocket()
{
	close(_fd);
}

bool ASocket::isConnected()
{
	return (_connected);
}
