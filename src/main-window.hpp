#pragma once
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/builder.h>
#include <string>
#include <sstream>
#include <functional>
#include "command-config.hpp"

class MainWindow 
{
public:
    enum ActiveActionType {
        ACTION_NOW = 0,
        ACTION_D30S,
        ACTION_D10M,
        ACTION_CANCEL 
    };
    using ActionApplyCallback = std::function<void(ActiveActionType)>;

    MainWindow(const Glib::RefPtr<Gtk::Builder>& builder, const CommandConfig& config);
    ~MainWindow();

    Gtk::Window& getWindow() const {
        return *p_main_window;
    };

    void updateCountdown(int seconds) {
        std::ostringstream oss;
        if(seconds > 0 && seconds < 60) {
            oss << "in " << seconds << " seconds";
        } else if(seconds >= 60) {
            oss << "in " << (seconds/60 + 1) << " minutes";
        } else if(seconds < 0) {
            disableWindow();
            oss << "Cancelled";
        } else {
            disableWindow();
            oss << "Action now...";
        }

        p_header_bar->set_subtitle(oss.str());
    }

    void setActionApplyCallback(const ActionApplyCallback& callback) {
        actionApplyCallback=callback;
    }


private:
    // Signal handlers
    bool on_window_delete(GdkEventAny*);
    void on_action_ok_btn_clicked();
    ActionApplyCallback actionApplyCallback;

    std::string textAction;
    bool _preventDestroy = true;
    void disableWindow() {
        p_action_combo_box->set_sensitive(false);
        p_action_ok_button->set_sensitive(false);
        _preventDestroy = false;
    }

    // Widgets
    Gtk::Window* p_main_window;
    Gtk::HeaderBar* p_header_bar;
    Gtk::Label* p_prompt_label;
    Gtk::ComboBoxText* p_action_combo_box;
    Gtk::Button* p_action_ok_button;
};
