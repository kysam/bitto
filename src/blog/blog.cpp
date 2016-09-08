#include "blog.h"
#include <stdarg.h>
#include <vector>

BLog* BLog::m_singleton = nullptr;

BLog::BLog() {}

BLog::~BLog() {}

BLog* BLog::Get() {
	if (m_singleton == nullptr) {
		m_singleton = new BLog;
	}
	return m_singleton;
}

void BLog::InitStream() {
	m_stream.open(BLOG_FILE, std::ios::in | std::ios::out | std::ios::trunc);
}

void BLog::Log(const char *section, const char *format, ...) {
	struct Token {
		Token(char* str, int size) {
			m_pos = -1;
			m_str = new char[size + 1];
			memcpy(m_str, str, size);

			m_str[size] = '\0';
			m_size = size;
		};

		void Dispose() {
			delete[] m_str;
		}

		int m_pos;
		char* m_str;
		int m_size;
	};

	if (!BLog::Get()->m_stream.is_open())
		BLog::Get()->InitStream();

	va_list args;
	va_start(args, format);

	int fmtLen = strlen(format);
	std::vector<Token> tokens;
	int newLen = strlen(format);

	for (int i = 0; i < fmtLen; i++) {
		if (format[i] != '%')
			continue;

		if (format[i + 1] == 's') {
			char* str = va_arg(args, char*);
			Token token(str, strlen(str));
			token.m_pos = i;
			tokens.push_back(token);
			newLen += strlen(str) - 2;
			i++;
		}
		else if (format[i + 1] == 'd') {
			int itg = va_arg(args, int);
			char buf[10];
			sprintf_s(buf, sizeof(buf), "%d", itg);

			Token token(buf, strlen(buf));
			token.m_pos = i;
			tokens.push_back(token);
			newLen += strlen(buf) - 2;
			i++;
		}
	}

	int marker = 0;	//marker in the original format string
	char* newStr = new char[newLen + 1];
	newStr[newLen] = '\0';
	int c = 0;

	for (int i = 0; i < tokens.size(); i++) {
		memcpy(&newStr[c], &format[marker], tokens[i].m_pos - marker);
		c += tokens[i].m_pos - marker;
		memcpy(&newStr[c], tokens[i].m_str, tokens[i].m_size);

		c += tokens[i].m_size;
		marker = tokens[i].m_pos + 2;
		tokens[i].Dispose();
	}

	memcpy(&newStr[c], &format[marker], strlen(format) - marker);	//copy the rest of the ori. fmt. string

	BLog::Get()->m_stream << "[" << section << "] ";
	BLog::Get()->m_stream << newStr;
	delete[] newStr;
	va_end(args);
}
