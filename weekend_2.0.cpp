/*  Weekend

  Вычисление выходных и рабочих дней при графике работы 2/2
  (Отсчёт ведётся с 1 февраля 2013 г. - выходной день)
  Version 2.0 - Beta

  Copyright 2013 Konstantin Zyryanov <post.herzog@gmail.com>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301, USA.
*/

#include <ctime>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

int calendar(int data), single_date(int data, bool ask_date), help(char *program_name), usage(char *program_name);

char *result_mon(tm *user_time), *result_wday(tm *user_time);

int day, month, year, data;
time_t rawtime;

tm *user_time;

int main(int argc, char **argv)
{
//Получение данных для текущей даты
	time(&rawtime);
	user_time=localtime(&rawtime);
	user_time->tm_hour=0;
	user_time->tm_min=0;
	user_time->tm_sec=0;
	data=mktime(user_time);
	
//Считывание позиционных параметров
	bool ask_date=false;
	if (argc == 1)
	{
		ask_date=true;
		single_date(data, ask_date);
		return 0;
	}
	char arg_token[4][10], token_day[2], token_month[2], token_year[4], *arg_point;
	if (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--date"))
	{
		int i=0;
		if (argv[2])
		{
			arg_point=strtok(argv[2], " ,.-");
//FIXME: incomplete sequence в arg_token[0]
			strcpy(arg_token[i], arg_point);
			while (arg_point != NULL)
			{
				i++;
				arg_point=strtok(NULL, " ,.-");
				if (arg_point == NULL)
					break;
				strcpy(arg_token[i], arg_point);
			}
		}
		switch (i)
		{
//TODO: проверить на неправильных и больших датах (54.14.2014, 4000 г. и т. д.)
			case 3:
				if (strlen(arg_token[2]) == 4)
				{
					strncpy(token_year, arg_token[2], 4);
					user_time->tm_year=atoi(token_year)-1900;
				}
				else if (strlen(arg_token[2]) == 2)
				{
					strncpy(token_year, arg_token[2], 2);
					user_time->tm_year=atoi(token_year)+100;
				}
				else
				{
//TODO: Продолжить выполнение case или сразу отправлять в single_date ?
					printf("Не указано что-либо определённое - будет выведен результат для текущей даты/n");
					single_date(data, ask_date);
					return 0;
				}
			case 2:
				strncpy(token_month, arg_token[1], 2);
				if (atoi(token_month) > 12)
				{
					printf("Не указано что-либо определённое - будет выведен результат для текущей даты/n");
					single_date(data, ask_date);
					return 0;
				}
				user_time->tm_mon=atoi(token_month)-1;
			case 1:
				strncpy(token_day, arg_token[0], 2);
				user_time->tm_mday=atoi(token_day);
		}
		data=mktime(user_time);
		single_date(data, ask_date);
	}
	else if (!strcmp(argv[1], "-c") || !strcmp(argv[1], "--calendar"))
	{
		int i=0;
		if (argv[2])
		{
			arg_point=strtok(argv[2], " ,.-");
//FIXME: incomplete sequence в arg_token[0]
			strcpy(arg_token[i], arg_point);
			while (arg_point != NULL)
			{
				i++;
				arg_point=strtok(NULL, " ,.-");
				if (arg_point == NULL)
					break;
				strcpy(arg_token[i], arg_point);
			}
		}
		switch (i)
		{
			case 2:
				if (strlen(arg_token[1]) == 4)
				{
					strncpy(token_year, arg_token[1], 4);
					user_time->tm_year=atoi(token_year)-1900;
				}
				else if (strlen(arg_token[1]) == 2)
				{
					strncpy(token_year, arg_token[1], 2);
					user_time->tm_year=atoi(token_year)+100;
				}
				else
				{
					printf("Напишите год в формате YYYY или YY");
					return 1;
				}
			case 1:
				strncpy(token_month, arg_token[0], 2);
				if (atoi(token_month) > 12)
				{
					printf("Укажите месяц числом от 1 (или 01) до 12");
					return 1;
				}
				user_time->tm_mon=atoi(token_month)-1;
		}
		user_time->tm_mday=1;
		data=mktime(user_time);
		calendar(data);
	}
	else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
	{
		char *program_name=argv[0];
		help(program_name);
	}
	else
	{
		char *program_name=argv[0];
		usage(program_name);
	}
	return 0;
}

//Функция вывода краткого списка возможных параметров
int usage(char *program_name)
{
	printf("Использование: \n");
	printf("%s [ [-d|--date [ДАТА]] | [-c|--calendar [МЕСЯЦ]] ] | [-h|--help]\n\n", program_name);
	return 0;
}

//Функция вывода краткой справки
int help(char *program_name) {
	printf("\nИспользование: \n");
	printf("%s [ [-d|--date [ДАТА]] | [-c|--calendar [МЕСЯЦ]] ] | [-h|--help]\n", program_name);
	printf("  -d|--date [ДАТА]		Вывод результата на определённую пользователем\n");
	printf("				ДАТУ, заданную в формате ДД.ММ.ГГ.\n");
	printf("				При указании параметра с неполной датой или\n");
	printf("				без даты - подставляется текущее значение\n");
	printf("				из календаря системы.\n\n");
	printf("  -c|--calendar [МЕСЯЦ]		Вывод результата в виде календаря\n");
	printf("				на определённый пользователем МЕСЯЦ,\n");
	printf("				заданный в формате ММ или ММ.ГГ.\n");
	printf("				Если не указаны определённый месяц или год -\n");
	printf("				подставляется текущее значение\n");
	printf("				из календаря системы.\n\n");
	printf("  -h|--help			Показать эту справку и выйти.\n\n");
	return 0;
}


//функция просчёта и вывода результата для одиночной даты
int single_date (int data, bool ask_date)
{
	int i=0;
//Запрос даты, если она не была задана при вызове программы
	if (ask_date)
	{
		char str[80], arg_token[4][10], token_day[2], token_month[2], token_year[4], *arg_point;
		printf("Введите дату [ДД.ММ.ГГ]: ");
		gets(str);
		if (strlen(str))
		{
			arg_point=strtok(str, " ,.-");
//FIXME: incomplete sequence в arg_token[0]
			strcpy(arg_token[i], arg_point);
			while (arg_point != NULL)
			{
				i++;
				arg_point=strtok(NULL, " ,.-");
				if (arg_point == NULL)
					break;
				strcpy(arg_token[i], arg_point);
			}
		}
		switch (i)
		{
//TODO: проверить на неправильных и больших датах (54.14.2014, 4000 г. и т. д.)
			case 3:
				if (strlen(arg_token[2]) == 4)
				{
					strncpy(token_year, arg_token[2], 4);
					user_time->tm_year=atoi(token_year)-1900;
				}
				else if (strlen(arg_token[2]) == 2)
				{
					strncpy(token_year, arg_token[2], 2);
					user_time->tm_year=atoi(token_year)+100;
				}
				else
				{
					printf("Не указано что-либо определённое - будет выведен результат для текущей даты\n");
					break;
				}
			case 2:
				strncpy(token_month, arg_token[1], 2);
				if (atoi(token_month) > 12)
				{
					printf("Не указано что-либо определённое - будет выведен результат для текущей даты\n");
					break;
				}
				user_time->tm_mon=atoi(token_month)-1;
			case 1:
				strncpy(token_day, arg_token[0], 2);
				user_time->tm_mday=atoi(token_day);
		}
		data=mktime(user_time);
	}
//Проверить нужность сборки user_time здесь (и правильность)
	time_t usertime=data;
	user_time=localtime(&usertime);
//Вычисление перебором от 1 февраля 2013 г. по суткам
	int difference;
	char result[9];
	difference=data-1359655200;
	difference=difference/86400+1;
	strcpy(result, "р");
	for (i=0; i < difference; i=i+2)
	{
		if (!strcmp(result, "в"))
		{
			strcpy(result, "р");
			continue;
		}
		if (!strcmp(result, "р"))
		{
			strcpy(result, "в");
			continue;
		}
	}
	if (!strcmp(result, "в"))
		strcpy(result, "Выходной");
	if (!strcmp(result, "р"))
		strcpy(result, "Рабочий");
//Вывод результата
	printf("\n%.2i %s %i\n%s\n\n%s\n\n", user_time->tm_mday, result_mon(user_time), user_time->tm_year+1900, result_wday(user_time), result);
	return 0;
}

//Функция просчёта и вывода графика для заданного месяца в виде календаря
int calendar(int data)
{
//Проверить нужность сборки user_time здесь (и правильность)
	time_t usertime=data;
	user_time=localtime(&usertime);
	int month_begin_with=user_time->tm_wday;
	int month_last_day, difference, i, j;
	int printed_day=1;
	char result[1];
	if (!month_begin_with)
		month_begin_with=7;
//Вычисление количества дней в заданном месяце
	switch (user_time->tm_mon)
	{
		case 0:
		case 2:
		case 4:
		case 6:
		case 7:
		case 9:
		case 11:
			month_last_day=31;
			break;
		case 3:
		case 5:
		case 8:
		case 10:
			month_last_day=30;
			break;
		case 1:
			if (user_time->tm_year%4)
				month_last_day=28;
			else
				month_last_day=29;
			break;
	}
	printf("\n%s %i:\n\n", result_mon(user_time), user_time->tm_year+1900);
	printf("ПН\tВТ\tСР\tЧТ\tПТ\tСБ\tВС\n");
//Циклическое вычисление перебором от 1 февраля 2013 г. по суткам
	while (month_last_day)
	{
		for (j=0; j < 7; j++)
		{
			if (month_begin_with > 1)
			{
				printf("\t");
				month_begin_with--;
				continue;
			}
			if (!month_last_day)
				break;
			difference=data-1359655200;
			difference=difference/86400+1;
			strcpy(result, "р");
			for (i=0; i < difference; i=i+2)
			{
				if (!strcmp(result, "в"))
				{
					strcpy(result, "р");
					continue;
				}
				if (!strcmp(result, "р"))
				{
					strcpy(result, "в");
					continue;
				}
			}
//Вывод результата по одному дню за цикл
			printf("%2i%s\t", printed_day, result);
			data=data+86400;
			printed_day++;
			month_last_day--;
		}
		printf("\n");
	}		
	printf("\n");
	return 0;
}

//Подстановка месяца в текстовом виде вместо числового
char *result_mon(tm *user_time)
{
//FIXME: подобрать оптимальую величину массива result_month[]
	static char result_month[20], *p;
	switch (user_time->tm_mon)
	{
		case 0:
			strcpy(result_month, "Январь");
			break;
		case 1:
			strcpy(result_month, "Февраль");
			break;
		case 2:
			strcpy(result_month, "Март");
			break;
		case 3:
			strcpy(result_month, "Апрель");
			break;
		case 4:
			strcpy(result_month, "Май");
			break;
		case 5:
			strcpy(result_month, "Июнь");
			break;
		case 6:
			strcpy(result_month, "Июль");
			break;
		case 7:
			strcpy(result_month, "Август");
			break;
		case 8:
			strcpy(result_month, "Сентябрь");
			break;
		case 9:
			strcpy(result_month, "Октябрь");
			break;
		case 10:
			strcpy(result_month, "Ноябрь");
			break;
		case 11:
			strcpy(result_month, "Декабрь");
			break;
	}
	p=result_month;
	return p;
}

//Подстановка дня недели в текстовом виде вместо числового
char *result_wday(tm *user_time)
{
//FIXME: подобрать оптимальую величину массива result_weekday[]
	static char result_weekday[30], *p;
	switch (user_time->tm_wday)
	{
		case 0:
			strcpy(result_weekday, "Воскресенье");
			break;
		case 1:
			strcpy(result_weekday, "Понедельник");
			break;
		case 2:
			strcpy(result_weekday, "Вторник");
			break;
		case 3:
			strcpy(result_weekday, "Среда");
			break;
		case 4:
			strcpy(result_weekday, "Четверг");
			break;
		case 5:
			strcpy(result_weekday, "Пятница");
			break;
		case 6:
			strcpy(result_weekday, "Суббота");
			break;	
	}
	p=result_weekday;
	return p;
}
