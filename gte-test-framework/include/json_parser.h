#ifndef GTE_JSON_PARSER_H
#define GTE_JSON_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <stdexcept>
#include <cstdint>

namespace gte {

using JsonValue = std::variant<int64_t, double, std::string, std::vector<JsonValue>, std::map<std::string, JsonValue>>;
using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

class JsonParser {
public:
    static JsonObject parse_object(const std::string& json);
    static JsonArray parse_array(const std::string& json);
    static JsonValue parse_value(const std::string& json);

private:
    static size_t pos_;
    static std::string input_;

    static void skip_whitespace();
    static char get_char();
    static char peek_char();
    static void expect_char(char c);
    static std::string parse_string();
    static int64_t parse_integer();
    static JsonArray parse_json_array();
    static JsonObject parse_json_object();
    static JsonValue parse_json_value();

    static int64_t parse_hex_string(const std::string& hex);
};

// Helper functions to extract values from JsonValue
int64_t get_int(const JsonValue& val, int64_t default_val = 0);
double get_double(const JsonValue& val, double default_val = 0.0);
std::string get_string(const JsonValue& val, const std::string& default_val = "");
JsonObject get_object(const JsonValue& val);
JsonArray get_array(const JsonValue& val);

JsonValue get_field(const JsonObject& obj, const std::string& field, const JsonValue& default_val = JsonValue{});
int64_t get_int_field(const JsonObject& obj, const std::string& field, int64_t default_val = 0);

}

#endif
