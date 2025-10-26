/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocketClass.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:17 by acoste            #+#    #+#             */
/*   Updated: 2025/10/23 17:51:18 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_SOCKETS_CLASS_HPP
#define LISTEN_SOCKETS_CLASS_HPP

#include "ASocketClass.hpp"

class ListenSocket : public ASocket {

	public:
		ListenSocket();
		~ListenSocket();

		int connect();
		void setport(int port);


	private :

	int _port;
};

#endif
