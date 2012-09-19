#include <Tray.h>

Tray::Tray(FileSystemScanner* pFileSystemScanner)
  : m_ButtonBox(Gtk::BUTTONBOX_SPREAD, 2),
    m_Button_blink("blink"),
    m_Button_hide("hide"),
    m_Button_close("close"),
    mpFileSystemScanner(pFileSystemScanner),
    mSyncIsActive(false)
{
  set_title("Gtk::StatusIcon example");
  set_border_width(5);
  set_default_size(120, 100);
  
  // Setting up the UIManager:
  Glib::RefPtr<Gtk::ActionGroup> refActionGroup = Gtk::ActionGroup::create();
  refActionGroup->add(Gtk::ToggleAction::create("Toggle0", "enable syncronization", "start or stop syncronication", false),sigc::mem_fun(*this, &Tray::start));
  refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),sigc::mem_fun(*this, &Tray::hide));

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(refActionGroup);

  Glib::ustring ui_info =
    "<ui>"
    "  <popup name='Popup'>"
    "    <menuitem action='Toggle0' />"
    "    <separator/>"
    "    <menuitem action='Quit' />"
    "  </popup>"
    "</ui>";

  m_refUIManager->add_ui_from_string(ui_info);

  // Setting up the StatusIcon:
  // You should use your own icon in real life.
  m_refStatusIcon = Gtk::StatusIcon::create(Gtk::Stock::APPLY);

  // StatusIcon's signals (GTK+)
  GtkStatusIcon* gobj_StatusIcon = m_refStatusIcon->gobj();
  g_signal_connect(G_OBJECT(gobj_StatusIcon), "activate", G_CALLBACK(on_statusicon_activated), this);
  g_signal_connect(G_OBJECT(gobj_StatusIcon), "popup-menu", G_CALLBACK(on_statusicon_popup), this);

  // Buttons' signals:
  m_Button_blink.signal_clicked().connect(sigc::mem_fun(*this, &Tray::on_blink_clicked));
  m_Button_hide.signal_clicked().connect(sigc::mem_fun(*this, &Tray::minimize));
  m_Button_close.signal_clicked().connect(sigc::mem_fun(*this, &Tray::hide));

  // Packing:
  m_ButtonBox.pack_start(m_Button_blink);
  m_ButtonBox.pack_start(m_Button_hide);
  m_ButtonBox.pack_start(m_Button_close);

  add(m_ButtonBox);
  show_all_children();
}

Tray::~Tray()
{
}

bool Tray::disactive_blinking()
{

  return false; // close the timeout connection
}

void Tray::minimize()
{
  iconify();
  set_skip_taskbar_hint(); // "hide" the window
}  

void Tray::show_popup_menu(guint button, guint activate_time)
{
  Gtk::Menu* pMenu = static_cast<Gtk::Menu*>(m_refUIManager->get_widget("/Popup"));
 
  if(pMenu)
    pMenu->popup(button, activate_time);
}

void Tray::on_blink_clicked()
{

}

bool Tray::on_delete_event(GdkEventAny* /* event */)
{
  minimize();
  return true;
}

void Tray::on_show(){
  Gtk::Window::on_show();
  minimize();
}

void Tray::start(){
  void* no_arg = NULL;
  if(mSyncIsActive){
    std::cerr << "\nC Stop Sync by GUI";
    mpFileSystemScanner->StopToScan();
    mSyncIsActive = false;
    //m_refStatusIcon->set(Gtk::Stock::REFRESH);
  }
  else{
    std::cerr << "\nC Start Sync by GUI";
    //mpFileSystemScanner->GetEventManager()->GetSyncManager()->SyncSourceFolder(mpFileSystemScanner->GetScanFolder());
    mpFileSystemScanner->StartToScan();
    mSyncIsActive = true;
    //m_refStatusIcon->set(Gtk::Stock::HOME);
  }
}

Glib::RefPtr<Gtk::StatusIcon> Tray::GetStatusIcon() const{
  return m_refStatusIcon;
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
