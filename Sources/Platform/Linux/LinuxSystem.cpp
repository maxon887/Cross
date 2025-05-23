#include "LinuxSystem.h"

#include <sys/time.h>

#include <gtk/gtk.h>

using namespace cross;

String LinuxSystem::AssetsPath() {
	return "Assets/";
}

String LinuxSystem::DataPath() {
	return "Data/";
}

U64 LinuxSystem::GetTime() {
	struct timeval ptv{};
	gettimeofday(&ptv, NULL);
	return (ptv.tv_usec + ptv.tv_sec * 1000000LL);
}

bool LinuxSystem::Alert(const String& msg) {
	if (!gtk_init_check(0, nullptr))
	{
		return false;
	}

	// Create a parent window to stop gtk_dialog_run from complaining
	GtkWidget* parent = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
											   GTK_DIALOG_MODAL,
											   GTK_MESSAGE_ERROR,
											   GTK_BUTTONS_OK_CANCEL,
											   "%s",
											   msg.ToCStr());
	gtk_window_set_title(GTK_WINDOW(dialog), "Something goes wrong");

	gtk_window_set_gravity(GTK_WINDOW(parent), GDK_GRAVITY_CENTER);
	gtk_window_set_gravity(GTK_WINDOW(dialog), GDK_GRAVITY_CENTER);
	gtk_window_set_position(GTK_WINDOW(parent), GTK_WIN_POS_CENTER);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

	gint resonce = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_dialog_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);

	gtk_widget_destroy(GTK_WIDGET(dialog));
	gtk_widget_destroy(GTK_WIDGET(parent));
	while (g_main_context_iteration(nullptr, false))


	if(resonce == GTK_RESPONSE_OK){
		return true;
	} else {
		*((volatile unsigned int*)0) = 0xDEAD;
	}
}