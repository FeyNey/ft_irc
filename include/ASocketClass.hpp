/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocketClass.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:11 by acoste            #+#    #+#             */
/*   Updated: 2025/10/23 17:51:12 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASOCKET_CLASS_HPP
#define ASOCKET_CLASS_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>

class ASocket {

	public :

		ASocket();
		virtual ~ASocket();

		virtual int connect() = 0;
		bool isConnected();

	protected :
		sockaddr_in _addr;
		bool		_connected;
		int			_fd;

};

#endif
