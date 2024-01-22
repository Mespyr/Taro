#include "parser.h"
#include <cstdint>

bool Parser::is_legal_name(Token token_name) {
	// if token is an integer or string
	if (token_name.type == TOKEN_INT || token_name.type == TOKEN_STRING) return false;
	// any illegal characters in names
	if (token_name.value.find('"') != std::string::npos) return false;
	if (token_name.value.find('^') != std::string::npos) return false;
	if (token_name.value.find('@') != std::string::npos) return false;
	if (token_name.value.find('&') != std::string::npos) return false;
	if (token_name.value.find('.') != std::string::npos) return false;
	if (token_name.value.front() == '<' || token_name.value.back() == '>') return false;
	// if token is builtin word or type name
	if (is_builtin_word(token_name.value)) return false;
	if (is_prim_type(token_name.value)) return false;

	return true;
}

std::string Parser::add_escapes_to_string(std::string str) {
	std::string buf;
	std::string ret;
	uint64_t i = 0;

	// can't get 2 chars if string is 1 or 0 chars
	if (str.length() < 2) return str;

	while (i < str.length()) {
		buf = str.substr(i, 2);

		if (buf == "\\a")       ret.push_back('\a');
		else if (buf == "\\b")  ret.push_back('\b');
		else if (buf == "\\f")  ret.push_back('\f');
		else if (buf == "\\n")  ret.push_back('\n');
		else if (buf == "\\r")  ret.push_back('\r');
		else if (buf == "\\t")  ret.push_back('\t');
		else if (buf == "\\v")  ret.push_back('\v');
		else if (buf == "\\\\") ret.push_back('\\');
		else if (buf == "\\'")  ret.push_back('\'');
		else if (buf == "\\\"") ret.push_back('\"');
		else if (buf == "\\\?") ret.push_back('\?');
		else if (buf == "\\0")  ret.push_back('\0');
		else {
			// if escape sequence not found, shift buffer over by one char to next section
			ret.push_back(buf.at(0));
			i++;
			continue;
		}
		// skip both chars if escape sequence found in buffer
		i+=2;
	}
	return ret;
}

int64_t Parser::eval_const_expression(Location definition_loc) {
	std::vector<int64_t> stack;
	bool inside_const_def = true;

	while (inside_const_def) {
		Op op = convert_token_to_op(tokens.at(i));

		switch (op.type) {

		case OP_PUSH_INT:
			stack.push_back(op.int_operand);
			break;

		case OP_PLUS: {
			if (stack.size() < 2) {
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "+", "addition");
				exit(1);
			}
			int64_t a = stack.back(); stack.pop_back();
			int64_t b = stack.back(); stack.pop_back();
			stack.push_back(a + b);
		} break;

		case OP_MINUS: {
			if (stack.size() < 2) {
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "-", "subtraction");
				exit(1);
			}
			// b, a = b - a
			int64_t a = stack.back(); stack.pop_back();
			int64_t b = stack.back(); stack.pop_back();
			stack.push_back(b - a);
		} break;

		case OP_MUL: {
			if (stack.size() < 2) {
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "*", "multiplication");
				exit(1);
			}
			int64_t a = stack.back(); stack.pop_back();
			int64_t b = stack.back(); stack.pop_back();
			stack.push_back(a * b);
		} break;

		case OP_DIV: {
			if (stack.size() < 2) {
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "/", "division");
				exit(1);
			}
			// b, a = b/a, b%a
			int64_t a = stack.back(); stack.pop_back();
			int64_t b = stack.back(); stack.pop_back();
			stack.push_back(b / a);
			stack.push_back(b % a);
		} break;

		case OP_POP: {
			if (stack.size() < 1) {
				print_not_enough_arguments_error(op.loc, 1, stack.size(), "pop");
				exit(1);
			}
			stack.pop_back();
		} break;

		case OP_SWP: {
			if (stack.size() < 2) {
				print_not_enough_arguments_error(op.loc, 2, stack.size(), "swp");
				exit(1);
			}
			// b, a = a, b
			int64_t a = stack.back(); stack.pop_back();
			int64_t b = stack.back(); stack.pop_back();
			stack.push_back(a);
			stack.push_back(b);
		} break;

		case OP_FUN:
		case OP_CONST:
		case OP_IMPORT:
		case OP_STRUCT:
			i--;
			inside_const_def = false;
			break;

		default:
			print_error_at_loc(op.loc, "unsuppored keyword in compile-time evaluation of expression");
			exit(1);
		}
		if (!inside_const_def)
			break;
		else i++;
		if (i >= tokens.size())
			inside_const_def = false;
	}

	if (stack.size() > 1) {
		print_error_at_loc(definition_loc, "too many values on stack for definition (expected 1, got " + std::to_string(stack.size()));
		exit(1);
	}
	else if (stack.size() < 1) {
		print_error_at_loc(definition_loc, "not enough values on stack for definition (expected 1, got 0)");
		exit(1);
	}
	return stack.back();
}
