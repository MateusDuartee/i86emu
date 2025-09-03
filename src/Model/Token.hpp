// i86emu - Intel 8086 emulator
// Copyright (c) 2025 Mateus Duarte
// Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include <string>

namespace disassembler
{

	enum TokenType
	{
		tUnknown = 0,
		tKeyword,
		tIdentifier,
		tNumber,
		tRegister,
		tLBracket,
		tRBracket,
		tComma,
		tColon,
		tPlus,
		tMinus
	};

	struct Token
	{
		Token(const std::string& token, const TokenType& type, bool hasSpace = false) : token(token), type(type), hasSpace(hasSpace) {}

		const std::string token;
		const TokenType type;
		bool hasSpace;
	};

	static const std::string TokenTypeToString(const TokenType& type) {
		switch (type) {

		case TokenType::tKeyword:
			return "Keyword";

		case TokenType::tIdentifier:
			return "Identifier";

		case TokenType::tNumber:
			return "Number";

		case TokenType::tRegister:
			return "Register";

		case TokenType::tLBracket:
			return "LBracket";

		case TokenType::tRBracket:
			return "RBracket";

		case TokenType::tComma:
			return "Comma";

		case TokenType::tColon:
			return "Colon";

		case TokenType::tPlus:
			return "Plus";

		case TokenType::tMinus:
			return "Minus";

		default:
			return "Unknown";
		}
	}

	static const TokenType StringToTokenType(const std::string& str) {
		if (str == "Keyword")
		{
			return TokenType::tKeyword;
		}

		if (str == "Identifier")
		{
			return TokenType::tIdentifier;
		}

		if (str == "Number")
		{
			return TokenType::tNumber;
		}

		if (str == "Register")
		{
			return TokenType::tRegister;
		}

		if (str == "LBracket")
		{
			return TokenType::tLBracket;
		}

		if (str == "RBracket")
		{
			return TokenType::tRBracket;
		}

		if (str == "Comma")
		{
			return TokenType::tComma;
		}

		if (str == "Colon")
		{
			return TokenType::tColon;
		}

		if (str == "Plus")
		{
			return TokenType::tPlus;
		}

		if (str == "Minus")
		{
			return TokenType::tMinus;
		}

		return TokenType::tUnknown;
	}

} // namespace disassembler