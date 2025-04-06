#pragma once

#include <string>
#include <vector>

class box
{
public:
    box() = default;

    void add_item(const std::string& item);
    void add_items(const std::string& item, std::size_t count);
    [[nodiscard]] std::size_t items_count() const noexcept;
    [[nodiscard]] const std::string& at(const std::size_t idx) const;
    void close() noexcept;
    std::size_t count(std::string_view item);

private:
    bool m_is_open{ true };
    std::vector<std::string> m_items;
};
