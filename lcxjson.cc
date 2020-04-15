#include "lcxjson.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>

elementValue::elementValue()
{
	shareCnt = new size_t(1);
}

elementValue::elementValue(const elementValue& rhs) : num(rhs.num), str(rhs.str), arrayAlloc(rhs.arrayAlloc), arrBegin(rhs.arrBegin),
arrEnd(rhs.arrEnd), arrSize(rhs.arrSize), type(rhs.type), shareCnt(rhs.shareCnt),
memberAlloc(rhs.memberAlloc), memBegin(rhs.memBegin), memEnd(rhs.memEnd), memSize(rhs.memSize)
{
	++* shareCnt;
}

elementValue& elementValue::operator=(const elementValue& rhs)
{
	++* rhs.shareCnt;
	if (-- * shareCnt == 0)
	{
		if (arrBegin)
		{
			for (auto it = arrEnd; it != arrBegin;)
			{
				if (it != nullptr)
					arrayAlloc.destroy(--it);
			}
			arrayAlloc.deallocate(arrBegin, arrSize - arrBegin);
			arrBegin = arrEnd = arrSize = nullptr;
		}
		if (memBegin)
		{
			for (auto it = memEnd; it != memBegin;)
			{
				memberAlloc.destroy(--it);
			}
			memberAlloc.deallocate(memBegin, memSize - memBegin);
			memBegin = memEnd = memSize = nullptr;
		}
		delete shareCnt;
	}
	num = rhs.num;
	str = rhs.str;
	arrayAlloc = rhs.arrayAlloc;
	shareCnt = rhs.shareCnt;
	arrBegin = rhs.arrBegin;
	arrEnd = rhs.arrEnd;
	arrSize = rhs.arrSize;
	memberAlloc = rhs.memberAlloc;
	memBegin = rhs.memBegin;
	memEnd = rhs.memEnd;
	memSize = rhs.memSize;
	type = rhs.type;
	return *this;
}

elementType elementValue::getType() const
{
	return type;
}

bool elementValue::isNull() const
{
	if (type == JSON_NULL)
		return true;
	else
		return false;
}

bool elementValue::getBool() const
{
	if (type == JSON_TRUE)
		return true;
	if (type == JSON_FALSE)
		return false;
	return false;
}

void elementValue::setBool(const bool& val)
{
	if (val)
	{
		type = JSON_TRUE;
	}
	else
	{
		type = JSON_FALSE;
	}
}

double elementValue::getNumber() const
{
	return num;
}

void elementValue::setNumber(const double& val)
{
	type = JSON_NUMBER;
	num = val;
}

std::string elementValue::getString() const
{
	return str;
}

void elementValue::setString(const std::string& val)
{
	type = JSON_STRING;
	str = val;
}

int elementValue::parse(const char* val)
{
	context c;
	int ret;
	c.json = val;
	type = JSON_NULL;
	parse_whitespace(&c);
	if ((ret = parse_value(&c)) == PARSE_OK)
	{
		parse_whitespace(&c);
		if (*c.json != '\0')
		{
			ret = PARSE_ROOT_NOT_SINGULAR;
			type = JSON_NULL;
		}
	}
	return ret;
}

size_t elementValue::getArraySize() const
{
	return arrEnd - arrBegin;
}

elementValue elementValue::getArrayElement(size_t index) const
{
	return arrBegin[index];
}

std::string elementValue::getObjectKey(size_t index) const
{
	return memBegin[index].key;
}

elementValue elementValue::getObjectValue(size_t index) const
{
	return memBegin[index].value;
}

size_t elementValue::getObjectSize() const
{
	return memEnd - memBegin;
}

void elementValue::toString(std::string& ret) const
{
	elementType t = getType();
	if (t == JSON_ARRAY)
	{
		ret += '[';
		for (size_t cur = 0; cur < getArraySize(); ++cur)
		{
			(arrBegin + cur)->toString(ret);
			if (cur!=getArraySize()-1)
				ret += ',';
		}
		ret += ']';
	}
	else if (t == JSON_OBJECT)
	{
		ret += '{';
		for (size_t cur = 0; cur < getObjectSize(); ++cur)
		{
			ret += "\""+getObjectKey(cur)+"\"";
			ret += ':';
			(memBegin + cur)->value.toString(ret);
			if (cur!=getObjectSize()-1)
				ret += ',';
		}
		ret += '}';
	}
	else
	{
		switch (t)
		{
		case JSON_NULL:
			ret += "null";
			break;
		case JSON_TRUE:
			ret += "true";
			break;
		case JSON_FALSE:
			ret += "false";
			break;
		case JSON_NUMBER:
			ret += std::to_string(num);
			break;
		case JSON_STRING:
			ret += "\"";
			for (const auto& curchar : str)
			{
				switch (curchar)
				{
				case '\"':
					ret += "\\\"";
					break;
				case '\\':
					ret += "\\\\";
					break;
				case '\b':
					ret += "\\b";
					break;
				case '\f':
					ret += "\\f";
					break;
				case '\n':
					ret += "\\n";
					break;
				case '\r':
					ret += "\\r";
					break;
				case '\t':
					ret += "\\t";
					break;
				default:
					ret += curchar;
					break;
				}
			}
			ret += "\"";
		}
	}
}

elementValue::~elementValue()
{
	//资源无共享时才释放
	--(*shareCnt);
	if (!*shareCnt)
	{
		if (arrBegin)
		{
			for (auto it = arrEnd; it != arrBegin;)
			{
				arrayAlloc.destroy(--it);
			}
			arrayAlloc.deallocate(arrBegin, arrSize - arrBegin);
			arrBegin = arrEnd = arrSize = nullptr;
		}
		if (memBegin)
		{
			for (auto it = memEnd; it != memBegin;)
			{
				memberAlloc.destroy(--it);
			}
			memberAlloc.deallocate(memBegin, memSize - memBegin);
			memBegin = memEnd = memSize = nullptr;
		}
		delete shareCnt;
	}
}

int elementValue::parse_value(context* c)
{
	switch (*c->json)
	{
	case 'n':
		return parse_literal(c, "null", JSON_NULL);
	case 't':
		return parse_literal(c, "true", JSON_TRUE);
	case 'f':
		return parse_literal(c, "false", JSON_FALSE);
	case '\"':
		return parse_string(c);
	case '[':
		return parse_array(c);
	case '{':
		return parse_object(c);
	default:
		return parse_number(c);
	}
}

int elementValue::parse_array(context* c)
{
	c->json += 1;
	parse_whitespace(c);
	if (*c->json == ']')
	{
		++c->json;
		type = JSON_ARRAY;
		return PARSE_OK;
	}
	arrEnd = arrBegin = arrayAlloc.allocate(10);
	arrSize = arrBegin + 10;
	while (1)
	{
		parse_whitespace(c);
		arrayAlloc.construct(arrEnd);
		arrEnd->parse_value(c);
		++arrEnd;
		if (arrEnd == arrSize)
		{
			elementValue* newBegin = arrayAlloc.allocate((arrSize - arrBegin) * 2);
			elementValue* newSize = newBegin + (arrSize - arrBegin) * 2;
			elementValue* newEnd = std::uninitialized_copy(arrBegin, arrEnd, newBegin);
			for (auto it = arrEnd; it != arrBegin;)
			{
				arrayAlloc.destroy(--it);
			}
			arrayAlloc.deallocate(arrBegin, arrEnd - arrBegin);
			arrBegin = newBegin;
			arrEnd = newEnd;
			arrSize = newSize;
		}
		parse_whitespace(c);
		if (*c->json == ',')
			++c->json;
		else if (*c->json == ']')
		{
			c->json++;
			type = JSON_ARRAY;
			break;
		}
		else
			return PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
	}
	return PARSE_OK;
}

int elementValue::parse_string(context* c)
{
	int ret;
	std::string ans;
	if ((ret = parse_string_only(c, ans)) == PARSE_OK)
		str = ans;
	return ret;
}

int elementValue::parse_string_only(context* c, std::string& dest)
{
	// 从左引号开始解析到右引号
	const char* p = c->json + 1;
	while (true)
	{
		char ch = *p++;
		switch (ch)
		{
		case '\"':
			c->json = p;
			type = JSON_STRING;
			return PARSE_OK;
		case '\\':
			switch (*p++)
			{
			case '\"':
				dest += '\"';
				break;
			case '\\':
				dest += '\\';
				break;
			case '/':
				dest += '/';
				break;
			case 'b':
				dest += '\b';
				break;
			case 'f':
				dest += '\f';
				break;
			case 'n':
				dest += '\n';
				break;
			case 'r':
				dest += '\r';
				break;
			case 't':
				dest += '\t';
				break;
			default:
				return PARSE_INVALID_STRING_ESCAPE;
			}
			break;
		case '\0':
			return PARSE_MISS_QUOTATION_MARK;
		default:
			if (static_cast<unsigned char>(ch) < 0x20)
			{
				return PARSE_INVALID_STRING_CHAR;
			}
			dest += ch;
		}
	}
}

void elementValue::parse_whitespace(context* c)
{
	const char* p = c->json;
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		p++;
	c->json = p;
}

int elementValue::parse_literal(context* c, const char* expect, elementType t)
{
	const char* cur_literal = expect;
	for (; *cur_literal != '\0'; ++cur_literal)
	{
		if ((*c->json++) != (*cur_literal))
			return PARSE_INVALID_VALUE;
	}
	type = t;
	return PARSE_OK;
}

int elementValue::parse_number(context* c)
{
	const char* end = c->json;
	//检验数字是否符合规范
	if (end[0] != '-' && !std::isdigit(end[0]))
		return PARSE_INVALID_VALUE;
	if (end[0] == '-')
		++end;
	//只有1个0
	if (*end >= '1' && *end <= '9')
	{
		for (; std::isdigit(*end); ++end)
			;
	}
	else
	{
		if (std::isdigit(*end))
			++end;
		else
			return PARSE_INVALID_VALUE;
	}
	if (*end == '.')
	{
		++end;
		if (!std::isdigit(*end))
			return PARSE_INVALID_VALUE;
		for (; std::isdigit(*end); ++end)
			;
	}
	if (*end == 'E' || *end == 'e')
	{
		++end;
		if (*end == '-' || *end == '+')
			++end;
		if (!std::isdigit(*end))
			return PARSE_INVALID_VALUE;
		for (; std::isdigit(*end); ++end)
			;
	}
	errno = 0;
	num = strtod(c->json, NULL);
	if (errno == ERANGE && ((num == HUGE_VAL) || (num == -HUGE_VAL)))
		return PARSE_NUMBER_TOO_BIG;
	c->json = end;
	type = JSON_NUMBER;
	return PARSE_OK;
}

int elementValue::parse_object(context* c)
{
	c->json += 1;
	parse_whitespace(c);
	if (*c->json == '}')
	{
		++c->json;
		type = JSON_OBJECT;
		return PARSE_OK;
	}
	const char* p = c->json;
	memEnd = memBegin = memberAlloc.allocate(10);
	memSize = memBegin + 10;
	while (1)
	{
		parse_whitespace(c);
		memberAlloc.construct(memEnd);
		if (*c->json != '\"')
			return PARSE_MISS_KEY;
		parse_string_only(c, memEnd->key);
		parse_whitespace(c);
		if (*c->json != ':')
			return PARSE_MISS_COLON;
		else
			++c->json;
		parse_whitespace(c);
		memEnd->value.parse_value(c);
		++memEnd;
		if (memEnd == memSize)
		{
			elementMember* newBegin = memberAlloc.allocate((memSize - memBegin) * 2);
			elementMember* newSize = newBegin + (memSize - memBegin) * 2;
			elementMember* newEnd = std::uninitialized_copy(memBegin, memEnd, newBegin);
			for (auto it = memEnd; it != memBegin;)
				memberAlloc.destroy(--it);
			memberAlloc.deallocate(memBegin, memEnd - memBegin);
			memBegin = newBegin;
			memEnd = newEnd;
			memSize = newSize;
		}
		parse_whitespace(c);
		if (*c->json == ',')
			++c->json;
		else if (*c->json == '}')
		{
			c->json++;
			type = JSON_OBJECT;
			break;
		}
		else
			return PARSE_MISS_COMMA_OR_CURLY_BRACKET;
	}
	return PARSE_OK;
}

#define PRINT_WS(step)                            \
	do                                            \
	{                                             \
		for (size_t cur = 0; cur < (step); ++cur) \
			std::cout << ' ';                     \
	} while (0)

void print_json(size_t step, const elementValue& elem, bool isws)
{
	std::string typeStr[]{ "(JSON_NULL)", "(JSON_FALSE)", "(JSON_TRUE)", "(JSON_NUMBER)", "(JSON_STRING)" };
	elementType t = elem.getType();
	if (t == JSON_ARRAY)
	{
		std::cout << '\n';
		PRINT_WS(step);
		std::cout << "(JSON_ARRAY_START)\n";
		for (size_t cur = 0; cur < elem.getArraySize(); ++cur)
		{
			print_json(step + 2, elem.getArrayElement(cur), true);
		}
		PRINT_WS(step);
		std::cout << "(JSON_ARRAY_END)\n";
	}
	else if (t == JSON_OBJECT)
	{
		std::cout << '\n';
		PRINT_WS(step);
		std::cout << "(JSON_OBJECT_START)\n";
		for (size_t cur = 0; cur < elem.getObjectSize(); ++cur)
		{
			PRINT_WS(step + 2);
			std::cout << "key: " << elem.getObjectKey(cur) << "\n";
			PRINT_WS(step + 2);
			std::cout << "value:";
			print_json(step + 2, elem.getObjectValue(cur), false);
		}
		PRINT_WS(step);
		std::cout << "(JSON_OBJECT_END)\n";
	}
	else
	{
		if (isws)
		{
			PRINT_WS(step + 2);
		}
		std::cout << ((t == JSON_FALSE || t == JSON_TRUE) ? ("(JSON_BOOL)") : typeStr[t]) << ' ';
		switch (t)
		{
		case JSON_NULL:
			std::cout << "null\n";
			break;
		case JSON_TRUE:
			std::cout << "true\n";
			break;
		case JSON_FALSE:
			std::cout << "false\n";
			break;
		case JSON_NUMBER:
			std::cout << elem.getNumber() << '\n';
			break;
		case JSON_STRING:
			std::cout << elem.getString() << '\n';
			break;
		default:
			break;
		}
	}
}