#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

// Global variables
GtkWidget *day_scale;
GtkWidget *eve_scale;
GtkWidget *night_scale;
GtkWidget *window;
GtkApplication *app;
int day_value = 6500;
int eve_value = 6500;
int night_value = 6500;
char *config_path;

// Function prototypes
static void save_values(void);
static void load_values(void);
static void on_day_scale_changed(GtkRange *range, gpointer user_data);
static void on_eve_scale_changed(GtkRange *range, gpointer user_data);
static void on_night_scale_changed(GtkRange *range, gpointer user_data);
static void on_reset_button_clicked(GtkButton *button, gpointer user_data);
static void on_window_hide(GtkWidget *widget, gpointer data);
static void activate(GtkApplication *app, gpointer user_data);
static void on_app_quit(GSimpleAction *action, GVariant *parameter, gpointer user_data);
static void on_app_show(GSimpleAction *action, GVariant *parameter, gpointer user_data);

// Define app actions
static GActionEntry app_actions[] = {
    {"quit", on_app_quit, NULL, NULL, NULL},
    {"show", on_app_show, NULL, NULL, NULL}
};

int main(int argc, char *argv[]) {
    int status;
    
    // Create config path
    const char *home_dir = g_get_home_dir();
    config_path = g_build_filename(home_dir, ".display_color_settings", NULL);
    
    // Create application using G_APPLICATION_DEFAULT_FLAGS instead of G_APPLICATION_FLAGS_NONE
    app = gtk_application_new("org.example.displaycolor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    // Add actions to the application
    g_action_map_add_action_entries(G_ACTION_MAP(app), app_actions, G_N_ELEMENTS(app_actions), app);
    
    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    // Clean up
    g_object_unref(app);
    g_free(config_path);
    
    return status;
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *reset_button;
    GError *error = NULL;
    
    // Load the UI from Glade file
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "display_color.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
        return;
    }
    
    // Get the main window
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_application_add_window(app, GTK_WINDOW(window));
    
    // Get UI elements
    day_scale = GTK_WIDGET(gtk_builder_get_object(builder, "day_scale"));
    eve_scale = GTK_WIDGET(gtk_builder_get_object(builder, "eve_scale"));
    night_scale = GTK_WIDGET(gtk_builder_get_object(builder, "night_scale"));
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    
    // Load saved values
    load_values();
    
    // Set initial values for the scales
    gtk_range_set_value(GTK_RANGE(day_scale), day_value);
    gtk_range_set_value(GTK_RANGE(eve_scale), eve_value);
    gtk_range_set_value(GTK_RANGE(night_scale), night_value);
    
    // Connect signals
    g_signal_connect(day_scale, "value-changed", G_CALLBACK(on_day_scale_changed), NULL);
    g_signal_connect(eve_scale, "value-changed", G_CALLBACK(on_eve_scale_changed), NULL);
    g_signal_connect(night_scale, "value-changed", G_CALLBACK(on_night_scale_changed), NULL);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset_button_clicked), NULL);
    g_signal_connect(window, "delete-event", G_CALLBACK(on_window_hide), NULL);
    
    // Show the window
    gtk_widget_show_all(window);
    
    // Create notification for system tray
    GNotification *notification = g_notification_new("Display Color Settings");
    g_notification_set_body(notification, "Display Color Settings is running in the background");
    g_notification_add_button(notification, "Show", "app.show");
    g_notification_add_button(notification, "Quit", "app.quit");
    g_application_send_notification(G_APPLICATION(app), "display-color-running", notification);
    g_object_unref(notification);
}

// Save values to config file
static void save_values(void) {
    FILE *config_file = fopen(config_path, "w");
    if (config_file) {
        fprintf(config_file, "%d %d %d", day_value, eve_value, night_value);
        fclose(config_file);
    }
}

// Load values from config file
static void load_values(void) {
    FILE *config_file = fopen(config_path, "r");
    if (config_file) {
        fscanf(config_file, "%d %d %d", &day_value, &eve_value, &night_value);
        fclose(config_file);
    }
}

// Callback for day scale value change
static void on_day_scale_changed(GtkRange *range, gpointer user_data) {
    day_value = gtk_range_get_value(range);
    save_values();
    printf("Day value: %d\n", day_value);
}

// Callback for evening scale value change
static void on_eve_scale_changed(GtkRange *range, gpointer user_data) {
    eve_value = gtk_range_get_value(range);
    save_values();
    printf("Eve value: %d\n", eve_value);
}

// Callback for night scale value change
static void on_night_scale_changed(GtkRange *range, gpointer user_data) {
    night_value = gtk_range_get_value(range);
    save_values();
    printf("Night value: %d\n", night_value);
}

// Callback for reset button click
static void on_reset_button_clicked(GtkButton *button, gpointer user_data) {
    day_value = 6500;
    eve_value = 6500;
    night_value = 6500;
    
    gtk_range_set_value(GTK_RANGE(day_scale), day_value);
    gtk_range_set_value(GTK_RANGE(eve_scale), eve_value);
    gtk_range_set_value(GTK_RANGE(night_scale), night_value);
    
    save_values();
}

// Hide window instead of closing the app
static void on_window_hide(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(widget);
    g_signal_stop_emission_by_name(widget, "delete-event");
    
    // Show a notification that the app is still running
    GNotification *notification = g_notification_new("Display Color Settings");
    g_notification_set_body(notification, "Display Color Settings is running in the background");
    g_notification_add_button(notification, "Show", "app.show");
    g_application_send_notification(G_APPLICATION(app), "display-color-minimized", notification);
    g_object_unref(notification);
}

// Quit action handler
static void on_app_quit(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    save_values();
    g_application_quit(G_APPLICATION(app));
}

// Show action handler
static void on_app_show(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    gtk_widget_show(window);
    gtk_window_present(GTK_WINDOW(window));
}
