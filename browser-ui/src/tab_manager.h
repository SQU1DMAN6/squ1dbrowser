#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

class Tab {
public:
    std::string title;
    std::string url;
    bool is_active;
    std::vector<uint8_t> rendered_content;

    Tab(const std::string& url, const std::string& title = "New Tab");
    void set_title(const std::string& title);
    void set_content(const std::vector<uint8_t>& content);
};

class TabManager {
private:
    std::vector<std::shared_ptr<Tab>> tabs;
    int active_tab_index;

public:
    TabManager();
    
    std::shared_ptr<Tab> create_tab(const std::string& url);
    void close_tab(int index);
    void switch_tab(int index);
    std::shared_ptr<Tab> get_active_tab() const;
    std::shared_ptr<Tab> get_tab(int index) const;
    int get_tab_count() const { return tabs.size(); }
    int get_active_index() const { return active_tab_index; }
    std::vector<std::shared_ptr<Tab>>& get_tabs() { return tabs; }
};
