#ifndef GTKMM_TRAY_H
#define GTKMM_TRAY_H

#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <sigc++/sigc++.h>
#include <FileSystemScanner.h>
#include <Profile.h>

class Tray : public Gtk::Window
{
public:
  Tray(vector<Profile> *pProfiles);
  virtual ~Tray();
  void OnEventManagerSignal(bool a, int b);
  //static Glib::RefPtr<Gtk::StatusIcon> mrStatusIcon;

protected:
  virtual void minimize();
  virtual void show_popup_menu(guint button, guint activate_time);
  void ToggleSync();
  friend void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);
  void on_show();
  void StopToScan();
  void StartToScan();
  void SetPauseIcon();
  void SetScanIcon();
  void SetSyncIcon();

  /* Member */
  Glib::RefPtr<Gtk::StatusIcon> mrStatusIcon;
  Glib::RefPtr<Gtk::UIManager> mrUIManager;
  vector<Profile> *mpProfiles;
  bool mSyncIsActive;

};



// Statusicon (GTK+) callbacks:
void on_statusicon_activated(GtkWidget*, gpointer object);
void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);

#endif /* GTKMM_TRAY_H */
