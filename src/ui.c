//
// Created by pusana on 8/16/26.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include "ui.h"
#include "error.h"

#define MAX_GRID_SIZE  256


typedef struct {
    GtkWidget *stack;
    GtkWidget *width;
    GtkWidget *height;
    GtkWidget *mine_count;
    GtkWidget *seed;
} ui_setup_t;


static ui_setup_t ui_setup;


static void on_new_game_clicked(GtkButton *button, gpointer user_data) {
    gtk_stack_set_visible_child_name(GTK_STACK(user_data), "game setup");
}


static game_error_t create_main_menu_page(GtkWidget **page_out, GtkStack *stack) {
    if (page_out == NULL || stack == NULL) {
        return NULL_POINTER_ERROR;
    }
    *page_out = NULL; // prevent stale returns in case of failure

    const gint spacing = 1; // placeholder

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    GtkWidget *label = gtk_label_new("gtkSweep");
    GtkWidget *button = gtk_button_new_with_label("New Game");

    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), button);

    g_signal_connect(button,
                     "clicked",
                     G_CALLBACK(on_new_game_clicked),
                     stack);

    *page_out = box;
    return GAME_ERROR_NONE;
}


static void on_start_clicked(GtkButton *button, gpointer user_data) {
    ui_setup_t *setup = user_data;
    uint64_t *seed = NULL;

    size_t width =
        gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(setup->width));
    size_t height =
        gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(setup->height));
    size_t mine_count =
        gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(setup->mine_count));

    const char *seed_str =  gtk_editable_get_text(GTK_EDITABLE(setup->seed));


    if (mine_count >= width * height) {
        // reject
    }
    if (seed_str == NULL) {
        uint64_t seed = ;
    }
    else {
        uint64_t seed = strtoull(seed_str, NULL, 0);
    }


}


static void on_back_clicked(GtkButton *button, gpointer user_data) {
    ui_setup_t *setup = user_data;

    gtk_stack_set_visible_child_name(
        GTK_STACK(setup->stack),
        "main menu"
    );
}


static game_error_t create_game_setup_page(GtkWidget **page_out, GtkStack *stack) {
    if (page_out == NULL) {
        return NULL_POINTER_ERROR;
    }
    *page_out = NULL; // prevent stale returns in case of failure

    const gint spacing = 1; // placeholder

    GtkWidget *page_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, spacing);

    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *back_button = gtk_button_new_with_label("Back");

    GtkWidget *grid = gtk_grid_new();

    GtkWidget *width = gtk_spin_button_new_with_range(3, MAX_GRID_SIZE, 1);
    GtkWidget *width_label = gtk_label_new("Width");

    GtkWidget *height = gtk_spin_button_new_with_range(3, MAX_GRID_SIZE, 1);
    GtkWidget *height_label = gtk_label_new("Height");

    GtkWidget *mine_count =
            gtk_spin_button_new_with_range(1, MAX_GRID_SIZE * MAX_GRID_SIZE - 1, 1);
    GtkWidget *mine_label = gtk_label_new("Mines");

    GtkWidget *seed = gtk_entry_new();
    GtkWidget *seed_label = gtk_label_new("Seed");

    ui_setup.width = width;
    ui_setup.height = height;
    ui_setup.mine_count = mine_count;
    ui_setup.seed = seed;
    ui_setup.stack = GTK_WIDGET(stack);

    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), mine_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mine_count, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), seed_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), seed, 1, 3, 1, 1);


    gtk_box_append(GTK_BOX(page_box), grid);

    gtk_box_append(GTK_BOX(button_box), start_button);
    gtk_box_append(GTK_BOX(button_box), back_button);

    gtk_box_append(GTK_BOX(page_box), button_box);


    *page_out = page_box;
    return GAME_ERROR_NONE;
}


static game_error_t create_game_page(GtkWidget **page_out) {
}


game_error_t ui_init(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "gtkSweep");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *stack = gtk_stack_new();
    gtk_window_set_child(GTK_WINDOW(window), stack);

    GtkWidget *main_menu_page = NULL;
    GtkWidget *game_setup_page = NULL;
    GtkWidget *game_page = NULL;

    game_error_handler(create_main_menu_page(&main_menu_page, GTK_STACK(stack)));
    game_error_handler(create_game_setup_page(&game_setup_page, GTK_STACK(stack)));
    game_error_handler(create_game_page(&game_page));

    gtk_stack_add_named(GTK_STACK(stack), main_menu_page, "main menu");
    gtk_stack_add_named(GTK_STACK(stack), game_setup_page, "game setup");
    gtk_stack_add_named(GTK_STACK(stack), game_page, "game");

    if (main_menu_page == NULL || game_setup_page == NULL
        || game_page == NULL) {
        return UI_CREATION_ERROR;
    }


    gtk_window_present(GTK_WINDOW(window));
}


game_error_t ui_show_page() {
}
