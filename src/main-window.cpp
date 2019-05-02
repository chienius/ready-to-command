#include "main-window.hpp"
#include <iostream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Builder>& builder, const CommandConfig& config) {
    builder->get_widget("main_window", p_main_window);
    builder->get_widget("header_bar", p_header_bar);
    builder->get_widget("prompt_label", p_prompt_label);
    builder->get_widget("action_combo_box", p_action_combo_box);
    builder->get_widget("action_ok_btn", p_action_ok_button);

    p_main_window->set_keep_above();

    // Link signals
    p_main_window->signal_delete_event().connect(
        sigc::mem_fun(*this, &MainWindow::on_window_delete)
    );

    p_action_ok_button->signal_clicked().connect(
        sigc::mem_fun(*this, &MainWindow::on_action_ok_btn_clicked)
    );

    // Setup command texts
    p_header_bar->set_title("Ready to " + config.textTitle);
    p_prompt_label->set_label(config.textDesc);

    p_action_combo_box->remove_all();
    p_action_combo_box->insert(ACTION_NOW, "Do " + config.textAction + " now");
    p_action_combo_box->insert(ACTION_D30S, "Delay 30 seconds");
    p_action_combo_box->insert(ACTION_D10M, "Delay 10 minutes");
    p_action_combo_box->insert(ACTION_CANCEL, "Cancel " + config.textAction);
    p_action_combo_box->set_active(ACTION_D30S);

    textAction = config.textAction;
}

MainWindow::~MainWindow() {
    // noop
}

bool MainWindow::on_window_delete(GdkEventAny*) {
    return _preventDestroy;
}

void MainWindow::on_action_ok_btn_clicked() {
    ActiveActionType active_action_row = static_cast<ActiveActionType>(p_action_combo_box->get_active_row_number());

    if(actionApplyCallback)
        actionApplyCallback(active_action_row);
    else
        std::cerr << "Action apply callback not set yet" << std::endl;
}