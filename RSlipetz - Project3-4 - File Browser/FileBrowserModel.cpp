#include "FileBrowserApp.h"
#include <filesystem>
#include <regex>
using namespace std::tr2::sys;

void FileBrowser::FileBrowserModel::RefreshModel() {
	_fileSize = _filesMatched = _filesSearched = 0;
	_results.clear();
	_tabs = "";
	_regexError = false;
	//Need to get everthing sorted and then perform the search!
	path pathToSearch(_filePath);
	
	PerformRecursiveSearch(pathToSearch);
	UpdateObservers();
}

void FileBrowser::FileBrowserModel::PerformRecursiveSearch(path const& pathToSearch) {
	recursive_directory_iterator recurIterator(pathToSearch);
	if (!_isRecursive)
		recurIterator.no_push();
	_results.push_back(_tabs + recurIterator->path().directory_string());
	recursive_directory_iterator endIterator;
	for (; recurIterator != endIterator; ++recurIterator) {
		if (is_directory(recurIterator->path()) && _isRecursive) {
			_tabs = _tabs + " ";
			_results.push_back(_tabs + recurIterator->path().directory_string());
		}
		if (is_regular_file(recurIterator->path())) {
			++_filesSearched;
			if (FileMatchesRegex(recurIterator->path())) {
				++_filesMatched;
				_fileSize += file_size(recurIterator->path());
				_results.push_back(_tabs + recurIterator->path().filename());
			}
		}
	}

}

bool FileBrowser::FileBrowserModel::FileMatchesRegex(path const& fileToMatch) {
	try {
		std::regex rgx(_regex);
		return std::regex_search(fileToMatch.filename(), std::regex(rgx));
	}
	catch (std::regex_error ex) {
		if (!_regexError) {
			MessageBoxA(NULL, "Regex Error, Please Input Valid Regex", "Error!", MB_OK);
			_regexError = true;
		}
		return false;
	}
}