#ifndef LCXJSON_H
#define LCXJSON_H

#include <string>
#include <codecvt>
#include <memory>

enum elementType
{
	JSON_NULL,
	JSON_FALSE,
	JSON_TRUE,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT
};

enum
{
	PARSE_OK = 0,
	PARSE_EXPECT_VALUE,
	PARSE_INVALID_VALUE,
	PARSE_ROOT_NOT_SINGULAR,
	PARSE_NUMBER_TOO_BIG,
	PARSE_MISS_QUOTATION_MARK,
	PARSE_INVALID_STRING_ESCAPE,
	PARSE_INVALID_STRING_CHAR,
	PARSE_INVALID_UNICODE_HEX,
	PARSE_INVALID_UNICODE_SURROGATE,
	PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
	PARSE_MISS_KEY,
	PARSE_MISS_COLON,
	PARSE_MISS_COMMA_OR_CURLY_BRACKET
};

struct context
{
	const char* json;
};
class elementMember;
class elementValue
{
	friend class elementMember;
public:
	elementValue();
	elementValue(const elementValue& rhs);
	elementValue& operator=(const elementValue& rhs);
	elementType getType() const;
	bool isNull() const;
	bool getBool() const;
	void setBool(const bool& val);
	double getNumber() const;
	void setNumber(const double& val);
	std::string getString() const;
	void setString(const std::string& val);
	int parse(const char* val);
	size_t getArraySize() const;
	elementValue getArrayElement(size_t index) const;
	elementValue& getArrayElementRef(size_t index);
	void addArrayElement(elementValue& elem);
	std::string getObjectKey(size_t index) const;
	elementValue getObjectValue(size_t index) const;
	elementValue& getObjectValueRef(size_t index);
	elementValue findObjectByKey(const std::string& key,bool& succ) const;
	elementValue& findObjectByKeyRef(const std::string& key,bool& succ);
	size_t getObjectSize() const;
	void addObject(std::string key, elementValue& elem);
	void toString(std::string& ret) const;
	~elementValue();
private:
	void parse_whitespace(context* c);
	int parse_value(context* c);
	int parse_array(context* c);
	int parse_string(context* c);
	int parse_string_only(context* c, std::string& dest);
	int parse_literal(context* c, const char* expect, elementType type);
	int parse_number(context* c);
	int parse_object(context* c);
	void array_check_alloc();
	void member_check_alloc();
	double num = 0;
	std::string str;
	std::allocator<elementValue> arrayAlloc;
	std::allocator<elementMember> memberAlloc;
	size_t* shareCnt;
	elementValue* arrBegin = nullptr;
	elementValue* arrEnd = nullptr;
	elementValue* arrSize = nullptr;
	elementMember* memBegin = nullptr;
	elementMember* memEnd = nullptr;
	elementMember* memSize = nullptr;
	elementType type = JSON_NULL;
};
class elementMember
{
	friend class elementValue;
private:
	std::string key;
	elementValue value;
};

void print_json(size_t step, const elementValue& elem, bool isws);
#endif // !LCXJSON_H
