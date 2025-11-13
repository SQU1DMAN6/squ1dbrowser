#include "tab_manager.h"

Tab::Tab(const std::string& url, const std::string& title)
    : url(url), title(title), is_active(false) {}

void Tab::set_title(const std::string& title) {
    this->title = title;
}

void Tab::set_content(const std::vector<uint8_t>& content) {
    rendered_content = content;
}

TabManager::TabManager() : active_tab_index(0) {
    // Create initial tab
    tabs.push_back(std::make_shared<Tab>("https://google.com", "New Tab"));
    tabs[0]->is_active = true;
}

std::shared_ptr<Tab> TabManager::create_tab(const std::string& url) {
    auto tab = std::make_shared<Tab>(url, "Loading...");
    tabs.push_back(tab);
    return tab;
}

void TabManager::close_tab(int index) {
    if (index < 0 || index >= static_cast<int>(tabs.size())) {
        return;
    }
    
    if (tabs[index]->is_active) {
        // Switch to another tab before closing
        if (tabs.size() > 1) {
            int new_active = index > 0 ? index - 1 : index + 1;
            switch_tab(new_active);
        }
    }
    
    tabs.erase(tabs.begin() + index);
    if (active_tab_index >= static_cast<int>(tabs.size()) && !tabs.empty()) {
        active_tab_index = tabs.size() - 1;
    }
}

void TabManager::switch_tab(int index) {
    if (index < 0 || index >= static_cast<int>(tabs.size())) {
        return;
    }
    
    if (active_tab_index >= 0 && active_tab_index < static_cast<int>(tabs.size())) {
        tabs[active_tab_index]->is_active = false;
    }
    
    active_tab_index = index;
    if (index >= 0 && index < static_cast<int>(tabs.size())) {
        tabs[index]->is_active = true;
    }
}

std::shared_ptr<Tab> TabManager::get_active_tab() const {
    if (active_tab_index >= 0 && active_tab_index < static_cast<int>(tabs.size())) {
        return tabs[active_tab_index];
    }
    return nullptr;
}

std::shared_ptr<Tab> TabManager::get_tab(int index) const {
    if (index >= 0 && index < static_cast<int>(tabs.size())) {
        return tabs[index];
    }
    return nullptr;
}
