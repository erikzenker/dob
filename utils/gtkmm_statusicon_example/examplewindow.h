// -*- c++ -*-
// examplewindow.h
//
// gtkmm's StatusIcon example

#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();
  virtual ~ExampleWindow();

protected:
  // Methods:
  virtual bool disactive_blinking();
  virtual void minimize();
  virtual void show_popup_menu(guint button, guint activate_time);

  //Signal handlers:
  virtual void on_blink_clicked();
  virtual bool on_delete_event(GdkEventAny* /* event */);

  // Child widgets:
  Gtk::VButtonBox m_ButtonBox;
  Gtk::Button m_Button_blink, m_Button_hide, m_Button_close;

  Glib::RefPtr<Gtk::StatusIcon> m_refStatusIcon;
  Glib::RefPtr<Gtk::UIManager> m_refUIManager;

  friend void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);
};

// Statusicon (GTK+) callbacks:
void on_statusicon_activated(GtkWidget*, gpointer object);
void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);

#endif // GTKMM_EXAMPLEWINDOW_H
