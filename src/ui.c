//
// Created by pusana on 8/16/26.
//

#include "gtk/gtk.h"
#include "ui.h"
#include "error.h"


static game_error_t create_main_menu_page(GtkWidget **page_out) {
    if (page_out == NULL) {
        return NULL_POINTER_ERROR;
    }
    page_out = NULL;  // prevent stale returns in case of failure

    static gint spacing = 1;  // placeholder

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, spacing);
    GtkWidget *label = gtk_label_new("gtkSweep");
    GtkWidget *button = gtk_button_new_with_label("New Game");

    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), button);

    *page_out = box;
    return GAME_ERROR_NONE;
}


static game_error_t create_game_setup_page(GtkWidget **page_out) {

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


    game_error_handler(create_main_menu_page(&main_menu_page));
    game_error_handler(create_game_setup_page(&game_setup_page));
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
