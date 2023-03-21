/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edos-san <edos-san@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 15:48:35 by edos-san          #+#    #+#             */
/*   Updated: 2022/11/24 16:51:32 by edos-san         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP


#define  PASSWORD_OK(nickname) std::string(":teste 001 "+nickname+" :Welcome to server, "+nickname)
#define  PASSWORD_ERROR(nickname) std::string("NOTICE "+nickname+" :Senha incorreta.")
#define  NICKNAME_ERROR(nickname) std::string("433 *"+nickname+"* :Nickname incorreta.")

#endif