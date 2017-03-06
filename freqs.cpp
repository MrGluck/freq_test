#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Need input and output file name in args\n";
		return 1;
	}
	std::wifstream in(argv[1]);
	if (!in)
	{
		std::cerr << "Can't open file " << argv[1] << " for reading\n";
		return 2;
	}
	std::wofstream out(argv[2]);
	if (!out)
	{
		std::cerr << "Can't open file " << argv[2] << " for writing\n";
		return 3;
	}

	// указываем на то, что данные будут в UTF-8
	in.imbue(std::locale(in.getloc(), new std::codecvt_utf8<wchar_t>));
	out.imbue(std::locale(out.getloc(), new std::codecvt_utf8<wchar_t>));

#ifdef _WIN32
	std::locale loc;
#else
	std::locale loc("en_US.UTF8");
#endif

	std::map<std::wstring, size_t> wordsWithCounter;
	size_t maxCounter = 0; // для определения нужного количества наборов

	for (std::wstring word; in >> word; )
	{
		// в задании требуется регистронезависимое сравнение
		if (std::isupper(word.front(), loc))
			word.front() = std::tolower(word.front(), loc);

		// удаляем знаки пунктуации (лучше бы, конечно, подошёл boost.split, но его использовать нельзя)
		while (!word.empty() && std::ispunct(word.back(), loc))
			word.pop_back();

		if (!word.empty() && maxCounter < ++wordsWithCounter[word])
			maxCounter = wordsWithCounter[word];
	}

	// коллекция наборов слов, где индекс равен количеству вхождений слова
	std::vector<std::vector<std::wstring>> collectionOfSets(maxCounter + 1);
	// обходим считанную мапу и заполняем набор. Слова уже отсортированы в лексикографическом порядке
	for (const auto &p : wordsWithCounter)
		collectionOfSets[p.second].emplace_back(p.first);

	// выводим в порядке уменьшения частоты вхождения (0 индекс игнорируем)
	for (size_t i = collectionOfSets.size() - 1; i; i--)
		for (const auto &w : collectionOfSets[i])
			out << i << " " << w << std::endl;
}
