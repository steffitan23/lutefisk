#include <fstream>
#include <unordered_map>
#include <memory>
#include "dialogue.h"
#include "nodes.h"
#include "json.hpp"
#include "iostream"

using nlohmann::json;

DialogueTree DialogueTree::parse_json_to_tree(const std::string &filename)
{
    std::ifstream file(filename);
    json data;
    file >> data;
    const auto &node_data = data["nodes"];
    NodeFactory factory;

    for (const auto &[node_name, node_content] : node_data.items())
    {
        const auto node_type = node_content["type"].get<std::string>();

        if (node_type == "exposition")
        {
            const auto node_text = node_content["text"].get<std::string>();
            factory.create_node<ExpositionNode>(node_name, node_text);

            if (node_content.contains("give_item"))
            {
            }
        }
        else if (node_type == "speech")
        {
            const auto node_text = node_content["text"].get<std::string>();
            factory.create_node<SpeechNode>(node_name, node_text, node_content["title"].get<std::string>());
        }
        else if (node_type == "options")
        {
            auto options_texts = node_content["options_text"].get<std::vector<std::string>>();
            auto options_next = node_content["options_next"].get<std::vector<std::string>>();

            std::vector<std::shared_ptr<Node>> placeholder_nexts(options_next.size(), nullptr);

            factory.create_node<OptionsNode>(
                node_name, node_content["title"].get<std::string>(), options_texts, placeholder_nexts, nullptr);
        }
        else
        {
            throw "invalid node_type";
        }
    }

    for (const auto &[node_name, node_content] : node_data.items())
    {
        const auto &node_type = node_content["type"].get<std::string>();
        auto node = factory.get_node(node_name);

        if (node_type == "exposition" || node_type == "speech")
        {
            if (node_content.contains("next"))
            {
                node->set_next(factory.get_node(node_content["next"].get<std::string>()));
            }
        }
        else if (node_type == "options")
        {
            auto options_next = node_content["options_next"].get<std::vector<std::string>>();
            std::vector<std::shared_ptr<Node>> options_vector;

            for (const auto &option_name : options_next)
            {
                options_vector.emplace_back(factory.get_node(option_name));
            }

            auto options_node = dynamic_cast<OptionsNode *>(node.get());
            options_node->set_options(std::move(options_vector));

            if (node_content.contains("options_else"))
            {
                const auto options_else = node_content["options_else"].get<std::string>();
                options_node->set_else(factory.get_node(options_else));
            }
        }
    }

    return DialogueTree(factory.get_node("start"));
}
