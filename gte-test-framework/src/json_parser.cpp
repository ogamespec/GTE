#include "json_parser.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

namespace gte {

size_t JsonParser::pos_ = 0;
std::string JsonParser::input_ = "";

void JsonParser::skip_whitespace() {
    while (pos_ < input_.size() && (input_[pos_] == ' ' || input_[pos_] == '\t' ||
           input_[pos_] == '\n' || input_[pos_] == '\r')) {
        ++pos_;
    }
}

char JsonParser::get_char() {
    if (pos_ >= input_.size()) {
        throw std::runtime_error("Unexpected end of JSON input");
    }
    return input_[pos_++];
}

char JsonParser::peek_char() {
    skip_whitespace();
    if (pos_ >= input_.size()) {
        throw std::runtime_error("Unexpected end of JSON input");
    }
    return input_[pos_];
}

void JsonParser::expect_char(char c) {
    char got = get_char();
    if (got != c) {
        std::stringstream ss;
        ss << "Expected '" << c << "' but got '" << got << "' at position " << (pos_ - 1);
        throw std::runtime_error(ss.str());
    }
}

std::string JsonParser::parse_string() {
    expect_char('"');
    std::string result;
    while (pos_ < input_.size() && input_[pos_] != '"') {
        if (input_[pos_] == '\\') {
            ++pos_;
            if (pos_ >= input_.size()) {
                throw std::runtime_error("Unexpected end of JSON in escape sequence");
            }
            switch (input_[pos_]) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                default:
                    throw std::runtime_error(std::string("Unknown escape sequence: ") + input_[pos_]);
            }
        } else {
            result += input_[pos_];
        }
        ++pos_;
    }
    if (pos_ >= input_.size() || input_[pos_] != '"') {
        throw std::runtime_error("Unterminated string");
    }
    ++pos_;
    return result;
}

int64_t JsonParser::parse_integer() {
    skip_whitespace();
    bool negative = false;
    if (pos_ < input_.size() && input_[pos_] == '-') {
        negative = true;
        ++pos_;
    }
    std::string num_str;
    while (pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '9') {
        num_str += input_[pos_];
        ++pos_;
    }
    if (num_str.empty()) {
        throw std::runtime_error("Expected integer at position " + std::to_string(pos_));
    }
    int64_t value = static_cast<int64_t>(std::atoll(num_str.c_str()));
    return negative ? -value : value;
}

int64_t JsonParser::parse_hex_string(const std::string& hex) {
    char* end;
    long val = std::strtol(hex.c_str(), &end, 16);
    return static_cast<int64_t>(val);
}

JsonValue JsonParser::parse_value(const std::string& json) {
    input_ = json;
    pos_ = 0;
    skip_whitespace();
    char c = peek_char();
    if (c == '{') {
        return parse_json_object();
    } else if (c == '[') {
        return parse_json_array();
    } else if (c == '"') {
        return parse_string();
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        return parse_integer();
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

JsonObject JsonParser::parse_object(const std::string& json) {
    JsonValue val = parse_value(json);
    if (auto* obj = std::get_if<JsonObject>(&val)) {
        return *obj;
    }
    throw std::runtime_error("Expected JSON object");
}

JsonArray JsonParser::parse_array(const std::string& json) {
    JsonValue val = parse_value(json);
    if (auto* arr = std::get_if<JsonArray>(&val)) {
        return *arr;
    }
    throw std::runtime_error("Expected JSON array");
}

JsonArray JsonParser::parse_json_array() {
    expect_char('[');
    skip_whitespace();
    JsonArray arr;
    if (peek_char() == ']') {
        get_char();
        return arr;
    }
    while (true) {
        arr.push_back(parse_json_value());
        skip_whitespace();
        char c = get_char();
        if (c == ']') {
            break;
        }
        if (c != ',') {
            throw std::runtime_error(std::string("Expected ',' or ']' in array, got '") + c + "'");
        }
    }
    return arr;
}

JsonObject JsonParser::parse_json_object() {
    expect_char('{');
    skip_whitespace();
    JsonObject obj;
    if (peek_char() == '}') {
        get_char();
        return obj;
    }
    while (true) {
        skip_whitespace();
        std::string key = parse_string();
        skip_whitespace();
        expect_char(':');
        obj[key] = parse_json_value();
        skip_whitespace();
        char c = get_char();
        if (c == '}') {
            break;
        }
        if (c != ',') {
            throw std::runtime_error(std::string("Expected ',' or '}' in object, got '") + c + "'");
        }
    }
    return obj;
}

JsonValue JsonParser::parse_json_value() {
    skip_whitespace();
    char c = peek_char();
    if (c == '{') {
        return parse_json_object();
    } else if (c == '[') {
        return parse_json_array();
    } else if (c == '"') {
        return parse_string();
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        return parse_integer();
    } else if (c == 't') {
        // true
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return static_cast<int64_t>(1);
        }
        throw std::runtime_error("Expected 'true'");
    } else if (c == 'f') {
        // false
        if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return static_cast<int64_t>(0);
        }
        throw std::runtime_error("Expected 'false'");
    } else if (c == 'n') {
        // null
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return static_cast<int64_t>(0);
        }
        throw std::runtime_error("Expected 'null'");
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

// Helper function implementations
int64_t get_int(const JsonValue& val, int64_t default_val) {
    if (auto* p = std::get_if<int64_t>(&val)) {
        return *p;
    }
    return default_val;
}

double get_double(const JsonValue& val, double default_val) {
    if (auto* p = std::get_if<double>(&val)) {
        return *p;
    }
    return default_val;
}

std::string get_string(const JsonValue& val, const std::string& default_val) {
    if (auto* p = std::get_if<std::string>(&val)) {
        return *p;
    }
    return default_val;
}

JsonObject get_object(const JsonValue& val) {
    if (auto* p = std::get_if<JsonObject>(&val)) {
        return *p;
    }
    return JsonObject{};
}

JsonArray get_array(const JsonValue& val) {
    if (auto* p = std::get_if<JsonArray>(&val)) {
        return *p;
    }
    return JsonArray{};
}

JsonValue get_field(const JsonObject& obj, const std::string& field, const JsonValue& default_val) {
    auto it = obj.find(field);
    if (it != obj.end()) {
        return it->second;
    }
    return default_val;
}

int64_t get_int_field(const JsonObject& obj, const std::string& field, int64_t default_val) {
    auto it = obj.find(field);
    if (it != obj.end()) {
        if (auto* p = std::get_if<int64_t>(&it->second)) {
            return *p;
        }
        if (auto* p = std::get_if<std::string>(&it->second)) {
            return JsonParser::parse_hex_string(*p);
        }
    }
    return default_val;
}

}
