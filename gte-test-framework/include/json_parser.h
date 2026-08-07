#ifndef GTE_JSON_PARSER_H
#define GTE_JSON_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <cstdint>

namespace gte {

struct JsonValue;
using JsonObject = std::map<std::string, std::shared_ptr<JsonValue>>;
using JsonArray = std::vector<std::shared_ptr<JsonValue>>;

struct JsonValue {
    enum Type { INTEGER, STRING, ARRAY, OBJECT };
    Type type;
    int64_t int_val;
    std::string str_val;
    JsonArray arr_val;
    JsonObject obj_val;

    JsonValue() : type(INTEGER), int_val(0) {}
    JsonValue(int64_t v) : type(INTEGER), int_val(v) {}
    JsonValue(const std::string& v) : type(STRING), str_val(v) {}
    JsonValue(const JsonArray& v) : type(ARRAY), arr_val(v) {}
    JsonValue(const JsonObject& v) : type(OBJECT), obj_val(v) {}
};

using JsonPtr = std::shared_ptr<JsonValue>;

class JsonParser {
public:
    static JsonPtr parse_object(const std::string& json);
    static JsonPtr parse_array(const std::string& json);
    static JsonPtr parse_value(const std::string& json);

private:
    static size_t pos_;
    static std::string input_;

    static void skip_whitespace();
    static char get_char();
    static char peek_char();
    static void expect_char(char c);
    static std::string parse_string();
    static int64_t parse_integer();
    static JsonPtr parse_json_array();
    static JsonPtr parse_json_object();
    static JsonPtr parse_json_value();
};

// Helper functions
int64_t get_int(const JsonPtr& val, int64_t default_val = 0);
std::string get_string(const JsonPtr& val, const std::string& default_val = "");
JsonObject get_object(const JsonPtr& val);
JsonArray get_array(const JsonPtr& val);
JsonPtr get_field(const JsonObject& obj, const std::string& field, const JsonPtr& default_val = JsonPtr{});
int64_t get_int_field(const JsonObject& obj, const std::string& field, int64_t default_val = 0);

}

#endif
