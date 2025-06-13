#pragma once
#include "dialogue.h"
#include <stdexcept>

enum class NodeType
{
    Exposition,
    Speech,
    Options,
    Base
};

class Node
{
public:
    Node() : m_text{""}, m_title{""} {}
    Node(std::string text, std::string title) : m_text{std::move(text)}, m_title{std::move(title)} {}
    virtual ~Node() = default;

    virtual std::string get_text() const { return m_text; }
    virtual std::vector<std::string> get_options() const { return {}; }
    virtual void select_option(size_t) {}
    virtual std::string get_title() const { return m_title; };
    virtual Node &get_next() const { return *m_next; }
    virtual bool has_next() { return (m_next != nullptr); }
    virtual void set_next(std::shared_ptr<Node> node) { m_next = node; }
    virtual NodeType get_node_type() const { return NodeType::Base; }

protected:
    std::shared_ptr<Node> m_next;
    std::string m_text;
    std::string m_title;
};

class NodeFactory
{
public:
    template <typename T, typename... Args>
    std::shared_ptr<T> create_node(const std::string &name, Args &&...args)
    {
        auto node = std::make_shared<T>(std::forward<Args>(args)...);
        m_nodes[name] = node;
        return node;
    }

    std::shared_ptr<Node> get_node(const std::string &name)
    {
        auto it = m_nodes.find(name);
        if (it == m_nodes.end())
        {
            throw std::runtime_error("Node '" + name + "' not found in m_nodes.");
        }

        return it->second;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;
};

class ExpositionNode : public Node
{
public:
    ExpositionNode(std::string text) : Node(text, "") {}
    NodeType get_node_type() const override { return NodeType::Exposition; }
};

class SpeechNode : public Node
{
public:
    SpeechNode(std::string text, std::string title) : Node(text, title) {}
    NodeType get_node_type() const override { return NodeType::Speech; }
};

class OptionsNode : public Node
{
public:
    OptionsNode(std::string title,
                const std::vector<std::string> &options_texts,
                const std::vector<std::shared_ptr<Node>> &options_ptrs,
                std::shared_ptr<Node> else_ptr)
        : Node("", title),
          m_visited(options_ptrs.size(), false),
          m_options_texts(options_texts),
          m_options_ptrs(options_ptrs),
          m_else_ptr(else_ptr)
    {
    }

    void set_options(std::vector<std::shared_ptr<Node>> &&options_ptrs)
    {
        m_options_ptrs = std::move(options_ptrs);
    }

    void set_else(const std::shared_ptr<Node> &else_ptr)
    {
        m_else_ptr = else_ptr;
    }

    std::vector<std::string> get_options() const override
    {
        return std::vector<std::string>{m_options_texts.begin(), m_options_texts.end()};
    }

    std::vector<bool> get_visited() const
    {
        return m_visited;
    }

    void select_option(size_t i) override
    {
        m_visited[i] = true;
        m_next = m_options_ptrs[i];
    }

    bool all_visited()
    {
        return std::all_of(m_visited.begin(), m_visited.end(), [](bool b)
                           { return b; });
    }

    Node &get_else()
    {
        return *m_else_ptr;
    }

    NodeType get_node_type() const override
    {
        return NodeType::Options;
    }

private:
    std::vector<bool> m_visited;
    std::vector<std::string> m_options_texts;
    std::vector<std::shared_ptr<Node>> m_options_ptrs;
    std::shared_ptr<Node> m_else_ptr;
};
