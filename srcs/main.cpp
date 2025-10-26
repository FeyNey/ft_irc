/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoste <acoste@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:50:54 by acoste            #+#    #+#             */
/*   Updated: 2025/10/23 17:50:55 by acoste           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ServerClass.hpp>
#include <RequestClass.hpp>
#include <unistd.h>
#include <signal.h>

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Try ./ircserv <port> <password>" << std::endl;
		exit(1);
	}

	try {

		std::string pwd = argv[2];

		Server	serv(std::atoi(argv[1]), pwd);

		serv.launchListenSocket();

		signal(SIGQUIT, Server::signal_handler);
		signal(SIGINT, Server::signal_handler);
		//signal(SIGPIPE, SIGING); //ignore sigpipe ?

		while (serv.signal == false)
			serv.pollLoop();

		return(0);

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

//boucle sur la requete si le client se deconnecte