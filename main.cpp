#include <gtk/gtk.h>

#include "hex_table.hex"

// HEX opacity table
const gchar* alpha_to_hex(gdouble alpha) {
    guint alpha_hex = (guint)(alpha * 127 + 0.5);

    if (alpha_hex >= 0 && alpha_hex < 128) {
        return alpha_hex_table[alpha_hex];
    } else {
        return NULL;
    }
}

// Color channel to HEX
const gchar* channel_to_hex(gdouble channel) {
    guint channel_hex = (guint)(channel * 255);
    gchar *hex_str = g_strdup_printf("%02X", channel_hex);
    return hex_str;
}

// RGBA to HEX with opacity
const gchar* color_to_hex(const GdkRGBA *color) {
    gchar *hex_str;
    const gchar* alpha_hex = alpha_to_hex(color->alpha);

    if (alpha_hex != NULL) {
        // Color to HEX with opacity
        hex_str = g_strdup_printf("0x%s%s%s%s", alpha_hex, channel_to_hex(color->red), channel_to_hex(color->green), channel_to_hex(color->blue));
    } else {
        // If transparency is outside the acceptable range, set it to 100%
        alpha_hex = "FF";
        hex_str = g_strdup_printf("0x%s%s%s%s", alpha_hex, channel_to_hex(color->red), channel_to_hex(color->green), channel_to_hex(color->blue));
    }
    return hex_str;
}

void color_dialog(GtkWidget *button, gpointer user_data) {
    GtkWidget *dialog;
    gint result;
    GdkRGBA color;
    GtkWidget *color_field = (GtkWidget*)user_data; // User color input

    dialog = gtk_color_chooser_dialog_new("Select Color", NULL);
    result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK) {
        gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(dialog), &color);

        // Color to HEX with opacity
        const gchar* hex_color = color_to_hex(&color);

        // Update label "color_field"
        gtk_label_set_text(GTK_LABEL(color_field), hex_color);

        // Clear RAM
        g_free((gpointer)hex_color);
    }

    gtk_widget_destroy(dialog);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;
    GtkWidget *color_field;

    // Vertical box
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Setup window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Color-Converter");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 215);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Label with user color in format: "0x+opacity+hex"
    color_field = gtk_label_new("Color will be displayed here");
    gtk_widget_set_size_request(color_field, 400, 100);
    gtk_label_set_selectable(GTK_LABEL(color_field), TRUE);

    // Color chooser button
    button = gtk_button_new_with_label("Open Color Chooser");
    gtk_widget_set_size_request(button, 400, 100);
    g_signal_connect(button, "clicked", G_CALLBACK(color_dialog), color_field);

    // container setup
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0); // Добавляем кнопку в контейнер без расширения и выравнивания
    gtk_box_pack_start(GTK_BOX(box), color_field, FALSE, FALSE, 0); // Добавляем метку в контейнер без расширения и выравнивания
    // container add to window
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("color.converter.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
