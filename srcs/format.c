/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itkimura <itkimura@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 21:43:45 by itkimura          #+#    #+#             */
/*   Updated: 2022/03/28 20:50:57 by itkimura         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	is_specifier(char **itr, char *str)
{
	int		i;
	char	c;

	i = 0;
	c = **itr;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void	print_format(t_format *f, va_list *ap)
{
	void	(*p_type[TYPE_NUM])(t_format *, va_list *,
			void (*p_flag[])(t_format *, char));
	void	(*p_flag[FLAG_NUM])(t_format *, char);

	p_type[TYPE_C] = print_c;
	p_type[TYPE_S] = print_s;
	p_type[TYPE_P] = print_nbr;
	p_type[TYPE_D] = print_nbr;
	p_type[TYPE_O] = print_nbr;
	p_type[TYPE_I] = print_nbr;
	p_type[TYPE_U] = print_nbr;
	p_type[TYPE_SX] = print_nbr;
	p_type[TYPE_LX] = print_nbr;
	p_type[TYPE_F] = print_f;
	p_type[TYPE_PER] = print_c;
	p_flag[NONE] = flag_none;
	p_flag[MINUS] = flag_minus;
	p_flag[ZERO] = flag_none;
	p_type[f->type](f, ap, p_flag);
}

void	put_width(char **itr, t_format *f, va_list *ap)
{
	int	nb;

	while (ft_isspace(**itr))
		(*itr)++;
	if (**itr == '*')
	{
		nb = va_arg(*ap, int);
		if (nb < 0)
		{
			nb *= -1;
			f->flag = MINUS;
		}
		f->width = nb;
		(*itr)++;
	}
	if (**itr >= '0' && **itr <= '9')
	{
		nb = 0;
		while (**itr >= '0' && **itr <= '9')
		{
			nb = nb * 10 + (**itr - '0');
			(*itr)++;
		}
		f->width = nb;
	}
}

void	put_precision(char **itr, t_format *f, va_list *ap)
{
	int	nb;

	nb = 0;
	(*itr)++;
	if (**itr == '*')
	{
		nb = va_arg(*ap, int);
		if (nb < 0)
			nb = INVALID;
		(*itr)++;
		f->precision = nb;
	}
	else if (**itr >= '0' && **itr <= '9')
	{
		while (**itr >= '0' && **itr <= '9')
		{
			nb = nb * 10 + (**itr - '0');
			(*itr)++;
		}
		f->precision = nb;
	}
	else
		f->precision = DOT_ONLY;
}

void	put_length(char **itr, t_format *f)
{
	int	i;

	i = 0;
	if (ft_strnstr(*itr, "hh", 2))
	{
		f->length[hh] = 1;
		(*itr) += 2;
	}
	else if (ft_strnstr(*itr, "ll", 2))
	{
		f->length[ll] = 1;
		(*itr) += 2;
	}
	else
	{
		i = is_specifier(itr, LEN);
		if (i > 0)
		{
			f->length[i] = 1;
			(*itr)++;
		}
	}
}
