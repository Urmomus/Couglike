#include "Map.h"
#include "view.h"
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

void menu_logic(char *menu_mode);

int main()
{
    char menu_mode = ' ';

    menu_logic(&menu_mode);

    if (menu_mode == 'g')
    {
        int selected_item_index = 0;
        char game_mode = 'g'; // g = game, i = inventory
        char player_name[20];

        GameMap game_map;

        game_map.units_list = NULL;
        game_map.items_list = NULL;
        game_map.data = NULL;

        MapSettings settings;

        settings.size_x = 20;
        settings.size_y = 20;
        settings.level = 1;

        int err_code = init_map(&game_map, settings);
        err_code = generate_maps_landscape(&game_map);
        show_input();
        enter_name();
        attron(COLOR_PAIR(SHOW_NAME_PAIR));
        getstr(player_name);
        attron(COLOR_PAIR(SHOW_NAME_PAIR));
        hide_input();

        err_code = generate_maps_content(&game_map, player_name);

        if (strcmp(player_name, "gamemode0") == 0)
        {
            game_map.units_list[PLAYER_INDEX].hp = 999;
            game_map.units_list[PLAYER_INDEX].dmg = 999;
            game_map.units_list[PLAYER_INDEX].defense = 999;
        }

        int game_is_finished = false;

        do
        {
            if (game_map.level == 11)
            {
                game_is_finished = true;
                break;
            }
            if (game_map.units_list[PLAYER_INDEX].hp <= 0)
            {
                game_is_finished = true;
                break;
            }

            if (game_mode == 'g')
            {
                selected_item_index = 0;
                print_map(&game_map);
            }

            if (game_mode == 'i')
                print_inventory(&game_map, selected_item_index);

            char cmd;
            cmd = getch();

            if (cmd == 'i')
            {
                // входим / выходим из инвентаря
                if (game_mode == 'g')
                    game_mode = 'i';
                else
                    game_mode = 'g';
            }

            if (game_mode == 'i')
            {
                switch (cmd)
                {
                case 'w':
                {
                    if (selected_item_index - 1 >= 0)
                        --selected_item_index;
                    break;
                }
                case 's':
                {
                    if (selected_item_index + 1 < game_map.units_list[PLAYER_INDEX].inventory.current_size)
                        ++selected_item_index;
                    break;
                }
                case 'e':
                {
                    ExceptionStatus exception;
                    exception = equip_from_inventory(game_map.units_list + PLAYER_INDEX, selected_item_index);
                    if (exception == ITEM_IS_EQUIPPED)
                        unequip(game_map.units_list + PLAYER_INDEX, game_map.units_list[PLAYER_INDEX].inventory.items[selected_item_index].type);
                    break;
                };
                case 'u':
                {
                    int exit_code;
                    exit_code = use(game_map.units_list + PLAYER_INDEX, selected_item_index);
                    printw("%d\n", exit_code);
                    if (selected_item_index > 0 && exit_code != NOT_CONSUMABLE)
                        --selected_item_index;
                    break;
                };
                case 'd':
                {
                    selected_item_index = game_map.units_list[PLAYER_INDEX].inventory.current_size - 1;
                    if (selected_item_index < 0)
                        selected_item_index = 0;
                };
                default:
                {
                    continue;
                }
                }
            }

            if (game_mode == 'g')
            {
                switch (cmd)
                {
                case 'a':
                {
                    move_player(&game_map, 'l');
                    break;
                };
                case 'w':
                {
                    move_player(&game_map, 'u');
                    break;
                };
                case 's':
                {
                    move_player(&game_map, 'd');
                    break;
                };
                case 'd':
                {
                    move_player(&game_map, 'r');
                    break;
                };
                case 'c':
                {
                    move_player(&game_map, 's');
                    break;
                };
                default:
                {
                    continue;
                };
                };

                move_monsters(&game_map);
            }
        } while (!game_is_finished);

        if (game_map.units_list[PLAYER_INDEX].hp <= 0)
        {
            save_to_leaderboard(game_map.units_list + PLAYER_INDEX);
            print_death_screen(&game_map);
            getch();
        }
        else
        {
            save_to_leaderboard(game_map.units_list + PLAYER_INDEX);
            print_win_screen(&game_map);
            getch();
        }

        err_code = delete_map(&game_map);
    }
    else if (menu_mode == 'l')
    {
        print_leaderboard();
        getch();
    }
};

void menu_logic(char *menu_mode)
{
    int selected_menu_index = 0;
    *menu_mode = ' ';

    srand(time(0));
    init_ncurses();

    show_menu(0);

    while (*menu_mode == ' ')
    {
        char command;
        command = getch();
        switch (command)
        {
        case 'w':
        {
            if (selected_menu_index > 0)
                --selected_menu_index;
            show_menu(selected_menu_index);
            break;
        }
        case 's':
        {
            if (selected_menu_index < 1)
                ++selected_menu_index;
            show_menu(selected_menu_index);
            break;
        }
        default:
        {
            *menu_mode = selected_menu_index == 0 ? 'g' : 'l';
        }
        }
    }
}
