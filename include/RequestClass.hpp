/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestClass.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:51:20 by acoste            #+#    #+#             */
/*   Updated: 2025/10/26 17:43:04 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_CLASS_HPP
#define REQUEST_CLASS_HPP

#include <map>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <vector>


class ClientSocket;

class Request {

	public :
		Request();
		~Request();

		void	show();
		size_t	size();
		void	receive(int fd, ClientSocket *client);
		int		ft_find(std::string str, char c);
		void	disconnected();
		std::string getCmd();
		std::string getArgs();
		void		clear();
		std::vector<std::string> split(std::string str);
		bool	getmsg_end();


	private :
		char	_buffer[4096];
		std::vector<std::string>	_requests;
		std::vector<std::string>	_cmds;
		std::vector<std::string>	_args;
		bool						_msg_end;


};

#endif
