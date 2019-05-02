#pragma once
#include <gtkmm.h>
#include <iostream>
#include "main-window.hpp"
#include "command-config.hpp"
#include "execution-thread.hpp"

class MainApplication: public Gtk::Application {
public:
    MainApplication(): Gtk::Application("com.chienius.ready-to-command", Gio::APPLICATION_HANDLES_COMMAND_LINE) {
        Glib::set_application_name("Ready to Command"); 
        add_main_option_entry(Gio::Application::OptionType::OPTION_TYPE_STRING, "command-title", 't', "The title of the command.");
        add_main_option_entry(Gio::Application::OptionType::OPTION_TYPE_STRING, "command-desc", 'd', "The description of the command.");
        add_main_option_entry(Gio::Application::OptionType::OPTION_TYPE_STRING, "command-action", 'a', "The action description of the command.");
        add_main_option_entry(Gio::Application::OptionType::OPTION_TYPE_STRING, "shell-exec", 'e', "The actual shell command of the command.");
    }

    static Glib::RefPtr<MainApplication> create()
    {
        return Glib::RefPtr<MainApplication>( new MainApplication() );
    }

    CommandConfig config;

    void joinCommandThreads() {
        if(cmtPtr) {
            cmtPtr->joinAll();
        }
    }

protected:
    std::unique_ptr<CommandExecutionThread> cmtPtr = nullptr;
    std::unique_ptr<MainWindow> mainWindowPtr = nullptr;

    void on_activate() override {
        auto builder = Gtk::Builder::create_from_file("res/main-window.glade");
        mainWindowPtr = std::make_unique<MainWindow>(builder, config);
        cmtPtr = std::make_unique<CommandExecutionThread>(std::ref(*mainWindowPtr), config.shellExec);

        mainWindowPtr->setActionApplyCallback([&](MainWindow::ActiveActionType action){
            switch(action) {
                case MainWindow::ACTION_NOW:
                    cmtPtr->triggerExecution();
                    break;
                case MainWindow::ACTION_D30S:
                    cmtPtr->delay(30);
                    break;
                case MainWindow::ACTION_D10M:
                    cmtPtr->delay(600);
                    break;
                case MainWindow::ACTION_CANCEL:
                    cmtPtr->cancel();
                    break;
            }
        });

        add_window(mainWindowPtr->getWindow());
        mainWindowPtr->getWindow().show();
    }

    int on_command_line(const Glib::RefPtr<Gio::ApplicationCommandLine>& command_line) override {
        const auto options = command_line->get_options_dict();
        if (!options)
            std::cerr << G_STRFUNC << ": options is null." << std::endl;

        //Parse command-line arguments that were passed either to the primary (first) instance
        //or to subsequent instances.
        //Note that this parsing is happening in the primary (not local) instance.
        Glib::ustring textTitle, textDesc, textAction, shellExec;

        get_arg_value(options, "command-title", textTitle);
        get_arg_value(options, "command-desc", textDesc);
        get_arg_value(options, "command-action", textAction);
        get_arg_value(options, "shell-exec", shellExec);

        if(textTitle.empty() || textDesc.empty() || textAction.empty() || shellExec.empty()) {
            std::cerr << "Please provide command options." << std::endl;
            return EXIT_FAILURE;
        }

        config.textTitle = textTitle;
        config.textDesc = textDesc;
        config.textAction = textAction;
        config.shellExec = shellExec;

        activate();

        return EXIT_SUCCESS;
    }

private:
    template <typename T_ArgType>
    static bool get_arg_value(const Glib::RefPtr<Glib::VariantDict> &options, const Glib::ustring &arg_name, T_ArgType &arg_value)
    {
        arg_value = T_ArgType();
        if (options->lookup_value(arg_name, arg_value))
        {
            //std::cout << "The \"" << arg_name << "\" value was in the options VariantDict." << std::endl;
            return true;
        }
        else
        {
            auto gvariant = g_variant_dict_lookup_value(options->gobj(), arg_name.c_str(), nullptr);
            if (!gvariant)
            {
                std::cerr << "The \"" << arg_name << "\" value was not in the options VariantDict." << std::endl;
            }
            else
            {
                std::cerr << "The \"" << arg_name << "\" value was of type " << g_variant_get_type_string(gvariant) << " instead of " << Glib::Variant<T_ArgType>::variant_type().get_string() << std::endl;
            }
        }

        return false;
    }
};
