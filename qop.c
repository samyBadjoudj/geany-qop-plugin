/*
 * plugin.c
 *
 * Copyright 2015 Samy <samy.badjoudj@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <geanyplugin.h>

GeanyPlugin     *geany_plugin;
GeanyData       *geany_data;
static GtkWidget *main_menu_item = NULL;
static const char * PLUGIN_NAME_QOP= "Quick Open File";
static const int    WINDOW_SIZE= 500;

int launch_widget(const int window_size);

PLUGIN_VERSION_CHECK(1)
PLUGIN_SET_INFO(PLUGIN_NAME_QOP, "Quick Open File (Edit->Pref.-> Key Bindings-> ","1.0", "Samy Badjoudj <samy.badjoudj@gmail.com>");

enum
{
    KB_QUICK_OPEN_FILE,
    KB_COUNT
};
enum
{
    COL_NAME = 0,
    COL_IDX,
    NUM_COLS
} ;
struct quick_open_file
{
    GtkTreeModel        *file_list;
    GtkTreeModel        *file_list_filter ;
    GtkTreeSelection    *tree_selection;
    GtkWidget           *window;
    GtkWidget           *view;
    GtkWidget           *box;
    GtkWidget           *text_entry;

} quick_open_file;

static void item_activate_cb(GtkMenuItem *menuitem, gpointer gdata)
{
    launch_widget(WINDOW_SIZE);
}
static void kb_activate(G_GNUC_UNUSED guint key_id)
{
    launch_widget(WINDOW_SIZE);
}
static void qop_launch(G_GNUC_UNUSED guint key_id)
{

    launch_widget(WINDOW_SIZE);
}
void plugin_init(GeanyData *data)
{
    main_menu_item = gtk_menu_item_new_with_mnemonic(PLUGIN_NAME_QOP);
    gtk_widget_show(main_menu_item);
    gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu_item);
    GeanyKeyGroup *key_group = plugin_set_key_group(geany_plugin,"quick_open_file", KB_COUNT, NULL);
    keybindings_set_item(key_group, KB_QUICK_OPEN_FILE, kb_activate, 0,0,"quick_open_file", PLUGIN_NAME_QOP, NULL);
    g_signal_connect(main_menu_item, "activate",G_CALLBACK(item_activate_cb), geany);
}
void plugin_cleanup(void)
{
    gtk_widget_destroy(main_menu_item);
}


static gboolean callback_button_press (GtkWidget *widget, GdkEventButton *event, gpointer   data)
{
    struct quick_open_file * qop = data;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    if (event->button == 1) {
        if (gtk_tree_selection_get_selected(qop->tree_selection, &model, &iter))
        {

            gchar *  name = NULL;
            gtk_tree_model_get (model, &iter, COL_NAME, &name, -1);
            GeanyDocument * current_doc = document_find_by_real_path(name);
            document_open_file(current_doc->file_name, FALSE, NULL, NULL);
        }
        gtk_widget_destroy(qop->window);
        g_free(qop);
    }

    return FALSE;
}

static gboolean callback_key_press (GtkWidget *widget, GdkEventKey  *event, gpointer   data)
{
    struct quick_open_file * qop = data;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    switch(event->keyval)
    {
    case 65307://escape
        gtk_widget_destroy(qop->window);
        g_free(qop);
        break;
    case 65293://enter

        if (gtk_tree_selection_get_selected(qop->tree_selection, &model, &iter))
        {

            gchar *  name = NULL;
            gtk_tree_model_get (model, &iter, COL_NAME, &name, -1);
            GeanyDocument * current_doc = document_find_by_real_path(name);
            document_open_file(current_doc->file_name, FALSE, NULL, NULL);
        }
        gtk_widget_destroy(qop->window);
        g_free(qop);
        break;
    default:
        return FALSE;

    }

    return FALSE;
}

static gboolean callback_display_element (GtkTreeModel *model,GtkTreeIter  *iter, gpointer data)
{
    gchar *str;
    struct quick_open_file * qop = data;
    gboolean visible = FALSE;
    gtk_tree_model_get (model, iter, COL_NAME, &str, -1);
    const gchar * text_value = gtk_entry_get_text(GTK_ENTRY(qop->text_entry));

    if (!text_value || g_strcmp0(text_value,"")==0 || (str && g_str_match_string (text_value, str, TRUE)))
    {
        visible = TRUE;
    }
    g_free (str);

    return visible;
}


static int callback_update_visibilty_elements(GtkWidget *widget, gpointer   data )
{
    struct quick_open_file * qop = data;
    gtk_tree_model_filter_refilter(GTK_TREE_MODEL_FILTER(qop->file_list_filter));
    return 0;
}


static void create_and_fill_model (struct quick_open_file *qop)
{
    GtkListStore  *store;
    GtkTreeIter    iter;
    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

    int i;
    for ( i= 0; i < geany->documents_array->len; i++)
    {
        GeanyDocument *current_doc = g_ptr_array_index (geany->documents_array, i);

       if(current_doc->file_name){
                gtk_list_store_append (store, &iter);
                gtk_list_store_set (store, &iter, COL_NAME, current_doc->file_name, -1);
        }

    }

    qop->file_list = GTK_TREE_MODEL (store);
}
static void  create_view_and_model (struct quick_open_file *qop)
{
    GtkCellRenderer     *renderer;
    qop->view = gtk_tree_view_new ();
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (qop->view), -1, "file_name", renderer, "text", COL_NAME, NULL);
    create_and_fill_model (qop);
    qop->file_list_filter = gtk_tree_model_filter_new (qop->file_list,NULL);
    gtk_tree_view_set_model (GTK_TREE_VIEW (qop->view), qop->file_list_filter);
    gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(qop->file_list_filter),callback_display_element,qop,NULL);
    g_object_unref (qop->file_list_filter);

}

static gboolean quit_qop (GtkWidget *widget,GdkEvent  *event,gpointer   data)
{
    return FALSE;
}

int launch_widget(const int window_size)
{

    struct quick_open_file * qop =  g_malloc(sizeof(quick_open_file));
    qop->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    qop->box = gtk_vbox_new(0,0);
    qop->text_entry = gtk_entry_new();
    create_view_and_model (qop);
    g_signal_connect (qop->window, "delete_event", G_CALLBACK(quit_qop), qop);
    g_signal_connect (qop->window, "key-press-event", G_CALLBACK(callback_key_press), qop);
    g_signal_connect (qop->window, "button-press-event", G_CALLBACK(callback_button_press), qop);
    qop->tree_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(qop->view));
    g_signal_connect (qop->text_entry, "changed", G_CALLBACK(callback_update_visibilty_elements), qop);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(qop->view),FALSE);
    GtkWidget * fixed = gtk_fixed_new ();
    GtkWidget * scrolled_win = gtk_scrolled_window_new(NULL,NULL);
    gtk_fixed_put(GTK_FIXED(fixed),qop->text_entry,0,0);
    gtk_window_set_resizable  (GTK_WINDOW(qop->window),FALSE);
    gtk_widget_set_size_request (scrolled_win,window_size,window_size);
    gtk_widget_set_size_request (qop->text_entry,window_size,25);
    gtk_widget_set_size_request (qop->window,window_size,window_size);
    gtk_container_add(GTK_CONTAINER(scrolled_win), qop->view);
    gtk_fixed_put(GTK_FIXED(fixed),scrolled_win,0,25);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(qop->window), fixed);
    gtk_widget_show_all (qop->window);

    return 0;
}


