#include <Tray.h>

Tray::Tray(FileSystemScanner* pFileSystemScanner)
  : mpFileSystemScanner(pFileSystemScanner),
    mSyncIsActive(false)
{
  set_title("Gtk::StatusIcon example");
  set_border_width(5);
  set_default_size(120, 100);
  
  // Setting up the UIManager:
  Glib::RefPtr<Gtk::ActionGroup> refActionGroup = Gtk::ActionGroup::create();
  refActionGroup->add(Gtk::ToggleAction::create("Toggle0", "enable syncronization", "start or stop syncronication", false),sigc::mem_fun(*this, &Tray::ToggleSync));
  refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),sigc::mem_fun(*this, &Tray::hide));

  mrUIManager = Gtk::UIManager::create();
  mrUIManager->insert_action_group(refActionGroup);

  Glib::ustring ui_info =
    "<ui>"
    "  <popup name='Popup'>"
    "    <menuitem action='Toggle0' />"
    "    <separator/>"
    "    <menuitem action='Quit' />"
    "  </popup>"
    "</ui>";

  mrUIManager->add_ui_from_string(ui_info);

  // Setting up the StatusIcon:
  // You should use your own icon in real life.
  mrStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::MEDIA_STOP);

  // StatusIcon's signals (GTK+)
  GtkStatusIcon* gobj_StatusIcon = mrStatusIcon->gobj();
  g_signal_connect(G_OBJECT(gobj_StatusIcon), "activate", G_CALLBACK(on_statusicon_activated), this);
  g_signal_connect(G_OBJECT(gobj_StatusIcon), "popup-menu", G_CALLBACK(on_statusicon_popup), this);

  // Buttons' signals:
  show_all_children();
}

Tray::~Tray()
{
}

void Tray::minimize()
{
  iconify();
  set_skip_taskbar_hint(); // "hide" the window
}  

void Tray::show_popup_menu(guint button, guint activate_time)
{
  Gtk::Menu* pMenu = static_cast<Gtk::Menu*>(mrUIManager->get_widget("/Popup"));
 
  if(pMenu)
    pMenu->popup(button, activate_time);
}


void Tray::on_show(){
  Gtk::Window::on_show();
  minimize();
}

void Tray::ToggleSync(){
  void* no_arg = NULL;
  if(mSyncIsActive){
    std::cerr << "\nC Stop Sync by GUI";
    mpFileSystemScanner->StopToScan();
    mSyncIsActive = false;
    SetPauseIcon();
  }
  else{
    std::cerr << "\nC Start Sync by GUI";
    mpFileSystemScanner->GetEventManager()->GetSyncManager()->SyncSourceFolder(mpFileSystemScanner->GetScanFolder());
    mpFileSystemScanner->StartToScan();
    mSyncIsActive = true;
    SetScanIcon();
  }
}

void Tray::SetPauseIcon(){
  mrStatusIcon->set(Gtk::Stock::MEDIA_STOP);
}

void Tray::SetSyncIcon(){
  mrStatusIcon->set(Gtk::Stock::REFRESH);
}

void Tray::SetScanIcon(){
  //mrStatusIcon
  //mrStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::MEDIA_PLAY);
  mrStatusIcon->set(Gtk::Stock::MEDIA_PLAY);
}

void Tray::OnEventManagerSignal(bool a, int b){
  switch(b){
  case 0:
    SetPauseIcon();
    break;
  case 1:
    SetSyncIcon();
    break;
  case 2:
    SetScanIcon();
    break;
  };
}


void on_statusicon_activated(GtkWidget* widget, gpointer object)
{
  Gtk::Window* window = static_cast<Gtk::Window*>(object);

  bool hided = window->get_skip_taskbar_hint();

  hided ? window->deiconify() : window->iconify();
  window->set_skip_taskbar_hint(not hided);
}

// This wraps the statusicon signal "popup-menu" and calls Tray::show_popup_menu()
void on_statusicon_popup(GtkStatusIcon* status_icon, guint button,
			 guint activate_time, gpointer object)
{
  return static_cast<Tray*>(object)->show_popup_menu(button, activate_time);
}

