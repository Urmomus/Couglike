#ifndef SAVING_SYSTEM_H
#define SAVING_SYSTEM_H

#include "units.h"

/******
 * @brief структура игрока, который сохранён в статистику
 * @param name имя чемпиона
 * @param kills кол-во поверженных чемпионом врагов
 * @param level уровень чемпиона
 * @param items_num кол-во вещей, подобранных чемпионом
*/
typedef struct
{
	char *name;
	int kills;
	int level;
	int items_num;
}
Leader;

/******
 * @brief структура статистики всех сохранённых чемпионов
 * @param data список чемпионов
 * @param num_of_leaders кол-во чемпионов
*/
typedef struct
{
	Leader *data;
	int num_of_leaders;
}
TableOfLeaders;

/****
 * @brief сохраняет таблицу лидеров в файл
 * @param table таблица лидеров
 * @param filename название файла
 * @return код ошибки
*/
int save_table_to_file(TableOfLeaders *table, char *filename);

/*****
 * @brief загружает таблицу лидеров из файла
 * @param table таблица лидеров
 * @param filename название файла
 * @return код ошибки
*/
int load_table_from_file(TableOfLeaders *table, char *filename);

/******
 * @brief добавляет игрока во статистику
 * @param table статистика
 * @param unit игрок
 * @return код ошибки
*/
int add_player_to_table(TableOfLeaders *table, Unit *player);

/*****
 * @brief очищает память из-под таблицы лидеров
 * @param table таблица лидеров
 * @return код ошибки
*/
int delete_table(TableOfLeaders *table);

#endif
