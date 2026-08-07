#include "json_parser.h"
#include <sstream>
#include <iomanip>
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
                    throw std::runtime_error(std::string("Unknown escape: ") + input_[pos_]);
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

JsonPtr JsonParser::parse_value(const std::string& json) {
    input_ = json;
    pos_ = 0;
    skip_whitespace();
    char c = peek_char();
    if (c == '{') {
        return parse_json_object();
    } else if (c == '[') {
        return parse_json_array();
    } else if (c == '"') {
        return std::make_shared<JsonValue>(parse_string());
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        return std::make_shared<JsonValue>(parse_integer());
    } else if (c == 't') {
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return std::make_shared<JsonValue>(static_cast<int64_t>(1));
        }
        throw std::runtime_error("Expected 'true'");
    } else if (c == 'f') {
        if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return std::make_shared<JsonValue>(static_cast<int64_t>(0));
        }
        throw std::runtime_error("Expected 'false'");
    } else if (c == 'n') {
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return std::make_shared<JsonValue>(static_cast<int64_t>(0));
        }
        throw std::runtime_error("Expected 'null'");
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

JsonPtr JsonParser::parse_object(const std::string& json) {
    return parse_value(json);
}

JsonPtr JsonParser::parse_array(const std::string& json) {
    return parse_value(json);
}

JsonPtr JsonParser::parse_json_array() {
    expect_char('[');
    skip_whitespace();
    JsonArray arr;
    if (peek_char() == ']') {
        get_char();
        return std::make_shared<JsonValue>(arr);
    }
    while (true) {
        arr.push_back(parse_json_value());
        skip_whitespace();
        char c = get_char();
        if (c == ']') {
            break;
        }
        if (c != ',') {
            throw std::runtime_error(std::string("Expected ',' or ']' in array"));
        }
    }
    return std::make_shared<JsonValue>(arr);
}

JsonPtr JsonParser::parse_json_object() {
    expect_char('{');
    skip_whitespace();
    JsonObject obj;
    if (peek_char() == '}') {
        get_char();
        return std::make_shared<JsonValue>(obj);
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
            throw std::runtime_error(std::string("Expected ',' or '}' in object"));
        }
    }
    return std::make_shared<JsonValue>(obj);
}

JsonPtr JsonParser::parse_json_value() {
    skip_whitespace();
    char c = peek_char();
    if (c == '{') {
        return parse_json_object();
    } else if (c == '[') {
        return parse_json_array();
    } else if (c == '"') {
        return std::make_shared<JsonValue>(parse_string());
    } else if (c == '-' || (c >= '0' && c <= '9')) {
        return std::make_shared<JsonValue>(parse_integer());
    } else if (c == 't') {
        if (input_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return std::make_shared<JsonValue>(static_cast<int64_t>(1));
        }
        throw std::runtime_error("Expected 'true'");
    } else if (c == 'f') {
        if (input_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return std::make_shared<JsonValue>(static_cast<int64_t>(0));
        }
        throw std::runtime_error("Expected 'false'");
    } else if (c == 'n') {
        if (input_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return std::make_shared<JsonValue>(static_cast<int64_t>(0));
        }
        throw std::runtime_error("Expected 'null'");
    } else {
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

// Helper implementations
int64_t get_int(const JsonPtr& val, int64_t default_val) {
    if (val && val->type == JsonValue::INTEGER) {
        return val->int_val;
    }
    return default_val;
}

std::string get_string(const JsonPtr& val, const std::string& default_val) {
    if (val && val->type == JsonValue::STRING) {
        return val->str_val;
    }
    return default_val;
}

JsonObject get_object(const JsonPtr& val) {
    if (val && val->type == JsonValue::OBJECT) {
        return val->obj_val;
    }
    return JsonObject{};
}

JsonArray get_array(const JsonPtr& val) {
    if (val && val->type == JsonValue::ARRAY) {
        return val->arr_val;
    }
    return JsonArray{};
}

JsonPtr get_field(const JsonObject& obj, const std::string& field, const JsonPtr& default_val) {
    auto it = obj.find(field);
    if (it != obj.end()) {
        return it->second;
    }
    return default_val;
}

int64_t get_int_field(const JsonObject& obj, const std::string& field, int64_t default_val) {
    auto it = obj.find(field);
    if (it != obj.end() && it->second) {
        if (it->second->type == JsonValue::INTEGER) {
            return it->second->int_val;
        }
        if (it->second->type == JsonValue::STRING) {
            char* end;
            return static_cast<int64_t>(std::strtol(it->second->str_val.c_str(), &end, 16));
        }
    }
    return default_val;
}

// Get raw GTE opcode field from test case JSON
// Supports both decimal and hex string values
// Example values: 1073742209 (decimal) or "0x40010001" (hex string)
int32_t get_raw_opcode_field(const JsonObject& obj, int32_t default_val) {
    auto it = obj.find("raw_opcode");
    if (it != obj.end() && it->second) {
        if (it->second->type == JsonValue::INTEGER) {
            return static_cast<int32_t>(it->second->int_val);
        }
        if (it->second->type == JsonValue::STRING) {
            char* end;
            return static_cast<int32_t>(std::strtol(it->second->str_val.c_str(), &end, 0));
        }
    }
    return default_val;
}

}
