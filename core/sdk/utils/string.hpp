namespace StringConverter {
	std::wstring ToUnicode(const std::string& str) 	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ToASCII(const std::wstring& wstr) 	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
}

class StringFormat {
public:
	template <typename ...Args>
	static std::wstring format(const std::wstring_view text, Args... args) 	{
		const auto size = static_cast<std::size_t>(std::swprintf(nullptr, 0, text.data( ), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<wchar_t[ ]> buffer(new wchar_t[ size ]);
		std::swprintf(buffer.get( ), size, text.data( ), std::forward<Args>(args)...);

		const auto str = std::wstring(buffer.get( ), buffer.get( ) + size - 1);

		return str;
	}

	template <typename ...Args>
	static std::string format(const std::string_view text, Args... args) 	{
		char temp[ 256 ]{};
		const auto size = static_cast<std::size_t>(std::sprintf(temp, text.data( ), std::forward<Args>(args)...) + 1);

		const std::unique_ptr<char[ ]> buffer(new char[ size ]);
		std::sprintf(buffer.get( ), text.data( ), std::forward<Args>(args)...);

		const auto str = std::string(buffer.get( ), buffer.get( ) + size - 1);

		return str;
	}
};